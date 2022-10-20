# Morsesender
Arduino sketch for sending Morse code.

This sketch reads characters from the serial line (e.g. from a PC connected to the Arduino via a serial terminal program) and sends them as Morse code to a digital output pin.

Additional function: there are two predefined text messages which can be sent by using the shortcuts "!1" or "!2". More text messages can simply be added in the source code.

This is only a simple and straight forward implementation. More improvements can be added easily.



##### TODO

- A more precise timing could be achieved by using a timer interrupt function for sending the Morse code symbols. However, in this case the output ports should be written directly and not by using the `digitalWrite()` function.

