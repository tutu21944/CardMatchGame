#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelGenerator.h"
#include "views/CardView.h"

USING_NS_CC;

void GameController::startGame(Node* parentNode)
{
    auto cfg = LevelConfigLoader::load();
    _model = GameModelGenerator::generate(cfg);

    _view = GameView::create();
    parentNode->addChild(_view);

    _pfCtrl.init(&_model, _view->getPlayFieldView(), _view->getStackView(), &_undoMgr);
    _stackCtrl.init(&_model, _view->getStackView(), &_undoMgr);

    // 关键！！！每张牌必须绑定点击回调
    for (auto& c : _model.playfieldCards)
    {
        auto cardView = CardView::create(&c);
        // 绑定控制器匹配逻辑
        cardView->setClickCallback([=](int id)
            {
                _pfCtrl.handleCardClick(id);
            });
        _view->getPlayFieldView()->addCardView(cardView, c.pos);
    }

    // Set initial tray card view from model.trayCard
    if (_model.trayCard.id != -1)
    {
        _view->getStackView()->setTrayCard(&_model.trayCard);
    }

    // Bind Undo button to GameController's undo so it can pass PlayFieldView for animation
    _view->getStackView()->onUndoClick = [this]() {
        _undoMgr.undo(_model, _view->getPlayFieldView(), _view->getStackView());
        // update undo button state
        _view->getStackView()->setUndoEnabled(!_model.undoModel.steps.empty());
    };
}