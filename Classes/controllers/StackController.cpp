#include "StackController.h"
#include "services/UndoService.h"
#include "views/CardView.h"
#include <random>
#include <algorithm>
USING_NS_CC;

void StackController::init(GameModel* m, StackView* v, UndoManager* um)
{
    _model = m;
    _view = v;
    _undoMgr = um;

    // GameController will bind undo to the global undo manager with PlayFieldView context
    _view->onStockClick = [=]()
        {
            drawNextStock();
        };
}

void StackController::drawNextStock()
{
    // if stock empty, regenerate a deck (excluding cards currently on playfield and tray) to allow continuous draws
    if (_model->stockCards.empty())
    {
        // build set of used (face,suit,pos) by playfield and tray
        std::set<std::pair<int,int>> used;
        for (const auto& c : _model->playfieldCards)
        {
            if (c.id != -1 && !c.isRemoved)
                used.insert({c.face, c.suit});
        }
        if (_model->trayCard.id != -1)
            used.insert({_model->trayCard.face, _model->trayCard.suit});

        // create fresh cards (standard 52 deck) excluding used set
        std::vector<CardModel> newStock;
        int nextId = 0;
        // find max id to continue ids
        for (const auto& c : _model->playfieldCards) nextId = std::max(nextId, c.id + 1);
        nextId = std::max(nextId, _model->trayCard.id + 1);
        for (int s = 0; s < 4; ++s)
        {
            for (int f = 1; f <= 13; ++f)
            {
                if (used.find({f, s}) != used.end()) continue;
                // position will be set later; mark as open=false initially
                newStock.emplace_back(nextId++, f, s, false, Vec2::ZERO);
            }
        }
        // shuffle
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(newStock.begin(), newStock.end(), g);
        // move into model.stockCards
        for (auto& c : newStock) _model->stockCards.push_back(c);
    }

    // Move first stock card to tray
    CardModel next = _model->stockCards.front();
    _model->stockCards.erase(_model->stockCards.begin());

    // record undo (store previous tray and new tray)
    UndoService::recordStepForDraw(_model->undoModel, _model->trayCard, next);

    // ensure new tray card is shown open
    next.isOpen = true;
    // set model tray to next
    _model->trayCard = next;
    _view->setTrayCard(&_model->trayCard);

    // animate newly created tray card from stock position to tray position
    CardView* trayCv = _view->getTrayCardView();
    CardView* stockCv = _view->getStockCardView();
    if (trayCv && stockCv)
    {
        // place tray view at stock position first, then move to tray position
        Vec2 startPos = stockCv->getPosition();
        trayCv->setPosition(startPos);
        trayCv->runAction(MoveTo::create(0.28f, _view->getTrayPosition()));
    }

    // enable undo button via StackView
    if (_view) _view->setUndoEnabled(!_model->undoModel.steps.empty());
}
