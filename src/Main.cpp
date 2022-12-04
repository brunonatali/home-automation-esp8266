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

IRAM_ATTR bool buttonClicked(void *self, uint16 buttonIndex)
{
  SERIALPRINT("clicked:");
  SERIALPRINT(buttonIndex, DEC);
  SERIALPRINT("-");
  SERIALPRINT(_buttonMode[buttonIndex], DEC);
  SERIALPRINT("=");

  if (_buttonMode[buttonIndex] >= 1 && _buttonMode[buttonIndex] <= 5)
  {
    uint8_t out = _buttonMode[buttonIndex] - 1;
    bool isOn = _outputPinController[out]->getValue() == light_state::ON;

    if (isOn)
      _outputPinController[out]->off();
    else
      _outputPinController[out]->on();

    SERIALPRINT(isOn ? "ON" : "OFF");
    SERIALPRINT(">");
    SERIALPRINTLN(_outputPinController[out]->getValue() == light_state::ON ? "ON" : "OFF");
    return true;
  }

  SERIALPRINTLN("no-out");
  return false;
}

void configureButton(uint8_t buttonIndex, uint8_t mode, bool setFlash)
{
  SERIALPRINT("cfg btn:");

  if (_buttonMode[buttonIndex] != 0xFA)
  {
    if (_buttonMode[buttonIndex] == mode)
    {
      SERIALPRINT("mode is same,");
      SERIALPRINTLN(mode);
      return;
    }

    delete _touchButton[buttonIndex]; // Delete current instance
  }

  if (mode == 0xC8)
  { // dimmer
    SERIALPRINT("dimmer");
  }
  else
  {
    SERIALPRINT("md->");
    SERIALPRINT(mode);
    if (_buttonMode[buttonIndex] == 0xC8)
    { // If this button was previously configured as dimmer
      dimmerButtonIndex = 0xFF;
    }

    if (mode >= 1 && mode <= 6)
    {
      for (int btnCnt = 0; btnCnt < BUTTON_COUNT; btnCnt++)
      {
        if (_buttonMode[btnCnt] == mode)
        {
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
  }

  _buttonMode[buttonIndex] = mode;
  if (setFlash)
    _flash->setButtonLightMode(buttonIndex + 1, mode);
}

bool getSimpleOnOffButtonValue(uint8_t buttonIndex, uint8_t dimmable)
{
  if (dimmable == 2)
    dimmable = (_buttonMode[buttonIndex] < 6 ? _outputPinController[_buttonMode[buttonIndex] - 1]->getDimmable() : 0);

  return _outputPinController[_buttonMode[buttonIndex] - 1]->getValue() == light_state::ON ? 1 : 0;
}

String getButtonJsonConfig(uint8_t buttonindex)
{
  bool dimmable = (_buttonMode[buttonindex] < 6 ? _outputPinController[_buttonMode[buttonindex] - 1]->getDimmable() : 0);

  return "{\"d\":" + String(dimmable) +
         ",\"dv\":" + String(dimmable ? _flash->getButtonDimmer(_buttonMode[buttonindex]) : 0) +
         ",\"f\":" + String(_buttonMode[buttonindex]) +
         ",\"s\":" + String(_buttonMode[buttonindex] < 6 ? getSimpleOnOffButtonValue(buttonindex, dimmable) : 0) + '}';
}

String getButtonsJsonList(void)
{
  String json = "";
  uint8_t btn;

  for (int btnCnt = 0; btnCnt < BUTTON_COUNT; btnCnt++)
  {
    btn = btnCnt + 1;

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
  if (_communication->webServer->arg("i") == "")
  { // No button number provided
#if SERIAL_DEBUG
    SERIALPRINTLN("not i");
#endif
    _communication->webServer->send(400);
    return;
  }
  if (_communication->webServer->arg("s") == "")
  { // No set configured
#if SERIAL_DEBUG
    SERIALPRINTLN("not s");
#endif
    _communication->webServer->send(400);
    return;
  }

  uint16_t buttonIndex = _communication->webServer->arg("i").toInt() - 1;

  if (buttonIndex > 5)
  { // Button index is wrong
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
  uint8_t out = _buttonMode[buttonIndex] - 1;
  if (out < 5)
  {
    SERIALPRINT(_outputPinController[out]->getValue() == light_state::ON ? "on" : "off");
    SERIALPRINT("-");
  }
  SERIALPRINT(_communication->webServer->arg("s").toInt());
#endif

  if (_buttonMode[buttonIndex] < 1 || _buttonMode[buttonIndex] > 5)
  { // Button mode not configured as output
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
  if (clickResult)
  {
    SERIALPRINTLN(":OK");
  }
  else
  {
    SERIALPRINTLN(":Er");
  }
#endif

  _communication->webServer->send(
      200,
      "application/json",
      "{\"b\":" + String(++buttonIndex) + ",\"r\":" + String(clickResult) + ",\"s\":" + String(buttonStatus) + "}");
}

void handleWebServerConfig(void)
{
#if SERIAL_DEBUG
  SERIALPRINT("req: config,");
#endif
  if (_communication->webServer->arg("i") == "")
  { // No button number provided
#if SERIAL_DEBUG
    SERIALPRINTLN("not i");
#endif
    _communication->webServer->send(400);
    return;
  }
  if (_communication->webServer->arg("f") == "" ||
      _communication->webServer->arg("d") == "" ||
      _communication->webServer->arg("dv") == "")
  { // No config provided
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
  uint16_t buttonIndex = _communication->webServer->arg("i").toInt() - 1;
  uint8_t btnMode = _communication->webServer->arg("f").toInt();

  if (_buttonMode[buttonIndex] != btnMode)
    configureButton(buttonIndex, btnMode);

  uint8_t outIndex = _buttonMode[buttonIndex] - 1;

  if (outIndex < 5)
  {
    dimmable = _communication->webServer->arg("d").toInt();

    if (dimmable != _outputPinController[outIndex]->getDimmable())
    {
      SERIALPRINT("setDimm-");

      if (_outputPinController[outIndex]->getLockDimm())
      {
        SERIALPRINT("lock,");

        result = 0;
        error = "dimm lock," + String(outIndex);
      }
      else if (!_outputPinController[outIndex]->setDimmable(dimmable))
      {
        SERIALPRINT("Err,");

        result = 0;
        error = "dimm cfg," + String(outIndex) + "," + String(dimmable);
      }
    }

    if (result)
    {
      if (dimmable)
      {
        dimmerValue = _communication->webServer->arg("dv").toInt();

        if (dimmerValue == 1)
          dimmerValue = 2;
        else if (dimmerValue > 100)
          dimmerValue = 100;

        if (dimmerValue && _outputPinController[outIndex]->getValue() != light_state::ON)
        {
          SERIALPRINT("DimmVal-");
          _outputPinController[outIndex]->on();
          SERIALPRINT("Rec,");

          if (!_flash->setButtonDimmer(_buttonMode[buttonIndex], dimmerValue))
          {
            SERIALPRINT("err");

            result = 0;
            error = "dimm rec," + String(buttonIndex) + "," + String(_buttonMode[buttonIndex]) + "," + String(dimmerValue);
          }
          else
          {
            SERIALPRINT("OK");
          }
        }
      }
      else
      {
        if (!_flash->setButtonDimmer(_buttonMode[buttonIndex], 0))
        {
#if SERIAL_DEBUG
          SERIALPRINT("err");
#endif
          result = 0;
          error = "dimm rec," + String(buttonIndex) + "," + String(_buttonMode[buttonIndex]);
        }
        else
        {
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
  if (outIndex < 5)
  {
    SERIALPRINT("d-");
    SERIALPRINT(dimmable);
    if (result && dimmable)
    {
      SERIALPRINT("dv-");
      SERIALPRINT(dimmerValue);
    }
  }
#endif

  _communication->webServer->send(
      200,
      "application/json",
      "{\"r\":" + String(result) + (!result ? ",\"e\":\"" + error + "\"" : "") + ",\"d\":" + getButtonJsonConfig(buttonIndex) + '}');
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

  // ALL LEDs brightness
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  // analogWrite(LED_BUILTIN, 20); // 0 - 255

/**
 * INITIALIZE OUTPUT PINS (LIGHT)
 */
#if SERIAL_DEBUG
  SERIALPRINT("\nInitializing OUTPUTS: ");
#endif
  delay(1000);

  bool pinDimmable;
  for (int outPinCnt = 0; outPinCnt < 5; outPinCnt++)
  {
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
        !_buttonPinDimmable[outPinCnt]);
    delay(500);
  }

/*
  INITIALIZE BUTTONS
*/
#if SERIAL_DEBUG
  SERIALPRINT("\nInitializing buttons: ");
  if (BUTTON_COUNT)
  {
    SERIALPRINT(BUTTON_COUNT);
    SERIALPRINTLN(": ");
  }
  else
  {
    SERIALPRINTLN("None");
  }
#endif
  delay(1000);

  for (int btnCnt = 0; btnCnt < BUTTON_COUNT; btnCnt++)
  {
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
  SERIALPRINT("Wifi mde:");
  SERIALPRINT(_flash->getWifiMode());
  SERIALPRINT("-");
  SERIALPRINTLN(WIFI_OPERATION_MODE_AP);
  delay(1000);

  //_communication->getWifiStatus();

  _communication->setWifiMode(_flash->getWifiMode());

  // wifiServer.begin();
  // delay(1000);

  Serial.setDebugOutput(SERIAL_DEBUG);
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
  if (critical)
    SERIALPRINT("[CRITICAL] ");
  SERIALPRINTLN("System reboot requested");

  ESP.restart();
}
