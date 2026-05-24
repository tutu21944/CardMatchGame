#ifndef STACK_CONTROLLER_H
#define STACK_CONTROLLER_H
#include "models/GameModel.h"
#include "views/StackView.h"
#include "managers/UndoManager.h"

class StackController
{
public:
    void init(GameModel* m, StackView* v, UndoManager* um);
    void drawNextStock(); // 抽备用牌函数声明

private:
    GameModel* _model = nullptr;
    StackView* _view = nullptr;
    UndoManager* _undoMgr = nullptr;
};

#endif