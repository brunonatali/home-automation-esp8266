
#include "MCommunication.h"


MCommunication::MCommunication(String ssid, String password)
{
    IPAddress WifiIpLocal();
    IPAddress WifiIpGateway();
    IPAddress WifiIpSubnet(255, 255, 255, 0);

    WifiSssid = ssid;
    WifiPassword = password;
}

MCommunication::~MCommunication(void)
{
    this->stopWifi();
}

bool MCommunication::setWifiMode(uint8_t mode, String ssid, String password)
{
    uint16_t time = 0;

    // Disable WiFi first
    if (WiFi.getMode() != WIFI_OFF)
        this->stopWifi();
    
    if (mode == WIFI_OPERATION_MODE_AP) {
        WiFi.mode(WIFI_AP);
        while (WiFi.getMode() != WIFI_AP && time++ < 400) // Give 20 sec to change wifi to Access Point
            delay(50);
        if (time > 400)
            return false;

        if (WifiIpLocal.toString() != "(IP unset)" && WifiIpGateway.toString() != "(IP unset)" && WifiIpSubnet.toString() != "(IP unset)") {
            if (!WiFi.softAPConfig(WifiIpLocal, WifiIpGateway, WifiIpSubnet))
                return false;
        }

        uint8_t channel = 1; // 1 - 13
        bool hidden = false; // hide ssid
        uint8_t maxConnection = 1; // 0 - 8

        if (ssid.length() != 0 && password.length() != 0) {
            WifiSssid = ssid;
            WifiPassword = password;
        }
        if (WifiSssid.length() == 0 || WifiPassword.length() == 0)
            return false;
        
        if (!WiFi.softAP(WifiSssid, WifiPassword, channel, hidden, maxConnection))
            return false;
        WifiIpLocal = WiFi.softAPIP();

        WifiMode = mode;

        Serial.print("AP IP address: ");
        Serial.println(WifiIpLocal);

    } else if (mode == WIFI_OPERATION_MODE_CLIENT) {

    }

    return true;
}

bool MCommunication::setWifiIp(IPAddress *local, IPAddress *gateway, IPAddress *subnet)
{
    WifiIpLocal = *local;
    WifiIpGateway = *gateway;
    WifiIpSubnet = *subnet;
}

bool MCommunication::startWifi(void)
{
    return this->setWifiMode(WifiMode, WifiSssid, WifiPassword);
}

bool MCommunication::stopWifi(void)
{
    uint8_t time = 0;

    WiFi.mode(WIFI_OFF);
    while (WiFi.getMode() != WIFI_OFF && time++ < 200) // 10 sec to disable wifi
        delay(50);
    if (time > 200)
        return false;

}

bool MCommunication::restartWifi(void)
{
    return this->stopWifi() && this->startWifi();
}

void MCommunication::getWifiStatus(void)
{
  Serial.print("Wifi status:");
  Serial.println(WiFi.status());
}

void setWifiSerialDebug(bool set)
{
    WiFi.printDiag(Serial);
    delay(1000);
}