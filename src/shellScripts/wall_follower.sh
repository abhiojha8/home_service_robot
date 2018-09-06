#!/bin/sh

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/skylark/catkin_ws/src/worlds/u_world" &

sleep 10

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_gazebo gmapping_demo.launch" &

sleep 10

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch" &

sleep 10

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; rosrun wall_follower wall_follower"
