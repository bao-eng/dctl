#pragma once

#include <deque>

#include "raylib.h"
#include "dctl_common.h"
#include <chrono>
#include "dctl_generated.h"

struct Snake
{
    int playerId;
    std::chrono::_V2::system_clock::time_point timeStamp;
    std::deque<Vector2> tail;
    Dir dir;
    Color color;
};

std::vector<char> PackGameState(const std::vector<Snake>& snakes);
std::vector<Snake> UnpackGameState(const std::vector<char>& snakes);
