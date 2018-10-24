#!/bin/sh
CATKIN_DIR="$( cd $( dirname $BASH_SOURCE[0] )/../.. >/dev/null && pwd )"
export TURTLEBOT_3D_SENSOR="kinect"
export ROBOT_INITIAL_POSE="-x -4.5 -y -3 -Y 1.571"
xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$CATKIN_DIR/src/World/ushaped_environment.world; " &
sleep 5
xterm  -e  " roslaunch turtlebot_navigation amcl_demo.launch map_file:=/$CATKIN_DIR/src/World/myMap.yaml" & 
sleep 5
xterm  -e  " rosrun rviz rviz -d $CATKIN_DIR/src/RvizConfig/home_service.rviz " &
sleep 5
xterm  -e  " rosrun pick_objects pick_objects " &
sleep 5
xterm  -e  " rosrun add_markers add_markers "