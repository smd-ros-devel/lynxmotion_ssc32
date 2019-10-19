// BSD 3-Clause License
// 
// Copyright (c) 2019, Matt Richard
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef SSC32U_CONTROLLERS__SERVO_CONTROLLER_HPP_
#define SSC32U_CONTROLLERS__SERVO_CONTROLLER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include "ssc32u_msgs/msg/servo_command_group.hpp"
#include "ssc32u_msgs/msg/discrete_output.hpp"
#include "ssc32u_msgs/srv/query_pulse_width.hpp"

namespace ssc32u_controllers
{

struct Joint
{
  int channel;
  double min_angle;
  double max_angle;
  double offset_angle; // this angle is considered to be 1500 uS
  double default_angle; // angle that the joint is initialized to (defaults to the offset_angle)
  bool initialize; // Indicates whether to initialize the servo to the default angle on startup.
  bool invert;
	std::string name;
};

class ServoController : public rclcpp::Node
{
public:
  explicit ServoController(const rclcpp::NodeOptions & options);

private:
  std::map<std::string, Joint> joints_map_;
  rclcpp::Subscription<trajectory_msgs::msg::JointTrajectory>::SharedPtr joint_trajectory_sub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_state_pub_;
  rclcpp::Publisher<ssc32u_msgs::msg::DiscreteOutput>::SharedPtr discrete_output_pub_;
  rclcpp::Publisher<ssc32u_msgs::msg::ServoCommandGroup>::SharedPtr servo_command_pub_;
  rclcpp::Service<std_srvs::srv::Empty>::SharedPtr relax_joints_srv_;
  rclcpp::Client<ssc32u_msgs::srv::QueryPulseWidth>::SharedPtr query_pw_client_;
  std::shared_ptr<rclcpp::TimerBase> joint_states_timer_;

  void publish_joint_states();
  void joint_command_callback(const trajectory_msgs::msg::JointTrajectory::SharedPtr msg);
  void relax_joints(const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<std_srvs::srv::Empty::Request> request,
    std::shared_ptr<std_srvs::srv::Empty::Response> response);

  int clamp_pulse_width(int pulse_width);
  int invert_pulse_width(int pulse_width);
};

}  // namespace ssc32u_controllers

#endif  // SSC32U_CONTROLLERS__SERVO_CONTROLLER_HPP_
