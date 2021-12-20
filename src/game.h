#pragma once

#include <unordered_map>
#include <cmath>

#include "dctl_common.h"
#include "snake.h"

class Game
{
public:
    Game(float mapWidth, float mapHeight, float scale, float speed, size_t maxLength);
    void SetSnake(int id, Snake snake);
    void Draw(std::chrono::_V2::system_clock::time_point tp);
    void SetDir(Dir dir);
    const Snake& Update(std::chrono::_V2::system_clock::time_point tp);

private:
    void DrawLineExRoundEnd(Vector2 startPos, Vector2 endPos, float thick,
                            Color color);
    bool isIntersecting(Vector2 &p1, Vector2 &p2, Vector2 &q1, Vector2 &q2);
    Vector2 newPos(Dir dir, float dist, Vector2 curPos);
    float mapWidth_;
    float mapHeight_;
    float scale_;
    float speed_;
    size_t maxLength_;
    size_t maxLength;
    int mySnake_{0};
    std::unordered_map<int, Snake> snakes_;
};
