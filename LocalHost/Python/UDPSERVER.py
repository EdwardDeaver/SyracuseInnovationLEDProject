import socket
import json
import requests
import sys
import pymongo
from pymongo import MongoClient



####################
## LOAD ENV FILE ###
####################
secrets = None
with open('secrets.json', "r") as outfile:
    secrets = json.load(outfile)

####################
##   MONGO DB     ##
####################
PWD = secrets["MONGOUSR"]
USR = secrets["MONGOPWD"]
client = MongoClient('localhost', 27017,username=USR,password= PWD,authSource='admin',authMechanism='SCRAM-SHA-256')
db = client.PUBLICINSTALATION
posts = db.posts
resp = posts.create_index([ ("timeSent", 1)], unique=True )


###################
##  UDP SOCKET   ##
###################
#Based on https://wiki.python.org/moin/UdpCommunication
UDP_IP = secrets["UDP_IP"]
UDP_PORT = secrets["UDP_PORT"]
SECRETVALUE = secrets["APIKEY"]
headers = {
    'Content-Type': 'application/json',
    }
sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

##################################
#####  WRITES TO MONGODB    ######
##################################
### Takes in JSON data
### Returns True if successful, False if fails.
##################################
def writeToMongo(JSONMESSAGE):
    try:
        result = posts.insert_one(JSONMESSAGE)
        return True
    except:
        print("FAILED")
        return False
    
###################################
#WRITES TO WEB SERVER POST REQUEST#
###################################
#Takes in JSON data
#Returns True if successful, False if fails.
###################################
def writeToWebServer(JSONMESSAGE):
    data = '{ "sensor1": '+ str(JSONPACKAGE["sensor1"]) + ', "sensor2": ' + str(JSONPACKAGE["sensor2"]) + ', "timesent": ' + str(JSONPACKAGE["timeSent"]) +', "secret":"'+str(SECRETVALUE)+'"}'
    try:
        response = requests.post(secrets["ENDPOINT"], headers=headers, data=data)
        return True
    except:
        print("error occured")
        return False

## RUN INDEFINETLY
while True:
    data, addr = sock.recvfrom(500)
    JSONPACKAGE = json.loads(data)
    #WRITE TO MONGO
    writeToMongo(JSONPACKAGE)
    #WRITE TO WEB SERVER
    writeToWebServer(JSONPACKAGE)
   
