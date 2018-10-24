#!/bin/sh
CATKIN_DIR="$( cd $( dirname $BASH_SOURCE[0] )/../.. >/dev/null && pwd )"
export TURTLEBOT_3D_SENSOR="kinect"
export ROBOT_INITIAL_POSE="-x -4.5 -y -3 -Y 1.571"
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$CATKIN_DIR/src/World/ushaped_environment.world; " &
sleep 10
xterm  -e  " roslaunch turtlebot_navigation gmapping_demo.launch " & 
sleep 5
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch "  &
sleep 5
xterm  -e  " rosrun wall_follower wall_follower "  
