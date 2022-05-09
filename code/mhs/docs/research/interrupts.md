## Interrupts

- Hardware Interrupts
- Software Interrupts

Interrupts respond to external events. Current process gets suspended and the Interrupt Handler gets called.
Afterwards jumping back to the suspended process.
Every Interrupt has a flag bit which is set by the hardware when an interrupt occurs.

- ISR (Interrupt Service Routine)
- NVIC (Nested Vector Interrupt Control)


[An interrupt can be triggered internally from the microcontroller (MCU) or externally,
by a peripheral. The interrupt alerts the central processing unit (CPU) to an occurrence 
such as a time-based event (a specified amount of time has elapsed or a specific time is reached, 
for example), a change of state, or the start or end of a process.](https://www.motioncontroltips.com/what-is-nested-vector-interrupt-control-nvic/)


---

## Teensy/Arduino Interrupts

```
 attachInterrupt(digitalPinToInterrupt(23), myInterrupt, RISING);
 
 void myInterrupt() {
    // Your code goes here
}
```
- shared variables need to be volatile if sharing from isr to main
- cli() -> [main_var] -> sei() need to be used if sharing from main to isr



