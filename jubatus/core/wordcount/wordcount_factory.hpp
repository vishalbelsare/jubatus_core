// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2015 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef JUBATUS_CORE_WORDCOUNT_WORDCOUNT_FACTORY_HPP_
#define JUBATUS_CORE_WORDCOUNT_WORDCOUNT_FACTORY_HPP_

#include <string>
#include "jubatus/util/lang/shared_ptr.h"

namespace jubatus {
namespace core {

namespace common {
namespace jsonconfig {

class config;

}  // namespace jsonconfig
}  // namespace common

namespace wordcount {

class wordcount_base;

class wordcount_factory {
 public:
  static jubatus::util::lang::shared_ptr<wordcount_base> create_wordcount(
    const std::string& name,
    const common::jsonconfig::config& param);
};

}  // namespace wordcount
}  // namespace core
}  // namespace jubatus

#endif  // JUBATUS_CORE_WORDCOUNT_WORDCOUNT_FACTORY_HPP_