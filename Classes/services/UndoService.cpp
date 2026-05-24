#include "UndoService.h"
#include "cocos2d.h"
USING_NS_CC;
// Note: recordStep with flipped info is implemented below (with default params in header)

void UndoService::recordStepForDraw(UndoModel& m, const CardModel& oldTray, const CardModel& newTray)
{
    UndoStep s;
    s.cardId = newTray.id;
    s.fromPos = newTray.pos;
    s.toPos = Vec2(540, 250);
    s.oldTrayFace = oldTray.face;
    s.oldTraySuit = oldTray.suit;
    s.oldTrayId = oldTray.id;
    s.flippedCardId = -1;
    s.flippedPrevOpen = false;
    m.steps.push_back(s);
}

// new overload
void UndoService::recordStep(UndoModel& m, const CardModel& c, int oldFace, int oldSuit, int flippedCardId, bool flippedPrevOpen)
{
    UndoStep s;
    s.cardId = c.id;
    s.fromPos = c.pos;
    s.toPos = Vec2(540, 250);
    s.oldTrayFace = oldFace;
    s.oldTraySuit = oldSuit;
    s.flippedCardId = flippedCardId;
    s.flippedPrevOpen = flippedPrevOpen;
    m.steps.push_back(s);
}


