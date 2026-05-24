#ifndef STACK_VIEW_H
#define STACK_VIEW_H
#include "cocos2d.h"
#include "views/CardView.h"
#include "models/CardModel.h"
#include <vector>
#include <functional>

USING_NS_CC;

class StackView : public Node
{
public:
    CREATE_FUNC(StackView);
    // 新版：接收 CardModel*，不再接收int
    void setTrayCard(CardModel* model);
    cocos2d::Vec2 getTrayPosition() const { return Vec2(540, 250); }
    std::function<void()> onUndoClick;
    std::function<void()> onStockClick;
    // return current tray CardView (may be nullptr)
    CardView* getTrayCardView() const;
    // return current stock/back CardView (may be nullptr)
    CardView* getStockCardView() const;
    void setUndoEnabled(bool enabled);

protected:
    virtual bool init() override;
    CardView* _trayCard = nullptr;
    CardView* _stockCard = nullptr;
    cocos2d::MenuItemLabel* _undoBtn = nullptr;
};

#endif