# distraction-free-cycle-computer

## Outcome:
I have created an electronics outcome with an aim to increase cyclist safety by reducing distractions. I have succeeded in this task by creating a small, battery powered cycle computer which is able to provide the user with telemetry while cycling as well as allowing them to control the music playback coming from their phone.
The outcome is powered by an ESP32, and a NEO-7M GNSS module, information is displayed to the user on a monochromatic dot-matrix LCD. The user is able to interact with the outcome through 3 buttons which allow them to access all the inbuilt functions of the device.

The purpose of the product is to provide cyclists with an alternative to traditional cycle computers or their mobile phone, both of which either lack features like music control, or create possible safety issues for the user.
I believe it is important to include music control in the product because music is a tool that many cyclists already use and have the benefit from, however most cycle computers do not have the ability to control the music being played, so including this means the user is able to find everything they want to control in one central location. This will make this tool more accessible to all users

The intended end users for this product will be cyclists, both on and off the road, who would like a high-powered, relatively feature rich cycle computer and music controller. The product should cover most of the most commonly used aspects of existing cycle computers, in order to appeal to the largest possible section of the market. It will be marketed towards cyclists that value their safety, and will provide a way to address existing safety implications while still providing users with an array of features.

## Installation/Use:
### Telemetry & GNSS:
To gain working telemetry data (Speed, Distance, Average Speed, etc.)
1. power on the device. The status LED will initially light up red.
2. Leave the device outside with a clear view of the sky.
3. Once the LED changes to green it shows that a sufficient number of satellites have been connected.
4. The device is ready to use.

### Music Control/App:
To gain music control functionality
1. Install the companion app (android only, You need to enable “Allow unknown sources” in the settings of the device) [Download](https://drive.google.com/file/d/1Jh82q7Yky1SGaJbtc7S9t7uBHAqpT1OG/view?usp=drive_link)
2. Pair the cycle computer to your phone via bluetooth
3. Power on the cycle computer, and launch the app
4. On app launch, Authorise connection with spotify account (if required)
5. Press the 'Connect Bluetooth' option
6. Wait for confirmation message

## Debugging:
### Visual bugs on the display:
Power off and power on the device
### Button inputs not working:
Wait a few seconds and try again / press and hold for a bit longer / power off power on
### Spotify Connectivity not working:
Restart the app to allow authorisation popup again / Remove app as trusted from spotify then try again / Contact me, some permissions may need to be changed
