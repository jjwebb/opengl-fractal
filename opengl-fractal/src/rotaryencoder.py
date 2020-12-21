#call modprobe uinput to start uinput service. Add to /etc/modules to start service at boot
import time
from time import sleep
import RPi.GPIO as GPIO
import uinput

#BCM pin numbers for Pi GPIO header
clk = 11
dt = 8
sw = 7

clk2 = 14
dt2 = 15
sw2 = 23

clk3 = 17
dt3 = 27
sw3 = 22

clk4 = 10
dt4 = 9
sw4 = 25

clk5 = 5
dt5  = 6
sw5  = 13

#Set up GPIO pins. Set internal pullup for encoders with no external resistor
GPIO.setmode(GPIO.BCM)
GPIO.setup(clk, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(sw, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(clk2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt2, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(sw2, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(clk3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(sw3, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(clk4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt4, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(sw4, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(clk5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(dt5, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(sw5, GPIO.IN, pull_up_down=GPIO.PUD_UP)

#Pair pins with their respective keys
pairs = {sw   : (uinput.KEY_H, uinput.KEY_J),
         sw2  : (uinput.KEY_I, uinput.KEY_C),
         sw3  : (uinput.KEY_BACKSPACE, uinput.KEY_BACKSPACE),
         sw4  : (uinput.KEY_ENTER, uinput.KEY_ENTER),
         sw5  : (uinput.KEY_R, uinput.KEY_DOT, uinput.KEY_T),
         clk  : (clk, dt, uinput.KEY_A, uinput.KEY_D),
         dt   : (clk, dt, uinput.KEY_A, uinput.KEY_D),
         clk2 : (clk2, dt2, uinput.KEY_W, uinput.KEY_S),
         dt2  : (clk2, dt2, uinput.KEY_W, uinput.KEY_S),
         clk3 : (clk3, dt3, uinput.KEY_LEFT, uinput.KEY_RIGHT),
         dt3  : (clk3, dt3, uinput.KEY_LEFT, uinput.KEY_RIGHT),
         clk4 : (clk4, dt4, uinput.KEY_UP, uinput.KEY_DOWN),
         dt4  : (clk4, dt4, uinput.KEY_UP, uinput.KEY_DOWN),
         clk5 : (clk5, dt5, uinput.KEY_P, uinput.KEY_N),
         dt5  : (clk5, dt5, uinput.KEY_P, uinput.KEY_N)
         }

#We need to tell uinput which keys we want to use
keys = [uinput.KEY_A, uinput.KEY_D, uinput.KEY_W, uinput.KEY_S,
        uinput.KEY_J, uinput.KEY_H, uinput.KEY_I, uinput.KEY_C,
        uinput.KEY_ENTER, uinput.KEY_BACKSPACE, uinput.KEY_T,
        uinput.KEY_UP, uinput.KEY_DOWN, uinput.KEY_LEFT, uinput.KEY_RIGHT,
        uinput.KEY_P, uinput.KEY_N, uinput.KEY_R, uinput.KEY_DOT]

device = uinput.Device(keys)

#These hold the last rotation state values of the encoders
rotary1 = [0, 0]
rotary2 = [0, 0]
rotary3 = [0, 0]
rotary4 = [0, 0]
rotary5 = [0, 0]

#Pair last states with their respective pins
states = {clk  : rotary1,
          dt   : rotary1,
          clk2 : rotary2,
          dt2  : rotary2,
          clk3 : rotary3,
          dt3  : rotary3,
          clk4 : rotary4,
          dt4  : rotary4,
          clk5 : rotary5,
          dt5  : rotary5}

time1 = time2 = 0
pressComplete = True
forwards = True

#Switch callback -- keeping track of time allows us to both filter out noise
#And assign multiple keys to one switch
def sw_callback(channel):
    swState = GPIO.input(channel)
    global time1, time2, pressComplete
    if swState:
        time2 = time.time()
        time3 = time2 - time1
        if time3 > 2 and channel == sw5:
            pressComplete = True
            time1 = time2
            device.emit_click(pairs[channel][2])
        elif time3 >= .3:
            pressComplete = True
            time1 = time2
            device.emit_click(pairs[channel][0])
        elif time3 >= .05:
            pressComplete = True
            time1 = time2
            device.emit_click(pairs[channel][1])
    else: #voltage falls on switch press
        time0 = time.time()
        if time0 - time1 > .1 and pressComplete:
            time1 = time0
            pressComplete = False
            #print("Set!")

#Using encoders without capacitors results in a lot of bounce/noise, we keep track of
#the last few states and filter this out in software 
def rotary_callback(channel):
    global states
    a = GPIO.input(pairs[channel][0])
    b = GPIO.input(pairs[channel][1])
    states[channel][0] = ((states[channel][0] << 1) | a) & 0b1111
    states[channel][1] = ((states[channel][1] << 1 ) | b) & 0b1111
    clk2State = states[channel][0]
    dt2State  = states[channel][1]
    if (clk2State == 0b0 or clk2State == 0b1 or clk2State == 0b1001 or clk2State == 0b1000) and dt2State == 0b11:
        device.emit_click(pairs[channel][2])
        #print(channel, " Backwards ", bin(clk2State), bin(dt2State))
    elif clk2State == 0b11 and (dt2State == 0b1 or dt2State == 0b0 or dt2State == 0b1001 or dt2State == 0b1000):
        device.emit_click(pairs[channel][3])
        #print(channel, " Forwards ", bin(clk2State), bin(dt2State))

#Bind callbacks to the appropriate pins
GPIO.add_event_detect(clk, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(sw, GPIO.BOTH, callback=sw_callback)

GPIO.add_event_detect(clk2, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt2, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(sw2, GPIO.BOTH, callback=sw_callback)

GPIO.add_event_detect(clk3, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt3, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(sw3, GPIO.BOTH, callback=sw_callback)

GPIO.add_event_detect(clk4, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt4, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(sw4, GPIO.BOTH, callback=sw_callback)

GPIO.add_event_detect(clk5, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt5,  GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(sw5,  GPIO.BOTH, callback=sw_callback)

print("Rotary encoder script is running, press Ctrl-C to exit...")

#Main loop
try:
    while True:
        sleep(3600)
except KeyboardInterrupt:
    print("DONE\n")
    GPIO.cleanup()
