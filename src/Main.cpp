/**
 * Main program for ESP8266 home automation
 * Copyright (c) 2021 Bruno Natali - b010010010n@gmail.com
 * 
 * License (MIT license):
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include "Main.h"


ICACHE_RAM_ATTR bool buttonClicked(void* self, uint16 buttonIndex)
{
  SERIALPRINT("clicked:"); 
  SERIALPRINT(buttonIndex);
  SERIALPRINT("-");
  SERIALPRINTLN(_buttonMode[buttonIndex]);
  
  if (_buttonMode[buttonIndex] >= 1 && _buttonMode[buttonIndex] <= 5) {
    uint8_t out = _buttonMode[buttonIndex] -1;

    if (_outputPinController[out]->getDimmable()){
      if (_outputPinController[out]->getValue() > 1) {
        _outputPinController[out]->on(); // Dimmer 100% is LOW state
      } else {
        _outputPinController[out]->dimmer(_flash->getButtonDimmer(_buttonMode[buttonIndex]));
      }
    } else {
      if (_outputPinController[out]->getValue() == HIGH) {
        _outputPinController[out]->on();
      } else {
        _outputPinController[out]->off();
      }
    }

    return true;
  }

  return false;
}

IRAM_ATTR void buttonHolded(void* self, uint16 buttonNumber)
{
  SERIALPRINT("holded:");
  SERIALPRINTLN(buttonNumber);
  
  // Enable dimmer button
  uint8_t tempBtnMode = _buttonMode[buttonNumber] -1;
  dimmerHoldButton = (tempBtnMode < 6 ? tempBtnMode : 0xFF);
  bool isButtonDimmable = 
    dimmerHoldButton != 0xFF ? 
    _outputPinController[dimmerHoldButton]->getDimmable() : 
    false;

  SERIALPRINT("dimmable:");
  SERIALPRINT(dimmerHoldButton);
  SERIALPRINT("-");
  SERIALPRINTLN(isButtonDimmable);

  if (isButtonDimmable) {
    (void) enableDimmerButton(); // Just enable dimmer if holded button is dimmable

    // Disable all other buttons
    for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
      if (btnCnt != buttonNumber && _buttonMode[btnCnt] < 6) { // Only disable light handle buttons
        SERIALPRINT("disable:");
        SERIALPRINT(btnCnt);
        SERIALPRINT("-");
        SERIALPRINTLN(_buttonMode[btnCnt]);

        _touchButton[btnCnt]->disable();
        _buttonMode[btnCnt] = BUTTON_DISABLED_BY_SOFTWARE;
      }
    }
  } else {
    dimmerHoldButton = 0xFF;
  }
}

IRAM_ATTR void buttonUnholded(void* self, uint16 buttonNumber)
{
  SERIALPRINT("un-holded");
  SERIALPRINTLN(buttonNumber);

  // Re-enable disabled buttons when hold
  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    if (btnCnt != buttonNumber && _buttonMode[btnCnt] == BUTTON_DISABLED_BY_SOFTWARE) {
      _touchButton[btnCnt]->enable();
      _buttonMode[btnCnt] = _flash->getButtonLightMode(btnCnt + 1);
    }
  }

  if (dimmerHoldButton != 0xFF) {
    int dimmVal = _outputPinController[_buttonMode[buttonNumber] -1]->getValue();
    if (_flash->getButtonDimmer(_buttonMode[buttonNumber]) != dimmVal) { // if dimmable & selected value is != on flash
      _flash->setButtonDimmer(_buttonMode[buttonNumber], dimmVal);
    }

    disableDimmerButton();
    dimmerHoldButton = 0xFF;
  }
}

IRAM_ATTR void dimmerButtonClicked(void)
{
#if SERIAL_DEBUG  
  SERIALPRINT("dimmerButtonClicked:");
#endif
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
    if (value > 80) 
      value = 20;
    else
      value = value + 20;

    _outputPinController[dimmerHoldButton]->dimmer(value);
#if SERIAL_DEBUG
    SERIALPRINTLN(value);
#endif
  } else {
#if SERIAL_DEBUG
  SERIALPRINTLN("OK");
#endif
  }
}

void enableDimmerButton(void)
{
#if SERIAL_DEBUG
  SERIALPRINT("enableDimmerButton:");
#endif
  if (dimmerButtonIndex == 0xFF)
    return;

  pinMode(_buttonPin[dimmerButtonIndex], INPUT);
  attachInterrupt(digitalPinToInterrupt(_buttonPin[dimmerButtonIndex]), &dimmerButtonClicked, RISING);
  dimmerEnabled = true;
#if SERIAL_DEBUG
  SERIALPRINTLN("OK");
#endif
}

void disableDimmerButton(void)
{
#if SERIAL_DEBUG
  SERIALPRINT("disableDimmerButton");
#endif
  if (dimmerButtonIndex == 0xFF)
    return;

  dimmerEnabled = false;
  pinMode(_buttonPin[dimmerButtonIndex], OUTPUT);
  digitalWrite(_buttonPin[dimmerButtonIndex], LOW);
  detachInterrupt(_buttonPin[dimmerButtonIndex]);
#if SERIAL_DEBUG
  SERIALPRINTLN("OK");
#endif
}

void configureButton(uint8_t buttonIndex, uint8_t mode, bool setFlash)
{
#if SERIAL_DEBUG
  SERIALPRINT("Cfg Btn:");
#endif

  if (_buttonMode[buttonIndex] != 0xFA) {
    if (_buttonMode[buttonIndex] == mode) {
#if SERIAL_DEBUG
    SERIALPRINT("mode is D same,");
    SERIALPRINT(mode);
#endif
      return;
    }

    delete _touchButton[buttonIndex]; // Delete current instance
  }

  if (mode == 0xC8) { // dimmer
#if SERIAL_DEBUG
    SERIALPRINT("dimmer");
#endif
    disableDimmerButton(); // Disable previous configured dimmer button

    // Configure new dimer button
    dimmerButtonIndex = buttonIndex;
    disableDimmerButton();
  } else {
#if SERIAL_DEBUG
    SERIALPRINT("md->");
    SERIALPRINT(mode);
#endif
    if (_buttonMode[buttonIndex] == 0xC8) { // If this button was previously configured as dimmer
      disableDimmerButton();
      dimmerButtonIndex = 0xFF;
    }

    if (mode >= 1 && mode <= 6) {
      for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
        if (_buttonMode[btnCnt] == mode) {
          delete _touchButton[btnCnt]; // Disable previously configured button with desired output
          break;
        }
      }
    }

    _touchButton[buttonIndex] = new TouchButtonModule(
      _buttonPin[buttonIndex], 
      buttonIndex, 
      _flash->getButtonLogicLevel(), 
      _flash->getButtonHoldTO(), 
      _flash->getButtonHoldPeriod() // 30
    );

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
  if (setFlash)
    _flash->setButtonLightMode(buttonIndex + 1, mode);
}

bool getSimpleOnOffButtonValue(uint8_t buttonIndex, uint8_t dimmable)
{
  if (dimmable == 2)
    dimmable = (_buttonMode[buttonIndex] < 6 ? _outputPinController[_buttonMode[buttonIndex] - 1]->getDimmable() : 0);

  return dimmable ? 
    (
      _outputPinController[_buttonMode[buttonIndex] - 1]->getValue() ? 
      1 : 
      0
    ) : 
    !_outputPinController[_buttonMode[buttonIndex] - 1]->getValue();
}

String getButtonJsonConfig(uint8_t buttonindex)
{
  bool dimmable = (_buttonMode[buttonindex] < 6 ? _outputPinController[_buttonMode[buttonindex] - 1]->getDimmable() : 0);

  return "{\"d\":" + String(dimmable) + 
      ",\"dv\":" + String(dimmable ? _flash->getButtonDimmer(_buttonMode[buttonindex]) : 0) +
      ",\"f\":" + String(_buttonMode[buttonindex]) +
      ",\"s\":" + String(
        _buttonMode[buttonindex] < 6 ? 
        getSimpleOnOffButtonValue(buttonindex, dimmable) : 
        0
      ) + '}';
}

String getButtonsJsonList(void)
{
  String json = "";
  uint8_t btn;

  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    btn = btnCnt +1;

#if SERIAL_DEBUG
    SERIALPRINT('a');
    SERIALPRINT(btnCnt);
    SERIALPRINT('-');
    SERIALPRINT(_buttonMode[btnCnt]);
    SERIALPRINT('-');
    SERIALPRINT(btn);
    SERIALPRINT('-');
    SERIALPRINTLN(_buttonMode[btnCnt]);
#endif

    if (json.length())
      json += ',';

    json = json + '\"' + btn + 
      "\":" + getButtonJsonConfig(btnCnt);
  }

  return json;
}

void handleWebServerRoot(void)
{
#if SERIAL_DEBUG
  SERIALPRINTLN("req: root page");
#endif
  _communication->webServer->send(200, "text/html", _communication->localWebPage + getButtonsJsonList() + "};</script>");
}

void handleWebServerSetOnOff(void)
{
#if SERIAL_DEBUG
  SERIALPRINT("req: set on-off,");
#endif
  if (_communication->webServer->arg("i") == "") { // No button number provided
#if SERIAL_DEBUG
    SERIALPRINTLN("not i");
#endif
    _communication->webServer->send(400);
    return;
  }
  if (_communication->webServer->arg("s") == "") { // No set configured
#if SERIAL_DEBUG
    SERIALPRINTLN("not s");
#endif
    _communication->webServer->send(400);
    return;
  }
    
  uint16_t buttonIndex = _communication->webServer->arg("i").toInt() -1;

  if (buttonIndex > 5) { // Button index is wrong
#if SERIAL_DEBUG
    SERIALPRINTLN("wrong i:");
    SERIALPRINTLN(buttonIndex);
#endif
    _communication->webServer->send(400);
    return;
  }

#if SERIAL_DEBUG
  SERIALPRINT("s(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT(")");
  uint8_t out = _buttonMode[buttonIndex] -1;
  if (out < 5) {
    SERIALPRINT(_outputPinController[out]->getValue());
    SERIALPRINT("-");
  }
  SERIALPRINT(_communication->webServer->arg("s").toInt());
#endif

  if (_buttonMode[buttonIndex] < 1 || _buttonMode[buttonIndex] > 5) { // Button mode not configured as output
#if SERIAL_DEBUG
    SERIALPRINTLN("wrong mode:");
    SERIALPRINTLN(_buttonMode[buttonIndex]);
#endif
    _communication->webServer->send(405);
    return;
  }

  bool clickResult = buttonClicked(nullptr, buttonIndex);
  bool buttonStatus = getSimpleOnOffButtonValue(buttonIndex);

#if SERIAL_DEBUG
  if (clickResult) {
    SERIALPRINTLN(":OK");
  } else {
    SERIALPRINTLN(":Er");
  }
#endif

  _communication->webServer->send(
    200, 
    "application/json", 
    "{\"b\":" + String(++buttonIndex) + ",\"r\":" + String(clickResult) + ",\"s\":" + String(buttonStatus) + "}"
  );
}

void handleWebServerConfig(void)
{
#if SERIAL_DEBUG
  SERIALPRINT("req: config,");
#endif
  if (_communication->webServer->arg("i") == "") { // No button number provided
#if SERIAL_DEBUG
    SERIALPRINTLN("not i");
#endif
    _communication->webServer->send(400);
    return;
  }
  if (_communication->webServer->arg("f") == "" || 
    _communication->webServer->arg("d") == "" || 
    _communication->webServer->arg("dv") == "") { // No config provided
#if SERIAL_DEBUG
    SERIALPRINTLN("not cfg");
#endif
    _communication->webServer->send(400);
    return;
  }

  bool dimmable;
  uint16_t dimmerValue;
  bool result = 1;
  String error;
  uint16_t buttonIndex = _communication->webServer->arg("i").toInt() -1;
  uint8_t btnMode = _communication->webServer->arg("f").toInt();
  
  if (_buttonMode[buttonIndex] != btnMode)
    configureButton(buttonIndex, btnMode);
    
  uint8_t outIndex = _buttonMode[buttonIndex] -1;
  
  if (outIndex < 5) {
    dimmable = _communication->webServer->arg("d").toInt();

    if (dimmable != _outputPinController[outIndex]->getDimmable()) {
#if SERIAL_DEBUG
      SERIALPRINT("setDimm-");
#endif
      if (_outputPinController[outIndex]->getLockDimm()) {
#if SERIAL_DEBUG
        SERIALPRINT("lock,");
#endif
        result = 0;
        error = "dimm lock," + String(outIndex);
      } else if (!_outputPinController[outIndex]->setDimmable(dimmable)) {
#if SERIAL_DEBUG
        SERIALPRINT("Err,");
#endif
        result = 0;
        error = "dimm cfg," + String(outIndex) + ","  + String(dimmable);
      } 
    }
  
    if (result) {
      if (dimmable) {
        dimmerValue = _communication->webServer->arg("dv").toInt();

        if (dimmerValue == 1)
          dimmerValue = 2;
        else if (dimmerValue > 100)
          dimmerValue = 100;

        if (dimmerValue && dimmerValue != _outputPinController[outIndex]->getValue()) {
#if SERIAL_DEBUG
          SERIALPRINT("DimmVal-");
#endif
          if (!_outputPinController[outIndex]->dimmer(dimmerValue)) {
#if SERIAL_DEBUG
            SERIALPRINT("Err,");
#endif
            result = 0;
            error = "dimm set," + String(outIndex) + ","  + String(dimmerValue);
          } else {
#if SERIAL_DEBUG
            SERIALPRINT("Rec,");
#endif
            if (!_flash->setButtonDimmer(_buttonMode[buttonIndex], dimmerValue)) {
#if SERIAL_DEBUG
              SERIALPRINT("err");
#endif
              result = 0;
              error = "dimm rec," + String(buttonIndex) + "," + String(_buttonMode[buttonIndex]) + "," + String(dimmerValue);
            } else {
#if SERIAL_DEBUG
              SERIALPRINT("OK");
#endif
            }
          }
        }
      } else {
        if (!_flash->setButtonDimmer(_buttonMode[buttonIndex], 0)) {
#if SERIAL_DEBUG
        SERIALPRINT("err");
#endif
          result = 0;
          error = "dimm rec," + String(buttonIndex) + ","  + String(_buttonMode[buttonIndex]);
        } else {
#if SERIAL_DEBUG
          SERIALPRINT("OK");
#endif
        }
      }
    }
  }
    
#if SERIAL_DEBUG
  SERIALPRINT("c(");
  SERIALPRINT(buttonIndex);
  SERIALPRINT(")f-");
  SERIALPRINT(_buttonMode[buttonIndex]);
  SERIALPRINT("->");
  SERIALPRINT(btnMode);
  if (outIndex < 5) {
    SERIALPRINT("d-");
    SERIALPRINT(dimmable);
    if (result && dimmable) {
      SERIALPRINT("dv-");
      SERIALPRINT(dimmerValue);
    }
  }
#endif

    _communication->webServer->send(
      200, 
      "application/json", 
      "{\"r\":" + String(result) + (!result ? ",\"e\":\"" + error + "\"" : "") + ",\"d\":" + getButtonJsonConfig(buttonIndex) + '}'
    );
}

void setup()
{
  delay(1000);

  // SYSTEM INFO
#if SERIAL_DEBUG
  Serial.begin(115200); 
  SERIALPRINTLN("");
  SERIALPRINTLN("System setup...");
  SERIALPRINT("SDK version: ");
  SERIALPRINTLN(system_get_sdk_version());
  SERIALPRINT("ChipID: ");
  SERIALPRINTLN(system_get_chip_id()); 
  // SERIALPRINT("MEM: ");
  // system_set_os_print(1);
  // system_print_meminfo();
  SERIALPRINT("Power: ");
  SERIALPRINTLN(system_get_vdd33());
#endif

  /**
   * Instantiate flash handler
  */
  _flash = new FlashMan();

