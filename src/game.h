#pragma once

#include <boost/circular_buffer.hpp>
#include <unordered_map>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_raylib.h"

struct ClientState {
  Input input;
  State state;
};

class Game {
 public:
  explicit Game(float map_width, float map_height, float scale, float speed,
                double dt, size_t max_length, const float head_diameter,
                const float tail_width);
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
  float head_diameter_;
  float tail_width_;
  std::unordered_map<int, Snake> snakes_;
  boost::circular_buffer<ClientState> client_buffer_{500};
};
