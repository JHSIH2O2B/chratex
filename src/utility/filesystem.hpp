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

#ifndef CHRATEX_UTILITY_FILESYSTEM_HPP
#define CHRATEX_UTILITY_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <config.hpp>

namespace chratex {
namespace filesystem {

boost::filesystem::path working_path();
boost::filesystem::path unique_path();
std::vector<boost::filesystem::path> remove_temporary_directories();
void open_or_create(std::fstream &, std::string const &);

// Reads a json object from the stream and if was changed, write the object back to the stream
template <typename T>
bool fetch_object (T & object, std::iostream & stream_a) {

	assert (
    stream_a.tellg () == std::streampos (0) || 
    stream_a.tellg () == std::streampos (-1)
  );
	assert (
    stream_a.tellp () == std::streampos (0) ||
    stream_a.tellp () == std::streampos (-1)
  );

	bool error = false;
	boost::property_tree::ptree tree;

	try {
		boost::property_tree::read_json (stream_a, tree);
	} catch (std::runtime_error const &) {
		auto pos (stream_a.tellg ());
		if (pos != std::streampos (0)) {
			error = true;
		}
	}
	if (!error) {
		auto updated (false);
		error = object.deserialize_json (updated, tree);
	}
	return error;
}
// Reads a json object from the stream and if was changed, write the object back to the stream
template <typename T>
bool fetch_object(
  T & object,
  boost::filesystem::path const & path_a,
  std::fstream &stream_a
) {
	bool error = false;
  chratex::filesystem::open_or_create(stream_a, path_a.string());
	if (!stream_a.fail()) {
		boost::property_tree::ptree tree;
		try {
			boost::property_tree::read_json(stream_a, tree);
		} catch (std::runtime_error const &) {
			auto pos = stream_a.tellg();
			if (pos != std::streampos(0)) {
				error = true;
			}
		}
		if (!error) {
			auto updated = false;
			error = object.deserialize_json(updated, tree);
			if (!error && updated) {
				stream_a.close();
				stream_a.open(
          path_a.string (), std::ios_base::out | std::ios_base::trunc
        );

				try {
					boost::property_tree::write_json(stream_a, tree);
				} catch (std::runtime_error const &) {
					error = true;
				}
			}
		}
	}
	return error;
}

}
}

#endif
