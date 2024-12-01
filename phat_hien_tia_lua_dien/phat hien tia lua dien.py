# -*- coding: utf-8 -*-
"""
Created on Fri Sep  4 23:02:17 2020

@author: HP USER
"""



import numpy as np
import cv2
import serial
import time
Fire_Reported = 0
gioi_han = 50  #độ nhạy 
fire_cascade = cv2.CascadeClassifier('train.xml')
#cascade.xml is the classifier file that contains the parameters of classifier
#checks for fire detection

ser1 = serial.Serial('COM3',9600) #kết nối với arduino
 
cap = cv2.VideoCapture(1) #khởi động camera nếu dùng cam máy tính là số 0, webcam là 1
count = 0
 
while cap.isOpened():
     
    ret, img = cap.read() #chụp 1 hình
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY) #chuyển đổi hình ảnh sang chế độ xám
    fire = fire_cascade.detectMultiScale(img, 12, 5) #Kiểm tra phát hiện có cháy tia lửa điện
    #cv2.imshow("output", output)
        
    for (x,y,w,h) in fire:
        #cv2.imshow("output", output)
        cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),2) #tạo viền cho vùng có lửa
        cv2.putText(img, 'PHAT HIEN TIA LUA DIEN', (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.4,(0,0,255), 1)
        roi_gray = gray[y:y+h, x:x+w,]
        roi_color = img[y:y+h, x:x+w]
        print( 'Fire is detected..!' + str(count)) 
        count = count + 1
        ser1.write(str.encode('p')) #write 'p' on serial COM port to arduino
        time.sleep(0.2) #wait
        break
    # Python program to create a close button
                 
    
    cv2.imshow('img', img)
    ser1.write(str.encode('s')) #write 's' if there is no fire
     
    k = cv2.waitKey(100) & 0xff
    if k == 27:
       break

ser1.close()
cap.release()

cv2.destroyAllWindows()

