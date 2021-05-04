
#include "MCommunication.h"


MCommunication::MCommunication(String ssid, String password)
{
    IPAddress WifiIpLocal();
    IPAddress WifiIpGateway();
    IPAddress WifiIpSubnet(255, 255, 255, 0);

    WifiSssid = ssid;
    WifiPassword = password;

    localWebPage = "<!DOCTYPE html><html><head><title>Home</title><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><style type='text/css'>body{width:100vw;height:100vh;display:table;margin:0px;background-color:white;font-family:Arial;}.pa{position:absolute;}.sb{display:none;opacity:0.5;top:0px;left:0px;z-index:500;background-color:#f2f2f2;width:100vw;height:100vh;}.s{display:none;top:calc(50% - (30vw/2));left:calc(50% - (30vw/2));border-radius: 50%;-webkit-animation: spin 2s linear infinite;animation: spin 2s linear infinite;border-top:6px solid #ffac12;width:30vw;height:30vw;}.g{top:50%;left:50%;transform:translate(-50%, -50%);font-size:20vw;color:#a6a3a3;}.g:after{content:'\\2699';}.swh{position:absolute;width:60px;}.swhl{left:5%;}.swhr{left:calc(95% - 60px);}.swht{top:5%}.swhm{top:calc(50vh - 11px)}.swhb{top:calc(95vh - 22px)}.swc{position:absolute;opacity:0;left:5%;}.swl{display:block;height:22px;border: 2px solid #E3E3E3;border-radius: 22px;background-color: #FFFFFF;transition: background-color 0.3s ease-in;}.swl:before{content: '';width: 22px;background:#2D5DEE;position:absolute;top:0;bottom:0;right:37px;border:2px solid #E3E3E3;border-radius:22px;transition:all 0.3s ease-in 0s;box-shadow:0px 0px 10px 5px #2cb7fb;}.swc:checked + .swl{background-color:#49E845;}.swc:checked + .swl,.swc:checked + .swl:before{border-color: #49E845;}.swc:checked + .swl:before {right: 0px;}.swcf{border-color: #ffae00;}@-webkit-keyframes spin {0% {-webkit-transform:rotate(0deg);}100% {-webkit-transform:rotate(360deg);}}@keyframes spin {0%{transform:rotate(0deg);}100% {transform:rotate(360deg);}}</style></head><body><div class='swh swhl swht'><input type='checkbox'class='swc'id='1'checked><label class='swl'for='1'></label></div><div class='swh swhl swhm'><input type='checkbox'class='swc'id='2'checked><label class='swl'for='2'></label></div><div class='swh swhl swhb'><input type='checkbox'class='swc'id='3'checked><label class='swl'for='3'></label></div><div class='swh swhr swht'><input type='checkbox'class='swc'id='4'checked><label class='swl'for='4'></label></div><div class='swh swhr swhm'><input type='checkbox'class='swc'id='5'checked><label class='swl'for='5'></label></div><div class='swh swhr swhb'><input type='checkbox'class='swc'id='6'checked><label class='swl'for='6'></label></div><div id='c'class='pa g'></div><div id='bld'class='pa sb'></div><div id='ld'class='pa s'></div><script>var d=document;var dblds=d.getElementById('bld').style;var dlds=d.getElementById('ld').style;var cing=!1;var bs={};var sl=()=>{dblds.display='block';dlds.display='block';};var hl=()=>{dblds.display='none';dlds.display='none';};var itB=(f)=>{for(let i=1;i<=6;i++)f(d.getElementById(i),i)};var uB=()=>{itB((b,i)=>{bs[i]=b.checked;b.checked=!1;});};var cB=()=>{itB((b,i)=>{b.checked=bs[i];});};var req=(u,fo,fe)=>{var xhr=new XMLHttpRequest();xhr.open('GET',u,1);xhr.setRequestHeader('Content-type','text/plain');xhr.timeout=5000;xhr.onreadystatechange=()=>{if(xhr.readyState==4&&xhr.status>=300)fe();};xhr.onload=(e)=>{fo(xhr.response, e);};xhr.send(null);};window.addEventListener('load',()=>{let m=d.documentElement;if(m.requestFullScreen)m.requestFullScreen();else if(m.mozRequestFullScreen)m.mozRequestFullScreen();else if(m.webkitRequestFullScreen)m.webkitRequestFullScreen();});itB((b)=>{b.addEventListener('change',(e)=>{if(cing){}else{sl();req('/?i='+e.target.id+'&s='+(e.target.checked?1:0),(x,y)=>{console.log(x,y);hl();});}},!1);});d.getElementById('c').onclick=()=>{if(!(cing=!cing)){cB();itB((b)=>{b.nextSibling.classList.remove('swcf');});}else{uB();itB((b)=>{b.nextSibling.classList.add('swcf');});}};</script></body></html>";
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
        webServer = new ESP8266WebServer(80);

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

        webServer->on("/", handleWebServerRequest);
        webServer->begin(80);

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


