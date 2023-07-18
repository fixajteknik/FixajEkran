#ifndef ButtonClickListener_h
#define ButtonClickListener_h

#include <Arduino.h>

class Button {
private:
  int buttonPin;        // The pin connected to the button
  void (*callback)();   // Pointer to the callback functions

public:
  /**
   * Initializes a new instance of the Button class.
   * @param pin: The pin number connected to the button.
   */
  Button(int pin);

  /**
   * Attaches a callback function to the button's click event.
   * @param cb: The callback function to be executed when the button is clicked.
   */
  void onClick(void (*cb)());

  /**
   * Checks the state of the button and executes the callback function if the button is clicked.s
   */
  void checkState();
};

#endif
