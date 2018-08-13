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

#pragma once

#ifndef CHRATEX_NET_SESSION_HPP
#define CHRATEX_NET_SESSION_HPP

#include <boost/asio.hpp>
#include <functional>
#include <memory>

namespace chratex {
namespace net {

class session : public std::enable_shared_from_this<session> {

  typedef std::function<void(
    std::shared_ptr<session>, std::string
  )> receive_callback;

  typedef std::function<void(std::shared_ptr<session>)> close_callback;

public:
  session(boost::asio::ip::tcp::socket socket);

  void start();

  void write(const std::string &str);

  void write(
    const std::string &str, 
    std::function<void(boost::system::error_code,size_t)>
  );

  void on_receive(receive_callback callback);

  void on_close(close_callback callback);

  boost::asio::ip::tcp::socket &get_socket();

private:

  boost::asio::ip::tcp::socket socket;

  enum { max_length = 1024 };

  char data[max_length];

  std::vector<receive_callback> on_receives;

  std::vector<close_callback> on_closes;

  void do_read();
};
} // namespace net
} // namespace chratex

#endif
