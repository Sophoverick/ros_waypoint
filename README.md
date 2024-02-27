# wp_send
Simple waypoint sender node for the ICUAS24_Competitions, this alone will scan all beds in a minimalist manner for the first stage of [ICUAS24 student competition](https://github.com/larics/icuas24_competition). All required packages are preinstalled in the docker image. In order to build the package simply do the following:
```
cd ~/sim_ws/src/uav_ros_simulation/ros_packages
mkdir -p catkin_ws/src
cd catkin_ws/src
git clone https://github.com/Sophoverick/wp_send
cd ../..
catkin build
source devel/setup.bash
```
The simple ROS node should be run when the simulation is running:
```
rosrun ros_waypoint wpsend
```
