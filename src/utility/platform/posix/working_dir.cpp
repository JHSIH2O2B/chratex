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

#include <utility/working_dir.hpp>
#include <pwd.h>
#include <sys/types.h>

namespace chratex {
namespace filesystem {

boost::filesystem::path app_path() {
  auto entry(getpwuid(getuid()));
  assert (entry != nullptr);
  boost::filesystem::path result(entry->pw_dir);
  return result;
}

}
}
