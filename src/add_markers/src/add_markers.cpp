#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"
#include <math.h>

typedef enum _MARKER_STATUS {
     HIDE,
     PICK_UP,
     DROP_OFF
} MARKER_STATUS;

bool pickupdone = false;
bool dropoffdone = false;
ros::Publisher marker_pub;
ros::Subscriber odom_sub;
visualization_msgs::Marker marker;
float pickupzone[2]={6.0,-1.0};
float dropoffzone[2]={1.0,-7.5};
float pos_thresh = 0.45f;

void updateMarker(const MARKER_STATUS status)
{
  //common properties
  {
  	marker.header.frame_id = "map";
  	marker.header.stamp = ros::Time::now();
  	// Set the namespace and id for this marker.  This serves to create a unique ID
  	// Any marker sent with the same namespace and id will overwrite the old one
  	marker.ns = "markers";
  	marker.id = 0;
    
  	marker.type = visualization_msgs::Marker::CUBE;
    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;    
    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;
    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 0.0f;
    marker.color.b = 0.0f;
    
    marker.pose.orientation.w = 1.0;
    marker.lifetime = ros::Duration();
  }
  if (status == HIDE) 
  {
   	ROS_INFO("Hide marker.\n");
    marker.color.a = 0;        
  }
  else if (status == PICK_UP)
  {
    ROS_INFO("Display marker at pickup zone.\n");
    marker.color.a = 0.8;
    marker.pose.position.x = pickupzone[0];
    marker.pose.position.y = pickupzone[1];
  }
  else if (status == DROP_OFF)
  {
    ROS_INFO("Display marker at drop off zone.\n");
    marker.color.a = 0.8;
    marker.pose.position.x = dropoffzone[0];
    marker.pose.position.y = dropoffzone[1];
  }
  // Publish the marker
  while (marker_pub.getNumSubscribers() < 1)
  {
      if (!ros::ok())
      {
        return ;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
  } 
  marker_pub.publish(marker);
  usleep(10); 
}

void odom_callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  float robotpos_x= msg->pose.pose.position.x;
  float robotpos_y=msg->pose.pose.position.y; 
  double distgoal;
  if (!pickupdone)
  {    
    distgoal=sqrt(pow(pickupzone[0]- robotpos_x,2) + pow(pickupzone[1] - robotpos_y,2));    
    if (distgoal<pos_thresh)
    {
      updateMarker(HIDE);  
      pickupdone=true;
    }
  }
  else if (!dropoffdone)
  {
    distgoal=sqrt(pow(dropoffzone[0]- robotpos_x,2) + pow(dropoffzone[1] - robotpos_y,2));
    if (distgoal<pos_thresh)
    {
      updateMarker(DROP_OFF);  
      dropoffdone=true;
    }
  } 
  //ROS_INFO("X: %f Y: %f Distance: %f",robotpos_x,robotpos_y,distgoal);
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  odom_sub = n.subscribe("odom", 1000, odom_callback);
  
  updateMarker(PICK_UP);  
  while (ros::ok())
  {
    ros::spinOnce();
    r.sleep();
  }
}
