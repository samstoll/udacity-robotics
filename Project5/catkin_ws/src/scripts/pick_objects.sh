#!/bin/sh
xterm  -e  "roslaunch my_robot world.launch" &
sleep 5
xterm  -e  "roslaunch my_robot amcl.launch" &
sleep 5
xterm  -e  " roslaunch turtlebot_rviz_launchers view_navigation.launch" & 
sleep 20
xterm  -e  " rosrun pick_objects pick_objects_node"
