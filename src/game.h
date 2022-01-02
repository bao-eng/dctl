#pragma once

#include <boost/circular_buffer.hpp>
#include <cmath>
#include <unordered_map>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_input_generated.h"
#include "snake.h"

struct ClientState {
  Input input;
  State state;
};

std::vector<char> PackInput(const Input &inp);
Input UnpackInput(const std::vector<char> &flat);

class Game {
 public:
  Game(float map_width, float map_height, float scale, float speed, double dt,
       size_t max_length);
  void DrawGame();
  void Process(const Input &inp);
  void SetState(const State &st);

 private:

  float map_width_;
  float map_height_;
  float scale_;
  float speed_;
  double dt_;
  size_t max_length_;
  std::unordered_map<int, Snake> snakes_;
  boost::circular_buffer<ClientState> client_buffer_{500};
};
