#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

#include "threadPool.hpp"
#include "server.hpp"
using namespace boost::asio;


int main() {
  io_context service;
  Server server(service, 12345);
  server.async_connect();
  service.run();
}