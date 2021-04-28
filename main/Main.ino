/*
Main program for ESP8266 home automation
Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com

License (MIT license):
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/
#include "Main.h"

//uint8_t *generalBuffer[100] = {0x00};


String ssid;

String wifiPass;

//ESP8266WebServer server(80);

WiFiServer wifiServer(10000);

/*
void handleRoot() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  #if SERIAL_DEBUG
    Serial.println("Client requested root");
  #endif
}
*/

ICACHE_RAM_ATTR bool buttonClicked(void* self, uint16 buttonNumber)
{
  Serial.print("clicked");
  Serial.println(buttonNumber);

  if (_buttonMode[buttonNumber] > 0 && _buttonMode[buttonNumber] < 6) {
    uint8_t out = _buttonMode[buttonNumber] -1;
    if (_outputPinController[out]->getValue()) {
      _outputPinController[out]->off();
    } else {
      if (_outputPinController[out]->getDimmable())
        _outputPinController[out]->dimmer(_flash->getButtonDimmer(_buttonMode[buttonNumber]));
      else
        _outputPinController[out]->on();
    }
  }
}

ICACHE_RAM_ATTR bool buttonHolded(void* self, uint16 buttonNumber)
{
  Serial.print("holded");
  Serial.println(buttonNumber);

  // Enable dimmer button
  enableDimmerButton();
  dimmerHoldButton = _buttonMode[buttonNumber] -1;

  // Disable all other buttons
  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    if (btnCnt != buttonNumber && _buttonMode[btnCnt] < 6) { // Only disable light handle buttons
      _touchButton[btnCnt]->disable();
      _buttonMode[btnCnt] = 0xFE; // Disabled by software -> re-enable after interaction
    }
  }
}

ICACHE_RAM_ATTR bool buttonUnholded(void* self, uint16 buttonNumber)
{
  Serial.print("un-holded");
  Serial.println(buttonNumber);

  // Disable dimmer button
  disableDimmerButton();
  dimmerHoldButton = 0xFF;

  // Re-enable disabled buttons on holded
  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    if (btnCnt != buttonNumber && _buttonMode[btnCnt] == 0xFE) { // Only enable previously disabled buttons
      _touchButton[btnCnt]->enable();
      _buttonMode[btnCnt] = _flash->getButtonLightMode(buttonNumber + 1);
    }
  }
}

void dimmerButtonClicked()
{
  if (dimmerEnabled && dimmerHoldButton != 0xFF && _outputPinController[dimmerHoldButton]->getDimmable()) {
    uint16_t value = _outputPinController[dimmerHoldButton]->getValue();

    if (value > 1000) 
      value = 204;
    else
      value = value + 204;

    _outputPinController[dimmerHoldButton]->dimmer(value);
  }
}

void enableDimmerButton()
{
  if (dimmerButtonIndex == 0xFF)
    return;

  pinMode(_buttonPin[dimmerButtonIndex], INPUT);
  attachInterrupt(digitalPinToInterrupt(_buttonPin[dimmerButtonIndex]), &dimmerButtonClicked, RISING);
  dimmerEnabled = true;
}

void disableDimmerButton()
{
  if (dimmerButtonIndex == 0xFF)
    return;

  dimmerEnabled = false;
  pinMode(_buttonPin[dimmerButtonIndex], OUTPUT);
  digitalWrite(_buttonPin[dimmerButtonIndex], LOW);
  detachInterrupt(_buttonPin[dimmerButtonIndex]);
}

void configureButton(uint8_t buttonIndex, uint8_t mode)
{
  delete _touchButton[buttonIndex]; // Delete current instance

  if (mode == 0xC8) { // dimmer
    dimmerButtonIndex = buttonIndex;
    dimmerEnabled = false;
  } else {
    if (_buttonMode[buttonIndex] >= 0xC8) { // If this button was previously configured as dimmer
      dimmerEnabled = false;
      disableDimmerButton();
      dimmerButtonIndex = 0xFF;
    }

    _touchButton[buttonIndex] = new TouchButtonModule(_buttonPin[buttonIndex], buttonIndex, _flash->getButtonLogicLevel(), _flash->getButtonHoldTO(), _flash->getButtonHoldPeriod());

    if (mode >= 0xFE) // disabled
      _touchButton[buttonIndex]->disable();

    _touchButton[buttonIndex]->setClickFunction(reinterpret_cast<clickcallback*>(&buttonClicked), nullptr);

    _touchButton[buttonIndex]->setHoldFunction(reinterpret_cast<holdcallback*>(&buttonHolded), nullptr);

    _touchButton[buttonIndex]->setUnholdFunction(reinterpret_cast<unholdcallback*>(&buttonUnholded), nullptr);
  }

  _buttonMode[buttonIndex] = mode;
}

