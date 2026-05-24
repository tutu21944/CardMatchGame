#ifndef CARD_CONTROLLER_H
#define CARD_CONTROLLER_H
#include "models/CardModel.h"
#include "views/CardView.h"
#include "cocos2d.h"
USING_NS_CC;
class CardController
{
public:
    CardController(CardModel* m, CardView* v) : _model(m), _view(v) {}
    void updateView();

private:
    CardModel* _model = nullptr;
    CardView* _view = nullptr;
};

#endif