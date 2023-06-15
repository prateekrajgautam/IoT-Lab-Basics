# Program 1 – LED Blink

```cpp
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include "clock.h"
#include "node.h"
#include "task.h"
//add dio.h for using led related APIs
#include "dio.h"
#include "pcInterface.h"
#include "config.h"
//task types to be used in this demo
#define LED_ON 0
#define LED_OFF 1
void startNode()
{ //set task to switch on the led
addTask(USER,LED_ON,1*SECOND);
//addTask(USER,LED_ON,5*SECOND); }
int count = 0;
uint32 time = 2*SECOND;
void userTaskHandler(uint8 taskType)
{
//check the current taskType and accordingly take the decision.
switch (taskType)
{
case LED_ON:
{
ledOn();
addTask(USER,LED_OFF,time);
break;
}
case LED_OFF:
{
ledOff();
addTask(USER,LED_ON,time);
count++;
if(count==5)
{
time = 5*SECOND;
}
break;
}}}
void userReceiveDataPacket(uint8* payload,uint8 length,uint16 prevAddr,uint8 linkQuality)
{}
void userCriticalTaskHandler(uint8 critTaskType)
{}
```





# Program 2 – Multihop_PanCoord

```cpp
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include "clock.h"
#include "node.h"
#include "task.h"
#include "pcInterface.h"
#include "mac.h"
#include "routing.h"
#include "phy.h"
#define USER_PACKET_TYPE 0x30
#define UART0
void startNode()
{
sendToPcInit();
macInit();
routingInit();
}
void userTaskHandler(uint8 taskType)
{}
void userReceiveDataPacket(uint8* payload,uint8 length,uint16 prevAddr,uint8 linkQuality)
{
uint8 tmp;
uint16 light;
uint16 nodeId;
if (payload[0]==USER_PACKET_TYPE)
{
tmp=payload[1];
light=payload[2];
light=light<<8;
light=light | payload[3];
nodeId=payload[4];
nodeId=nodeId<<8;
nodeId=nodeId|payload[5];
updateSTLdb(nodeId,light,tmp); }
msdelay(50);
uint8 packetToPC[100];
int i,j;
packetToPC[0]=0x48;
packetToPC[1]=0x00;
packetToPC[2]=0x00;
for(i=length-1,j=3;i>4;i=i-2,j=j+2)
{
packetToPC[j]=payload[i-1];
packetToPC[j+1]=payload[i]; }
updateGendb(packetToPC,j); }
```

# Program 3 – Pressure

```cpp
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include "clock.h"
#include "node.h"
#include "task.h"
#include "dio.h"
#include "pcInterface.h"
#include "tempPressure.h"
#include "humidSensor.h"
#include "i2c.h"
#define READ_MET 0
void startNode()
{
pressureInit();
humInit();
sendToPcInit();
setUpperHumidThreshold(50,45);
addTask(USER,READ_MET,2*SECOND);
}
int fla=0;
void userTaskHandler(uint8 taskType)
{
if(fla==0)
{
fla=1;
ledOn();
}
else
{
fla=0;
ledOff();
}
uint32 pressure;
uint16 temperature;
uint16 humid;
uint8 packet[4];
readTempPressure(&pressure, &temperature);
humid=readHumid();
updateMetdb(getNodeId(),humid,pressure,temperature);
addTask(USER,READ_MET,2*SECOND);
}
void userReceiveDataPacket(uint8* payload,uint8 length,uint16 prevAddr,uint8 linkQuality)
{}
void userCriticalTaskHandler(uint8 critTaskType)
{}

```



# Program 4 – Light and Temperature



```cpp
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include "clock.h"
#include "node.h"
#include "task.h"
#include "dio.h"
#include "tmpSensor.h"
#include "lightSensor.h"
#include "pcInterface.h"
#define READ_SENSOR 0
void startNode()
{
//initialize sensors
tmpInit();
lightSensorInit();
//initialize port to communicate with PC
sendToPcInit();
//set task to read sensor info and send to PC
addTask(USER,READ_SENSOR,1*SECOND);
}
int count = 0;
uint32 time = 2*SECOND;
void userTaskHandler(uint8 taskType)
{
uint8 tmp;
uint16 light;
tmp=readTmp();
light=readLux();
//update info in database present in PC
updateSTLdb(getNodeId(),light,tmp);
addTask(USER,READ_SENSOR,time);
count++;
if(count==5)
{
time = 5*SECOND;
}
}
void userReceiveDataPacket(uint8* payload,uint8 length,uint16 prevAddr,uint8 linkQuality)
{}
```





# Program 5 – Light and Temperature (Update Database)



```cpp
#include <jendefs.h>
#include <AppHardwareApi.h>
#include <AppQueueApi.h>
#include "clock.h"
#include "node.h"
#include "task.h"
#include "config.h"
#include "tmpSensor.h"
#include "lightSensor.h"
#include "dio.h"
#include "pcInterface.h"
#define LED_OFF 1
void startNode()
{
//initialize uart
sendToPcInit();
//initialize sensors
dioInit();
tmpInit();
lightSensorInit();
setLightThreshold(2000,20);
}
void userTaskHandler(uint8 taskType)
{
ledOff();
clearLightInterrupt();
}
void userReceiveDataPacket(uint8* payload,uint8 length,uint16 prevAddr,uint8 linkQuality)
{}
void userCriticalTaskHandler(uint8 critTaskType)
{
if (critTaskType==LIGHT_CRIT_TASK)
{
uint8 tmp;
uint16 light;
//read sensor values
tmp=readTmp();
light=readLux();
//update info in database present in PC
ledOn();
addTask(USER,LED_OFF,60);
updateSTLdb(getNodeId(),light,tmp); }
}
void pcInterruptHandler()
{}
```



# Program 6 – Clock Rate

```cpp

```
