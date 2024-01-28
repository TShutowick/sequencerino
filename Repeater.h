/* 
 Repeater calls a function at an interval.
 Example: flash an LED every half second

   unsigned long now;

   bool led_on = false;
   Repeater led = Repeater([]{
      led_on = !led_on;
      digitalWrite(LED_PIN, led_on); 
   },500,&now);

   void loop() {
  	now = millis();
  	led.loop();
   }
*/
class Repeater {
    public:
		// How long, in milliseconds, between callbacks.
        unsigned int interval;
		// How much time has passed since the last callback.
        unsigned long last_invocation;
		// Reference to the time in milliseconds that has passed since initialization.
        unsigned long* now;
		// Function to call at each interval.
        void (*callback)();
        Repeater(void (*_callback)() = []{}, unsigned int _interval = 0, unsigned long* _now = 0) {
            callback = _callback;
            interval = _interval;
            now = _now;
            last_invocation = *now;
        }

		// if enough time has passed, invoke the callback and reset the timer.
        void loop() {
            if (!interval || !now) return;
            if (*now - last_invocation > interval) {
                last_invocation = *now;
                callback();
            }
        }
};
