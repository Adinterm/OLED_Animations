#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Eye Shape
const int eyeW = 30; 
const int eyeH = 14; 
const int eyeSpace = 44; 

// Physics & States
float curX = 0, curY = 0;
float tarX = 0, tarY = 0;
float speed = 0.15;
int state = 0; // 0: Idle, 1: Angry Focus

unsigned long stateTimer = 0;
unsigned long actionTimer = 0;
unsigned long blinkTimer = 0;
bool isBlinking = false;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // CRITICAL: Increase I2C speed to 400kHz for smoother refresh
  Wire.setClock(400000); 
  
  display.clearDisplay();
  stateTimer = millis();
}

void drawRobotEyes(float x, float y, bool angry, bool blinking) {
  display.clearDisplay();
  
  int leftX = (SCREEN_WIDTH / 2) - (eyeSpace / 2) - (eyeW / 2) + (int)x;
  int rightX = (SCREEN_WIDTH / 2) + (eyeSpace / 2) - (eyeW / 2) + (int)x;
  int eyeYPos = (SCREEN_HEIGHT / 2) - (eyeH / 2) + (int)y;

  if (blinking) {
    // Draw a thin slit for the blink
    display.fillRoundRect(leftX, (SCREEN_HEIGHT/2) - 1, eyeW, 3, 1, SSD1306_WHITE);
    display.fillRoundRect(rightX, (SCREEN_HEIGHT/2) - 1, eyeW, 3, 1, SSD1306_WHITE);
  } else {
    display.fillRoundRect(leftX, eyeYPos, eyeW, eyeH, 2, SSD1306_WHITE);
    display.fillRoundRect(rightX, eyeYPos, eyeW, eyeH, 2, SSD1306_WHITE);

    if (angry) {
      // Slanted Angry Brows
      display.fillTriangle(leftX, eyeYPos, leftX + eyeW, eyeYPos, leftX + eyeW, eyeYPos + 7, SSD1306_BLACK);
      display.fillTriangle(rightX, eyeYPos, rightX + eyeW, eyeYPos, rightX, eyeYPos + 7, SSD1306_BLACK);
    }
  }
  display.display();
}

void loop() {
  unsigned long now = millis();

  // 1. PHYSICS ENGINE: Always running, never blocked by delays
  // Using a slightly lower lerp for "silky" movement
  curX += (tarX - curX) * speed;
  curY += (tarY - curY) * speed;

  // 2. NON-BLOCKING BLINK LOGIC
  if (isBlinking && (now - blinkTimer > 120)) {
    isBlinking = false;
  }

  // 3. STATE MACHINE
  if (state == 0) { // IDLE
    if (now - actionTimer > 2000) {
      if (random(2) == 0) {
        isBlinking = true;
        blinkTimer = now;
      } else {
        tarX = (random(2) == 0) ? -18 : 18;
        tarY = (random(2) == 0) ? -5 : 5;
        speed = 0.08; // Smooth, slow glance
      }
      actionTimer = now;
    }
    if (now - stateTimer > 6000) { state = 1; stateTimer = now; }
  } 
  else if (state == 1) { // ANGRY
    speed = 0.25; // Snappy but controlled
    if (now - actionTimer > 600) {
      tarX = random(-6, 7);
      tarY = random(1, 5); // Focused downwards
      actionTimer = now;
    }
    if (now - stateTimer > 8000) { // Angry for 8 seconds as requested
      state = 0; stateTimer = now; tarX = 0; tarY = 0;
    }
  }

  drawRobotEyes(curX, curY, (state == 1), isBlinking);
  
  // Use a small 10ms delay to cap frame rate and prevent I2C bus congestion
  delay(10); 
}