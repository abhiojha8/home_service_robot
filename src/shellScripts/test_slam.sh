#!/bin/sh

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/skylark/catkin_ws/src/worlds/u_world" &

sleep 5

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_navigation gmapping_demo.launch" &

sleep 5

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch" &

sleep 5

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_teleop keyboard_teleop.launch"

