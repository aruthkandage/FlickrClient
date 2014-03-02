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
