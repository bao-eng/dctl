#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

#include "dctl-common/src/dctl_common.h"
#include "game.h"
#include "raylib.h"

using boost::asio::ip::udp;

const float kMapWidth = 107;
const float kMapHeight = 54;
const float kScale = 15;
const float kHeadDiameter = 1;
const float kTailWidth = 0.65;
const float kSpeed = 58.0f / 6;

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

  Game game(kMapWidth, kMapHeight, kScale, kSpeed, dt, 60, kHeadDiameter,
            kTailWidth);

  State st;
  st.sequence = 0;

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

  boost::asio::io_service io_service;
  udp::endpoint receiver_endpoint(boost::asio::ip::make_address("127.0.0.1"),
                                  7777);
  udp::socket socket(io_service);
  socket.connect(receiver_endpoint);

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    auto new_time = std::chrono::system_clock::now();
    auto frame_time =
        std::chrono::duration<double>(new_time - current_time).count();
    // std::cout << frame_time << std::endl;
    if (frame_time > 0.25) frame_time = 0.25;
    current_time = new_time;

    accumulator += frame_time;
    Input inp{sequence,
              0,
              IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A),
              IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D),
              IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W),
              IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)};

    while (accumulator >= dt) {
      inp.sequence = sequence;
      game.Process(inp);
      boost::array<char, 1> send_buf = {0};
      auto flat = PackInput(inp);
      // std::cout << "bytes sent: " << flat.size() << std::endl;
      socket.send(boost::asio::buffer(flat.data(), flat.size()));
      t += dt;
      sequence++;
      accumulator -= dt;
    }
    boost::array<char, 64000> recv_buffer_;
    while (socket.available()) {
      auto sz = socket.receive(boost::asio::buffer(recv_buffer_));
      std::vector<char> buf(sz);
      std::copy(recv_buffer_.begin(), recv_buffer_.begin() + sz, buf.begin());
      game.SetState(UnpackState(buf));
    }

    BeginDrawing();

    game.DrawGame();

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
