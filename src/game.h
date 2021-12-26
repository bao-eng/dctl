#pragma once

#include <boost/circular_buffer.hpp>
#include <cmath>
#include <unordered_map>

#include "dctl_common.h"
#include "dctl_input_generated.h"
#include "snake.h"

struct Input {
  uint32_t sequence;
  bool left;
  bool right;
  bool up;
  bool down;
};

struct ClientState {
  Input input;
  State state;
};

std::vector<char> PackInput(const Input &inp);
Input UnpackInput(const std::vector<char> &flat);

class Game {
 public:
  Game(float mapWidth, float mapHeight, float scale, float speed, double dt,
       size_t maxLength);
  void Draw();
  void Process(const Input &inp);
  void SetState(const State &st);

 private:
  void DrawLineExRoundEnd(Vector2 startPos, Vector2 endPos, float thick,
                          Color color);
  bool isIntersecting(Vector2 &p1, Vector2 &p2, Vector2 &q1, Vector2 &q2);
  State NextState(const State &st, const Input &inp);
  Vector2 newPos(Dir dir, float dist, Vector2 curPos);
  Dir NewDir(const Dir &curDir, const Input &inp);
  float mapWidth_;
  float mapHeight_;
  float scale_;
  float speed_;
  double dt_;
  size_t maxLength_;
  size_t maxLength;
  int mySnake_{0};
  std::unordered_map<int, Snake> snakes_;
  boost::circular_buffer<ClientState> clientBuffer_{500};
};
