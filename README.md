# Animated OLED Robot Eyes

A lightweight Arduino project that generates a pair of smooth, animated robot eyes on a 128x32 I2C OLED display. Ideal for adding personality to robotics projects, desktop companions, or DIY electronics.

---

## ✨ Features

* **Wake-Up Animation:** A stylish startup sequence where the eyes gradually open.
* **Smooth Motion:** Uses interpolation to ensure the eyes look around fluidly rather than snapping rigidly.
* **Natural Blinking:** A built-in state machine handles random blinking, including occasional double-blinks for a more life-like look.
* **Customizable Shapes:** Easy-to-adjust parameters for changing the eye roundness (from sharp rectangles to soft pills/ovals).

---

## 🛠️ Hardware Requirements

* **Microcontroller:** Any Arduino-compatible board (Uno, Nano, ESP32, etc.)
* **Display:** 128x32 I2C SSD1306 OLED Display
* **Connections:** * VCC ➡️ 5V / 3.3V
    * GND ➡️ GND
    * SCL ➡️ I2C Clock Pin
    * SDA ➡️ I2C Data Pin

---

## 📦 Software Dependencies

Make sure you have the following libraries installed via the Arduino Library Manager:

* `Adafruit_SSD1306`
* `Adafruit_GFX`
* `Wire` (Built-in)

---

## ⚙️ Customization

You can easily tweak the look and feel by changing the constants at the top of the code:

```cpp
const int eyeRadius = 3;       // 0 = Sharp boxes, 6 = Round ovals
const float smoothness = 0.1;  // Higher = faster looking, Lower = slower/smoother
