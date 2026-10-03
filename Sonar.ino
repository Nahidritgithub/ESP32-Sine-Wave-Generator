#include <Arduino.h>
#include <math.h>

#define TURBIDITY_PIN A0
#define TEMP_PIN A1
#define DEPTH_PIN A2

#define STEPS 200

// Stable ADC reading
int readStableADC(int pin) {
  long total = 0;

  for (int i = 0; i < 20; i++) {
    total += analogRead(pin);
    delay(100);
  }

  return total / 20;
}

// Hann window
float hannWindow(int i, int N) {
  return 0.5 * (1.0 - cos(2.0 * PI * i / (N - 1)));
}

// Software waveform simulation
void generateChirp(float startFreq,
                   float endFreq,
                   float durationMs,
                   float amplitude) {

  Serial.println("\n------ SIMULATED LFM WAVEFORM ------");
  Serial.println("Time(ms)\tFreq(Hz)\tWindow\tSignal");

  float duration = durationMs / 1000.0;
  float k = (endFreq - startFreq) / duration;

  for (int i = 0; i < STEPS; i++) {

    float t = duration * i / (STEPS - 1);

    float frequency = startFreq + k * t;

    float window = hannWindow(i, STEPS);

    float phase = 2.0 * PI *
                  (startFreq * t + 0.5 * k * t * t);

    float signal = (amplitude / 100.0) *    
                   window * sin(phase);

    // Display every 20th calculated sample
    if (i % 20 == 0) {

      Serial.print(t * 1000, 2);
      Serial.print("\t");

      Serial.print(frequency, 1);
      Serial.print("\t");

      Serial.print(window, 3);
      Serial.print("\t");

      Serial.println(signal, 4);
    }
  }
}

void setup() {

  Serial.begin(115200);

  analogReadResolution(12);

  pinMode(TURBIDITY_PIN, INPUT);
  pinMode(TEMP_PIN, INPUT);
  pinMode(DEPTH_PIN, INPUT);

  Serial.println("ADAPTIVE SONAR SOFTWARE SIMULATION");
}

void loop() {

  // Read three potentiometers
  int tADC = readStableADC(TURBIDITY_PIN);
  int tempADC = readStableADC(TEMP_PIN);
  int dADC = readStableADC(DEPTH_PIN);

  // Convert into simulated environmental values
  int turbidity = map(tADC, 0, 4095, 0, 100);

  float temperature =
      10.0 + (tempADC / 4095.0) * 25.0;

  int depth = map(dADC, 0, 4095, 0, 200);

  Serial.println("\n------ ENVIRONMENT ------");

  Serial.print("Turbidity: ");
  Serial.print(turbidity);
  Serial.println(" NTU");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Depth: ");
  Serial.print(depth);
  Serial.println(" m");

  // Normalization
  float U = turbidity / 100.0;
  float D = depth / 200.0;

  float T;

  if (temperature >= 15 && temperature <= 30)
    T = 1.0;
  else if (temperature < 15)
    T = temperature / 15.0;
  else
    T = (35.0 - temperature) / 5.0;

  T = constrain(T, 0.0, 1.0);

  // Waveform parameters
  float frequency;
  float bandwidth;
  float duration;
  float amplitude;

  String mode;

  if (turbidity > 70 && depth > 100) {

    mode = "SEARCH";

    frequency = 4000 - 2500 * (0.6 * U + 0.4 * D);
    bandwidth = 1500 - 1000 * (0.7 * U + 0.3 * D);
    duration = 50 + 150 * (0.6 * U + 0.4 * D);
    amplitude = 50 + 40 * (0.7 * U + 0.3 * D);

  } else if (turbidity < 40 && depth < 100 &&
             temperature >= 15 && temperature <= 30) {

    mode = "HIGH RESOLUTION";

    frequency = 1500 + 2500 * T - 1000 * U - 500 * D;
    bandwidth = 500 + 1200 * T - 600 * U;
    duration = 100 - 50 * T + 40 * U;
    amplitude = 65 + 20 * U + 10 * D;

  } else {

    mode = "BALANCED";

    frequency = 1000 + 2000 * T - 1000 * U - 500 * D;
    bandwidth = 300 + 1000 * T - 500 * U;
    duration = 80 + 100 * U + 50 * D;
    amplitude = 55 + 25 * U + 15 * D;
  }

  frequency = constrain(frequency, 500, 4000);
  bandwidth = constrain(bandwidth, 100, 1500);
  duration = constrain(duration, 50, 200);
  amplitude = constrain(amplitude, 40, 100);

  float startFreq = constrain(
      frequency - bandwidth / 2, 500, 4000);

  float endFreq = constrain(
      frequency + bandwidth / 2, 500, 4000);

  Serial.println("\n------ WAVEFORM PARAMETERS ------");

  Serial.print("Mode: ");
  Serial.println(mode);

  Serial.print("Center Frequency: ");
  Serial.println(frequency);

  Serial.print("Bandwidth: ");
  Serial.println(bandwidth);

  Serial.print("Pulse Duration: ");
  Serial.println(duration);

  Serial.print("Amplitude: ");
  Serial.println(amplitude);

  Serial.print("Start Frequency: ");
  Serial.println(startFreq);

  Serial.print("End Frequency: ");
  Serial.println(endFreq);

  // Generate mathematical waveform
  generateChirp(startFreq, endFreq, duration, amplitude);

  delay(1000);
}