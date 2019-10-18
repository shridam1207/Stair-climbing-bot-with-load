#include<Wire.h>
#include<Servo.h>

#include <MPU6050_tockn.h>
//RCL - A5 ; R
Servo s1,s2,s3;
MPU6050 mpu6050(Wire);

const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265; int maxVal=402;

double x,x1; double y,y1; double z,z1;

void setup()
{
  s1.attach(5);
  s2.attach(4);
  s3.attach(3);
  s3.write(90);
  s1.write(90);
  s2.write(90);
  delay(3000);
  Wire.begin(); 
  mpu6050.begin();                                                             
  mpu6050.calcGyroOffsets(true);                                               
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); Wire.write(0); 
  Wire.endTransmission(true); 
  Serial.begin(9600); 
  Wire.beginTransmission(MPU_addr); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_addr,14,true); 
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); AcZ=Wire.read()<<8|Wire.read(); int xAng = map(AcX,minVal,maxVal,-90,90); int yAng = map(AcY,minVal,maxVal,-90,90); int zAng = map(AcZ,minVal,maxVal,-90,90);

x1= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); y1= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI); z1= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);


}


void loop()
{
  Wire.beginTransmission(MPU_addr); Wire.write(0x3B); Wire.endTransmission(false); Wire.requestFrom(MPU_addr,14,true); AcX=Wire.read()<<8|Wire.read(); AcY=Wire.read()<<8|Wire.read(); AcZ=Wire.read()<<8|Wire.read(); int xAng = map(AcX,minVal,maxVal,-90,90); int yAng = map(AcY,minVal,maxVal,-90,90); int zAng = map(AcZ,minVal,maxVal,-90,90);

x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI); y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
// z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

Serial.print("AngleX= "); Serial.println(x-x1);
s3.write(90-x+x1);            //pITCH
Serial.print("AngleY= "); Serial.println(y-y1);
s1.write(90+y-y1);            //ROLL
mpu6050.update();
z = mpu6050.getAngleZ();
Serial.print("AngleZ= "); Serial.println(z); Serial.println("-----------------------------------------"); 
//delay(500);
s2.write(90+z);               //   YAWN
