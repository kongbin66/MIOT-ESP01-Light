#include <Arduino.h>
 #include "ESP8266WiFi.h"



#define BLINKER_MIOT_LIGHT
#define BLINKER_WIFI

#include <Blinker.h>

char auth[] = "e351f69e58c6";   //换成APP获取到的密匙
char ssid[] = "UROBEI";          //WiFi账号
char pswd[] = "UROBEI123";   //WIFI密码



int GPIOpin=2;//定义GPIO口用于控制继电器
int GPIOFENGS=0;//风扇连接�?�?

#define BUTTON_1 "ButtonKey"


BlinkerButton Button1("12");     //这里需要根�?�?己在BLINKER里面设置的名字进行更�?


void button1_callback(const String & state)
{
    //BLINKER_LOG("get button state: ", state);
    Serial.println("in button1\n");
    Blinker.vibrate(); 
    if ((state=="123")||(state=="on")) {
        //digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(GPIOFENGS,LOW);
     
        Button1.print("on");
    } else if((state=="456")||(state=="off")){
        //digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(GPIOFENGS,HIGH);
        Button1.print("off");
    }
}



void miotPowerState(const String & state)
{
    Serial.println("in miotpowerState\n");
    BLINKER_LOG("need set power state: ",state);

    if (state == BLINKER_CMD_OFF) 
    {//如果�?音接收到�?关闭�?就�?�置GPIO口为高电�?
        digitalWrite(GPIOFENGS,HIGH);
        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
    else if (state == BLINKER_CMD_ON) 
    {
        digitalWrite(GPIOFENGS,LOW);
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
    }
}

void mySmartWifiConfig()
{
  WiFi.mode(WIFI_STA );
  Serial.println("SmartwifiConfig:");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartwifiConfig ok\n");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf("PSW:%s", WiFi.psk().c_str());
      break;
    }
  }
}

bool autoConfig()
{
  WiFi.disconnect(true);
  WiFi.begin();
  for (size_t i = 0; i < 20; i++)
  {
    int wifiStatus = WiFi.status();
    if (wifiStatus == WL_CONNECTED)
    {
      Serial.println("connected!\n");
      return 1;
    }
    else
    {
      delay(1000);
      Serial.println("wait...");
    }
  }
  Serial.println("no connect!");
  return 0;
}

void setup() {
    Serial.begin(115200);
    pinMode(GPIOpin,OUTPUT);
    pinMode(GPIOFENGS,OUTPUT);
    digitalWrite(GPIOFENGS,LOW);
    digitalWrite(GPIOpin,HIGH);//初�?�化，由于继电器�?低电平触发。所以刚开始�?�为高电�?

    if (!autoConfig())
    {
        mySmartWifiConfig();
    }
    //pinMode(LED_BUILTIN, OUTPUT);
    
    //digitalWrite(LED_BUILTIN, HIGH);
    Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
    Button1.attach(button1_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);//这�?�代码一定�?�加，不加小爱同学控制不�?,务必在回调函数中反�?��?�控制状�?
    

}

void loop()
{
    if (!WiFi.isConnected())
  {
    Serial.println("no");
    digitalWrite(GPIOpin,HIGH);
    delay(1000);
  }else digitalWrite(GPIOpin,LOW);
  
    Blinker.run();
}

