/**
 * @file offb_node.cpp
 * @brief offboard example node, written with mavros version 0.14.2, px4 flight
 * stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <mavros_msgs/CommandHome.h>
#include <mavros_msgs/HomePosition.h>

mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("mavros/state", 10, state_cb);

    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 10);
    ros::Publisher home_position_pub = nh.advertise<mavros_msgs::HomePosition>("/mavros/home_position/set", 2, true);


    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    ros::ServiceClient set_home = nh.serviceClient<mavros_msgs::CommandHome>("/mavros/home_position/set");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && current_state.connected){
        ros::spinOnce();
        rate.sleep();
   }

    geometry_msgs::PoseStamped pose;
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 0;

    mavros_msgs::HomePosition home_pos;
    home_pos.position.x = 0;
    home_pos.position.y = 0;
    home_pos.position.z = 0;
    home_pos.orientation.w = 1;
    home_pos.orientation.x = 0;
    home_pos.orientation.y = 0;
    home_pos.orientation.z = 0;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        home_position_pub.publish(home_pos);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();
    bool b_home_set=false;
    while(ros::ok()){
        if(current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0))){
            if(set_mode_client.call(offb_set_mode.request, offb_set_mode.response)){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        }else{
            if( !current_state.armed && (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) && arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }
        /*if(!b_home_set)
        {
            mavros_msgs::CommandHome home_pos;
            home_pos.request.
            home_pos.request.latitude = 0;
            home_pos.request.longitude = 0;
            set_home.call(home_pos);
            if(home_pos.response.success){
                ROS_INFO("set_home: succeeded");
                b_home_set=true;
            }else{
                ROS_WARN("set_home: failed");
            }
        }*/
        home_position_pub.publish(home_pos);
        local_pos_pub.publish(pose);


        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
