#include "LevelConfigLoader.h"

LevelConfig LevelConfigLoader::load()
{
    LevelConfig cfg;
    // Try to load JSON level config from resources (res/level1.json)
    // Try multiple candidate paths for level config to be robust across resource layouts
    std::vector<std::string> candidates = {"res/level1.json", "level1.json", "Resources/res/level1.json", "Resources/level1.json"};
    std::string foundPath;
    for (auto& p : candidates)
    {
        std::string full = FileUtils::getInstance()->fullPathForFilename(p);
        if (!full.empty())
        {
            foundPath = full;
            break;
        }
    }
    if (!foundPath.empty())
    {
        ValueMap root = FileUtils::getInstance()->getValueMapFromFile(foundPath);
        // parse playfield
        if (root.find("playfield") != root.end())
        {
            ValueVector pv = root["playfield"].asValueVector();
            for (auto& v : pv)
            {
                if (v.getType() == Value::Type::MAP)
                {
                    ValueMap m = v.asValueMap();
                    int face = m.count("face") ? m["face"].asInt() : 0;
                    int suit = m.count("suit") ? m["suit"].asInt() : 0;
                    Vec2 pos = Vec2::ZERO;
                    if (m.count("pos"))
                    {
                        ValueMap p = m["pos"].asValueMap();
                        pos.x = p.count("x") ? p["x"].asFloat() : 0;
                        pos.y = p.count("y") ? p["y"].asFloat() : 0;
                    }
                    else
                    {
                        pos.x = m.count("x") ? m["x"].asFloat() : 0;
                        pos.y = m.count("y") ? m["y"].asFloat() : 0;
                    }
                    cfg.playfield.push_back({face, suit, pos});
                }
            }
        }
        // parse stack
        if (root.find("stack") != root.end())
        {
            ValueVector sv = root["stack"].asValueVector();
            for (auto& v : sv)
            {
                if (v.getType() == Value::Type::MAP)
                {
                    ValueMap m = v.asValueMap();
                    int face = m.count("face") ? m["face"].asInt() : 0;
                    int suit = m.count("suit") ? m["suit"].asInt() : 0;
                    Vec2 pos = Vec2::ZERO;
                    if (m.count("pos"))
                    {
                        ValueMap p = m["pos"].asValueMap();
                        pos.x = p.count("x") ? p["x"].asFloat() : 0;
                        pos.y = p.count("y") ? p["y"].asFloat() : 0;
                    }
                    else
                    {
                        pos.x = m.count("x") ? m["x"].asFloat() : 0;
                        pos.y = m.count("y") ? m["y"].asFloat() : 0;
                    }
                    cfg.stack.push_back({face, suit, pos});
                }
            }
        }
        return cfg;
    }

    // Fallback hardcoded config
    cfg.playfield = {
        {2, 0, Vec2(200, 400)},
        {3, 0, Vec2(320, 400)},
        {4, 1, Vec2(440, 400)},
        {5, 0, Vec2(200, 550)},
        {6, 0, Vec2(320, 550)},
        {7, 3, Vec2(440, 550)}
    };
    cfg.stack = {
        {2, 0, Vec2(0, 0)},
        {0, 2, Vec2(0, 0)},
        {3, 0, Vec2(0, 0)}
    };
    return cfg;
}