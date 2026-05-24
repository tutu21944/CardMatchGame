#ifndef CARD_RES_CONFIG_H
#define CARD_RES_CONFIG_H
#include <string>
#include "cocos2d.h"
USING_NS_CC;
class CardResConfig
{
public:
    static std::string getCardBackRes();
    static std::string getCardFaceRes(int face, int suit);
};

#endif