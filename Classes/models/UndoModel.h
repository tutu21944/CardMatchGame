#ifndef UNDO_MODEL_H
#define UNDO_MODEL_H
#include "cocos2d.h"

struct UndoStep
{
    int cardId;
    cocos2d::Vec2 fromPos;
    cocos2d::Vec2 toPos;
    int oldTrayFace;
    int oldTraySuit;
    int oldTrayId;
    int flippedCardId; // if an internal card was auto-flipped as part of the step
    bool flippedPrevOpen;
};

class UndoModel
{
public:
    std::vector<UndoStep> steps;
};

#endif