
#include "MCommunication.h"


MCommunication::MCommunication(String ssid, String password)
{
    IPAddress WifiIpLocal();
    IPAddress WifiIpGateway();
    IPAddress WifiIpSubnet(255, 255, 255, 0);

    WifiSssid = ssid;
    WifiPassword = password;

    localWebPage = "<!DOCTYPE html><html><head><title>Home</title><meta charset=UTF-8><meta name=viewport content=\"width=device-width,initial-scale=1\"><style type=text/css>:root{--vh:1vh}body{width:100vw;height:calc(var(--vh) * 100);display:table;margin:0;background-color:#fff;font-family:Arial;overflow:hidden}.pa{position:absolute}.if{font-size:20vw;color:#a6a3a3}.c{top:50%;left:50%;-webkit-transform:translate(-50%,-50%);transform:translate(-50%,-50%)}.h{opacity:0}.t{transition:all .3s ease-in 0s}.sb{display:none;z-index:500;opacity:.5;top:0;left:0;background-color:#f2f2f2;width:100vw;height:calc(var(--vh) * 100)}.s{display:none;z-index:501;top:calc(50% - (30vw/2));left:calc(50% - (30vw/2));border-radius:50%;-webkit-animation:spin 2s linear infinite;animation:spin 2s linear infinite;border-top:6px solid #ffac12;width:30vw;height:30vw}.m:after{content:'\\2630'}.mp:after{content:'\\2715'}.wp{top:25%;opacity:1}.w:after{content:'\\1F4F6'}.cnp{top:75%;opacity:1}.cn:after{content:'\\2699'}.sbc{float:right;font-size:10vw;margin-top:10vw}.sbc:after{content:'\\2705'}.dd{float:right}.swh{position:absolute;width:60px}.swhl{left:5%}.swhr{left:calc(95% - 60px)}.swht{top:5%}.swhm{top:calc(calc(var(--vh) * 50) - 11px)}.swhb{top:calc(calc(var(--vh) * 95) - 22px)}.swc{position:absolute;left:5%}.swl{display:block;height:22px;border:2px solid #E3E3E3;border-radius:22px;background:#FFF;transition:background-color .3s ease-in}.swl:before{content:'';width:22px;background:#2D5DEE;position:absolute;top:0;bottom:0;right:37px;border:2px solid #E3E3E3;border-radius:22px;transition:all .3s ease-in 0s;box-shadow:0 0 10px 5px #2cb7fb}.swc:checked+.swl{background:#49E845}.swc:checked+.swl,.swc:checked+.swl:before{border-color:#49E845}.swc:checked+.swl:before{right:0}.swcf{border-color:#ffae00!important}.swdm{position:relative;width:60px}.swld:before{box-shadow:0 0 #fff;background:#000}.swdml:before{box-shadow:0 0 #fff;background:#F9A154}.mo{display:none;background:#fdbf39;z-index:600;width:75vw;border-radius:20px;padding:5vw}.no{top:-100vh;max-width:95vw;padding:15px;transition:all 1s linear;border-radius:5px}.nos{z-index:700;top:5vh!important}@-webkit-keyframes spin{0%{-webkit-transform:rotate(0deg)}100%{-webkit-transform:rotate(360deg)}}@keyframes spin{0%{transform:rotate(0deg)}100%{transform:rotate(360deg)}}</style></head><body><div class=\"swh swhl swht\"><input type=checkbox class=\"swc h uE\" id=1><label class=swl for=1></label></div><div class=\"swh swhl swhm\"><input type=checkbox class=\"swc h uE\" id=2><label class=swl for=2></label></div><div class=\"swh swhl swhb\"><input type=checkbox class=\"swc h uE\" id=3><label class=swl for=3></label></div><div class=\"swh swhr swht\"><input type=checkbox class=\"swc h uE\" id=4><label class=swl for=4></label></div><div class=\"swh swhr swhm\"><input type=checkbox class=\"swc h uE\" id=5><label class=swl for=5></label></div><div class=\"swh swhr swhb\"><input type=checkbox class=\"swc h uE\" id=6><label class=swl for=6></label></div><div id=w class=\"pa if h c t w\"></div><div id=c class=\"pa if h c t cn\"></div><div id=m class=\"pa if c m\"></div><div id=sb class=\"pa sb\"></div><div id=ld class=\"pa s\"></div><div id=mo class=\"pa c mo\"></div><div id=mo class=\"pa c mo\"></div><div id=msg class=\"pa c no\"></div></body></html><script>var cBt,cKB,cKL,d=document,w=window,sbe=d.getElementById(\"sb\"),sbes=sbe.style,dlds=d.getElementById(\"ld\").style,M=d.getElementById(\"m\"),cM=d.getElementById(\"c\"),wM=d.getElementById(\"w\"),mO=d.getElementById(\"mo\"),mOs=mO.style,cing=!1,ming=!1,bs={},btLn=6,sb=()=>{sbes.display=\"block\"},sl=()=>{sb(),dlds.display=\"block\",sbe.onclick=(()=>!1)},hb=()=>{sbes.display=\"none\",sbe.onclick=(()=>!1),cing&&(d.getElementById(cBt).checked=!1)},hl=()=>{ming?sbe.onclick=hm:hb(),dlds.display=\"none\"},itB=e=>{[...d.getElementsByClassName(\"swc uE\")].forEach(e)},uB=()=>{itB(e=>{bs[e.id]=e.checked,e.checked=!1})},cB=()=>{itB(e=>{e.checked=bs[e.id]})},hm=()=>{ming=!1,hb(),mOs.display=\"none\"},sm=()=>{ming=1,sb(),mOs.display=\"block\",sbe.onclick=hm},sVH=()=>{d.documentElement.style.setProperty(\"--vh\",.01*w.innerHeight+\"px\")},pBt=()=>{itB((e,s)=>{console.log(e,s);(typeof bcfg[e.id] != 'undefined' && bcfg[e.id].f<200)?e.checked=bcfg[e.id].s:e.nextSibling.classList.add(\"swld\")})},msg=(e,s)=>{var t=d.getElementById(\"msg\");t.innerText=e,t.style.backgroundColor=s?\"#67fa67\":\"#ff7b64\",t.classList.add(\"nos\"),setTimeout(()=>{t.classList.remove(\"nos\")},s?3e3:5e3)},req=(e,s,t)=>{console.log(e);var d=new XMLHttpRequest;d.open(\"GET\",e,1),d.setRequestHeader(\"Content-type\",\"text/plain\"),d.onreadystatechange=(()=>{4==d.readyState&&d.status>=300&&t()});var c=setTimeout(()=>{d.abort(),t()},5e3);d.onload=(e=>{clearTimeout(c),s(d.response,e)}),d.send(null)},cK=()=>{cKB.checked?cKL.display=\"block\":cKL.display=\"none\"};w.addEventListener(\"load\",()=>{void 0===bcfg[6]&&(btLn=5,d.getElementsByClassName(\"swc\")[0].parentElement.remove()),pBt(),sVH(),w.addEventListener(\"resize\",()=>{sVH()}),itB(e=>{e.addEventListener(\"change\",e=>{let s=6==btLn?e.target.id:e.target.id-1;console.log(s,bcfg[s].f);var t=()=>{hl(),mOs.zIndex=600};if(cing){let c,l;cBt=e.target.id;for(let e=1;e<=5;e++)c+=\"<option value='\"+e+\"' \"+(bcfg[s].f==e?'selected=\"selected\"':\"\")+\"'>\"+e+\"</option>\";for(let e=2;e<101;e++)l+=\"<option value='\"+e+\"' \"+(bcfg[s].dv==e?'selected=\"selected\"':\"\")+\">\"+e+\"%</option>\";mO.innerHTML=\"Botão \"+e.target.id+\"<p>Saída:<select id='bOut' class='dd'>\"+c+\"<option value='200' \"+(200==bcfg[s].f?'selected=\"selected\"':\"\")+\">Dimmer</option><option value='255' \"+(bcfg[s].f>253?'selected=\"selected\"':\"\")+\">Desabilitado</option></select></p>Dimerizável:<div class='dd swdm'><input type='checkbox'class='swc h'id='dmm'onclick='cK();' \"+(bcfg[s].d?\"checked\":\"\")+\"><label class='swdml swl'for='dmm'></label></div><p><div id='dL' style='display:none;'>Valor do dimmer:<select id='bDv' class='dd'>\"+l+\"</select></div><p><div id='sbc'class='sbc'></div></p>\",cKB=d.getElementById(\"dmm\"),cKL=d.getElementById(\"dL\").style,sm(),cK(),d.getElementById(\"sbc\").onclick=(()=>{sl(),mOs.zIndex=300,req(\"/cfg/?i=\"+e.target.id+\"&f=\"+d.getElementById(\"bOut\").value+\"&d=\"+(cKB.checked?1:0)+\"&dv=\"+d.getElementById(\"bDv\").value,(s,d)=>{t();let c=JSON.parse(s);1==c.r?(msg(\"Configurado\",1),hm(),bcfg[e.target.id]=c.d):void 0!==c.e?(hl(),msg(\"err:\"+c.e,0)):(hl(),msg(\"unknown error\",0))},t)})}else{if(bcfg[s].f>199)return void(e.target.checked=!1);sl(),req(\"/set/?i=\"+e.target.id+\"&s=\"+(e.target.checked?1:0),(s,t)=>{let c=JSON.parse(s);1==c.r?(msg(\"OK\",1),bcfg[e.target.id].s=c.s,d.getElementsByClassName(\"swc\")[e.target.id-1].checked=c.s):void 0!==c.e?msg(\"err:\"+c.e,0):msg(\"unknown error\",0),hl()},t)}},!1)})}),M.onclick=(()=>{(cing=!cing)?(uB(),itB(e=>{e.nextElementSibling.classList.add(\"swcf\")}),M.classList.add(\"mp\"),M.classList.remove(\"m\"),wM.classList.add(\"wp\"),cM.classList.add(\"cnp\")):(cB(),itB(e=>{e.nextElementSibling.classList.remove(\"swcf\")}),M.classList.add(\"m\"),M.classList.remove(\"mp\"),wM.classList.remove(\"wp\"),cM.classList.remove(\"cnp\"))}),wM.onclick=(()=>{mO.innerText=\"Não disponível\",sm()}),cM.onclick=(()=>{mO.innerText=\"Não disponível\",sm()});var bcfg={";
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

        webServer->on("/", handleWebServerRoot);
        webServer->on("/set/", handleWebServerSetOnOff);
        webServer->on("/cfg/", handleWebServerConfig);

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

void setWifiSerialDebug(void)
{
    WiFi.printDiag(Serial);
    delay(1000);
}


