#call modprobe uinput to start uinput service. Add to /etc/modules to start service at boot
import time
from time import sleep
import RPi.GPIO as GPIO
import uinput

clk = 17
dt = 18
sw = 27
clk2 = 22
dt2 = 23
sw2 = 24

GPIO.setmode(GPIO.BCM)
GPIO.setup(clk, GPIO.IN)
GPIO.setup(dt, GPIO.IN)
GPIO.setup(sw, GPIO.IN)
GPIO.setup(clk2, GPIO.IN)
GPIO.setup(dt2, GPIO.IN)
GPIO.setup(sw2, GPIO.IN)

#counter = 0
clkLastState = GPIO.input(clk)
device = uinput.Device([uinput.KEY_A, uinput.KEY_D, uinput.KEY_W, uinput.KEY_S, uinput.KEY_J, uinput.KEY_ENTER, uinput.KEY_BACKSPACE])
clkState = dtState = clk2State = dt2State = 0
time1 = time2 = 0
pressComplete = True
lastClk = 0

def sw_callback(channel):
    sw2State = GPIO.input(sw2)
    global time1, time2, pressComplete
    if channel == sw:
        device.emit_click(uinput.KEY_J)
    elif channel == sw2:
        if sw2State:
            time2 = time.time()
            time3 = time2 - time1
            #print("Rising! ", time1, time2, time3)
            if time3 >= .3:
                print("BACKSPACE")
                pressComplete = True
                time1 = time2
                device.emit_click(uinput.KEY_BACKSPACE)
            elif time3 >= .05:
                print("ENTER")
                pressComplete = True
                time1 = time2
                device.emit_click(uinput.KEY_ENTER)
        else: #voltage falls on switch press
            time0 = time.time()
            if time0 - time1 > .1 and pressComplete:
                time1 = time0
                pressComplete = False
                print("Set!")
            #print("Falling! ", time1, time2)
    #print("Switch pressed!", channel)

def rotary_callback(channel):
    global clkState, dtState
    a = GPIO.input(clk)
    b = GPIO.input(dt)
    clkState = ((clkState << 1) | a) & 0b1111
    dtState = ((dtState << 1 ) | b) & 0b1111
    if (clkState == 0b0 or clkState == 0b1 or clkState == 0b1001 or clkState == 0b1000) and dtState == 0b11:
        device.emit_click(uinput.KEY_A)
        print("Backwards ", bin(clkState), bin(dtState))
    elif clkState == 0b11 and (dtState == 0b1 or dtState == 0b0 or dtState == 0b1001 or dtState == 0b1000):
        device.emit_click(uinput.KEY_D)
        print("Forwards ", bin(clkState), bin(dtState))
    #else:
    #    print("no op ", bin(clkState), bin(dtState))
def rotary_callback2(channel):
    global clk2State, dt2State
    a = GPIO.input(clk2)
    b = GPIO.input(dt2)
    clk2State = ((clk2State << 1) | a) & 0b1111
    dt2State = ((dt2State << 1 ) | b) & 0b1111
    if (clk2State == 0b0 or clk2State == 0b1 or clk2State == 0b1001 or clk2State == 0b1000) and dt2State == 0b11:
        device.emit_click(uinput.KEY_W)
        print("Backwards ", bin(clk2State), bin(dt2State))
    elif clk2State == 0b11 and (dt2State == 0b1 or dt2State == 0b0 or dt2State == 0b1001 or dt2State == 0b1000):
        device.emit_click(uinput.KEY_S)
        print("Forwards ", bin(clk2State), bin(dt2State))
 
counter = 0
clkLastState = GPIO.input(clk)
GPIO.add_event_detect(clk, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(dt, GPIO.BOTH, callback=rotary_callback)
GPIO.add_event_detect(clk2, GPIO.BOTH, callback=rotary_callback2)
GPIO.add_event_detect(dt2, GPIO.BOTH, callback=rotary_callback2)
GPIO.add_event_detect(sw, GPIO.BOTH, callback=sw_callback, bouncetime=300)
GPIO.add_event_detect(sw2, GPIO.BOTH, callback=sw_callback)
#input("Enter anything\n")
#GPIO.cleanup()
try:
    while True:
        sleep(3600)
except KeyboardInterrupt:
    print("DONE\n")
    GPIO.cleanup()
