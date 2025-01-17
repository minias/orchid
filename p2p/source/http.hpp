/* Orchid - WebRTC P2P VPN Market (on Ethereum)
 * Copyright (C) 2017-2019  The Orchid Authors
*/

/* GNU Affero General Public License, Version 3 {{{ */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
/* }}} */


#ifndef ORCHID_HTTP_HPP
#define ORCHID_HTTP_HPP

#include <map>
#include <string>

#include <rtc_base/openssl_certificate.h>

#include "task.hpp"

namespace orc {

class Adapter;
class Locator;

task<std::string> Request(Adapter &adapter, const std::string &method, const Locator &locator, const std::map<std::string, std::string> &headers, const std::string &data, const std::function<bool (const rtc::OpenSSLCertificate &)> &verify = nullptr);

task<std::string> Request(const std::string &method, const Locator &locator, const std::map<std::string, std::string> &headers, const std::string &data, const std::function<bool (const rtc::OpenSSLCertificate &)> &verify = nullptr);

}

#endif//ORCHID_HTTP_HPP
