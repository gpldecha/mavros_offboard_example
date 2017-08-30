# mavros-offboard-example
An example of using mavros on a raspberry pi 3 with a Pixhawk2 equipped with a rover.
This example is derived from [MAVROS offboard control example](https://dev.px4.io/en/ros/mavros_offboard.html). 

# Setting up the raspberry pi 3

* 32 GB SanDisk Ultra 32 GB MicroSDHC UHS-I Memory Card with SD Adapter/. The default 8 GB SD card provided 
by rasberry will not do. Later one we will be installing ros and other libraries.
* Falshed SD card with [ubuntu-mate](https://ubuntu-mate.org/raspberry-pi/) with 
ddrescue command (I tried various GUI options, but always resulted in the SD card being corrupted, somehow).

* Edit the file **/boot/config.txt** add the following lines:

```bash
dtoverlay=pi3-disable-bt
enable_uart=1
```

# Troubleshooting

*  FCU: DeviceError:serial:open: Permission denied
```bash
 sudo gpasswd -a $USER dialout 
```
