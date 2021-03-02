#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "/map";
  goal.target_pose.header.stamp = ros::Time::now();

  // Define a position and orientation for the robot to reach
  goal.target_pose.pose.position.x = -1.7;
  goal.target_pose.pose.position.y = -0.8;
  goal.target_pose.pose.orientation.z = 1;

  // Send the pickup position and orientation for the robot to reach
  ROS_INFO("Sending pickup");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("Successfully reached pickup location, waiting 5s");
    sleep(5);
  } else {
    ROS_INFO("Failed to reach pickup location");
  }
  // Set the drop off location and orientation
  goal.target_pose.pose.position.x = 1.1;
  goal.target_pose.pose.position.y = 0.75;
  goal.target_pose.pose.orientation.z = 0;
  goal.target_pose.pose.orientation.w = 1.0;

  // Send the pickup position and orientation for the robot to reach
  ROS_INFO("Sending dropoff");
  ac.sendGoal(goal);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Successfully reached dropoff location");
  else
    ROS_INFO("Failed to reach dropoff location");

  return 0;
}
