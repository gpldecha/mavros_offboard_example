# mavros-offboard-example
An example of using mavros on a raspberry pi 3 with a Pixhawk2 equipped with a rover.
This example is derived from [MAVROS offboard control example](https://dev.px4.io/en/ros/mavros_offboard.html). 

# Setting up the raspberry pi 3

1. Edit the file **/boot/config.txt** add the following lines:

```bash
dtoverlay=pi3-disable-bt
enable_uart=1
```
# Troubleshooting

*  FCU: DeviceError:serial:open: Permission denied

crw-rw----   1  777 dialout 204,  64 Feb 11  2016 ttyAMA0