#if SERIAL_DEBUG
  SERIALPRINT("btn led PWM:");
  SERIALPRINTLN(analogRead(2));
#endif
delay(500);
  
  pinMode(2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(2, HIGH);  // Turn off light when plugedin 
  //analogWrite(LED_BUILTIN, 100);


/**
 * INITIALIZE OUTPUT PINS (LIGHT)
*/
#if SERIAL_DEBUG
  SERIALPRINT("\nInitializing OUTPUTS: ");
#endif
delay(1000);

  bool pinDimmable;
  for (int outPinCnt = 0 ; outPinCnt < 5 ; outPinCnt++) {
#if SERIAL_DEBUG
    if (outPinCnt)
      SERIALPRINT(", ");
    SERIALPRINT(outPinCnt);
    SERIALPRINT("->");
    SERIALPRINT(_outputPin[outPinCnt]);
#endif

    pinDimmable = (_buttonPinDimmable[outPinCnt] && _flash->getButtonDimmer(outPinCnt + 1) != 0);

    _outputPinController[outPinCnt] = new Lighter(
      _outputPin[outPinCnt], 
      pinDimmable ? HIGH : LOW, 
      pinDimmable ? LOW : HIGH, 
      pinDimmable, 
      !_buttonPinDimmable[outPinCnt]
    );
    delay(500);
  }


/*
  INITIALIZE BUTTONS
*/
#if SERIAL_DEBUG
  SERIALPRINT("\nInitializing buttons: ");
  if (BUTTON_COUNT) {
    SERIALPRINT(BUTTON_COUNT);
    SERIALPRINTLN(": ");
  } else {
    SERIALPRINTLN("None");
  }
#endif
delay(1000);

  for (int btnCnt = 0 ; btnCnt < BUTTON_COUNT ; btnCnt++) {
    uint8_t tmpBtnMode = _flash->getButtonLightMode(btnCnt + 1);
#if SERIAL_DEBUG
    SERIALPRINT(btnCnt);
    SERIALPRINT("->");
    SERIALPRINT(_buttonPin[btnCnt]);
    SERIALPRINT("-");
    SERIALPRINTLN(tmpBtnMode);
#endif

    configureButton(btnCnt, tmpBtnMode, false);
    delay(500);
  }
#if SERIAL_DEBUG
    SERIALPRINTLN("");
#endif

  _communication = new MCommunication(_flash->getSsid(), _flash->getWifiPass());

delay(1000);
#if SERIAL_DEBUG
  SERIALPRINT("Wifi mde:");
  SERIALPRINT(_flash->getWifiMode());
  SERIALPRINT("-");
  SERIALPRINTLN(WIFI_OPERATION_MODE_AP);
#endif
delay(1000);

//_communication->getWifiStatus();

_communication->setWifiMode(_flash->getWifiMode());

  //wifiServer.begin();
  //delay(1000);


  Serial.setDebugOutput(true);
}


// the loop function runs over and over again forever
void loop()
{
  // Handle HTTP server  
  if (_flash->getWifiMode() == WIFI_OPERATION_MODE_AP)
    _communication->webServer->handleClient();

}

void reboot(bool critical)
{
#if SERIAL_DEBUG 
  if (critical)
    SERIALPRINT("[CRITICAL] ");
  SERIALPRINTLN("System reboot requested");
#endif
  ESP.restart(); 
}
