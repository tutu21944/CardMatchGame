#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H
#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"

class UndoManager
{
public:
    void undo(GameModel& m, PlayFieldView* pfView, StackView* stackView);
};

#endif