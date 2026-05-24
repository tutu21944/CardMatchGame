#include "StackView.h"
#include "models/CardModel.h"

USING_NS_CC;

bool StackView::init()
{
    if (!Node::init()) return false;

    // 修复Undo按钮回调，可撤销
    _undoBtn = MenuItemLabel::create(
        Label::createWithTTF("Undo", "fonts/Marker Felt.ttf", 30),
        [=](Ref*) { if (onUndoClick) onUndoClick(); }
    );
    _undoBtn->setPosition(Vec2(700, 100));
    _undoBtn->setEnabled(false); // disabled initially
    auto menu = Menu::create(_undoBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);

    // 安全创建备用牌
    CardModel emptyModel(-1, 0, 0, false, Vec2::ZERO);
    _stockCard = CardView::create(&emptyModel);
    if (_stockCard)
    {
        _stockCard->setPosition(Vec2(200, 250));
        // enable touch and bind callback to notify controller via onStockClick
        _stockCard->setTouchEnabled(true);
        _stockCard->setClickCallback([this](int){ if (onStockClick) onStockClick(); });
        addChild(_stockCard);

        // Also add a dedicated touch listener on the stock card node to ensure clicks are reliable
        auto stockListener = EventListenerTouchOneByOne::create();
        stockListener->setSwallowTouches(true);
        stockListener->onTouchBegan = [this](Touch* touch, Event*) -> bool {
            if (!_stockCard || !_stockCard->isVisible()) return false;
            Vec2 loc = _stockCard->convertToNodeSpace(touch->getLocation());
            Size s = _stockCard->getContentSize();
            Vec2 a = _stockCard->getAnchorPoint();
            // expand hit area by margin to improve touch sensitivity
            float margin = 40.0f;
            Rect rect(-s.width * a.x - margin, -s.height * a.y - margin, s.width + margin * 2, s.height + margin * 2);
            return rect.containsPoint(loc);
        };
        stockListener->onTouchEnded = [this](Touch*, Event*) {
            if (onStockClick) onStockClick();
        };
        _stockCard->getEventDispatcher()->addEventListenerWithSceneGraphPriority(stockListener, _stockCard);
    }

    return true;
}

void StackView::setUndoEnabled(bool enabled)
{
    if (_undoBtn)
    {
        _undoBtn->setEnabled(enabled);
        _undoBtn->setColor(enabled ? Color3B::WHITE : Color3B(100,100,100));
    }
}

CardView* StackView::getTrayCardView() const
{
    return _trayCard;
}

CardView* StackView::getStockCardView() const
{
    return _stockCard;
}

void StackView::setTrayCard(CardModel* model)
{
    if (_trayCard)
    {
        _trayCard->removeFromParent();
        _trayCard = nullptr;
    }
    if (!model) return;

    // create a view that references the provided model (model is owned by GameModel)
    _trayCard = CardView::create(model);
    if (_trayCard)
    {
        _trayCard->setPosition(getTrayPosition());
        addChild(_trayCard, 10);
    }
}