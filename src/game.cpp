#include "game.h"

#include <iostream>

ClientEngine::ClientEngine(const Settings &settings)
    : settings_(settings), scale_(1.5) {}

void ClientEngine::DrawGame() {
  Draw(client_buffer_.back().state, settings_.map_width, settings_.map_height,
       scale_, settings_.head_diameter, settings_.tail_width);
}

void ClientEngine::Process(const Input &inp) {
  client_buffer_.push_back(
      {inp, NextState(client_buffer_.back().state, {{inp.player_id, inp}},
                      settings_.dt, settings_.speed, settings_.max_length)});
}

void ClientEngine::SetState(const State &st) {
  if (!client_buffer_.empty()) {
    if (client_buffer_.front().state.sequence <= st.sequence) {
      while (client_buffer_.front().state.sequence < st.sequence) {
        client_buffer_.pop_front();
      }
      if (client_buffer_.front().state.sequence == st.sequence) {
        client_buffer_.front().state = st;
      }
    }
  } else {
    client_buffer_.push_back({{}, st});
  }

  if (client_buffer_.size() > 1) {
    for (auto i = client_buffer_.begin() + 1; i != client_buffer_.end(); i++) {
      i->state = NextState((i - 1)->state, {{i->input.player_id, i->input}},
                           settings_.dt, settings_.speed, settings_.max_length);
    }
  }
}
