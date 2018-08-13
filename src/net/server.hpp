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

#ifndef CHRATEX_NET_SERVER_HPP
#define CHRATEX_NET_SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <functional>

namespace chratex {
namespace net {

class session;

class server {

  public:

    typedef std::function<void(std::shared_ptr<session>)> connect_callback;

    server(unsigned short port);

    void on_connect(connect_callback);

    void start();

    unsigned short get_port() const;

  private:

    const unsigned short port;

    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;

    boost::asio::io_service ios;

    std::vector<connect_callback> callbacks;

    void do_accept();
};
}
}

#endif
