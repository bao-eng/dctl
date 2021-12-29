#pragma once

#include <chrono>
#include <deque>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_state_generated.h"
#include "raylib.h"

std::vector<char> PackGameState(uint32_t sequence, const std::vector<Snake>& snakes);
State UnpackGameState(const std::vector<char>& buf);
