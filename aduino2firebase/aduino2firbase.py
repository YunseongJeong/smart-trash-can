import serial

import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

import time

cred = credentials.Certificate("smart-trash-can-application-firebase-adminsdk-rz79l-53889be09d.json")
firebase_admin.initialize_app(cred,{
        'databaseURL' : 'https://smart-trash-can-application-default-rtdb.firebaseio.com/'
    })


PORT = 'COM3'
BaudRate = 9600
ARD= serial.Serial(PORT,BaudRate)

def Decode(A):
    A = A.decode()
    A = str(A)
    fillData = A[1:4]
    airData = A[5:8]
    timeData = time.strftime('%Y%m%d%H%M%S', time.localtime(time.time()))
    dir = db.reference('Data/' + timeData)
    dir.update({'time' : timeData})
    dir.update({'fillData' : fillData})
    dir.update({'airData' : airData})
                
    return  fillData + ' ' + airData
def Ardread(): # return list [Ard1,Ard2]

    if ARD.readable():
        LINE = ARD.readline()
        code=Decode(LINE)
        print(code)
        return code
    else :
        print("읽기 실패 from _Ardread_")

while (True):
    Ardread()
