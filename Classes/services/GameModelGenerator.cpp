#include "GameModelGenerator.h"

GameModel GameModelGenerator::generate(const LevelConfig& cfg)
{
    GameModel m;
    int id = 0;
    // Create playfield cards (initially closed), group by column (x) to open top two per column
    std::map<int, std::vector<std::pair<int, float>>> columns; // x -> vector of (index, y)
    for (auto& cc : cfg.playfield)
    {
        int face = cc.face;
        if (face < 1 || face > 13) face = std::max(1, std::min(13, face));
        int suit = cc.suit;
        if (suit < 0 || suit > 3) suit = 0;
        m.playfieldCards.emplace_back(id++, face, suit, false, cc.pos);
    }
    // group by integer x coordinate
    for (size_t i = 0; i < m.playfieldCards.size(); ++i)
    {
        int key = static_cast<int>(m.playfieldCards[i].pos.x);
        columns[key].push_back({(int)i, m.playfieldCards[i].pos.y});
    }
    // for each column, sort by y descending and open top two
    for (auto& kv : columns)
    {
        auto& vec = kv.second;
        std::sort(vec.begin(), vec.end(), [](const std::pair<int,float>& a, const std::pair<int,float>& b){ return a.second > b.second; });
        for (size_t k = 0; k < vec.size() && k < 2; ++k)
        {
            int idx = vec[k].first;
            if (idx >= 0 && idx < (int)m.playfieldCards.size())
                m.playfieldCards[idx].isOpen = true;
        }
    }
    for (auto& cc : cfg.stack)
    {
        int face = cc.face;
        if (face < 1 || face > 13) face = std::max(1, std::min(13, face));
        int suit = cc.suit;
        if (suit < 0 || suit > 3) suit = 0;
        m.stockCards.emplace_back(id++, face, suit, true, cc.pos);
    }
    // Initialize trayCard from the first stock card if present
    if (!m.stockCards.empty())
    {
        m.trayCard = m.stockCards.front();
        // remove from stock since it's now the tray card
        m.stockCards.erase(m.stockCards.begin());
    }
    return m;
}