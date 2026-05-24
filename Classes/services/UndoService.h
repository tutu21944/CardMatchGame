#ifndef UNDO_SERVICE_H
#define UNDO_SERVICE_H
#include "models/UndoModel.h"
#include "models/CardModel.h"

class UndoService
{
public:
    static void recordStep(UndoModel& m, const CardModel& c, int oldFace, int oldSuit, int flippedCardId = -1, bool flippedPrevOpen = false);
    static void recordStepForDraw(UndoModel& m, const CardModel& oldTray, const CardModel& newTray);
};

#endif