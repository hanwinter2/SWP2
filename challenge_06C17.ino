#define PIN_LED 7
#define PERIOD 10000

int period_us = PERIOD;
int duty_percent = 0;
unsigned long triangle_start;

void setup() {
  digitalWrite(PIN_LED, HIGH);
  pinMode(PIN_LED, OUTPUT);

  set_period(PERIOD);
  set_duty(0);

  triangle_start = micros();
}

void loop() {
  unsigned long phase =
      (micros() - triangle_start) % 1000000UL;

  int duty;

  if (phase < 500000UL) {
    duty = phase / 5000UL;
  } else {
    duty = 100 - (phase - 500000UL) / 5000UL;
  }

  set_duty(duty);

  unsigned long cycle_start = micros();
  output_pwm_cycle(cycle_start);
}

void set_period(int period) {
  period_us = constrain(period, 100, 10000);
}

void set_duty(int duty) {
  duty_percent = constrain(duty, 0, 100);
}

void wait_elapsed(unsigned long start, unsigned long target_us) {
  while (true) {
    unsigned long elapsed = micros() - start;

    if (elapsed >= target_us) {
      return;
    }

    unsigned long remaining = target_us - elapsed;

    if (remaining > 20UL) {
      delayMicroseconds((unsigned int)(remaining - 12UL));
    }
  }
}

void output_pwm_cycle(unsigned long cycle_start) {
  unsigned long on_us =
      (unsigned long)period_us * duty_percent / 100UL;

  if (duty_percent == 0) {
    digitalWrite(PIN_LED, HIGH);
  } else if (duty_percent == 100) {
    digitalWrite(PIN_LED, LOW);
  } else {
    digitalWrite(PIN_LED, LOW);
    wait_elapsed(cycle_start, on_us);
    digitalWrite(PIN_LED, HIGH);
  }

  wait_elapsed(cycle_start, (unsigned long)period_us);
}
