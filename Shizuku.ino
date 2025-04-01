#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>

#define OLED_CS 7
#define OLED_RESET 10
#define OLED_DC 6
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64

#define ITEM_BOOTPAGE_PREVIEW 0
#define HELLO_WORLD 1
#define ABOUT 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);

// 引脚
#define TOUCH 9

#define BTN_SHORT_PRESS 1
#define BTN_LONG_PRESS 2

// 绘制启动界面
void DBootPage(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Shizuku"));
  display.println(F("Booting up..."));
  display.display();
}

// 绘制主页
void DHomePage(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Home page"));
  display.println(F(" "));
  display.println(F("Hello world!"));
  display.println(F("Have a nice day!"));
  display.display();
}

// 绘制项
void DItem(uint8_t item){
  Serial.println("[Display] Draw item");
  switch(item){
    case ITEM_BOOTPAGE_PREVIEW:
      display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
      display.setCursor(0, 18);
      display.print(F("Test-BootPage"));
      display.display();
      break;
    case HELLO_WORLD:
      display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
      display.setCursor(0, 18);
      display.print(F("Hello World"));
      display.display();
      break;
    case ABOUT:
      display.fillRect(0, 16, 128, 16, SSD1306_BLACK);
      display.setCursor(0, 18);
      display.print(F("About"));
      display.display();
      break;
    default: Serial.printf("[Display] item = %d\n", item);
  }
}

// 绘制HelloWorld页
void DHelloWorld(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("Hello World!"));
  display.display();
}

// 绘制关于页
void DAbout(){
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(F("About Shizuku:"));
  display.println(F("Version: V0.3"));
  display.println(F("Ready to serve for you!"));
  display.display();
}

// 按键扫描
uint8_t ScanKey() {
  uint8_t btn = 0;
  int press_tick = 0;
  int duration = 0;
  while (digitalRead(TOUCH) == LOW){
    delay(10);
    press_tick = millis();
    while (digitalRead(TOUCH) == LOW) duration = millis() - press_tick;
  }
  if(duration > 1000){
    Serial.println("[debug] Trigger Long Press");
    return BTN_LONG_PRESS;
  }
  if(duration > 0 && duration < 1000){
    Serial.println("[debug] Trigger Short Press");
    return BTN_SHORT_PRESS;
  }
  return 0;
}

// 预览启动页面
void BootPagePreview(){
  Serial.println("[BootPagePreview]");
  DBootPage();
  delay(1000);
}

// 你好,世界
void HelloWorld(){
  Serial.println("[HelloWorld]");
  DHelloWorld();
  while(1){
    switch(ScanKey()){
      case BTN_SHORT_PRESS:
        break;
      case BTN_LONG_PRESS:
        return;
    }
  }
}

void About(){
  Serial.println("[About]");
  DAbout();
  while(1){
    switch(ScanKey()){
      case BTN_SHORT_PRESS:
        break;
      case BTN_LONG_PRESS:
        return;
    }
  }
}

// 主页
void HomePage(){
  DHomePage();
  delay(1000);
  uint8_t currentItem = 0;
  DItem(currentItem);
  while(1){
    switch(ScanKey()){
      case BTN_SHORT_PRESS:
        Serial.println("[debug] Next");
        if(++currentItem > ABOUT) currentItem = 0;
        DItem(currentItem);
        break;
      case BTN_LONG_PRESS:
        Serial.println("[debug] Enter");
        switch(currentItem){
          case ITEM_BOOTPAGE_PREVIEW: BootPagePreview(); break;
          case HELLO_WORLD: HelloWorld(); break;
          case ABOUT: About(); break;
          default: Serial.printf("[debug] Enter item %d\n", currentItem);
        }
        return;
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println("\n[Shizuku] Booting...");
  display.begin(SSD1306_SWITCHCAPVCC);
  display.setFont();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  DBootPage();
  // GPIO配置
  pinMode(TOUCH, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("[Shizuku] Shizuku V0.3 Ready!");
}

void loop() {
  HomePage();
}