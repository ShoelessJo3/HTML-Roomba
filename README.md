# HTML-Roomba

So this arduino code is designed for a nodemcu ESP-32s

The nodemcu is attached to the Roomba and writes commands to it 
(Documentation can be found here: https://cdn-shop.adafruit.com/datasheets/create_2_Open_Interface_Spec.pdf)

The nodemcu acts as a wifi access-point you can connect to with a local web page.  When you click on buttons on the page the nodemcu recieves html requests which turn into drive directions for the roomba
(Ip of the page is http://192.168.4.1)


It's a pretty simple base to work with, you can add your own buttons for a manipulator to the webpage


There's also a function for writing digits to the Roomba's display

In the future the webpage could also return sensor data from the roomba (like battery level etc)

# Here's how to hook up your roomba
![alt text](https://i.imgur.com/pVh2iMH.png) 

Connect 6 to GND on the nodemcu 

Connect 3 to Pin 23 on the nodemcu    

Connect 5 to Pin 22 on the nodemcu 

Connect 4 to Pin 21 on the nodemcu

You should also add a usb battery pack to power the nodemcu so your computer doesn't need to be attached to the Roomba

# These are the pins on the nodemcu board

![alt text](http://www.shenzhen2u.com/image/catalog/Module/NodeMCU-32S/nodemcu_32s_pin.png)

