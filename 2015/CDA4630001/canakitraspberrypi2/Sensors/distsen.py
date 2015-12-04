import RPi.GPIO as GPIO
import time
GPIO.setmode(GPIO.BCM)

TRIG = 26
ECHO = [5,6,13,19]
distance = [0,0,0,0]
print "Distance Measurement In Progress"

GPIO.setup(TRIG,GPIO.OUT)
GPIO.setup(ECHO[0],GPIO.IN)
GPIO.setup(ECHO[1],GPIO.IN)
GPIO.setup(ECHO[2],GPIO.IN)
GPIO.setup(ECHO[3],GPIO.IN)

inf_loop = 1

while inf_loop <= 10:
        for i in range(len(ECHO)):

                GPIO.output(TRIG, False)
                time.sleep(0.25)

                GPIO.output(TRIG, True)
                time.sleep(0.00001)
                GPIO.output(TRIG, False)

                while GPIO.input(ECHO[i])==0:
                        pulse_start = time.time()

                while GPIO.input(ECHO[i])==1:
                        pulse_end = time.time()
                pulse_duration = pulse_end - pulse_start

                distance[i] = pulse_duration * 17150
                distance[i] = round(distance[i], 2)

        print "Distance sensor 1:",distance[0],"cm"
        print "Distance sensor 2:",distance[1],"cm"
        print "Distance sensor 3:",distance[2],"cm"
        print "Distance sensor 4:",distance[3],"cm"

        inf_loop= inf_loop + 1;
GPIO.cleanup()
