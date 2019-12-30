# SyracuseInnovationLEDProject

# Localhost Requirements 
## Physical Materials

* 2x Adarfruit Huzzah ESP8266 boards
* 2x Ultrasonic Distance Sensor - 3V or 5V - HC-SR04 compatible - RCWL-1601 
* 1x Mac OS device
* 1x FadeCandy
* 4x WS8211 Neopixel lights (60 LEDS/M)

## Software Requirements
* MongoDB Community Edition (https://www.mongodb.com/what-is-mongodb)
* Brew (https://brew.sh) 
* Mac OS version 10.13.6
* Python 3
* CP210x USB to UART Bridge VCP Drivers from SiLabs https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

## MAC OS CONFIG SETTINGS
* Firewall should be on and set to block all incomming connections. 

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

* OF App is started by
I chmod +x The app
I then made a desktop app for the app in the config autostart folder
I then used the exec to point to the app . DO NOT NEED A dot in frnt of it.
https://learn.sparkfun.com/tutorials/how-to-run-a-raspberry-pi-program-on-startup/method-2-autostart



* Fadecandy is autoran using this procedure https://learn.adafruit.com/1500-neopixel-led-curtain-with-raspberry-pi-fadecandy/fadecandy-server-setup



* Pythonscript will be ran by cron tab https://www.raspberrypi-spy.co.uk/2013/07/running-a-python-script-at-boot-using-cron/

