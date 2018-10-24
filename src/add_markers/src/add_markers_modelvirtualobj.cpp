#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  bool pickup = true;
  
  // Set shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "markers";
    marker.id = 0;

    // Set the marker type
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;
  
    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.5f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

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
    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    if (pickup)
    {
      marker.pose.position.x = 6.0;
      marker.pose.position.y = -1.0;
      marker.pose.orientation.w = 1.0;
      marker.lifetime = ros::Duration(5.0);
    }
    else
    {
      marker.pose.position.x = 1.0;
      marker.pose.position.y = -7.5;
      marker.pose.orientation.w = 1.0;
      marker.lifetime = ros::Duration();
    }    
    marker_pub.publish(marker);    
    //ROS_INFO("Time Second: %f \n",ros::Time::now().toSec());
    if (pickup)	
    {
      ros::Duration(10.0).sleep();
      pickup=false;
    }
    r.sleep();
  }
}