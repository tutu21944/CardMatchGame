#include "UndoManager.h"
#include "cocos2d.h"
USING_NS_CC;

void UndoManager::undo(GameModel& m, PlayFieldView* pfView, StackView* stackView)
{
    // 1) protect against empty stack
    if (m.undoModel.steps.empty()) return;

    // 2) get last step (copy) and pop
    auto step = m.undoModel.steps.back();
    m.undoModel.steps.pop_back();

    // 3) restore tray card data first
    m.trayCard.face = step.oldTrayFace;
    m.trayCard.suit = step.oldTraySuit;
    if (step.oldTrayId != 0)
    {
        m.trayCard.id = step.oldTrayId;
    }

    // 4) attempt to animate tray card back to playfield original position
    // find tray CardView in StackView
    CardView* trayCv = nullptr;
    if (stackView)
    {
        trayCv = stackView->getTrayCardView();
    }

    // compute world target from playfield model pos
    Vec2 targetWorld = Vec2::ZERO;
    if (pfView)
    {
        targetWorld = pfView->convertToWorldSpace(step.fromPos);
    }

    // If tray view exists, animate it back to the target (convert world to its parent space)
    if (trayCv && stackView)
    {
        Vec2 targetInStack = stackView->convertToNodeSpace(targetWorld);
        // capture step by value, m by reference, pfView and stackView by value
        trayCv->runAction(Sequence::create(MoveTo::create(0.28f, targetInStack), CallFunc::create([step, &m, pfView, stackView]() {
            // after animation, ensure playfield card exists and is visible
            if (pfView)
            {
                CardView* pv = pfView->getCardViewById(step.cardId);
                if (pv)
                {
                    // restore model entry
                    for (auto& cm : m.playfieldCards)
                    {
                        if (cm.id == step.cardId)
                        {
                            cm.isRemoved = false;
                            cm.pos = step.fromPos;
                            break;
                        }
                    }
                    pv->setPosition(step.fromPos);
                    pv->setVisible(true);
                    pv->updateView();
                }
            }
            // reset stack view tray to restored model.trayCard
            if (stackView)
            {
                stackView->setTrayCard(&m.trayCard);
                stackView->setUndoEnabled(!m.undoModel.steps.empty());
            }
        }), nullptr));
    }
    else
    {
        // fallback: directly restore playfield card view if exists
        if (pfView)
        {
            CardView* pv = pfView->getCardViewById(step.cardId);
            if (pv)
            {
                // restore model entry
                for (auto& cm : m.playfieldCards)
                {
                    if (cm.id == step.cardId)
                    {
                        cm.isRemoved = false;
                        cm.pos = step.fromPos;
                        break;
                    }
                }
                pv->setPosition(step.fromPos);
                pv->setVisible(true);
                pv->updateView();
            }
        }
        if (stackView) stackView->setUndoEnabled(!m.undoModel.steps.empty());
    }

    // 5) restore flipped card state if any
    if (step.flippedCardId != -1)
    {
        for (auto& c : m.playfieldCards)
        {
            if (c.id == step.flippedCardId)
            {
                c.isOpen = step.flippedPrevOpen;
                if (pfView) pfView->updateCardView(c.id);
                break;
            }
        }
    }
}
