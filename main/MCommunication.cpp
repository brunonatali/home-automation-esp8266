
#include "MCommunication.h"


MCommunication::MCommunication(String ssid, String password)
{
    IPAddress WifiIpLocal();
    IPAddress WifiIpGateway();
    IPAddress WifiIpSubnet(255, 255, 255, 0);

    WifiSssid = ssid;
    WifiPassword = password;

    localWebPage = "<!DOCTYPE html><html><head><title>Home</title><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><style type='text/css'>:root{--vh:1vh;};body{width:100vw;height:calc(var(--vh)*100);display:table;margin:0px;background-color:white;font-family:Arial;overflow:hidden;}.pa{position:absolute;}.sb{display:none;z-index:500;opacity:0.5;top:0px;left:0px;background-color:#f2f2f2;width:100vw;height:calc(var(--vh)*100);}.s{display:none;z-index:501;top:calc(50% - (30vw/2));left:calc(50% - (30vw/2));border-radius: 50%;-webkit-animation: spin 2s linear infinite;animation: spin 2s linear infinite;border-top:6px solid #ffac12;width:30vw;height:30vw;}.if{font-size:20vw;color:#a6a3a3;}.c{top:50%;left:50%;-webkit-transform:translate(-50%,-50%);transform:translate(-50%,-50%);}.h{opacity:0;}.t{transition:all 0.3s ease-in 0s;}.m:after{content:'\\2630';}.mp:after{content:'\\2715';}.wp{top:25%;opacity:1;}.w:after{content:'\\1F4F6';}.cnp{top:75%;opacity:1;}.cn:after{content:'\\2699';}.sbc{float:right;font-size:10vw;margin-top:10vw;}.sbc:after{content:'\\2705';}.dd{float:right;}.swh{position:absolute;width:60px;}.swhl{left:5%;}.swhr{left:calc(95% - 60px);}.swht{top:5%}.swhm{top:calc(calc(var(--vh)*50) - 11px)}.swhb{top:calc(calc(var(--vh)*95) - 22px)}.swc{position:absolute;left:5%;}.swl{display:block;height:22px;border: 2px solid #E3E3E3;border-radius: 22px;background: #FFFFFF;transition: background-color 0.3s ease-in;}.swl:before{content: '';width: 22px;background:#2D5DEE;position:absolute;top:0;bottom:0;right:37px;border:2px solid #E3E3E3;border-radius:22px;transition:all 0.3s ease-in 0s;box-shadow:0px 0px 10px 5px #2cb7fb;}.swc:checked + .swl{background:#49E845;}.swc:checked + .swl,.swc:checked + .swl:before{border-color: #49E845;}.swc:checked + .swl:before {right: 0px;}.swcf{border-color: #ffae00 !important;}.swdm{position:relative;width:60px;}.swld:before{box-shadow:0px 0px 0px 0px #fff;background:#000;}.swdml:before{box-shadow:0px 0px 0px 0px #fff;background:#F9A154;}.mo{display:none;background:#fdbf39;z-index:600;width:75vw;border-radius:20px;padding: 5vw;}@-webkit-keyframes spin {0% {-webkit-transform:rotate(0deg);}100% {-webkit-transform:rotate(360deg);}}@keyframes spin {0%{transform:rotate(0deg);}100% {transform:rotate(360deg);}}</style></head><body><div class='swh swhl swht'><input type='checkbox'class='swc h'id=\"1\"><label class='swl'for='1'></label></div><div class='swh swhl swhm'><input type='checkbox'class='swc h'id=\"2\"><label class='swl'for='2'></label></div><div class='swh swhl swhb'><input type='checkbox'class='swc h'id=\"3\"><label class='swl'for='3'></label></div><div class='swh swhr swht'><input type='checkbox'class='swc h'id=\"4\"><label class='swl'for='4'></label></div><div class='swh swhr swhm'><input type='checkbox'class='swc h'id=\"5\"><label class='swl'for='5'></label></div><div class='swh swhr swhb'><input type='checkbox'class='swc h'id=\"6\"><label class='swl'for='6'></label></div><div id='w'class='pa if h c t w'></div><div id='c'class='pa if h c t cn'></div><div id='m'class='pa if c m'></div><div id='sb'class='pa sb'></div><div id='ld'class='pa s'></div><div id='mo'class='pa c mo'></div></body></html><script>var d=document;var w=window;var sbe=d.getElementById('sb');var sbes=sbe.style;var dlds=d.getElementById('ld').style;var M=d.getElementById('m');var cM=d.getElementById('c');var wM=d.getElementById('w');var mO=d.getElementById('mo');var mOs=mO.style;var cing=!1;var ming=!1;var bs={};var cBt,btLn=5;var sb=()=>{sbes.display='block';};var sl=()=>{sb();dlds.display='block';sbe.onclick=()=>{return !1;};};var hb=()=>{sbes.display='none';sbe.onclick=()=>{return !1;};if(cing)d.getElementById(cBt).checked=!1};var hl=()=>{if(!ming)hb();else sbe.onclick=hm;dlds.display='none';};var itB=(f)=>{[...d.getElementsByClassName(\"swc\")].forEach(f);};var uB=()=>{itB((b,i)=>{bs[i+1]=b.checked;b.checked=!1;});};var cB=()=>{itB((b,i)=>{b.checked=bs[i+1];});};var hm=()=>{ming=!1;hb();mOs.display='none';};var sm=()=>{ming=1;sb();mOs.display='block';sbe.onclick=hm;};var sVH=()=>{d.documentElement.style.setProperty('--vh',(w.innerHeight*0.01)+'px');};var pBt=()=>{itB((b,i)=>{if(bcfg[++i].f<200)b.checked=bcfg[i].s;else b.nextSibling.classList.add('swld');});};var req=(u,fo,fe)=>{console.log(u);var xhr=new XMLHttpRequest();xhr.open('GET',u,1);xhr.setRequestHeader('Content-type','text/plain');xhr.onreadystatechange=()=>{if(xhr.readyState==4&&xhr.status>=300)fe();};var xhrT=setTimeout(()=>{xhr.abort();fe();}, 5000);xhr.onload=(e)=>{clearTimeout(xhrT);fo(xhr.response, e);};xhr.send(null);};var cKB,cKL,cK=()=>{if(cKB.checked)cKL.display=\"block\";else cKL.display=\"none\";};w.addEventListener('load',()=>{if(typeof bcfg[6]=='undefined'){btLn=5;d.getElementsByClassName(\"swc\")[0].parentElement.remove();}pBt();sVH();w.addEventListener('resize',()=>{sVH();});itB((b)=>{b.addEventListener('change',(e)=>{let mE=(btLn==6?e.target.id:e.target.id-1);var rC=()=>{hl();mOs.zIndex=600;};if(cing){cBt=e.target.id;let too,tod;for(let i=1;i<btLn;i++)too+=\"<option value='\"+i+\"' \"+(bcfg[mE].f==i?'selected=\"selected\"':'')+\"'>\"+i+\"</option>\";for(let n=2;n<101;n++)tod+=\"<option value='\"+n+\"' \"+(bcfg[mE].dv==n?'selected=\"selected\"':'')+\">\"+n+\"%</option>\";mO.innerHTML=\"Botão \"+e.target.id+\"<p>Saída:<select id='bOut' class='dd'>\"+too+\"<option value='200' \"+(bcfg[mE].f==200?'selected=\"selected\"':'')+\">Dimmer</option><option value='255' \"+(bcfg[mE].f>253?'selected=\"selected\"':'')+\">Desabilitado</option></select></p>Dimerizável:<div class='dd swdm'><input type='checkbox'class='swc h'id='dmm'onclick='cK();' \"+(bcfg[mE].d?'checked':'')+\"><label class='swdml swl'for='dmm'></label></div><p><div id='dL' style='display:none;'>Valor do dimmer:<select id='bDv' class='dd'>\"+tod+\"</select></div><p><div id='sbc' class='sbc'></div></p>\";cKB=d.getElementById('dmm');cKL=d.getElementById('dL').style;sm();cK();d.getElementById('sbc').onclick=()=>{sl();mOs.zIndex=300;req('/?i='+e.target.id+'&f='+d.getElementById('bOut').value+'&d='+(cKB.checked?1:0)+'&dv='+d.getElementById('bDv').value,(x,y)=>{rC();let j=JSON.parse(x);if(j.r==1)hm();pBt();},rC);};}else{if(bcfg[mE].f>199){e.target.checked=!1;return;}sl();req('/?i='+e.target.id+'&s='+(e.target.checked?1:0),(x,y)=>{console.log(x,y);hl();},rC);}},!1);});});M.onclick=()=>{if(!(cing=!cing)){cB();itB((b)=>{b.nextSibling.classList.remove('swcf');});M.classList.add('m');M.classList.remove('mp');wM.classList.remove('wp');cM.classList.remove('cnp');}else{uB();itB((b)=>{b.nextSibling.classList.add('swcf');});M.classList.add('mp');M.classList.remove('m');wM.classList.add('wp');cM.classList.add('cnp');}};wM.onclick=()=>{mO.innerText='Não disponível';sm();};cM.onclick=()=>{mO.innerText='Não disponível';sm();};var bcfg={";
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
#if SERIAL_DEBUG
        Serial.print("AP IP address: ");
        Serial.println(WifiIpLocal);
#endif

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
#if SERIAL_DEBUG
    Serial.print("Wifi status:");
    Serial.println(WiFi.status());
#endif
}

void setWifiSerialDebug(bool set)
{
    WiFi.printDiag(Serial);
    delay(1000);
}


