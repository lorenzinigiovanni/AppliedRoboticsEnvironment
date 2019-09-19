#pragma once

#include <ros/ros.h>
#include <vector>

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PolygonStamped.h"
#include "nav_msgs/Odometry.h"

#include "extended_kalman_filter.hpp"

namespace localization {

struct RobotState{
  RobotState(){}
  RobotState(const ros::Time& stamp, const double& x, 
                  const double& y, const double& theta): stamp(stamp), x(x), y(y), theta(theta){}
    
  ros::Time stamp;
  double x, y, theta;
};

class LocalizationHandle {

public:
  // Constructor
  explicit LocalizationHandle();
    
  void onInit(ros::NodeHandle &nodeHandle);

private:
  // Methods
  void loadParameters();  
  void publishToTopics();
  void subscribeToTopic(); 
  
  ExtendedKalmanFilter ekf_;

  // ROS node handle
  ros::NodeHandle nh_;
  bool initialized_; // check if node handle is initialized
  int queue_size_;
  
  // ROS communication
  ros::Subscriber sub_gps_, sub_odom_;
  ros::Publisher pub_map_odom_, pub_dt_;
  bool has_gps_, has_odom_;

  // TOPICS  
  std::string sub_gps_topic_name_, sub_odom_topic_name_;
  std::string pub_map_odom_topic_name_, pub_dt_topic_name_;

  ros::Time last_update_stamp_;
  std::list<RobotState> odom_list_;
  std::pair<RobotState, RobotState> odom_map_correspondence_;

  Matrix2 R_;
  Vec2 t_;

  // Callback
  void gpsCb(const geometry_msgs::PoseStampedPtr robot_pose);
  void odomCb(const nav_msgs::OdometryPtr odom);
};
}
