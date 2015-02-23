-- Lua OS hardware test 001

term = hardware.getDevice(0)
term:putstr("TEST COMPLETE; PRESS ANY KEY\n")
term:getch()
hardware.shutdown()
