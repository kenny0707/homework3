from itertools import count
from urllib.error import ContentTooShortError
import paho.mqtt.client as paho
import time
import matplotlib.pyplot as plt
import numpy as np

# https://os.mbed.com/teams/mqtt/wiki/Using-MQTT#python-client


# MQTT broker hosted on local machine
mqttc = paho.Client()
# Settings for connection
# TODO: revise host to your IP
host = "172.20.10.6"
topic = "Mbed"

global data
data = {}
t = np.arange(0,5,0.1)
x = np.arange(0,5,0.1)
y = np.arange(0,5,0.1)
z = np.arange(0,5,0.1)

global flag
flag = 0

cnttt = 0

# Callbacks
def on_connect(self, mosq, obj, rc):
    print("Connected rc: " + str(rc))

def on_message(mosq, obj, msg):
    global cnttt
    global flag
    message = str(msg.payload)
    print("[Received] Topic: " + msg.topic + ", Message: " + message + "\n")
    data = message.split()
    if cnttt < 50:
        x[cnttt] = int(data[1])
        y[cnttt] = int(data[2])
        z[cnttt] = int(data[3])
        cnttt += 1
    if cnttt == 49:
        flag = 1

def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed OK")

def on_unsubscribe(mosq, obj, mid, granted_qos):
    print("Unsubscribed OK")

# Set callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_subscribe = on_subscribe
mqttc.on_unsubscribe = on_unsubscribe

# Connect and subscribe
print("Connecting to " + host + "/" + topic)
mqttc.connect(host, port=1883, keepalive=60)
mqttc.subscribe(topic, 0)

# Publish messages from Python
num = 0
while num != 5:
    mqttc.loop()
    time.sleep(1.5)
    num += 1

while flag != 1:
    mqttc.loop()
    time.sleep(0.1)

fig, ax = plt.subplots(3, 1)
ax[0].plot(t,x)
ax[0].set_xlabel('Time')
ax[0].set_ylabel('x Amplitude')
ax[1].plot(t,y)
ax[1].set_xlabel('Time')
ax[1].set_ylabel('y Amplitude')
ax[2].plot(t,z)
ax[2].set_xlabel('Time')
ax[2].set_ylabel('z Amplitude')
plt.show()

# Loop forever, receiving messages
mqttc.loop_forever()