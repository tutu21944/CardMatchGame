#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H
#include "cocos2d.h"
USING_NS_CC;
class CommonUtils
{
public:
    static bool isMatch(int a, int b)
    {
        return abs(a - b) == 1;
    }
};

#endif