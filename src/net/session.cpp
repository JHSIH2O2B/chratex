/**
 * Copyright (c) 2018 chratex developers (see AUTHORS)
 *
 * This file is part of chratex
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <net/session.hpp>

#include <boost/bind.hpp>

#include <iostream>
#include <memory>

namespace chratex {
namespace net {

session::session(
  boost::asio::ip::tcp::socket socket
) : socket(std::move(socket)) {
}

boost::asio::ip::tcp::socket &session::get_socket() {
  return socket;
}

void session::start() {
  do_read();
}

void session::write(const std::string &str) {
  write(str, [](boost::system::error_code, size_t) {});
}

void session::write(
  const std::string &str,
  std::function<void(boost::system::error_code,size_t)> callback
) {
  auto buffer = boost::asio::buffer(str.c_str(), str.size());
  boost::asio::async_write(socket, buffer, callback);
}

void session::do_read() {
  auto self(shared_from_this());
  socket.async_read_some(
    boost::asio::buffer(data, max_length),
    [this, self](boost::system::error_code err, size_t length) {
      if (!err) {

        auto str = std::string(data);

        for (auto &it : on_receives) {
          it(shared_from_this(), str);
        }

        do_read();

      } else {
        std::cerr << "err (recv): " << err.message() << std::endl;
      }
    });
}

void session::on_receive(receive_callback callback) {
  on_receives.push_back(callback);
}

void session::on_close(close_callback callback) {
  on_closes.push_back(callback);
}

} // namespace net
} // namespace chratex