void setup()
{
  delay(1000);
/*
  SYSTEM INFO
*/
#if SERIAL_DEBUG
  Serial.begin(115200); 
  Serial.println("");
  Serial.println("System setup...");
  Serial.print("SDK version: ");
  Serial.println(system_get_sdk_version());
  Serial.print("ChipID: ");
  Serial.println(system_get_chip_id()); 
  // Serial.print("MEM: ");
  // system_set_os_print(1);
  // system_print_meminfo();
  Serial.print("Power: ");
  Serial.println(system_get_vdd33());
#endif

  _flash = new FlashMan();
  
  //if (!checkFlash())
  //  reboot(true);
  /*
    this->getWifiMode();
    this->getButtonHoldTO();
    this->getButtonHoldPeriod();
    this->getSsid();
    this->getWifiPass();
  */

  uint8_t wifiMode = _flash->getWifiMode();

delay(1000);
  Serial.print("Wifi mde:");
  Serial.print(wifiMode);
  Serial.print("-");
  Serial.println(WIFI_OPERATION_MODE_AP);
  WiFi.mode(WIFI_OFF);
delay(1000);
  if (wifiMode == WIFI_OPERATION_MODE_AP) {
    WiFi.mode(WIFI_AP);
    while (WiFi.getMode() != WIFI_AP) {
      delay(50);
    }
    if(!WiFi.softAPConfig(IPAddress(192, 168, 5, 1), IPAddress(192, 168, 5, 1), IPAddress(255, 255, 255, 0))){
      Serial.println("AP Config Failed");
    }

    WiFi.softAP(_flash->getSsid(), _flash->getWifiPass());
    IPAddress myIP = WiFi.softAPIP();

    Serial.print("AP IP address: ");
    Serial.println(myIP);
    //server.on("/", handleRoot);
    //server.begin();
    Serial.println("HTTP server started");
  } else if (wifiMode == WIFI_OPERATION_MODE_CLIENT) {

  }
  
delay(1000);
  Serial.print("Wifi status:");
  Serial.println(WiFi.status());
delay(1000);

  WiFi.printDiag(Serial);
  delay(5000);

  wifiServer.begin();
  delay(1000);
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);  // Turn off light when plugedin 
  //analogWrite(LED_BUILTIN, 500);

/*
  BUTTONS
*/
#if SERIAL_DEBUG
  Serial.print("Initializing buttons: ");
  if (BUTTON_COUNT) {
    Serial.print(BUTTON_COUNT);
    Serial.print(": ");
  } else {
    Serial.println("None");
  }
#endif
delay(1000);
  /*
  for (int outPinCnt = 0 ; outPinCnt < 5 ; outPinCnt++) {
    _outputPinController[outPinCnt] = new Lighter(
      _outputPin[outPinCnt], 
      !_flash->getButtonLogicLevel(), 
      _flash->getButtonLogicLevel(), 
      _buttonPinDimmable[_buttonMode[outPinCnt] -1], 
      !_buttonPinDimmable[_buttonMode[outPinCnt] -1]
    );
    delay(500);
  }
  */

  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
#if SERIAL_DEBUG
    if (btnCnt)
      Serial.print(", ");
    Serial.print(btnCnt);
    Serial.print("->");
    Serial.print(_buttonPin[btnCnt]);
#endif

    configureButton(btnCnt, _flash->getButtonLightMode(btnCnt + 1));
    delay(500);
    
  }
#if SERIAL_DEBUG
    Serial.println("");
#endif



  Serial.setDebugOutput(true);
}


// the loop function runs over and over again forever
void loop()
{
  // server.handleClient();

    WiFiClient client = wifiServer.available();
 
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        
        _touchButton[0]->enable();

        Serial.write(c);
      }
 
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
  }
}

void reboot(bool critical)
{
#if SERIAL_DEBUG 
  if (critical)
    Serial.print("[CRITICAL] ");
  Serial.println("System reboot requested");
#endif
  ESP.restart(); 
}

/*
bool calcCrcFlash(unsigned char partition)
{
  unsigned int crcLimit = 504;
  unsigned int flashCounter;
  unsigned int crcCounter;

  if (partition == 1) {
    flashCounter = 0;
    crcCounter = 504;
  }
  else if (partition == 2) {
    flashCounter = 250;
    crcCounter = 508;
  } else {
    return false;
  }

  unsigned short crc = getFlashCrc(partition);

  for (unsigned char i = 0 ; i < 4 ; i++) {
    EEPROM.write(i, 0x00);
    crcCounter ++;
  }
}
*/





