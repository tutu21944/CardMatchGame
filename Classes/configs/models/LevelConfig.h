#ifndef LEVEL_CONFIG_H
#define LEVEL_CONFIG_H
#include <vector>
#include "cocos2d.h"

USING_NS_CC;
struct CardConfig
{
    int face;
    int suit;
    cocos2d::Vec2 pos;
};

class LevelConfig
{
public:
    std::vector<CardConfig> playfield;
    std::vector<CardConfig> stack;
};

#endif