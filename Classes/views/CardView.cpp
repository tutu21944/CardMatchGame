#include "CardView.h"

USING_NS_CC;

CardView* CardView::create(CardModel* model)
{
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(model))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(CardModel* model)
{
    if (!Node::init()) return false;

    _model = model;
    _bigNum = nullptr;
    _smallNum = nullptr;
    _suit = nullptr;

    // use correct resource paths under res/
    _bg = nullptr;
    // Try loading background sprite from multiple candidate paths using fullPathForFilename
    std::vector<std::string> bgCandidates = {"res/card_general.png", "card_general.png", "Resources/res/card_general.png", "Resources/card_general.png", "res/res/card_general.png"};
    std::string bgFull;
    for (auto& p : bgCandidates)
    {
        std::string fp = FileUtils::getInstance()->fullPathForFilename(p);
        if (!fp.empty()) { bgFull = fp; break; }
    }
    if (!bgFull.empty())
    {
        _bg = Sprite::create(bgFull);
        CCLOG("CardView: loaded bg from %s", bgFull.c_str());
    }
    else
    {
        CCLOG("CardView: background not found in candidates");
    }
    if (_bg)
    {
        // put bg centered in this node
        setContentSize(_bg->getContentSize());
        _bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        _bg->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.5f));
        addChild(_bg, 0);
    }
    else
    {
        // create a placeholder rectangle as background to avoid rendering null textures
        auto dn = DrawNode::create();
        Size cs = Size(140, 200);
        // draw centered rectangle so child positions can use node center as origin
        Vec2 hw = Vec2(cs.width * 0.5f, cs.height * 0.5f);
        Vec2 verts[4] = { Vec2(-hw.x, -hw.y), Vec2(hw.x, -hw.y), Vec2(hw.x, hw.y), Vec2(-hw.x, hw.y) };
        dn->drawSolidPoly(verts, 4, Color4F(0.9f, 0.9f, 0.9f, 1.0f));
        // set content size and position drawnode at center
        setContentSize(cs);
        dn->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.5f));
        addChild(dn, 0);
    }

    setScale(1.0f);

    // 添加触摸监听器：只在触点落在卡牌区域时接收，防止被其它节点拦截
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool
    {
        if (!_touchEnabled) return false;
        if (!this->isVisible()) return false;
        Vec2 loc = this->convertToNodeSpace(touch->getLocation());
        Size s = this->getContentSize();
        Vec2 a = this->getAnchorPoint();
        Rect rect(-s.width * a.x, -s.height * a.y, s.width, s.height);
        return rect.containsPoint(loc);
    };
    listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);
    // 使用节点的事件分发器注册，确保基于场景图的优先级
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    updateView();
    return true;
}

