#include "game.h"

Game::Game(float mapWidth, float mapHeight, float scale, float speed,
           size_t maxLength)
    : mapWidth_(mapWidth),
      mapHeight_(mapHeight),
      scale_(scale),
      speed_(speed),
      maxLength_(maxLength) {}

void Game::Draw(std::chrono::_V2::system_clock::time_point tp) {
  ClearBackground(BLACK);
  for (auto i = 0; i < mapWidth_; i++) {
    for (auto j = 0; j < mapHeight_; j++) {
      DrawPixel((i + 1) * scale_, (j + 1) * scale_, GRAY);
    }
  }

  for (auto &s : snakes_) {
    auto &curTail = s.second.tail;
    auto &curColor = s.second.color;
    size_t thick = (size_t)std::round((float)scale_ / 1.5);
    for (auto it = curTail.begin(); it != curTail.end() - 1; it++) {
      DrawLineExRoundEnd(
          {it->x * (float)scale_, it->y * (float)scale_},
          {(it + 1)->x * (float)scale_, (it + 1)->y * (float)scale_}, thick,
          curColor);
    }
    std::chrono::duration<double> dur = tp - s.second.timeStamp;
    float dist = speed_ * dur.count();
    auto newP = newPos(s.second.dir, dist, curTail.back());

    DrawLineExRoundEnd(
        {curTail.back().x * (float)scale_, curTail.back().y * (float)scale_},
        {newP.x * (float)scale_, newP.y * (float)scale_}, thick, curColor);
    DrawCircle(newP.x * (float)scale_, newP.y * (float)scale_,
               (float)scale_ / 2, curColor);
  }
}

void Game::SetSnake(Snake snake) { snakes_[snake.playerId] = snake; }

void Game::SetDir(Dir dir) {
  auto myDir = snakes_.at(mySnake_).dir;
  if (((myDir == up) || (myDir == down)) && ((dir == left) || (dir == right)) ||
      ((myDir == left) || (myDir == right)) && ((dir == up) || (dir == down))) {
    snakes_[mySnake_].dir = dir;
    snakes_[mySnake_].tail.emplace_back(snakes_[mySnake_].tail.back());
  }
  if (snakes_.at(mySnake_).tail.size() > maxLength_)
    snakes_[mySnake_].tail.pop_front();
}

const Snake &Game::Update(std::chrono::_V2::system_clock::time_point tp) {
  std::chrono::duration<double> dur = tp - snakes_.at(mySnake_).timeStamp;
  float dist = speed_ * dur.count();
  auto &s = snakes_[mySnake_];
  s.tail.back() = newPos(s.dir, dist, s.tail.back());
  s.timeStamp = tp;
  return snakes_.at(mySnake_);
}

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
