#include "game.h"

#include <iostream>

// std::vector<char> PackInput(const Input &i) {
//   flatbuffers::FlatBufferBuilder builder;
//   auto input = DCTL::CreateInput(builder, i.sequence, i.player_id, i.left,
//                                  i.right, i.up, i.down);
//   builder.Finish(input);
//   std::vector<char> result(builder.GetBufferPointer(),
//                            builder.GetBufferPointer() + builder.GetSize());
//   return result;
// }
// Input UnpackInput(const std::vector<char> &flat) {}

Game::Game(float mapWidth, float mapHeight, float scale, float speed, double dt,
           size_t maxLength)
    : mapWidth_(mapWidth),
      mapHeight_(mapHeight),
      scale_(scale),
      speed_(speed),
      dt_(dt),
      maxLength_(maxLength) {}

void Game::Draw() {
  ClearBackground(BLACK);
  for (auto i = 0; i < mapWidth_; i++) {
    for (auto j = 0; j < mapHeight_; j++) {
      DrawPixel((i + 1) * scale_, (j + 1) * scale_, GRAY);
    }
  }
  for (auto &s : clientBuffer_.back().state.snakes) {
    auto &curTail = s.tail;
    auto &curColor = s.color;
    size_t thick = (size_t)std::round((float)scale_ / 1.5);
    for (auto it = curTail.begin(); it != curTail.end() - 1; it++) {
      DrawLineExRoundEnd(
          {it->x * (float)scale_, it->y * (float)scale_},
          {(it + 1)->x * (float)scale_, (it + 1)->y * (float)scale_}, thick,
          curColor);
    }
    DrawCircle(curTail.back().x * (float)scale_,
               curTail.back().y * (float)scale_, (float)scale_ / 2, curColor);
  }
}

void Game::Process(const Input &inp) {
  clientBuffer_.push_back({inp, NextState(clientBuffer_.back().state, inp)});
}

State Game::NextState(const State &st, const Input &inp) {
  State ns;
  for (auto &i : st.snakes) {
    auto tmp = i;
    tmp.tail.back() = newPos(tmp.dir, dt_ * speed_, tmp.tail.back());
    if (i.playerId == mySnake_) {
      auto newDir = NewDir(i.dir, inp);
      if (newDir != i.dir) {
        tmp.dir = newDir;
        tmp.tail.push_back(tmp.tail.back());
        if (tmp.tail.size() > maxLength_) {
          tmp.tail.pop_front();
        }
      }
    }
    ns.snakes.push_back(tmp);
  }
  ns.sequence = inp.sequence;  // st.sequence + 1;
  return ns;
}

void Game::SetState(const State &st) {
  if (!clientBuffer_.empty()) {
    while (clientBuffer_.front().state.sequence < st.sequence) {
      clientBuffer_.pop_front();
    }
    clientBuffer_.front().state = st;
  } else {
    clientBuffer_.push_back({{false, false, false, false}, st});
  }

  if (clientBuffer_.size() > 1) {
    for (auto i = clientBuffer_.begin() + 1; i != clientBuffer_.end(); i++) {
      i->state = NextState((i - 1)->state, i->input);
    }
  }
}

Dir Game::NewDir(const Dir &curDir, const Input &inp) {
  if (((curDir == up) || (curDir == down)) && (inp.left != inp.right)) {
    if (inp.left) return left;
    if (inp.right) return right;
  }
  if (((curDir == left) || (curDir == right)) && (inp.up != inp.down)) {
    if (inp.up) return up;
    if (inp.down) return down;
  }
  return curDir;
}

// const Snake &Game::SetDir(Dir dir,
//                           std::chrono::_V2::system_clock::time_point tp) {
//   auto myDir = snakes_.at(mySnake_).dir;
//   if (((myDir == up) || (myDir == down)) && ((dir == left) || (dir == right))
//   ||
//       ((myDir == left) || (myDir == right)) && ((dir == up) || (dir ==
//       down))) {
//     auto &s = snakes_[mySnake_];
//     std::chrono::duration<double> dur = tp - snakes_.at(mySnake_).timeStamp;
//     float dist = speed_ * dur.count();
//     s.tail.back() = newPos(s.dir, dist, s.tail.back());
//     s.dir = dir;
//     s.timeStamp = tp;
//     // snakes_[mySnake_].tail.emplace_back(snakes_[mySnake_].tail.back());
//   }
//   if (snakes_.at(mySnake_).tail.size() > maxLength_) {
//     snakes_[mySnake_].tail.pop_front();
//   }
//   return snakes_.at(mySnake_);
// }

// const Snake &Game::Update(std::chrono::_V2::system_clock::time_point tp) {
//   std::chrono::duration<double> dur = tp - snakes_.at(mySnake_).timeStamp;
//   float dist = speed_ * dur.count();
//   auto &s = snakes_[mySnake_];
//   s.tail.back() = newPos(s.dir, dist, s.tail.back());
//   s.timeStamp = tp;
//   return snakes_.at(mySnake_);
// }

void Game::DrawLineExRoundEnd(Vector2 startPos, Vector2 endPos, float thick,
                              Color color) {
  DrawLineEx(startPos, endPos, thick, color);
  DrawCircleV(startPos, thick / 2, color);
  DrawCircleV(endPos, thick / 2, color);
}

// https://stackoverflow.com/a/14177062
bool Game::isIntersecting(Vector2 &p1, Vector2 &p2, Vector2 &q1, Vector2 &q2) {
  return (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x)) *
              ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) <
          0) &&
         (((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x)) *
              ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) <
          0);
}

Vector2 Game::newPos(Dir dir, float dist, Vector2 curPos) {
  switch (dir) {
    case up:
      return {curPos.x, curPos.y - dist};
      break;
    case down:
      return {curPos.x, curPos.y + dist};
      break;
    case left:
      return {curPos.x - dist, curPos.y};
      break;
    case right:
      return {curPos.x + dist, curPos.y};
      break;
  }
  return curPos;
}
