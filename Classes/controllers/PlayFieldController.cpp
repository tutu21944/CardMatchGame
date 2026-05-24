#include "PlayFieldController.h"
#include "managers/UndoManager.h"
#include "views/StackView.h"
#include "services/MatchService.h"
#include "services/UndoService.h"

void PlayFieldController::init(GameModel* m, PlayFieldView* v, StackView* sv, UndoManager* um)
{
    _model = m;
    _view = v;
    _stackView = sv;
    _undoMgr = um;

    // bind view callback
    if (_view)
    {
        _view->onCardClick = [this](int id) { this->handleCardClick(id); };
    }
}

static bool isTopTwoCard(const GameModel* model, const CardModel* clicked)
{
    if (!model || !clicked) return false;
    std::vector<std::pair<float, const CardModel*>> col;
    for (const auto& c : model->playfieldCards)
    {
        if (c.isRemoved) continue;
        if (static_cast<int>(c.pos.x) == static_cast<int>(clicked->pos.x))
            col.push_back({c.pos.y, &c});
    }
    if (col.empty()) return false;
    std::sort(col.begin(), col.end(), [](const std::pair<float,const CardModel*>& a, const std::pair<float,const CardModel*>& b){ return a.first > b.first; });
    for (size_t i = 0; i < col.size() && i < 2; ++i)
    {
        if (col[i].second->id == clicked->id) return true;
    }
    return false;
}

void PlayFieldController::handleCardClick(int cardId)
{
    // find clicked card
    CardModel* clicked = nullptr;
    for (auto& c : _model->playfieldCards)
    {
        if (c.id == cardId) { clicked = &c; break; }
    }
    if (!clicked) return;

    // only top two per column
    if (!isTopTwoCard(_model, clicked)) return;

    // flip closed
    if (!clicked->isOpen)
    {
        clicked->isOpen = true;
        if (_view) _view->updateCardView(cardId);
        return;
    }

    // tray card
    CardModel* tray = &_model->trayCard;
    if (!tray || tray->id == -1) return;

    // match by face only
    if (!MatchService::canMatchByFace(*clicked, *tray)) return;

    // compute target in playfield coordinates
    Vec2 trayLocal = _stackView ? _stackView->getTrayPosition() : Vec2(540,250);
    Vec2 trayWorld = _stackView ? _stackView->convertToWorldSpace(trayLocal) : trayLocal;
    Vec2 targetInPlayfield = _view ? _view->convertToNodeSpace(trayWorld) : Vec2(540,250);

    // record undo: previous tray face/suit and whether we auto-flipped another card
    int oldTrayFace = tray->face;
    int oldTraySuit = tray->suit;

    // find below card to auto-flip after removal (the new top card in this column)
    int flippedCardId = -1;
    bool flippedPrevOpen = false;
    {
        // gather column cards and find the next visible top card after clicked
        std::vector<std::pair<float,int>> col; // y, index
        for (size_t i = 0; i < _model->playfieldCards.size(); ++i)
        {
            const auto& c = _model->playfieldCards[i];
            if (c.isRemoved) continue;
            if (static_cast<int>(c.pos.x) == static_cast<int>(clicked->pos.x))
                col.push_back({c.pos.y, (int)i});
        }
        std::sort(col.begin(), col.end(), [](const std::pair<float,int>& a, const std::pair<float,int>& b){ return a.first > b.first; });
        // find clicked index in that col and take the next one below
        for (size_t i = 0; i < col.size(); ++i)
        {
            if (_model->playfieldCards[col[i].second].id == clicked->id)
            {
                if (i + 1 < col.size())
                {
                    flippedCardId = _model->playfieldCards[col[i+1].second].id;
                    flippedPrevOpen = _model->playfieldCards[col[i+1].second].isOpen;
                }
                break;
            }
        }
    }

    // move and then set as new tray
    _view->moveCardToPos(cardId, targetInPlayfield, 0.32f, [this, clicked, cardId, oldTrayFace, oldTraySuit, flippedCardId, flippedPrevOpen]() {
        // first copy clicked data into tray so tray view remains visible
        _model->trayCard = *clicked;
        // then remove the card from playfield (its view will hide)
        clicked->isRemoved = true;
        if (_stackView) _stackView->setTrayCard(&_model->trayCard);
        // auto-flip next card if exists
        if (flippedCardId != -1)
        {
            for (auto& c : _model->playfieldCards)
            {
                if (c.id == flippedCardId)
                {
                    c.isOpen = true;
                    if (_view) _view->updateCardView(c.id);
                    break;
                }
            }
        }
        // record undo step with flipped info
        UndoService::recordStep(_model->undoModel, *clicked, oldTrayFace, oldTraySuit, flippedCardId, flippedPrevOpen);
        // enable undo button on stack view
        if (_stackView) _stackView->setUndoEnabled(!_model->undoModel.steps.empty());
        if (_view) _view->updateCardView(cardId);
    });
}