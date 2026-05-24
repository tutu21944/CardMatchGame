#ifndef MATCH_SERVICE_H
#define MATCH_SERVICE_H

#include "models/CardModel.h"
#include <cstdlib>

namespace MatchService
{
    // 仅比较点数差 1（忽略花色）
    inline bool canMatchByFace(const CardModel& tableCard, const CardModel& trayCard)
    {
        return std::abs(tableCard.face - trayCard.face) == 1;
    }
}

#endif // MATCH_SERVICE_H
