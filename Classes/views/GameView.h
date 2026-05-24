#ifndef GAME_VIEW_H
#define GAME_VIEW_H
#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"

USING_NS_CC;

class GameView : public Node
{
public:
    CREATE_FUNC(GameView);
    PlayFieldView* getPlayFieldView() { return _playFieldView; }
    StackView* getStackView() { return _stackView; }

protected:
    virtual bool init() override;
    PlayFieldView* _playFieldView = nullptr;
    StackView* _stackView = nullptr;
};

#endif