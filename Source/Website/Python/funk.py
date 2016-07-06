from socketIO_client import SocketIO, BaseNamespace
import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BOARD)

GPIO.setup(26, GPIO.OUT) 	# PIN#26 (GPIO7) als Ausgang
GPIO.output(26, True)		# Pin#26 auf High --> macht Motor an



class LEDRotorNamespace(BaseNamespace):
    def on_connect(self,*args):
        print("connected")

    def on_maxVote(self,*args):
        print(args[0]['code'])
        value = args[0]['code']
        GPIO.output(26, False)	# stoppe den Motor
        time.sleep(15)			# warte 15 Sekunden, bis der Rotor zum Stillstand kommt
        #os.system("sudo ./codesend %d"%value)	#sende value an Arduino per Funk
        time.sleep(1)
        GPIO.output(26, True)
        
socketIO = SocketIO("localhost", 3000, LEDRotorNamespace)
lr_namespace = socketIO.define(LEDRotorNamespace, '/ledrotor')
while True:
    socketIO.wait(seconds=1)