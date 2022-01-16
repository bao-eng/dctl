#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <deque>
#include <iostream>

#include "dctl-common/src/dctl_common.h"

class UDPClient {
 public:
  explicit UDPClient(const std::string& addr, const int port);

  void Send(const std::vector<char>& buf);
  size_t ReceiveNonBlock(std::vector<char>& buf);

 private:
  boost::asio::io_service io_service_;
  boost::asio::ip::udp::socket socket_;
  boost::asio::ip::udp::endpoint endpoint_;
  InputPack inputs_;
};
