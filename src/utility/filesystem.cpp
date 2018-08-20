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
 
#include <utility/filesystem.hpp>
#include <utility/working_dir.hpp>
#include <boost/iostreams/stream.hpp>
#include <iostream>

static std::vector<boost::filesystem::path> all_unique_paths; 

namespace chratex {
namespace filesystem {

boost::filesystem::path working_path() {
  
  auto result = app_path();

  switch (chratex::network::current) {
    case chratex::networks::test_network:
#ifdef __APPLE__
      result /= "ChratexTest";
#else
      result /= "chratextest";
#endif
      break;
    case chratex::networks::beta_network:
#ifdef __APPLE__
      result /= "ChratexBeta";
#else
      result /= "chratexbeta";
#endif
      break;
    case chratex::networks::live_network:
#ifdef __APPLE__
      result /= "Chratex";
#else
      result /= "chratex";
#endif
      break;
    default:
      break;
  }

  return result;
}

boost::filesystem::path unique_path() {
  auto result = working_path() / boost::filesystem::unique_path();
  all_unique_paths.push_back(result);
  return result;
}

std::vector<boost::filesystem::path> remove_temporary_directories() {
  for (auto &path : all_unique_paths) {
    boost::system::error_code ec;
    boost::filesystem::remove_all(path, ec);

    if (ec) {
      std::cerr << "Could not remove temporary directory: "
                << ec.message() 
                << std::endl;
    }

    auto lockfile = path;
    lockfile += "-lock";
    boost::filesystem::remove(lockfile, ec);

    if (ec) { 
      std::cerr << "Could not remove temporary lccok file: "
                << ec.message()
                << std::endl;
    }
  }

  return all_unique_paths;
}

}
}
