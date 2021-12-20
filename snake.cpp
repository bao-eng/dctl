#include "snake.h"

std::vector<char> PackGameState(const std::vector<Snake> &snakes)
{
    flatbuffers::FlatBufferBuilder builder;
    std::vector<flatbuffers::Offset<DCTL::Snake>> snakes_vector;

    for (auto &s : snakes)
    {
        auto dur = s.timeStamp.time_since_epoch();
        int64_t tmstmp = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

        std::vector<DCTL::Vec2> tail_vector;
        for (auto &i : s.tail)
        {
            DCTL::Vec2 point{i.x, i.y};
            tail_vector.emplace_back(point);
        }
        auto tail = builder.CreateVectorOfStructs(tail_vector);

        DCTL::Dir dir;
        switch (s.dir)
        {
        case up:
            dir = DCTL::Dir_Up;
            break;
        case down:
            dir = DCTL::Dir_Down;
            break;
        case left:
            dir = DCTL::Dir_Left;
            break;
        case right:
            dir = DCTL::Dir_Right;
            break;
        default:
            break;
        }

        DCTL::Color colorF{s.color.r, s.color.g, s.color.b};

        auto snakeBuf = DCTL::CreateSnake(builder, s.playerId, tmstmp, tail, dir, &colorF);
        snakes_vector.push_back(snakeBuf);
    }

    auto snakesBuf = builder.CreateVector(snakes_vector);

    auto gameState = DCTL::CreateGameState(builder, snakesBuf);

    builder.Finish(gameState);

    std::vector<char> result(builder.GetSize());
    std::vector<char> serialized(builder.GetBufferPointer(),
                                 builder.GetBufferPointer() + builder.GetSize());

    memcpy(result.data(), builder.GetBufferPointer(), result.size());
    return result;
};

std::vector<Snake> UnpackGameState(const std::vector<char> &buf)
{
    std::vector<Snake> result;
    auto game_state = DCTL::GetGameState(buf.data());
    auto snakesVec = game_state->snakes();

    int64_t t;
    for (unsigned int i = 0; i < snakesVec->size(); i++)
    {
        Snake tmp;
        std::chrono::milliseconds dur(snakesVec->Get(i)->time_stamp());
        std::chrono::time_point<std::chrono::system_clock> tp(dur);
        tmp.timeStamp = tp;
        tmp.playerId = snakesVec->Get(i)->player_id();
        auto color = snakesVec->Get(i)->color();
        tmp.color = {color->r(),color->g(),color->b(),255};
        Dir dir;
        switch (snakesVec->Get(i)->dir())
        {
        case DCTL::Dir_Up:
            dir = up;
            break;
        case DCTL::Dir_Down:
            dir = down;
            break;
        case DCTL::Dir_Left:
            dir = left;
            break;
        case DCTL::Dir_Right:
            dir = right;
            break;
        default:
            break;
        }
        tmp.dir = dir;
        for (unsigned int j = 0; j < snakesVec->Get(i)->tail()->size(); j++)
        {
            tmp.tail.push_back({snakesVec->Get(i)->tail()->Get(j)->x(),snakesVec->Get(i)->tail()->Get(j)->y()});
        }
    result.push_back(tmp);
    }
    return result;
};
