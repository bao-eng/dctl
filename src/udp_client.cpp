#include "udp_client.h"

UDPClient::UDPClient(const std::string& addr, const int port)
    : io_service_(),
      socket_(io_service_),
      endpoint_(boost::asio::ip::make_address(addr), port) {
  socket_.connect(endpoint_);
}

void UDPClient::SendInput(const std::vector<Input>& inp_vec) {
  inputs_.insert(inputs_.end(),inp_vec.begin(), inp_vec.end());
  std::vector<Input> tmp;
  std::copy(inputs_.begin(), inputs_.end(), tmp.begin());
  auto flat = PackInput(tmp);
  socket_.send(boost::asio::buffer(flat.data(), flat.size()));
}

std::optional<State> UDPClient::Receive() {
  State st;
  if (socket_.available()) {
    boost::array<char, 64000> recv_buffer_;
    while (socket_.available()) {
      auto sz = socket_.receive(boost::asio::buffer(recv_buffer_));
      std::vector<char> buf(sz);
      std::copy(recv_buffer_.begin(), recv_buffer_.begin() + sz, buf.begin());
      auto new_state = UnpackState(buf);
      if(new_state.sequence > st.sequence) st = new_state;
    }
    while (inputs_.front().sequence <= st.sequence) {
      inputs_.pop_front();
    }
    return st;
  }
  return {};
}
