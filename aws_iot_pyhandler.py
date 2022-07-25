import serial
import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

#setting up serial communication between RPi and Arduino through USB Port 
print("Trying to connect to Serial.")
while True:
    try:
        ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1.0) #port number we need to find accordingly
        print("Successfully connected to Serial.")
        break
    except serial.SerialException:
        print("Could not connect to Serial. Retrying in 1 second...")
        time.sleep(1)


# wait after setup
print("Waiting for 3 seconds...")
time.sleep(3)
ser.reset_input_buffer()
updater.start_polling()
print("OK. Main loop is starting.")


#setting up connections with AWS IoT Core
myMQTTClient = AWSIoTMQTTClient("ShipmentTrackerID") #random key, if another connection using the same key is opened the previous one is auto closed by AWS IOT
myMQTTClient.configureEndpoint("a2zfh5fqze9la8-ats.iot.us-east-1.amazonaws.com", 8883) #endpoint and port number

myMQTTClient.configureCredentials("/home/pi/AWSIoT/root-ca.pem", "/home/pi/AWSIoT/private.pem.key", "/home/pi/AWSIoT/certificate.pem.crt") #locations of thing certificates and keys

myMQTTClient.configureOfflinePublishQueueing(-1) # Infinite offline Publish queueing
myMQTTClient.configureDrainingFrequency(2) # Draining: 2 Hz
myMQTTClient.configureConnectDisconnectTimeout(10) # 10 sec
myMQTTClient.configureMQTTOperationTimeout(5) # 5 sec
print ('Initiating Realtime Data Transfer From Raspberry Pi...')
myMQTTClient.connect()



#running main loop
try:
    while True:
        time.sleep(0.01)
        if serial.in_waiting>0:
            lattitude=ser.readline().decode('utf-8').rstrip() #reading from arduino
            print(lattitude)
            #now publish this data to a topic
            myMQTTClient.publish(topic="shipmenttracker/lattitude",
                                QoS=1,
                                payload='{"Lattitude":"'+str(lattitude)+'"}') #paylaod can be json document also
        else:
            ser.reset_input_buffer()    
        time.sleep(5)  #synchronizing with arduino
        if serial.in_waiting>0:
            longitude=ser.readline().decode('utf-8').rstrip() #reading from arduino
            print(longitude)
            #now publish this data to a topic
            myMQTTClient.publish(topic="shipmenttracker/longitude",
                                QoS=1,
                                payload='{"Longitude":"'+str(longitude)+'"}') #paylaod can be json document also
        else:
            ser.reset_input_buffer()      
        time.sleep(5)  #synchronizing with arduino
        if serial.in_waiting>0:
            datetime=ser.readline().decode('utf-8').rstrip() #reading from arduino
            print(datetime)
            #now publish this data to a topic
            myMQTTClient.publish(topic="shipmenttracker/datetime",
                                QoS=1,
                                payload='{"Date&Time":"'+str(datetime)+'"}') #paylaod can be json document also
        else:
            ser.reset_input_buffer() 
except KeyboardInterrupt:
    print("---")
    print("Closing Serial communication.")
    ser.close()
    print("Stopping Telegram updater, wait a few seconds...")
    updater.stop()
    print("End of program.")                            

# if we get '*' it is an invalid parameters(for not detecting any signal from satellites)
#now we will go to AWS MQTT Client to subscribe to our these three topics and see results on AWS IoT Console by simultaneously running this program on our device
#this project can be more enhanced by using device shadows and device jobs for commercial use :) 
