

/* By intijk (intijk@gmail.com)
 * For arm Robot
 */
#include <EEPROM.h>
#include <Servo.h> 


/* 
For each servo, save NParameter(now 5) parameters, each as an 8-bit integer.
AMin AMax VMin VMax ADef
*/
#define BUF_SIZE 256
#define NServo 5
#define NParameter 5
#define FrameDelayMs 100
#define debug

int i,j,k;
char buf[BUF_SIZE];

Servo S[NServo];
int Spin[NServo]={5,6,7,8,9};
int AMin[NServo];
int AMax[NServo];
int VMin[NServo];
int VMax[NServo];
int ADef[NServo];

void ReadParameter()
{
  for(i=0;i<NServo;i++){
    AMin[i]=EEPROM.read(i*NParameter);
    AMax[i]=EEPROM.read(i*NParameter+1);
    VMin[i]=EEPROM.read(i*NParameter+2);
    VMax[i]=EEPROM.read(i*NParameter+3);
    ADef[i]=EEPROM.read(i*NParameter+4);
 #ifdef debug
    Serial.print("D: AMin["); Serial.print(i, DEC); Serial.print("]=");Serial.print(AMin[i], DEC); Serial.print(" ");
    Serial.print("AMax["); Serial.print(i, DEC); Serial.print("]=");Serial.print(AMax[i], DEC); Serial.print(" ");
    Serial.print("VMin["); Serial.print(i, DEC); Serial.print("]=");Serial.print(VMin[i], DEC); Serial.print(" ");
    Serial.print("VMax["); Serial.print(i, DEC); Serial.print("]=");Serial.print(VMax[i], DEC); Serial.print(" ");
    Serial.print("ADef["); Serial.print(i, DEC); Serial.print("]=");Serial.print(ADef[i], DEC); Serial.print(" ");
    Serial.println("");
 #endif
  }
}
void VWrite(int n, int V)
{
  S[n].write(V);
  Serial.println(V);
}
void AWrite(int n, int A)
{
  int v=map(A, AMin[n], AMax[n], VMin[n], VMax[n]);
  if(v>VMax[n]) v=VMax[n];
  else if(v<VMin[n]) v=VMin[n];
  //Serial.println(v, DEC);
  S[n].write(v);
}
void WriteDefault()
{
  for(i=0;i<NServo;i++){
    AWrite(i, ADef[i]);
  }
}
void ClearBuf(){
  for(i=0;i<BUF_SIZE;i++){
    buf[i]=0;
  }
}
void setup()
{ 
  Serial.begin(115200); 
  for(i=0;i<NServo;i++){
    S[i].attach(Spin[i]);
  }
  ReadParameter();
  WriteDefault();
}


int readCommand()
{
  int ind=0;
  if(!Serial.available()){
    return 0;
  }
  while(Serial.available()){
    if(ind>=BUF_SIZE){
      do{
        Serial.read();
      }while(Serial.available());
      Serial.println("Buffer overflow");
      return 0; //buffer overflow
    }
    buf[ind++]=Serial.read();
  }
  if(buf[0]>='A' && buf[0]<='Z'){
    return 1;
  }
#ifdef debug
  Serial.print("D: ");
  Serial.println(buf);
  Serial.println("Format Error");
#endif
  return 0;
}
void loop() 
{
  if(readCommand()){
    Serial.println(buf);
    
    if(buf[0]=='A'){ //move to angle
      //A a0 a1 a2 a3 a4
      int ind=0;
      for(i=0;i<NServo;i++){
        while(buf[ind]!=' ')ind++;//skip string
        while(buf[ind]==' ')ind++;//skip space
        int a=atoi(buf+ind);
        if(a>=0){
          AWrite(i, a);
        }
      }
    }else if(buf[0]=='V'){ //value to servo
      //V v0 v1 v2 v3 v4
      int ind=0;
      for(i=0;i<NServo;i++){
        while(buf[ind]!=' ')ind++;//skip string
        while(buf[ind]==' ')ind++;//skip space
        int v=atoi(buf+ind);
        if(v>=0){
          VWrite(i, v);
        }
      }
    }else if(buf[0]=='P'){ //Set parameter 
      //P 1 AMin AMax VMin VMax ADef
      int ind=0, n;
      while(buf[ind]!=' ')ind++;//skip string
      while(buf[ind]==' ')ind++;//skip space
      n=atoi(buf+ind);
      for(i=0;i<NParameter;i++){
        while(buf[ind]!=' ')ind++;//skip string
        while(buf[ind]==' ')ind++;//skip space
        int p=atoi(buf+ind);
        if(p>=0){
          switch(i){
              case(0): AMax[n]=p; break;
              case(1): AMin[n]=p; break;
              case(2): VMax[n]=p; break;
              case(3): VMin[n]=p; break;
              case(4): ADef[n]=p; break;
              default: Serial.println("Unsupported Parameter");
          }
          EEPROM.write(n*NParameter+i,(unsigned char)p);
        }
      }
    }
    ClearBuf();
  }
  delay(FrameDelayMs);
} 
