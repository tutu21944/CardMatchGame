#ifndef LEVEL_CONFIG_LOADER_H
#define LEVEL_CONFIG_LOADER_H

#include "configs/models/LevelConfig.h"
#include "cocos2d.h"
USING_NS_CC;
class LevelConfigLoader
{
public:
    static LevelConfig load();
};

#endif