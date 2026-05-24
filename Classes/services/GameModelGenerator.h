#ifndef GAME_MODEL_GENERATOR_H
#define GAME_MODEL_GENERATOR_H
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"
#include "cocos2d.h"
USING_NS_CC;
class GameModelGenerator
{
public:
    static GameModel generate(const LevelConfig& cfg);
};

#endif