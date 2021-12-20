import serial

import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

from pyfcm import FCMNotification

import time


#for Noticitation
APIKEY = "AAAACnMBZZU:APA91bFzQm-_t0iKH6btv6y3vtxdJqUMfzreRLePbgiY_De9gOB94FVV7vTFkytxpe4VkHnQBMEXZC0f5LKwHnjVu7TYOJ3lyi1QAAJvBlHA1T_0scaFRItIpX4irHbAidjZMIMaMu1d"
TOKEN = "djq_H7BjRxeOswWm4ZGfoY:APA91bHhstvMPW3C_aWFCtU1-wpGKahOtRm8meARM-MwL9k2WfAVaVOr8N3E8_CSwtooOIg-R5VNFgDSiztuIGZrq9eKwgSOrIKwj8fRlR7uCSPUOMuroWjwPBYrR53c6-EFE9HO5NJW"
push_service = FCMNotification(APIKEY)

def sendMessage(body, title):
    data_message = {
        "body":body,
        "title":title
        }
    result = push_service.single_device_data_message(registration_id=TOKEN, data_message=data_message)
    print(result)

#for Database
cred = credentials.Certificate("smart-trash-can-application-firebase-adminsdk-rz79l-7428c90d74.json")
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
    

