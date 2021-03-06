// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2015 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#include "epsilon_decreasing.hpp"

#include <string>
#include <vector>
#include <limits>
#include <algorithm>

#include "../common/exception.hpp"
#include "../framework/packer.hpp"
#include "../common/version.hpp"

namespace jubatus {
namespace core {
namespace bandit {

epsilon_decreasing::epsilon_decreasing(const config& conf)
    : d_(conf.decreasing_rate), s_(conf.assume_unrewarded) {
  if (conf.decreasing_rate <= 0 || 1 <= conf.decreasing_rate) {
    throw JUBATUS_EXCEPTION(
        common::invalid_parameter("0 < d < 1"));
  }
  if (conf.seed) {
    if (*conf.seed < 0 || std::numeric_limits<uint32_t>::max() < *conf.seed) {
      throw JUBATUS_EXCEPTION(
          common::config_exception() << common::exception::error_message(
              "seed must be within unsigned 32 bit integer"));
    }
    rand_ = jubatus::util::math::random::mtrand(*conf.seed);
  }
}

std::string epsilon_decreasing::select_arm(const std::string& player_id) {
  const std::vector<std::string>& arms = s_.get_arm_ids();
  if (arms.empty()) {
    throw JUBATUS_EXCEPTION(
        common::exception::runtime_error("arm is not registered"));
  }
  std::string result;
  int total_trial = s_.get_total_trial_count(player_id) + 1;
  double log_total_trial = std::log(total_trial);
  double eps = 5 * arms.size() * (log_total_trial / total_trial) / (d_ * d_);
  double dec_eps = std::min(1.0, eps);
  if (rand_.next_double() < dec_eps) {
    // exploration
    result = arms[rand_.next_int(arms.size())];
  } else {
    // exploitation
    result = arms[0];
    double exp_max = s_.get_expectation(player_id, arms[0]);
    for (size_t i = 1; i < arms.size(); ++i) {
      double exp = s_.get_expectation(player_id, arms[i]);
      if (exp > exp_max) {
        result = arms[i];
        exp_max = exp;
      }
    }
  }
  s_.notify_selected(player_id, result);
  return result;
}

bool epsilon_decreasing::register_arm(const std::string& arm_id) {
  return s_.register_arm(arm_id);
}
bool epsilon_decreasing::delete_arm(const std::string& arm_id) {
  return s_.delete_arm(arm_id);
}

bool epsilon_decreasing::register_reward(const std::string& player_id,
                                     const std::string& arm_id,
                                     double reward) {
  return s_.register_reward(player_id, arm_id, reward);
}

arm_info_map epsilon_decreasing::get_arm_info(
  const std::string& player_id) const {
  return s_.get_arm_info_map(player_id);
}

bool epsilon_decreasing::reset(const std::string& player_id) {
  return s_.reset(player_id);
}
void epsilon_decreasing::clear() {
  s_.clear();
}

void epsilon_decreasing::pack(framework::packer& pk) const {
  pk.pack(s_);
}
void epsilon_decreasing::unpack(msgpack::object o) {
  o.convert(&s_);
}

void epsilon_decreasing::get_diff(diff_t& diff) const {
  s_.get_diff(diff);
}
bool epsilon_decreasing::put_diff(const diff_t& diff) {
  return s_.put_diff(diff);
}
void epsilon_decreasing::mix(const diff_t& lhs, diff_t& rhs) const {
  s_.mix(lhs, rhs);
}

storage::version epsilon_decreasing::get_version() const {
  return storage::version();
}

}  // namespace bandit
}  // namespace core
}  // namespace jubatus
