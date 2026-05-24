#include "CardResConfig.h"
#include <string>
#include "cocos2d.h"

USING_NS_CC;

std::string CardResConfig::getCardBackRes()
{
    return "card_back.png";
}

std::string CardResConfig::getCardFaceRes(int face, int suit)
{
    return "card_" + std::to_string(suit) + "_" + std::to_string(face) + ".png";
}