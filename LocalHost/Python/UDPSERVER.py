import socket
import json
import requests
#Based on https://wiki.python.org/moin/UdpCommunication
UDP_IP = "127.0.0.1"
UDP_PORT = 11999
SECRETVALUE = ""#REMOVED FROM GIT REPO
headers = {
    'Content-Type': 'application/json',
    }
sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    print(data)
    JSONPACKAGE = json.loads(data)

    data = '{ "sensor1": '+ str(JSONPACKAGE["sensor1"]) + ', "sensor2": ' + str(JSONPACKAGE["sensor2"]) + ', "secret":"'+str(SECRETVALUE)+'"}'
    print(data)
    response = requests.post('https://serene-tundra-34002.herokuapp.com/sendsensorsata', headers=headers, data=data)
    print(response.status_code)
    print ("received message:", JSONPACKAGE)
