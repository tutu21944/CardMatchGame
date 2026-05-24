#ifndef PLAYFIELD_VIEW_H
#define PLAYFIELD_VIEW_H
#include "cocos2d.h"
#include "CardView.h"
#include <vector>
#include <functional>

USING_NS_CC;

class PlayFieldView : public Node
{
public:
    CREATE_FUNC(PlayFieldView);
    void addCardView(CardView* v, const Vec2& pos);
    // move card with optional completion callback
    void moveCardToPos(int id, const Vec2& pos, float t, const std::function<void()>& onComplete = nullptr);
    void updateCardView(int id);
    // find CardView by card id
    CardView* getCardViewById(int id);
    std::function<void(int)> onCardClick;

protected:
    virtual bool init() override;
    std::vector<CardView*> _cards;
    int _pressedCardId = -1;
};

#endif