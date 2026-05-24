#include "GameView.h"

USING_NS_CC;

bool GameView::init()
{
    if (!Node::init()) return false;
    setContentSize(Size(1080, 2080));

    _playFieldView = PlayFieldView::create();
    _playFieldView->setPosition(Vec2(0, 580));
    addChild(_playFieldView);

    _stackView = StackView::create();
    _stackView->setPosition(Vec2(0, 0));
    addChild(_stackView);

    return true;
}