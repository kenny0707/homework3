# homework3

##erpc generate cpp and h hile for mbed
use erpcgen to generate codes,copy controller_server.cpp,controller_server.h,controller.h,stm32l475e_iot01_accelero.h to mbed  
copy main file to main  
add library https://github.com/ARMmbed/wifi-ism43362 and https://gitlab.larc-nthu.net/ee2405_2022/paho_mqtt.git with master to mbed  

##connect wifi and start mosquitto
connect mbed and PC to Kenny  
use powershell and type cd C:\Users\<User Name>\Mbed Programs and & 'C:\Program Files\mosquitto\mosquitto.exe' -c mosquitto.conf -v  

##use python to send remote call to mbed
open control_client.py in git bash,use git bash and then type cd ~/Mbed\ Programs/homework3 and python control_client.py <your port>   
type 1,mbed should start publish accelerometer data.type 0,mbed should stop publish  
  
##use python to get data from mbed and plot the accelerometer data
open mqtt_client.py in VS code and run  
type 1 in git bash,VS code terminal should start receive data  
type 0 in git bash,VS code terminal should stop receive data and then generate three plots  
after generate plots,mqtt_client.py start loop forever mode  
  

