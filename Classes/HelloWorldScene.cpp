#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}
bool HelloWorld::init()
{
    if (!Layer::init()) return false;

    // 关键：设置背景为浅灰色，不再黑屏
    auto bg = LayerColor::create(Color4B(220, 220, 220, 255));
    this->addChild(bg, -1); // -1：放在最底层

    setContentSize(Size(1080, 2080));
    _gameCtrl.startGame(this);
    return true;
}
//bool HelloWorld::init()
//{
//    if (!Layer::init()) return false;
//    setContentSize(Size(1080, 2080));
//    _gameCtrl.startGame();
//    return true;
//}