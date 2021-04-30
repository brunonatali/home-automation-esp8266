/*
Main program for ESP8266 home automation
Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com

License (MIT license):
  Permission is hereby granted, free of charge, to any person obtain®ing a copy
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


//String ssid;

//String wifiPass;

//ESP8266WebServer server(80);

//WiFiServer wifiServer(10000);

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
  Serial.print("clicked:");
  Serial.print(buttonNumber);
  Serial.print("-");
  Serial.println(_buttonMode[buttonNumber]);

  if (_buttonMode[buttonNumber] >= 1 && _buttonMode[buttonNumber] <= 5) {
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
  Serial.print("holded:");
  Serial.println(buttonNumber);

  // Enable dimmer button
  uint8_t tempBtnMode = _buttonMode[buttonNumber] -1;
  dimmerHoldButton = (tempBtnMode < 6 ? tempBtnMode : 0xFF);
  Serial.print("dimmable:");
  Serial.print(dimmerHoldButton);
  Serial.print("-");
  Serial.println(_outputPinController[dimmerHoldButton]->getDimmable());
  if (_outputPinController[dimmerHoldButton]->getDimmable())
    enableDimmerButton(); // Just enable dimmer if holded button is dimmable

  // Disable all other buttons
  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    if (btnCnt != buttonNumber && _buttonMode[btnCnt] < 6) { // Only disable light handle buttons
  Serial.print("disable:");
  Serial.print(btnCnt);
  Serial.print("-");
  Serial.println(_buttonMode[btnCnt]);
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

    /**
     * 204 is 5 dimmer stages
     * 1 - 204
     * 2 - 408
     * 3 - 612
     * 4 - 816
     * 5 - 1020
    */
    if (value > 819) 
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
    disableDimmerButton();
  } else {
    if (_buttonMode[buttonIndex] >= 0xC8) { // If this button was previously configured as dimmer
      dimmerEnabled = false;
      disableDimmerButton();
      dimmerButtonIndex = 0xFF;
    }

    _touchButton[buttonIndex] = new TouchButtonModule(_buttonPin[buttonIndex], buttonIndex, _flash->getButtonLogicLevel(), _flash->getButtonHoldTO(), 30); //_flash->getButtonHoldPeriod()

    if (mode >= 0xFE) // disabled
      _touchButton[buttonIndex]->disable();
    _touchButton[buttonIndex]->clickCallback = reinterpret_cast<clickcallback>(buttonClicked);
    _touchButton[buttonIndex]->clickCallbackArg = nullptr;
    _touchButton[buttonIndex]->holdCallback = reinterpret_cast<holdcallback>(buttonHolded);
    _touchButton[buttonIndex]->holdCallbackArg = nullptr;
    _touchButton[buttonIndex]->unholdCallback = reinterpret_cast<unholdcallback>(buttonUnholded);
    _touchButton[buttonIndex]->unholdCallbackArg = nullptr;
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

  /**
   * Instantiate flash handler
  */
  _flash = new FlashMan();
  
  Serial.print("btn led PWM:");
  Serial.println(analogRead(2));
delay(500);
  
  pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(2, HIGH);  // Turn off light when plugedin 
  //analogWrite(LED_BUILTIN, 500);


/**
 * OUTPUT PINS (LIGHT)
*/
#if SERIAL_DEBUG
  Serial.print("Initializing OUTPUTS: ");
#endif
delay(1000);

  for (int outPinCnt = 0 ; outPinCnt < 5 ; outPinCnt++) {
#if SERIAL_DEBUG
    if (outPinCnt)
      Serial.print(", ");
    Serial.print(outPinCnt);
    Serial.print("->");
    Serial.print(_outputPin[outPinCnt]);
#endif

    _outputPinController[outPinCnt] = new Lighter(
      _outputPin[outPinCnt], 
      !_flash->getButtonLogicLevel(), 
      _flash->getButtonLogicLevel(), 
      (_flash->getButtonDimmer(outPinCnt + 1) != 0 && _buttonPinDimmable[outPinCnt]), 
      !_buttonPinDimmable[outPinCnt]
    );
    delay(500);
  }


/*
  BUTTONS
*/
#if SERIAL_DEBUG
  Serial.print("\nInitializing buttons: ");
  if (BUTTON_COUNT) {
    Serial.print(BUTTON_COUNT);
    Serial.println(": ");
  } else {
    Serial.println("None");
  }
#endif
delay(1000);

  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    uint8_t tmpBtnMode = _flash->getButtonLightMode(btnCnt + 1);
#if SERIAL_DEBUG
    Serial.print(btnCnt);
    Serial.print("->");
    Serial.print(_buttonPin[btnCnt]);
    Serial.print("-");
    Serial.println(tmpBtnMode);
#endif

    configureButton(btnCnt, tmpBtnMode);
    delay(500);
    
  }
#if SERIAL_DEBUG
    Serial.println("");
#endif


  _communication = new MCommunication(_flash->getSsid(), _flash->getWifiPass());

  uint8_t wifiMode = _flash->getWifiMode();

delay(1000);
  Serial.print("Wifi mde:");
  Serial.print(wifiMode);
  Serial.print("-");
  Serial.println(WIFI_OPERATION_MODE_AP);
delay(1000);

//_communication->getWifiStatus();

_communication->setWifiMode(wifiMode);

  //wifiServer.begin();
  //delay(1000);


  Serial.setDebugOutput(true);
}


// the loop function runs over and over again forever
void loop()
{
  // server.handleClient();

  /*
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
  */
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
