import serial

import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

from pyfcm import FCMNotification

import time


#for Noticitation
APIKEY = "Your Server Key"
TOKEN = "Your Token"
push_service = FCMNotification(APIKEY)

def sendMessage(body, title):
    data_message = {
        "body":body,
        "title":title
        }
    result = push_service.single_device_data_message(registration_id=TOKEN, data_message=datamessage)
    print(result)

#for Database
cred = credentials.Certificate("smart-trash-can-application-firebase-adminsdk-rz79l-53889be09d.json")
firebase_admin.initialize_app(cred,{
        'databaseURL' : 'https://smart-trash-can-application-default-rtdb.firebaseio.com/'
    })

#for Serial Comunication
PORT = 'COM3'
BaudRate = 9600
ARD= serial.Serial(PORT,BaudRate)
 
push_service = FCMNotification(APIKEY)

def Decode(A):
    A = A.decode()
    A = str(A)
    fillData = A[1:4]
    airData = A[5:9]
    timeData = time.strftime('%Y%m%d%H%M%S', time.localtime(time.time()))
    dir = db.reference('Data/'+'20211219003103') #+ timeData)
    dir.update({'time' : timeData})
    dir.update({'fillData' : fillData})
    dir.update({'airData' : airData})
    return  fillData + ' ' + airData

def Ardread():

    if ARD.readable():
        LINE = ARD.readline()
        code=Decode(LINE)
        print(code)
        return code
    else :
        print("읽기 실패 from _Ardread_")


#Execution area
sendMessage("for test", "test notification")
while (True):
    Ardread()
    

