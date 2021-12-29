#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <vector>

#include "dctl_input_generated.h"
#include "dctl_state_generated.h"
#include "game.h"
#include "raylib.h"
#include "raymath.h"

using boost::asio::ip::udp;

const size_t kMapWidth = 107;
const size_t kMapHeight = 54;
const size_t kScale = 15;
const float kSpeed = (float)58 / 6;

// class Snake {
// public:
//   Snake(Vector2 pos, Dir dir, Color color, size_t maxLength) : dir_(dir),
//   color_(color), maxLength_(maxLength) {
//     tail_.emplace_back(pos);
//     tail_.emplace_back(pos);
//   }
//   Vector2 move(float dist) {
//     switch (dir_) {
//     case up:
//       tail_.back() = {tail_.back().x, tail_.back().y - dist};
//       break;
//     case down:
//       tail_.back() = {tail_.back().x, tail_.back().y + dist};
//       break;
//     case left:
//       tail_.back() = {tail_.back().x - dist, tail_.back().y};
//       break;
//     case right:
//       tail_.back() = {tail_.back().x + dist, tail_.back().y};
//       break;
//     }
//     return tail_.back();
//   }
//   const std::deque<Vector2> &get_tail() const { return tail_; }
//   Color get_color() const { return color_; }

//   void set_dir(Dir dir) {
//     if (((dir_ == up) || (dir_ == down)) && ((dir == left) || (dir == right))
//     ||
//         ((dir_ == left) || (dir_ == right)) && ((dir == up) || (dir ==
//         down))) {
//       dir_ = dir;
//       tail_.emplace_back(tail_.back());
//     }
//     if (tail_.size() > maxLength_)
//       tail_.pop_front();
//   }

//   bool SelfIntersect() {
//     if (tail_.size() > 4) {
//       auto head = tail_.rbegin();
//       auto neck = tail_.rbegin() + 1;
//       for (auto it = tail_.rbegin() + 2; it != tail_.rend(); it++) {
//         if (isIntersecting(*head, *neck, *it, *(it - 1))) {
//           return true;
//         }
//       }
//     }
//     return false;
//   }

// private:
//   std::deque<Vector2> tail_;
//   Dir dir_;
//   Color color_;
//   size_t maxLength_;
// };

int main() {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  InitWindow((kMapWidth + 1) * kScale, (kMapHeight + 1) * kScale, "snake");
  // InitWindow(1920, 1080, "snake");
  // ToggleFullscreen();

  SetTargetFPS(60);

  uint32_t sequence{0};
  double t = 0.0;
  double dt = 0.01;
  auto current_time = std::chrono::system_clock::now();
  double accumulator = 0.0;

  Game game(kMapWidth, kMapHeight, kScale, kSpeed, dt, 60);

  State st;
  st.sequence = sequence;

  Vector2 p1{3, (float)kMapHeight / 2};
  Snake s1{0, {p1, p1}, Dir::kRight, (Color){0, 255, 255, 255}};  // CYAN
  Vector2 p2{kMapWidth - 3, (float)kMapHeight / 2};
  Snake s2{1, {p2, p2}, Dir::kLeft, (Color){255, 0, 255, 255}};  // MAGENTA
  Vector2 p3{(float)kMapWidth / 2, 3};
  Snake s3{2, {p3, p3}, Dir::kDown, (Color){0, 255, 0, 255}};  // GREEN
  Vector2 p4{(float)kMapWidth / 2, kMapHeight - 3};
  Snake s4{3, {p4, p4}, Dir::kUp, (Color){255, 255, 0, 255}};  // YELLOW

  st.snakes.push_back(s1);
  st.snakes.push_back(s2);
  st.snakes.push_back(s3);
  st.snakes.push_back(s4);

  game.SetState(st);
  t += dt;
  sequence++;

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    auto new_time = std::chrono::system_clock::now();
    auto frame_time =
        std::chrono::duration<double>(new_time - current_time).count();
    std::cout << frame_time << std::endl;
    if (frame_time > 0.25) frame_time = 0.25;
    current_time = new_time;

    accumulator += frame_time;
    Input inp{sequence,
              0,
              IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A),
              IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D),
              IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W),
              IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)};
    // Input inp{sequence,
    //           0,
    //           IsKeyDown(KEY_LEFT),
    //           IsKeyDown(KEY_RIGHT),
    //           IsKeyDown(KEY_UP),
    //           IsKeyDown(KEY_DOWN)};
    while (accumulator >= dt) {
      inp.sequence = sequence;
      game.Process(inp);
      t += dt;
      sequence++;
      accumulator -= dt;
    }
    // auto mySnake = game.Update(std::chrono::_V2::system_clock::now());

    // auto res = PackGameState({mySnake, mySnake, mySnake});
    // auto unpacked = UnpackGameState(res);

    // boost::asio::io_service io_service;
    // udp::resolver resolver(io_service);
    // udp::resolver::query query(udp::v4(), "localhost", "daytime");
    // udp::endpoint receiver_endpoint = *resolver.resolve(query);

    // udp::socket socket(io_service);
    // socket.open(udp::v4());

    // boost::array<char, 1> send_buf = {0};
    // socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

    // boost::array<char, 128> recv_buf;
    // udp::endpoint sender_endpoint;
    // size_t len =
    //     socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
    // async_receive_from (1 of 2 overloads)

    // std::cout.write(recv_buf.data(), len);

    BeginDrawing();

    game.Draw();

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

// auto dist = GetFrameTime() * SPEED;
// auto it = snakes.begin();

// while (it != snakes.end()) {
//   it->set_dir(dir);
//   auto pos = it->move(dist);
//   if (it->SelfIntersect()) {
//     it = snakes.erase(it);
//     break;
//   }
//   auto tail = it->get_tail();
//   if (pos.x < 0 || pos.y < 0 || pos.x > MAPWIDTH || pos.y > MAPHEIGHT) {
//     it = snakes.erase(it);
//   } else {
//     ++it;
//   }
// }
