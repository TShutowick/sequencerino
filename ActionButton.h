#ifndef ARDUINO
#include "t/lib/MockArduino.h"
#endif

class ActionButton {
  public:
    bool pressed = false;
    int pin;
    void (*on_press)();
    void (*on_release)();
    void loop() {
        bool pressed_now = digitalRead(pin);
        // do nothing unless the state has changed.
        if (pressed_now == pressed) return;
        pressed = pressed_now;
        if (pressed) {
          on_press();
        } else {
          on_release();  
        }
    }
    ActionButton(int _pin = 0, void (*_on_press)() = []{}, void (*_on_release)() = []{}) {
      pin = _pin;
      on_press = _on_press;
      on_release = _on_release;
    }
};
