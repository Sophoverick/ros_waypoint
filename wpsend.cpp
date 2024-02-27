#include<cstdlib>
#include"ros/ros.h"
#include"geometry_msgs/PoseStamped.h"
#include"nav_msgs/Odometry.h"


double odo_x;
double odo_y;
double odo_z;

void wp_callback(const nav_msgs::Odometry::ConstPtr &wpr)
{
  odo_x=(wpr->pose.pose.position.x);
  odo_y=(wpr->pose.pose.position.y);
  odo_z=(wpr->pose.pose.position.z);
}
  int main(int argc, char **argv)
{
  ros::init(argc, argv, "wpsend");
  ros::NodeHandle n1;
  int wp_enum=20;
  geometry_msgs::PoseStamped wp;
  /*Quaternions for a yaw of 0*/
    
  wp.pose.orientation.z = 0;
  wp.pose.orientation.w = 0;
  double positions[wp_enum][3]=
  {
  /*scan the top*/
  {2,4,8},
  {2,25,8},
  /*descend and scan the middle*/
  {2,25,4.5},
  {2,4,4.5},
  /*descend and scan the bottom*/
  {2,4,1.5},
  {2,26,1.5},
  /*move to the next row*/
  {8,26,1.5},
  /*scan the bottom*/
  {8,4,1.5},
  /*ascend and scan the middle*/
  {8,4,4.5},
  {8,25,4.5},
  /*ascend and scan the top*/
  {8,25,8},
  {8,3,8},
  /*move to the next row*/
  {14,3,8},
  /*scan the top*/
  {14,25,8},
  /*descend and scan the middle*/
  {14,25,4.5},
  {14,4,4.5},
  /*descend and scan the bottom*/
  {14,4,1.5},
  {14,26,1.5},
  /*get original position in x axis*/
  {1.5,26,1.5},
  /*get original position in y axis*/
  {1.5,1.5,2.3}
  };
  ros::Publisher wp_pub = n1.advertise<geometry_msgs::PoseStamped>
  ("/red/tracker/input_pose", 1000);
  ros::Subscriber sub = n1.subscribe("/red/odometry", 1000, wp_callback);
  ros::Rate loop_rate(1);
  int count = 0;
  /*waypoint sent once or twice*/
  int repcount = 0;
  while (ros::ok())
  {
    wp.pose.position.x = positions[count][0];
    wp.pose.position.y = positions[count][1];
    wp.pose.position.z = positions[count][2];
    /*Get Odometry */
    ros::spinOnce();
    /*Print current position*/
    ROS_INFO("Odometry says: [%.2f, %.2f, %.2f]",odo_x,odo_y,odo_z);
    /*See if waypoint is reached*/
    if (abs(odo_x-wp.pose.position.x) < 0.4
    && abs(odo_y-wp.pose.position.y) < 0.4
    && abs(odo_z-wp.pose.position.z) < 0.4){
    ROS_INFO("Waypoint reached.");
    count++;
    repcount=0;
    }
    else
    {
      /*See if waypoint has been sent*/
      if ((repcount < 2 && count < 1) || (count >= 1 && repcount==0)){
        ROS_INFO("Waypoint not reached.");
        wp_pub.publish(wp);
        ROS_INFO("Waypoint of (%.2f, %.2f, %.2f) sent.",
        wp.pose.position.x,wp.pose.position.y,wp.pose.position.z);
        repcount++;
      }
      else{
      ROS_INFO("Waypoint still not reached.");
      }
    }
    if (count == wp_enum)
        break;
    loop_rate.sleep();
  }
  return 0;
}
