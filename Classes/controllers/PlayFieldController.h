#ifndef PLAYFIELD_CONTROLLER_H
#define PLAYFIELD_CONTROLLER_H
#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include "managers/UndoManager.h"
#include "cocos2d.h"
USING_NS_CC;
class PlayFieldController
{
public:
    void init(GameModel* m, PlayFieldView* v, class StackView* sv, UndoManager* um);
    void handleCardClick(int id);

private:
    GameModel* _model = nullptr;
    PlayFieldView* _view = nullptr;
    class StackView* _stackView = nullptr;
    UndoManager* _undoMgr = nullptr;
};

#endif