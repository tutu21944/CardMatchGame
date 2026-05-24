#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "managers/UndoManager.h"

USING_NS_CC;

class GameController
{
public:
    void startGame(Node* parentNode);

private:
    GameModel _model;
    GameView* _view = nullptr;
    PlayFieldController _pfCtrl;
    StackController _stackCtrl;
    UndoManager _undoMgr;
};

#endif