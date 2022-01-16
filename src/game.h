#pragma once

#include <boost/circular_buffer.hpp>
#include <unordered_map>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_raylib.h"

struct ClientState {
  Input input;
  State state;
};

class ClientEngine {
 public:
  explicit ClientEngine(const Settings &settings);
  void DrawGame();
  void Process(const Input &inp);
  void SetState(const State &st);

 private:
  Settings settings_;
  float scale_;
  std::unordered_map<int, Snake> snakes_;
  boost::circular_buffer<ClientState> client_buffer_{500};
};
