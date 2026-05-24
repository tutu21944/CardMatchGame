#include "PlayFieldView.h"

USING_NS_CC;

bool PlayFieldView::init()
{
    if (!Node::init()) return false;
    _cards.clear();
    _pressedCardId = -1;

    // Add centralized touch listener to detect card taps (works around individual card listeners)
    auto listener = EventListenerTouchOneByOne::create();
    // Do not swallow touches so other UI (Undo button) can also receive events when appropriate
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this](Touch* touch, Event*)
    {
        Vec2 world = touch->getLocation();
        for (auto& c : _cards)
        {
            if (!c || !c->isVisible()) continue;
            Vec2 loc = c->convertToNodeSpace(world);
            Size s = c->getContentSize();
            Vec2 a = c->getAnchorPoint();
            Rect rect(-s.width * a.x, -s.height * a.y, s.width, s.height);
            if (rect.containsPoint(loc))
            {
                _pressedCardId = c->getCardId();
                return true;
            }
        }
        return false;
    };
    listener->onTouchEnded = [this](Touch* touch, Event* event)
    {
        if (_pressedCardId != -1)
        {
            // find the CardView object and invoke its stored click callback (if any)
            for (auto& cv : _cards)
            {
                if (cv && cv->getCardId() == _pressedCardId)
                {
                    // first call per-card callback if set
                    cv->invokeClickCallback();
                    break;
                }
            }
            // then forward to PlayFieldView handler for controller-level handling
            if (onCardClick) onCardClick(_pressedCardId);
            _pressedCardId = -1;
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void PlayFieldView::addCardView(CardView* v, const Vec2& pos)
{
    v->setPosition(pos);
    // ensure card-level touch enabled
    v->setTouchEnabled(true);
    addChild(v);
    _cards.push_back(v);
}

void PlayFieldView::moveCardToPos(int id, const Vec2& pos, float t, const std::function<void()>& onComplete)
{
    for (auto& c : _cards)
    {
        if (c->getCardId() == id)
        {
            auto move = MoveTo::create(t, pos);
            if (onComplete)
            {
                auto seq = Sequence::create(move, CallFunc::create(onComplete), nullptr);
                c->runAction(seq);
            }
            else
            {
                c->runAction(move);
            }
            break;
        }
    }
}

void PlayFieldView::updateCardView(int id)
{
    for (auto& c : _cards)
    {
        if (c->getCardId() == id)
        {
            c->updateView();
            break;
        }
    }
}

CardView* PlayFieldView::getCardViewById(int id)
{
    for (auto& c : _cards)
    {
        if (c && c->getCardId() == id) return c;
    }
    return nullptr;
}
