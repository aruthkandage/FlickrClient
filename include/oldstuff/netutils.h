/**
 *  Qt Flickr Client
 *  Copyright (C) 2014 Aruth Kandage
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*/ 

#ifndef __NETUTILS_H__
#define __NETUTILS_H__

#include <string>

namespace netutils {

/* 
 * RFC 3986 percent encode a given string 
 */
std::string percent_encode(const std::string& /* str */);

/*
 * TODO: RFC 3986 percent decode a given string
 */

/*
 * Generate HMAC-SHA1 of the given string
 */
std::string hmac_sha1(const std::string& /* key */, const std::string& /* str */);

}

#endif
