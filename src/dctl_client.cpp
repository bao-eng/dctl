#include <chrono>
#include <iostream>

#include "dctl-common/src/dctl_common.h"
#include "dctl-common/src/dctl_raylib.h"
#include "game.h"
#include "raygui/src/raygui.h"
#include "raylib.h"
#include "udp_client.h"

using boost::asio::ip::udp;

int main(int argc, char** argv) {
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(1000, 500, "dctl-client");
  // InitWindow(1920, 1080, "snake");
  // ToggleFullscreen();

  SetTargetFPS(60);

  UDPClient client("127.0.0.1", 7777);
  Request req{"test"};
  client.Send(PackRequest(req));

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // client.Update();

    BeginDrawing();

    // client.Draw();

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

//   uint32_t sequence{0};
//   double t = 0.0;
//   double dt = 0.01;
//   auto current_time = std::chrono::system_clock::now();
//   double accumulator = 0.0;

//   ClientEngine game(kMapWidth, kMapHeight, kScale, kSpeed, dt, 60,
//   kHeadDiameter,
//             kTailWidth);

//   State st;
//   st.sequence = 0;

//   game.SetState(st);
//   t += dt;
//   sequence++;
//   UDPClient udp_client("127.0.0.1", 7777);

//   while (!WindowShouldClose())  // Detect window close button or ESC key
//   {
//     auto new_time = std::chrono::system_clock::now();
//     auto frame_time =
//         std::chrono::duration<double>(new_time - current_time).count();
//     if (frame_time > 0.25) frame_time = 0.25;
//     current_time = new_time;

//     accumulator += frame_time;
//     Input inp{sequence,
//               local_player_id,
//               IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A),
//               IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D),
//               IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W),
//               IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)};
//     std::vector<Input> inp_vec;
//     while (accumulator >= dt) {
//       inp.sequence = sequence;
//       game.Process(inp);
//       // std::cout << inp.player_id << std::endl;
//       inp_vec.push_back(inp);
//       t += dt;
//       sequence++;
//       accumulator -= dt;
//     }
//     udp_client.SendInput(inp_vec);
//     auto new_state = udp_client.Receive();
//     if(new_state.has_value()){
//       game.SetState(new_state.value());
//       if(new_state.value().snakes.size()==0){
//         std::cout << "ts: " << new_state.value().sequence << " " <<
//         new_state.value().snakes.size() << " !" << std::endl;
//       }
//     }
//     else{
//       std::cout << "no new state recieved" << std::endl;
//     }

//     BeginDrawing();

//     game.DrawGame();

//     DrawFPS(10, 10);
//     EndDrawing();
//   }

//   CloseWindow();

//   return 0;
// }
