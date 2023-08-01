# UCSD COSMOS 2023 Cluster 7 - H4O: Hacking for Oceans
<h4>Science Team: Cynthia, Tavisha, Sebastian, Ethan, Harjyot, Anastasia </h4>
<p>The Science Team worked to use radios, GPS, and sensors (pH, temperature, TDS, ORP, and Dissolved Oxygen) to make a buoy that attaches to the USVs, collects data from the water, and sends that data to the database via radio. We also attempted to send sensors underwater using a winch to collect data from different depths. </p>
<p> The code that we used for our sensors can be found in the "Sensors" folder, code for the radio can be found in "telemetry", code for the winch can be found in "testing", and the code that puts all the pieces of our buoy together can be found in the main folder.</p>

<h3>Project Timeline</h3>
<p><b>Week 1:</b> During the first week, we began to investigate our sensors and calibrate them. We also began to work with the GPS and radios. Although we initially struggled with the radios, we were able to send data to the data science team.</p>
<img src=https://github.com/frawgmanman/COSMOS23-C7-H4O/blob/main/photos/calibr.jpg?raw=true width=500 >

<p><b>Week 2:</b> In the second week, we prepared our first buoy for a pool test and our first trip to the lake. We drilled holes in the buoy, wired pH, temperature, radio and GPS to a microcontroller and battery, and set it up inside the buoy to test out at the pool. However, we were not able to fully test our sensors at the pool due to an accident with the batteries, since our transistor was set on 12 volts instead of 5 volts, which caused the microcontroller to start smoking and fried a couple of sensors. For our first lake day, we wired up the rest of the sensors and successfully transmitted data from the buoy to the database.</p>
<img src=https://github.com/frawgmanman/COSMOS23-C7-H4O/blob/main/photos/IMG_0988.jpg?raw=true width=500 >
<img src=https://github.com/frawgmanman/COSMOS23-C7-H4O/blob/main/photos/fried.jpg?raw=true width=500 >
<img src=https://github.com/frawgmanman/COSMOS23-C7-H4O/blob/main/photos/wires.jpg?raw=true width=500 >

<p><b>Week 3:</b> On week three, we prepared all of our sensors and worked on improving the radio for our second lake day. A couple of days before our lake trip, we decided to use String objects and try to send all of the data in one message to the radio, instead of multiple. We also simplified the message structure to make the radio transmission easier and more reliable.</p>
<!--<img src=https://github.com/frawgmanman/COSMOS23-C7-H4O/blob/main/photos/__.jpg?raw=true width=500 >-->

<p><b>Week 4:</b> During the last week, we worked on implementing the winch system. We 3D printed many parts and created code that would send information from the sensors in the underwater package to the buoy, which would then send the data to the database team's radios. Although we were not able to test the winch system in water, we were excited to try making it.</p>
