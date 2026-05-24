#ifndef CARD_MODEL_H
#define CARD_MODEL_H
#include "cocos2d.h"

enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,
    CST_DIAMONDS,
    CST_HEARTS,
    CST_SPADES,
    CST_NUM
};

enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM
};

class CardModel
{
public:
    CardModel(): id(-1), face(0), suit(0), isOpen(false), isRemoved(false), pos(cocos2d::Vec2::ZERO) {}
    int id;
    int face;
    int suit;
    bool isOpen;        // 翻开/覆盖
    bool isRemoved;     // 是否已移除（匹配到托盘）
    cocos2d::Vec2 pos;  // 原始位置

    CardModel(int id_, int face_, int suit_, bool isOpen_, cocos2d::Vec2 pos_, bool isRemoved_ = false)
        : id(id_), face(face_), suit(suit_), isOpen(isOpen_), isRemoved(isRemoved_), pos(pos_) {
    }
};

#endif