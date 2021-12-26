#pragma once

#include <chrono>
#include <deque>

#include "dctl_common.h"
#include "dctl_state_generated.h"
#include "raylib.h"

struct Snake {
  int playerId;
  std::deque<Vector2> tail;
  Dir dir;
  Color color;
};

struct State{
  uint32_t sequence;
  std::vector<Snake> snakes;
};

std::vector<char> PackGameState(uint32_t sequence, const std::vector<Snake>& snakes);
State UnpackGameState(const std::vector<char>& buf);
