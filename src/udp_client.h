#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <deque>
#include "dctl-common/src/dctl_common.h"

class UDPClient {
 public:
  explicit UDPClient(const std::string& addr, const int port);

  void SendInput(const std::vector<Input>& inp_vec);
  std::optional<State> Receive();
 private:
  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint endpoint_;
  boost::asio::ip::udp::endpoint receiver_endpoint_;
  boost::array<char, 1024> recv_buffer_;
  std::deque<Input> inputs_;
};
