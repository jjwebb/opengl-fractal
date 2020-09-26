#call modprobe uinput to start uinput service. Add to /etc/modules to start service at boot
import RPi.GPIO as GPIO
from time import sleep
import uinput

clk = 17
dt = 18
sw = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(clk, GPIO.IN)
GPIO.setup(dt, GPIO.IN)
GPIO.setup(sw, GPIO.IN)

#counter = 0
clkLastState = GPIO.input(clk)
device = uinput.Device([uinput.KEY_A, uinput.KEY_D, uinput.KEY_J])

def sw_callback(channel):
    device.emit_click(uinput.KEY_J)
    print("Switch pressed!")

def rotary_callback(channel):
    global clkLastState
    global counter
    """
    try:
        clkState = GPIO.input(clk)
        if clkState !=clkLastState:
            dtState = GPIO.input(dt)
            if dtState != clkState:
                counter += 1
            else:
                counter -=1
        print (counter)
        clkLastState = clkState
        #sleep(.01)
    except:
        print("Exception!")
    finally:
        #print("Ending")
    """
#    events = (uinput.KEY_A, uinput.KEY_D)
    clkState = GPIO.input(clk)
    dtState = GPIO.input(dt)
    if dtState == 1 and clkState == 1:
        print("Backwards, DT ", dtState, " CLK ", clkState)
 #       with uinput.Device(events) as device:
           # sleep(.1)
        device.emit_click(uinput.KEY_A)
    elif dtState == 0 and clkState == 1:
        print("Forwards, DT ", dtState, " CLK ", clkState)
        #with uinput.Device(events) as device:
            #sleep(.1)
        device.emit_click(uinput.KEY_D)
    #else:
    #    print("No op, DT ", dtState, " CLK ", clkState)
counter = 0
clkLastState = GPIO.input(clk)
GPIO.add_event_detect(clk, GPIO.RISING, callback=rotary_callback, bouncetime=20)
GPIO.add_event_detect(sw, GPIO.RISING, callback=sw_callback, bouncetime=300)
#input("Enter anything\n")
#GPIO.cleanup()
try:
    while True:
        sleep(3600)
except KeyboardInterrupt:
    print("DONE\n")
    GPIO.cleanup()
