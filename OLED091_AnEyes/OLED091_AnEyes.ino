#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- Eye Dimensions & Shape ---
const int eyeW = 30; 
const int eyeH = 14;
const int eyeSpace = 30;

// CHANGE THIS FOR ROUNDNESS: 
// 0 = Sharp rectangles
// 4 = Soft corners (Default)
// 6 = Perfect pill/oval shapes (Max recommended for 12px height)
const int eyeRadius = 3; 

// --- Animation Parameters ---
const float smoothness = 0.1; // Speed/fluidity of movement (0.05 to 0.3)

// --- Motion Variables ---
float curX = 0, curY = 0;
float tarX = 0, tarY = 0;

// --- Timing & States ---
unsigned long nextMoveTime = 0;
unsigned long nextBlinkTime = 0;
int blinkStage = 0; // 0: Open, 1: Closing, 2: Closed, 3: Opening

void setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    for(;;); 
  }
  
  display.clearDisplay();
  randomSeed(analogRead(0));

  // ==========================================
  // WAKE UP ANIMATION (Uses eyeRadius)
  // ==========================================
  int baseX = (SCREEN_WIDTH - (eyeW * 2 + eyeSpace)) / 2;
  int baseY = (SCREEN_HEIGHT - eyeH) / 2;

  for (int h = 0; h <= eyeH; h++) {
    display.clearDisplay();
    
    if (h > 2) {
      // Dynamic radius limiting so the corners don't glitch while opening narrow
      int currentRadius = min(eyeRadius, h / 2); 
      display.fillRoundRect(baseX, baseY + (eyeH - h) / 2, eyeW, h, currentRadius, WHITE);
      display.fillRoundRect(baseX + eyeW + eyeSpace, baseY + (eyeH - h) / 2, eyeW, h, currentRadius, WHITE);
    } else {
      display.drawFastHLine(baseX, baseY + (eyeH / 2), eyeW, WHITE);
      display.drawFastHLine(baseX + eyeW + eyeSpace, baseY + (eyeH / 2), eyeW, WHITE);
    }
    
    display.display();
    delay(90); 
  }
  
  delay(800); 
  nextBlinkTime = millis() + random(3000, 6000);
}

void drawEye(int x, int y, int blinkState) {
  int h = eyeH;
  
  if (blinkState == 1 || blinkState == 3) h = eyeH / 2; // Half closed
  if (blinkState == 2) h = 1;                           // Fully closed

  if (h > 2) {
    // Dynamic radius adjustment prevents visual glitches during a blink transition
    int currentRadius = min(eyeRadius, h / 2);
    display.fillRoundRect(x, y + (eyeH - h) / 2, eyeW, h, currentRadius, WHITE);
  } else {
    display.drawFastHLine(x, y + (eyeH / 2), eyeW, WHITE);
  }
}

void loop() {
  display.clearDisplay();
  unsigned long now = millis();

  // ==========================================
  // 1. SMOOTH MOVEMENT
  // ==========================================
  curX += (tarX - curX) * smoothness;
  curY += (tarY - curY) * smoothness;

  if (now > nextMoveTime) {
    tarX = random(-15, 16); 
    tarY = random(-10, 11); 
    nextMoveTime = now + random(1000, 3500); 
  }

  // ==========================================
  // 2. BLINK GENERATOR
  // ==========================================
  if (now > nextBlinkTime && blinkStage == 0) {
    blinkStage = 1; 
  }

  int baseX = (SCREEN_WIDTH - (eyeW * 2 + eyeSpace)) / 2;
  int baseY = (SCREEN_HEIGHT - eyeH) / 2;

  drawEye(baseX + (int)curX, baseY + (int)curY, blinkStage);
  drawEye(baseX + eyeW + eyeSpace + (int)curX, baseY + (int)curY, blinkStage);

  display.display();

  // ==========================================
  // 3. BLINK STATE MACHINE
  // ==========================================
  if (blinkStage > 0) {
    delay(25); 
    blinkStage++;
    
    if (blinkStage > 3) {
      blinkStage = 0; 
      
      if (random(0, 100) < 25) {
        nextBlinkTime = now + 120; 
      } else {
        nextBlinkTime = now + random(3000, 7500); 
      }
    }
  }
}
