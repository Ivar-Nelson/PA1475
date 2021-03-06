Assignment name: 
“Install and forget” light intensity sensor 
Assignment description: 
Smart buildings rely on sensory data to decrease energy costs and improve the ambient climate. 
While commercial buildings are now often built with this in mind, retrofitting sensors to existing 
houses is often a problem due to the lack of communication and power lines. The alternative is to 
use wireless communication and battery powered systems. 
The currently available devices1 on the market have several drawbacks:
1. For in-door use only, i.e. no waterproofing and operating temperature range that suggests 
that they would not work in the winter.
2. Battery powered, i.e. the power source needs to be replaced at regular intervals. 
The sensor is primarily intended to be used to automate blinds or any other kind of device that shuts 
out light. It is therefore important to be able to position the sensor optimally, i.e. on the house 
façade. A sensor placed inside the house might not get a reliable reading, due to the varying position 
and light intensity of the sun during the year, and obstructions in the house.
Objectives: 
The sensor is used by tech-savvy homeowners that build their own home automation solutions. The 
sensor must fulfill the following high-level characteristics:
 “Install and forget”: since the sensor is placed on the façade of a house, at any level, it might 
be hard to reach. Hence it must be autonomous once it is installed (a minimum lifetime of 5 
years is expected).
 Since the sensor is retrofitted, there are no power or communication lines available. The 
sensor must get energy from other sources and communicate wirelessly.
 Frequency of sensory data transmission must be at least once per minute (if state has 
changed by a certain delta). This is necessary to be able to close blinds as soon as light 
intensity rises above a certain level.

 Housing must be waterproof and protect against high and low temperatures that are 
common in Sweden. 
Prerequisites:

 Technical: 
There is no limitation/constraint on the technical solution if all four bullets from the 
objectives are fulfilled.
 Other dependencies: 
The sensor must integrate with Home Assistant2  and use a standard communication protocol 
for IOT (Internet of Things) devices, for example, MQTT.

Customer/contact: 
Dr. Michael Unterkalmsteiner (mun@bth.se)

1 for example: https://xiaomi-mi.com/sockets-and-sensors/xiaomi-mi-light-detection-sensor-zigbee-30/2 https://www.home-assistant.io/
