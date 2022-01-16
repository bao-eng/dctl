#include "udp_client.h"

UDPClient::UDPClient(const std::string& addr, const int port)
    : io_service_(),
      socket_(io_service_),
      endpoint_(boost::asio::ip::make_address(addr), port) {
  socket_.connect(endpoint_);
}

void UDPClient::Send(const std::vector<char>& buf) {
  socket_.send(boost::asio::buffer(buf.data(), buf.size()));
}

size_t UDPClient::ReceiveNonBlock(std::vector<char>& buf) {
  size_t sz{0};
  if (socket_.available()) {
    boost::array<char, 64000> recv_buffer;
    sz = socket_.receive(boost::asio::buffer(recv_buffer));
    std::copy(recv_buffer.begin(), recv_buffer.begin() + sz, buf.begin());
  }
  return sz;
}

// void UDPClient::SendInputPack(const InputPack& inpp) {
//   std::copy(inpp.vec.begin(), inpp.vec.begin(),
//   std::back_inserter(inputs_.vec)); auto flat = PackInputPack(inputs_);
//   socket_.send(boost::asio::buffer(flat.data(), flat.size()));
// }

// std::optional<State> UDPClient::Receive() {
//   State st;
//   if (socket_.available()) {
//     boost::array<char, 64000> recv_buffer;
//     while (socket_.available()) {
//       auto sz = socket_.receive(boost::asio::buffer(recv_buffer));
//       std::vector<char> buf(sz);

//       std::copy(recv_buffer.begin(), recv_buffer.begin() + sz, buf.begin());
//       auto incoming = UnpackMessage(buf);
//       if(std::holds_alternative<State>(incoming)){
//       // std::cout << "received: " << sz << "ts: " << new_state.sequence <<
//       "snakes: " << new_state.snakes.size() << std::endl;
//       if(new_state.sequence > st.sequence){
//         st = new_state;
//       }
//     }
//     while (inputs_.front().sequence < st.sequence) {
//       inputs_.pop_front();
//     }
//     return st;
//   }
//   return {};
// }
