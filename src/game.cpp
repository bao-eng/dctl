#include "game.h"

#include <iostream>

// std::vector<char> PackInput(const Input &inp) {
//   flatbuffers::FlatBufferBuilder builder;
//   auto input = DCTL::CreateInput(builder, inp.sequence, mySnake_, inp.left,
//                                  inp.right, inp.up, inp.down);
//   builder.Finish(input);
//   std::vector<char> result(builder.GetBufferPointer(),
//                            builder.GetBufferPointer() + builder.GetSize());
//   return result;
// }
// Input UnpackInput(const std::vector<char> &flat) {}

Game::Game(float map_width, float map_height, float scale, float speed, double dt,
           size_t max_length)
    : map_width_(map_width),
      map_height_(map_height),
      scale_(scale),
      speed_(speed),
      dt_(dt),
      max_length_(max_length) {}

void Game::Draw() {
  ClearBackground(BLACK);
  for (auto i = 0; i < map_width_; i++) {
    for (auto j = 0; j < map_height_; j++) {
      DrawPixel((i + 1) * scale_, (j + 1) * scale_, GRAY);
    }
  }
  for (auto &s : client_buffer_.back().state.snakes) {
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
  client_buffer_.push_back({inp, NextState(client_buffer_.back().state, inp)});
}

State Game::NextState(const State &st, const Input &inp) {
  State ns;
  for (auto &i : st.snakes) {
    auto tmp = i;
    tmp.tail.back() = newPos(tmp.dir, dt_ * speed_, tmp.tail.back());
    if (i.player_id == my_snake_) {
      auto newDir = NewDir(i.dir, inp);
      if (newDir != i.dir) {
        tmp.dir = newDir;
        tmp.tail.push_back(tmp.tail.back());
        if (tmp.tail.size() > max_length_) {
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
  if (!client_buffer_.empty()) {
    while (client_buffer_.front().state.sequence < st.sequence) {
      client_buffer_.pop_front();
    }
    client_buffer_.front().state = st;
  } else {
    client_buffer_.push_back({{false, false, false, false}, st});
  }

  if (client_buffer_.size() > 1) {
    for (auto i = client_buffer_.begin() + 1; i != client_buffer_.end(); i++) {
      i->state = NextState((i - 1)->state, i->input);
    }
  }
}

Dir Game::NewDir(const Dir &cur_dir, const Input &inp) {
  if (((cur_dir == kUp) || (cur_dir == kDown)) && (inp.left != inp.right)) {
    if (inp.left) return kLeft;
    if (inp.right) return kRight;
  }
  if (((cur_dir == kLeft) || (cur_dir == kRight)) && (inp.up != inp.down)) {
    if (inp.up) return kUp;
    if (inp.down) return kDown;
  }
  return cur_dir;
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

void Game::DrawLineExRoundEnd(Vector2 start_pos, Vector2 end_pos, float thick,
                              Color color) {
  DrawLineEx(start_pos, end_pos, thick, color);
  DrawCircleV(start_pos, thick / 2, color);
  DrawCircleV(end_pos, thick / 2, color);
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

Vector2 Game::newPos(Dir dir, float dist, Vector2 cur_pos) {
  switch (dir) {
    case kUp:
      return {cur_pos.x, cur_pos.y - dist};
      break;
    case kDown:
      return {cur_pos.x, cur_pos.y + dist};
      break;
    case kLeft:
      return {cur_pos.x - dist, cur_pos.y};
      break;
    case kRight:
      return {cur_pos.x + dist, cur_pos.y};
      break;
  }
  return cur_pos;
}
