#!/bin/sh
xterm  -e  "roslaunch my_robot world.launch" &
sleep 5
xterm  -e  "roslaunch my_robot amcl.launch map_file:=/home/workspace/udacity-robotics/Project5/catkin_ws/src/map/map.yaml" &
sleep 5
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch" & 
sleep 5