void CardView::updateView()
{
    if (!_model) return;

    if (_bigNum) { _bigNum->removeFromParent(); _bigNum = nullptr; }
    if (_bigLabel) { _bigLabel->removeFromParent(); _bigLabel = nullptr; }
    if (_suit) { _suit->removeFromParent(); _suit = nullptr; }

    if (_model->isOpen)
    {
        bool isRed = (_model->suit == 1 || _model->suit == 2);
        std::string color = isRed ? "red" : "black";

        // big number texture path (res/number/...)
        // try multiple candidate paths for number sprite
        // map face to token (A,2..10,J,Q,K)
        std::string faceToken;
        switch (_model->face)
        {
        case 1: faceToken = "A"; break;
        case 11: faceToken = "J"; break;
        case 12: faceToken = "Q"; break;
        case 13: faceToken = "K"; break;
        default: faceToken = std::to_string(_model->face); break;
        }
        std::string faceTokenNum = std::to_string(_model->face);
        std::vector<std::string> numCandidates;
        // prefer token-based filenames (A,J,Q,K) then numeric
        numCandidates.push_back(StringUtils::format("res/number/big_%s_%s.png", color.c_str(), faceToken.c_str()));
        numCandidates.push_back(StringUtils::format("number/big_%s_%s.png", color.c_str(), faceToken.c_str()));
        numCandidates.push_back(StringUtils::format("res/res/number/big_%s_%s.png", color.c_str(), faceToken.c_str()));
        numCandidates.push_back(StringUtils::format("Resources/res/number/big_%s_%s.png", color.c_str(), faceToken.c_str()));
        // also try numeric filenames as fallback
        numCandidates.push_back(StringUtils::format("res/number/big_%s_%s.png", color.c_str(), faceTokenNum.c_str()));
        numCandidates.push_back(StringUtils::format("number/big_%s_%s.png", color.c_str(), faceTokenNum.c_str()));
        bool loadedNum = false;
        for (auto& bigPath : numCandidates)
        {
            std::string full = FileUtils::getInstance()->fullPathForFilename(bigPath);
            CCLOG("CardView: trying number candidate %s -> fullPath: %s", bigPath.c_str(), full.c_str());
            if (!full.empty())
            {
                _bigNum = Sprite::create(full);
                if (_bigNum)
                {
                    _bigNum->setPosition(Vec2(getContentSize().width*0.5f, getContentSize().height*0.5f));
                    addChild(_bigNum, 10);
                    CCLOG("CardView: loaded number sprite from %s", full.c_str());
                    loadedNum = true;
                    break;
                }
            }
        }
        if (!loadedNum)
        {
            // fallback: show face label when texture missing
            std::string faceStr;
            switch (_model->face)
            {
            case 1: faceStr = "A"; break;
            case 11: faceStr = "J"; break;
            case 12: faceStr = "Q"; break;
            case 13: faceStr = "K"; break;
            default: faceStr = std::to_string(_model->face); break;
            }
            _bigLabel = Label::createWithSystemFont(faceStr, "Arial", 48);
            if (_bigLabel)
            {
                _bigLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
                _bigLabel->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.5f));
                _bigLabel->setTextColor(isRed ? Color4B::RED : Color4B::BLACK);
                addChild(_bigLabel, 10);
            }
        }

        std::string suitBase;
        switch (_model->suit)
        {
        case 0: suitBase = "suits/club.png"; break;
        case 1: suitBase = "suits/diamond.png"; break;
        case 2: suitBase = "suits/heart.png"; break;
        case 3: suitBase = "suits/spade.png"; break;
        default: suitBase = "";
        }
        std::vector<std::string> suitCandidates;
        if (!suitBase.empty())
        {
            suitCandidates.push_back(StringUtils::format("res/%s", suitBase.c_str()));
            suitCandidates.push_back(suitBase);
            suitCandidates.push_back(StringUtils::format("res/res/%s", suitBase.c_str()));
            suitCandidates.push_back(StringUtils::format("Resources/%s", suitBase.c_str()));
        }
        for (auto& sp : suitCandidates)
        {
            if (sp.empty()) continue;
            std::string full = FileUtils::getInstance()->fullPathForFilename(sp);
            CCLOG("CardView: trying suit candidate %s -> fullPath: %s", sp.c_str(), full.c_str());
            if (!full.empty())
            {
                _suit = Sprite::create(full);
                if (_suit)
                {
                    // suit should render above the number and anchored to the top-right corner
                    // Use right-top anchor so we can position it with margins from the edges
                    _suit->setAnchorPoint(Vec2(1.0f, 1.0f));
                    // margin from right and top edges (in pixels or relative)
                    float marginX = getContentSize().width * 0.12f; // 12% from right
                    float marginY = getContentSize().height * 0.12f; // 12% from top
                    Vec2 suitPos = Vec2(getContentSize().width - marginX, getContentSize().height - marginY);
                    _suit->setPosition(suitPos);
                    // make suit slightly smaller than number to mimic real cards
                    _suit->setScale(0.6f);
                    addChild(_suit, 11);
                    CCLOG("CardView: loaded suit sprite from %s", full.c_str());
                }
                break;
            }
        }
        // ensure number is centered and rendered above bg
        if (_bigNum)
        {
            _bigNum->setAnchorPoint(Vec2(0.5f, 0.5f));
            _bigNum->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.5f));
            _bigNum->setLocalZOrder(10);
        }
        if (_bigLabel)
        {
            _bigLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
            _bigLabel->setPosition(Vec2(getContentSize().width * 0.5f, getContentSize().height * 0.5f));
            _bigLabel->setLocalZOrder(10);
        }

        if (_bg) _bg->setColor(Color3B::WHITE);
    }
    else
    {
        _bg->setColor(Color3B(100, 100, 100));
    }
    // hide removed cards
    if (_model->isRemoved)
    {
        this->setVisible(false);
    }
}

// 点击结束时触发回调（无论牌是否已经翻开）
void CardView::onTouchEnded(Touch*, Event*)
{
    if (!_touchEnabled) return;
    if (_clickCallback && _model)
    {
        _clickCallback(_model->id);
    }
}

void CardView::setClickCallback(const std::function<void(int)>& cb)
{
    _clickCallback = cb;
}

void CardView::invokeClickCallback()
{
    if (_clickCallback && _model)
    {
        _clickCallback(_model->id);
    }
}

int CardView::getCardId() const
{
    return _model ? _model->id : -1;
}
