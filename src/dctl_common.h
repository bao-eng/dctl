#pragma once

#include "raylib.h"
#include <vector>
#include <deque>
#include <stdint.h>

enum Dir { kNone, kUp, kDown, kLeft, kRight };

struct Input {
  uint32_t sequence;
  int player_id;
  bool left;
  bool right;
  bool up;
  bool down;
};

struct Snake {
  int player_id;
  std::deque<Vector2> tail;
  Dir dir;
  Color color;
};

struct State{
  uint32_t sequence;
  std::vector<Snake> snakes;
};

Dir NewDir(const Dir &cur_dir, const Input &inp);

Vector2 NewPos(Dir dir, float dist, Vector2 cur_pos);
