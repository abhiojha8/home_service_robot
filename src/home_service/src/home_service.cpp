#include <ros/ros.h>
#include <tf/tf.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>

geometry_msgs::Pose2D current_pose;
ros::Publisher pub_pose2d;
ros::Publisher marker_pub;

int bot_location = 0;
float x_pos, y_pos, z_pos = 0.0;

void odomCallback(const nav_msgs::OdometryConstPtr& msg)
{
    // linear position
    x_pos = msg->pose.pose.position.x;
    y_pos = msg->pose.pose.position.y;
    z_pos = msg->pose.pose.position.z;
    // current_pose.w = msg->pose.pose.orientation.w;
    
    // pub_pose2d.publish(current_pose);

     // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  while (ros::ok())
  {
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "marker_0";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 1;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    switch(bot_location) {
      // this case will show marker at pickup zone
      case 0:
          ROS_INFO("Publishing marker at pickup zone");
          // Publish the marker

          while (marker_pub.getNumSubscribers() < 1)
          {
            if (!ros::ok())
            {
              return;
            }
            ROS_WARN_ONCE("Please create a subscriber to the marker");
            sleep(1);
          }

          marker_pub.publish(marker);
          
          //Update bot_location
          bot_location = 1;
          return;
          break;
          

      // wait for arrival at pickup zone. Delete marker after 5 seconds
      case 1:

          //wait for the robot to reach pick up zone
          ROS_INFO("Waiting for the robot to reach pick up zone");

          if(std::abs(x_pos - marker.pose.position.x) > 0.5 || 
              std::abs(y_pos - marker.pose.position.y) > 0.5 ||
              std::abs(z_pos - marker.pose.position.z) > 0.5) {
              
              ros::Duration(0.5).sleep();
              //remain in the same bot_location till robot reaches pick up location
              bot_location = 1;
          }

          else {
          // Wait for 5 seconds
          ROS_INFO("Simulating pick up");
          ros::Duration(5.0).sleep();

          //delete the marker
          ROS_INFO("Deleting marker from pick up zone");
          marker.action = visualization_msgs::Marker::DELETE;
          marker_pub.publish(marker);
          
          //Update the bot_location
          bot_location = 2;
          }
          return;
          break;
          

      // wait for arrival at drop off zone and display marker there
      case 2:

          marker.action = visualization_msgs::Marker::ADD;

          // Set the second pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
          marker.pose.position.x = 3;
          marker.pose.position.y = 0;
          marker.pose.position.z = 0;
          marker.pose.orientation.x = 0.0;
          marker.pose.orientation.y = 0.0;
          marker.pose.orientation.z = 0.0;
          marker.pose.orientation.w = 0.5;

          //wait until robot reaches drop-off zone
          ROS_INFO("Waiting for robot to reach drop off zone");

          if(std::abs(x_pos - marker.pose.position.x) > 0.5 || 
                std::abs(y_pos - marker.pose.position.y) > 0.5 ||
                std::abs(z_pos - marker.pose.position.z) > 0.5)
          {
              ros::Duration(0.5).sleep();
              //remain in the same bot_location till robot reaches drop off location
              bot_location = 2;
              return;
          }

          else {    
          ros::Duration(0.3).sleep();
          ROS_INFO("Robot reached drop-off zone - publishing marker");
          marker_pub.publish(marker);
          //Update the bot_location
          bot_location = 3;
          }
          return;
          break;
          

      // the end!
      case 3:
          ros::Duration(200.0).sleep();
          //remain in this bot_location now    
          bot_location == 3;
          return;
          break;

    }
  }
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "home_service");
  ros::NodeHandle n;
  ros::Rate r(1);
  marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  ros::Subscriber sub_odometry = n.subscribe("odom", 1000, odomCallback);
  ros::spin();
  return 0;
}
