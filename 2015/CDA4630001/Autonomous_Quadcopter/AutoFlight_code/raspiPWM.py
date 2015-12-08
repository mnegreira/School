# External module imports
import RPi.GPIO as GPIO
import time

freq = 50

# Pin Definitons:
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

tpin=12 #throttle PWM0
ppin=5  #pitch
rpin=6  #roll
ypin=16 #yaw
mpin=26 #flight mode

GPIO.setup(tpin, GPIO.OUT)
GPIO.setup(ppin, GPIO.OUT)
GPIO.setup(rpin, GPIO.OUT)
GPIO.setup(ypin, GPIO.OUT)
GPIO.setup(mpin, GPIO.OUT)

pwmThrotle = GPIO.PWM(tpin,freq)
pwmPitch = GPIO.PWM(ppin,freq)
pwmRoll = GPIO.PWM(rpin,freq)
pwmYaw = GPIO.PWM(ypin,freq)

GPIO.output(mpin,False)

dc = 20 # duty cycle (0-100) for PWM pin

pwmThrotle.start(dc)

print("Here we go! Press CTRL+C to exit")
try:
    while True:
        kp = input("Enter [d]decrease cycle [i]increase [s]stop")
        if (kp == "d"):
                dc = dc - 5
                print "DC ",dc
        else:
                if (kp == "i"):
                        dc = dc + 5
                        print "DC ",dc
                else:
                        if (kp == "s"):
                                break
        pwmThrotle.ChangeDutyCycle(dc)

except KeyboardInterrupt: # If CTRL+C is pressed, exit cleanly:
    pwm.stop() # stop PWM
    GPIO.cleanup() # cleanup all GPIO
