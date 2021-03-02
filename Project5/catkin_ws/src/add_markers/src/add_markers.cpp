#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

ros::Publisher marker_pub;
visualization_msgs::Marker marker;

bool hasPickedUp = false;
bool hasDroppedOff = false;

const float PICKUP_X = -1.7;
const float PICKUP_Y = -0.8;
const float DROPOFF_X = 1.1;
const float DROPOFF_Y = 0.75;
const float MARGIN = 0.1;

void pose_callback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& pose_msg){
  float pos_x = pose_msg->pose.pose.position.x;
  float pos_y = pose_msg->pose.pose.position.y;
  //ROS_INFO("%f %f", pos_x, pos_y);

  if (!hasPickedUp) {
    if (std::abs(PICKUP_X - pos_x) < MARGIN && std::abs(PICKUP_Y - pos_y) < MARGIN) {
      ROS_INFO("Clearing pickup marker");
      // clear marker
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
      hasPickedUp = true;
    }
  } else if (!hasDroppedOff) {
    if (std::abs(DROPOFF_X - pos_x) < MARGIN && std::abs(DROPOFF_Y - pos_y) < MARGIN) {
      ROS_INFO("Publishing dropoff marker");
      // Show marker at dropoff
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = DROPOFF_X;
      marker.pose.position.y = DROPOFF_Y;
      marker.pose.position.z = 0;
      marker.pose.orientation.x = 0.0;
      marker.pose.orientation.y = 0.0;
      marker.pose.orientation.z = 0.0;
      marker.pose.orientation.w = 2.0;
      marker_pub.publish(marker);
      hasDroppedOff = true;
    }
  }
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // Subscribe to acml pose
  ros::Subscriber pose_sub = n.subscribe("amcl_pose", 100, pose_callback);

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  if (ros::ok())
  {
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "add_markers";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = PICKUP_X;
    marker.pose.position.y = PICKUP_Y;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.25;
    marker.scale.y = 0.25;
    marker.scale.z = 0.25;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }
    marker_pub.publish(marker);
    ROS_INFO("Published pickup marker");
    while(ros::ok()){
      ros::spinOnce();
      r.sleep();
    }
  }
}

