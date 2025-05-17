#pragma once
#include <iostream>
#include "matrix.hpp"

using namespace boost::asio;

enum class Commands {
  multiply,
  result,
  status,
  none
};

Commands cnvrt(std::string& s) {
  if (s == "MULTIPLY") return Commands::multiply;
  if (s == "RESULT") return Commands::result;
  if (s == "STATUS") return Commands::status;
  return Commands::none;
}

class Session : public std::enable_shared_from_this<Session> {
public:
  explicit Session(ip::tcp::socket&& socket,
                   TaskQueue<std::function<void()>>& q) : 
    socket_(std::move(socket)),
    q_(q)
    {};
  ~Session() = default;

  void start() {
    auto self = shared_from_this();
    write_to_client("HELLO!!!", [&](boost::system::error_code err,
      std::size_t transferred) {
        if (err) {
          //err handler
        }
      });

    async_read_until(
      socket_,
      buff_,
      '\n',
      [&](boost::system::error_code err,
          std::size_t transferred) {
        if (!err) {
          std::istream is(&buff_);
          std::string command;
          is >> command;

          switch (cnvrt(command))
          {
          case Commands::multiply:
            mulMatrixes();
            break;
          case Commands::result:
            getResult();
            break;
          case Commands::status:
            getStatus();
            break;
          default:
            write_to_client("WRONG COMMAND", [&](boost::system::error_code err,
              std::size_t transferred) {
                if (err) {
                  //err handler
                }
            });
          }
        }
        else {
          //err handler
        }
    });

  };

private:
  void mulMatrixes() {
    
  };

  void getResult() {
    write_to_client("NOT AVAILABLE YET", [&](boost::system::error_code err,
      std::size_t transferred) {
        if (err) {
          //err handler
        }
      });
  };

  void getStatus() {
    write_to_client("NOT AVAILABLE YET", [&](boost::system::error_code err,
      std::size_t transferred) {
        if (err) {
          //err handler
        }
      });
  };

  void write_to_client(const std::string& message,
                       std::function<void(boost::system::error_code, std::size_t)>&& callback) {
    std::ostream os(&buff_);
    buff_.consume(buff_.size());
    os << message;
    async_write(socket_, buff_, std::move(callback));
  }

  void calcMatrixesValues(const Matrix<int>& a, const Matrix<int>& b, int row, int col, Matrix<int>& result) {
    if (a.getWidth() != b.getHeight()) return;
    int k = a.getWidth();
    for (int i = 0; i < k; ++i) {
      result[row][col] += a[row][i] * b[i][col];
    }
  }

  TaskQueue<std::function<void()>>& q_;
  ip::tcp::socket socket_;
  streambuf buff_;
};


class Server {
public:

  Server(io_context& service, std::uint16_t port) :
    service_(service),
    acceptor_(service, ip::tcp::endpoint(ip::tcp::v4(), port)),
    pool_ (6, q_) {};
  ~Server() = default;

  void async_connect() {

    auto socket = std::make_shared<ip::tcp::socket>(service_);
    acceptor_.async_accept(*socket, [&](boost::system::error_code err) {
      if (!err) {
        auto session = std::make_shared<Session>(std::move(*socket), q_);
        session->start();
      }
      async_connect();
      });

  };
private:

  io_context& service_;
  ip::tcp::acceptor acceptor_;
  TaskQueue<std::function<void()>> q_;
  ThreadPool<std::function<void()>> pool_;
};