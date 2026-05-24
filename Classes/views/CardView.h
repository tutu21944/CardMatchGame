
#ifndef CARD_VIEW_H
#define CARD_VIEW_H

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

USING_NS_CC;

class CardView : public cocos2d::Node
{
public:
    static CardView* create(CardModel* model);
    bool init(CardModel* model);
    void updateView();
    void setClickCallback(const std::function<void(int)>& cb);
    void invokeClickCallback();
    void setTouchEnabled(bool enabled) { _touchEnabled = enabled; }
    int getCardId() const;

private:
    void onTouchEnded(cocos2d::Touch*, cocos2d::Event*);

    CardModel* _model = nullptr;
    cocos2d::Sprite* _bg = nullptr;
    cocos2d::Sprite* _bigNum = nullptr;
    cocos2d::Sprite* _smallNum = nullptr;
    cocos2d::Label* _bigLabel = nullptr;
    cocos2d::Sprite* _suit = nullptr;
    std::function<void(int)> _clickCallback;
    bool _touchEnabled = true;
};

#endif // CARD_VIEW_H
