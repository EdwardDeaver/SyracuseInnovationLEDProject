# SyracuseInnovationLEDProject

# Localhost Requirements 
## Physical Materials

* 2x Adarfruit Huzzah ESP8266 boards
* 2x Ultrasonic Distance Sensor - 3V or 5V - HC-SR04 compatible - RCWL-1601
* 1x Raspberry Pi 4
* 1x FadeCandy
* 4x WS8211 Neopixel lights (60 LEDS/M)

## Software Requirements
* CP210x USB to UART Bridge VCP Drivers from SiLabs https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
* Debian Buster

# Server Requirements
## Heroku
* NodeJS 
* Secret key set in the Heroku instance configuration

# Website Requirements
## Github IO
* Nothing. 

# AWS SETUP
* Sign up for AWS
* Create an IAM User in the group for DynamoDB access, copy your Key and Access key to a text file to use later 
* Install AWS CLI https://aws.amazon.com/cli/
* Install Boto3 https://boto3.amazonaws.com/v1/documentation/api/latest/guide/quickstart.html
* Run 'aws configure' Put in your AWS Access key and other key there, and set region to us-east-1.
* Go to DynamoDB and create table with the name 'instalationdata' and the key (string) 'timesent'
* Now set your table name in the secrets file. 

# AUTOSTART

* The OF App is started  via a desktop file placed in the autostart folder (.config/autostart)
https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/method-2-autostart
* Python Script is started by a bash script that is ran via a desktop file places in the autostart folder (.config/autostart)
https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/method-2-autostart
* Fadecandy is autoran using this procedure https://learn.adafruit.com/1500-neopixel-led-curtain-with-raspberry-pi-fadecandy/fadecandy-server-setup

