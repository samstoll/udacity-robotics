#!/bin/sh
xterm  -e  "roslaunch my_robot world.launch" &
sleep 20
xterm  -e  "roslaunch my_robot amcl.launch map_file:=/home/workspace/udacity-robotics/Project5/catkin_ws/src/map/map.yaml" &
sleep 5
xterm  -e  " roslaunch add_markers view_navigation.launch" & 
sleep 10
xterm  -e  " rosrun add_markers add_markers" &
sleep 10
xterm  -e  " rosrun pick_objects pick_objects_node"
