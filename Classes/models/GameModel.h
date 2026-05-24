#ifndef GAME_MODEL_H
#define GAME_MODEL_H
#include <vector>
#include "models/CardModel.h"
#include "models/UndoModel.h"

class GameModel
{
public:
    std::vector<CardModel> playfieldCards;  // 主牌区
    std::vector<CardModel> stockCards;      // 备用牌堆（统一名字，不是stackCards）
    CardModel trayCard;                     // 当前托盘牌（单独保存）
    UndoModel undoModel;

    GameModel() : trayCard() {}
};

#endif