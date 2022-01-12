#include <chrono>
#include <iostream>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_raylib.h"
#include "game.h"
#include "udp_client.h"
#include "raylib.h"
#include "raygui/src/raygui.h"

using boost::asio::ip::udp;

const float kMapWidth = 107;
const float kMapHeight = 54;
const float kScale = 15;
const float kHeadDiameter = 1;
const float kTailWidth = 0.65;
const float kSpeed = 58.0f / 6;

int main(int argc, char** argv) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);

  InitWindow((kMapWidth + 1) * kScale, (kMapHeight + 1) * kScale, "dctl-client");
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

  game.SetState(st);
  t += dt;
  sequence++;
  UDPClient udp_client("127.0.0.1", 7777);

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    auto new_time = std::chrono::system_clock::now();
    auto frame_time =
        std::chrono::duration<double>(new_time - current_time).count();
    if (frame_time > 0.25) frame_time = 0.25;
    current_time = new_time;

    accumulator += frame_time;
    Input inp{sequence,
              std::stoi(argv[1]),
              IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A),
              IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D),
              IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W),
              IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)};
    std::vector<Input> inp_vec;
    while (accumulator >= dt) {
      inp.sequence = sequence;
      game.Process(inp);
      inp_vec.push_back(inp);
      t += dt;
      sequence++;
      accumulator -= dt;
    }
    udp_client.SendInput(inp_vec);
    auto new_state = udp_client.Receive();
    if(new_state.has_value()) game.SetState(new_state.value());

    BeginDrawing();

    game.DrawGame();

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
