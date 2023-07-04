#include "ButtonClickListener.h"

Button::Button(int pin) {
  buttonPin = pin;                    // Set the buttons' physical pin number --- returns HIGH or LOW
  callback = NULL;                    // Initialize the callback function pointer to NULL
  pinMode(buttonPin, INPUT_PULLUP);   // Configure the button pin as input with internal pull-up resistor because of a digital input
}

void Button::onClick(void (*cb)()) {
  callback = cb;                       // Assign the provided callback function to the callback pointer
}

void Button::checkState() {
  if (digitalRead(buttonPin) == LOW) { // If the button is pressed (LOW state)
    if (callback != NULL) {            // Check if a callback function is attached
      callback();                      // Execute the callback function
    }
    // Delay to avoid multiple triggering due to button debouncing
    delay(200);
  }
}