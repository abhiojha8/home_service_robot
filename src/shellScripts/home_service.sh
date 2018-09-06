#!/bin/sh

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/skylark/catkin_ws/src/worlds/u_world" &

sleep 5

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/skylark/catkin_ws/src/worlds/u_world_map.yaml" &

sleep 5

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch" &

sleep 5

# xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; rosrun pick_object pick_object"
xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; rosrun home_service home_service" &

xterm -e "cd /home/skylark/catkin_ws/; source devel/setup.bash; rosrun pick_object pick_object"


