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

#include <rpc/server.hpp>

namespace chratex {

rpc::server::server() {
  net_server = std::make_shared<net::server>(9555);
}

void rpc::server::start() {
  net_server->on_connect(
    std::bind(&rpc::server::on_connect,
    this,
    std::placeholders::_1)
  );

  net_server->start();
}

void rpc::server::stop() {

}

void rpc::server::on_connect(std::shared_ptr<net::session> session) {
  session->on_receive(
    std::bind(
      &rpc::server::on_message,
      this,
      std::placeholders::_1,
      std::placeholders::_2
    )
  );

  session->write("Connected to Chratex.\n");
}

void rpc::server::on_message(
  std::shared_ptr<net::session> session,
  const std::string &message
) {

  session->write("chratex echo: " + message + "\n");
}

}
