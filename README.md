# AWS-IoT-Shipment-Tracker
![image](https://user-images.githubusercontent.com/95514274/180837906-5c29de94-dc23-4f35-93b3-5cd1cb75251e.png)

Hello! This is my simple IoT project using Amazon Web Services(AWS) IoT Core(a platform for hosting our IoT devices) and integrated with Arduino IoT Cloud to control our Arduino(to extract location coordinates and send to Rpi).This project is a shipment tracker which tracks the position of anything where this device system is attatched to and also in the region of a open Wifi network.We can navigate our shipment from any part of the world having internet connectivity.

The hardware parts needed are-Arduino Uno Board,Ardino Wifi Shield,Raspberry Pi 4 (both boards are connected by USB Cable),BN 180 GPS 2 pin serial output device.The software part is handled by C++ and Python accordingly.

The AWS IoT core uses Message Queuing Telemetry Transport(MQTT) Protocol,a fundamental data transfer protocol for IoT devices which is publish-subscribe model between any clients handled by a broker server.I have used AWSMQTT SDK in Python to publish messages for specific topics for location data generated by the GPS device which can be accessed by subscribing to the topic through the AWS IoT core broker server.

I have used TinyGPSplus library of Arduino to extract coordinates(longitude and lattitude) of our shipment and transfer this data to RPi by serial communication through USB cable.This data recieved by RPi is published.The BN 180 GPS device also needs serial communication to happen between it and Arduino Board hence I also used the Software Serial Library so we can use any two pins of Arduino Board as TX-RX pins for serial data.The wifi library is also included as we need to perform a scan of networks for Arduino to connect with the Arduino IoT Cloud by the help of Create Agent.The Things library is also included to connect to the Arduino IoT Core.This whole mechanism of Arduino functionality can be managed by using dashboard of Arduino IoT Cloud,we will trigger our switch (our boolean variable is controlled by the switch) on dashboard and accordingly the location can be gathered.

When we will trigger our switch on our dashboard then only we will extract data and send to RPi.The lattitude,longitude and date/time will be send in 5 sec intervals so we can publish in a proper manner.

Coming again to AWS IoT core,this device needed to be registered as a new 'Thing' and new policies,jobs needed to attatch to it.The core creates certificates and keys for identification of our 'Thing' and hence connect it to AWS broker.
To check our location of our shipment we need to subscribe to proper topics in the IoT Console's MQTT Client and bravo! we can track our shipment.
Finally I would ask anyone visiting this repo for suggestions,corrections and feedback.

Happy Coding :)
