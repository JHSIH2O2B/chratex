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

#include <net/server.hpp>
#include <net/session.hpp>

#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::tcp;

chratex::net::server::server(unsigned short port) : port(port) {
  
  acceptor = std::make_shared<tcp::acceptor>(
    ios, tcp::endpoint(tcp::v4(), port)
  );

  do_accept();
}

void chratex::net::server::on_connect(connect_callback callback) {
  callbacks.push_back(callback);
}

void chratex::net::server::start() {
  ios.run();
}

void chratex::net::server::do_accept() {
  acceptor->async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        auto session = std::make_shared<chratex::net::session>(std::move(socket));
        for (auto &it : this->callbacks) {
          it(session);
        }
        session->start();
        }

      this->do_accept();
    }
  );
}

unsigned short chratex::net::server::get_port() const {
  return port;
}

