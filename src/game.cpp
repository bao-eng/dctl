#include "game.h"

#include <iostream>

Game::Game(float map_width, float map_height, float scale, float speed,
           double dt, size_t max_length)
    : map_width_(map_width),
      map_height_(map_height),
      scale_(scale),
      speed_(speed),
      dt_(dt),
      max_length_(max_length) {}

void Game::DrawGame() {
  Draw(client_buffer_.back().state, map_width_, map_height_, scale_);
}

void Game::Process(const Input &inp) {
  client_buffer_.push_back(
      {inp, NextState(client_buffer_.back().state, {{inp.player_id, inp}}, dt_,
                      speed_, max_length_)});
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
      i->state = NextState((i - 1)->state, {{i->input.player_id, i->input}},
                           dt_, speed_, max_length_);
    }
  }
}
