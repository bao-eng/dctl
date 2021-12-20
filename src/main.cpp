#include <cmath>
#include <cstddef>
#include <deque>
#include <list>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "game.h"
#include <chrono>
#include "dctl_generated.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>

const size_t MAPWIDTH = 107;
const size_t MAPHEIGHT = 54;
const size_t SCALE = 15;
const size_t SNAKES = 10;
const float SPEED = (float)58 / 6;

// class Snake {
// public:
//   Snake(Vector2 pos, Dir dir, Color color, size_t maxLength) : dir_(dir), color_(color), maxLength_(maxLength) {
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
//     if (((dir_ == up) || (dir_ == down)) && ((dir == left) || (dir == right)) ||
//         ((dir_ == left) || (dir_ == right)) && ((dir == up) || (dir == down))) {
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

int main()
{

  bool allowMove = false;

  SetConfigFlags(FLAG_MSAA_4X_HINT);

  InitWindow((MAPWIDTH + 1) * SCALE, (MAPHEIGHT + 1) * SCALE, "snake");
  // InitWindow(1920, 1080, "snake");
  // ToggleFullscreen();

  SetTargetFPS(100);

  Game game(MAPWIDTH, MAPHEIGHT, SCALE, SPEED, 60);

  auto tp{std::chrono::_V2::system_clock::now()};
  Vector2 s1{3, (float)MAPHEIGHT / 2};
  game.SetSnake(0, {0, tp, {s1, s1}, Dir::right, (Color){0, 255, 255, 255}}); // CYAN
  Vector2 s2{MAPWIDTH - 3, (float)MAPHEIGHT / 2};
  game.SetSnake(1, {1, tp, {s2, s2}, Dir::left, (Color){255, 0, 255, 255}}); // MAGENTA
  Vector2 s3{(float)MAPWIDTH / 2, 3};
  game.SetSnake(2, {2, tp, {s3, s3}, Dir::down, (Color){0, 255, 0, 255}}); // GREEN
  Vector2 s4{(float)MAPWIDTH / 2, MAPHEIGHT - 3};
  game.SetSnake(3, {3, tp, {s4, s4}, Dir::up, (Color){255, 255, 0, 255}}); // YELLOW

  // std::list<Snake> snakes;

  // snakes.emplace_back((Vector2){3, (float)MAPHEIGHT / 2}, right,
  //                     (Color){0, 255, 255, 255}); // CYAN
  // snakes.emplace_back((Vector2){MAPWIDTH - 3, (float)MAPHEIGHT / 2}, left,
  //                     (Color){255, 0, 255, 255}); // MAGENTA
  // snakes.emplace_back((Vector2){(float)MAPWIDTH / 2, 3}, down,
  //                     (Color){0, 255, 0, 255}); // GREEN
  // snakes.emplace_back((Vector2){(float)MAPWIDTH / 2, MAPHEIGHT - 3}, up,
  //                     (Color){255, 255, 0, 255},60); // YELLOW

  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    Dir dir{none};

    if (IsKeyPressed(KEY_RIGHT) && allowMove)
    {
      dir = right;
      allowMove = false;
    }
    if (IsKeyPressed(KEY_LEFT) && allowMove)
    {
      dir = left;
      allowMove = false;
    }
    if (IsKeyPressed(KEY_UP) && allowMove)
    {
      dir = up;
      allowMove = false;
    }
    if (IsKeyPressed(KEY_DOWN) && allowMove)
    {
      dir = down;
      allowMove = false;
    }
    allowMove = true;
    game.SetDir(dir);

    auto mySnake = game.Update(std::chrono::_V2::system_clock::now());
    
    auto res = PackGameState({mySnake,mySnake,mySnake});
    auto unpacked = UnpackGameState(res);

    BeginDrawing();

    game.Draw(std::chrono::_V2::system_clock::now());

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
