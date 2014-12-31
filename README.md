FreezeTime
==========

Arduino - based kitchen multi timer.

This is a simple project I started to provide our kitchen with the ability to track two (but, in a theory, with many more!) kitchen timers running simultaneously. You know, to satistfy your coffee brewing needs, when your SO is busy timing the dinner or what not.

My first approach was to buy another kitchen timer, but after some thought I decided that would be seriously uncool.
This is the second approach. :)

Some basic info:
- the schema utilizes LCD display from Nokia 5110. It's very cheap, it's very bright and easy to work with. But beware - it runs on 3V only, so if you try to reprogramm the microprocessor using USB, you MIGHT destroy it (I tried it, however, and mine survived quite a prolonged exposure to 5V). Reprogramming using external programmer (or simple 5V -> 3V level shifter) is preferred.
- the board for Eagle is 2-sided, which makes things slightly more complicated. There may be ways how to rearrange the board and make it single sided, but I wanted to have controls on the one side of the board and batteries & connector on the other side. The board is routed for a simple CNC router with 0.3 engraving V tip.
- the schematics is pretty straighforward, with microprocessor, binary encoder, LCD and switch. The only thing that is not absolutely basic here is the charge pump to sound the buzzer. I did not found a good buzzer with reasonable 3V input, so I use a simple 7660 charge pump to boost the voltage to 5V. Also, MOSFET is used to drive the buzzer, as it can sink a reasonable amount of current that would not be healthy for ATMEL / Arduino pins.
- if you decide to change the tone the buzzer beeps, beware - I encountered strange behaviour (microprocessor resetting itself...) with some frequencies.

- STL file for 3d printed enclosure will be added later.



