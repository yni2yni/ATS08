// ADS Touch Sensor Test Example Program (IC P/N:ATS08)
// Code:
// Date: 2026.02.04  Ver.: 0.0.3
// H/W Target: ARDUINO UNO R3, S/W: Arduino IDE  2.3.7
// Author: Park Byoungbae (yni2yni@hanmail.net)
// Note: More information? Please ,send to e-mail.
// Uno R3, A4:SDA,A5: SCL, Leonardo 2:SDA,3:SCL, Nano V3.0  A4:SDA,A5:SCL
// **********************************************************************************************
// Register setting values are subject to change without prior notice to improve touch operation.
// **********************************************************************************************

#include <Wire.h>

#define LF        0x0A //New Line
#define CR        0x0D //Carriage  return
#define SPC       0x20 //Space

//---------------------- ATS08 Register Map. BEGIN----------------------------------------
// Register Name: Register Address : Description 
#define FUNC_OPT     0x10 //S/W Reset
#define CH_EN        0x11 //Touch Channel Enable/Disable (1= Enable, 0=Disable)
#define OUT_OPT0     0x12 //Touch Output Port Option(0: Touch State Output, 1: PWM Output)
#define LED_DRV      0x13 //Touch Output Port Option(0: Touch Output. 1: LED PWM Drive)

// PWM Control Registe
#define PWM0         0x14 // PWM Output Low Duty Control(D2,D1 Port)
#define PWM1         0x15 // PWM Output Low Duty Control(D4,D3 Port)
#define PWM2         0x16 // PWM Output Low Duty Control(D6,D5 Port)
#define PWM3         0x17 // PWM Output Low Duty Control(D8,D7 Port)

#define DEV_OPT      0x18 //INT Pin Mode, Debounce Count Option
#define Cal_opt      0x19 //Up Calibration Speed, Down Calibration Speed
#define Sense_Time0  0x1A //Sensint time
#define Sense_Time1  0x1B //Sensint time

#define PreCharge_time  0x1C // Precharge time, First  Preschare time
#define IT           0x1D //Expire time, ITI Enable/Disable, ITAI time Control
#define BFT          0x1E //AFT time, IT Time
#define Hysteresis   0x1F //Calibration Hold Set, Hysteresis Set

// Touch Sensitivity level
// touch output threshold Count, Register Value X 4 = (1 Step=4 Count)							
#define CH1_Threshold   0x20 // Value x 4 Count , Low Value is  High Sensitivity 
#define CH2_Threshold   0x21 // Value x 4 Count
#define CH3_Threshold   0x22 // Value x 4 Count
#define CH4_Threshold   0x23 // Value x 4 Count
#define CH5_Threshold   0x24 // Value x 4 Count
#define CH6_Threshold   0x25 // Value x 4 Count
#define CH7_Threshold   0x26 // Value x 4 Count
#define CH8_Threshold   0x27 // Value x 4 Count

#define PRS_SEED0       0x28 // Sense Randomizer seed value
#define PRS_SEED1       0x29 // Sense Randomizer seed value
#define I2C_SLV_ADDR0   0x2A // I2C Address 

// -------------- IC Test Function Reg. BEGIN ---------------------------
// Do not change the register value.
#define EFuseRW         0X2B // Fuse Control Register 
#define EFuseCon        0X2C // Fuse Control Register
#define Fuse_Data00     0X2D // Fuse Control Register
#define Fuse_Data01     0X2E // Fuse Control Register
#define Fuse_Data02     0X2F // Fuse Control Register
#define Fuse_Data03     0X30 // Fuse Control Register

#define ANADATA0     0x31 // IC test option
#define ANADATA1     0x32 // IC test option
#define ANADATA2     0x33 // IC test option
#define ANADATA3     0x34 // IC test option
#define ANADATA4     0x35 // IC test option
#define ANADATA5     0x36 // IC test option
#define ANADATA6     0x37 // IC test option
#define ANADATA7     0x38 // IC test option

#define MANUAL_CON   0x39 // IC test option
#define EFuseStat    0x3A // IC test option
#define System_Status   0x3B // IC test option
// ----------------- IC Test Function Reg. END-----------------------------

// ------------------ Touch Key Output Register ---------------------------
#define OUTPUT0   0x3C //Touch Key Output Data Register
#define OUTPUT1   0xBC //Touch Key Output Checksum data Register address

// ------------ Sense / Base Count Monitoring Register --------------------
#define SEN_CS0_L    0x3D //Touch Debug Register
#define SEN_CS0_H    0x3E //Touch Debug Register
#define BAS_CS0_L    0x3F //Touch Debug Register
#define BAS_CS0_H    0x40 //Touch Debug Register

#define SEN_CS1_L    0x41 //Touch Debug Register
#define SEN_CS1_H    0x42 //Touch Debug Register
#define BAS_CS1_L    0x43 //Touch Debug Register
#define BAS_CS1_H    0x44 //Touch Debug Register

#define SEN_CS2_L    0x45 //Touch Debug Register
#define SEN_CS2_H    0x46 //Touch Debug Register
#define BAS_CS2_L    0x47 //Touch Debug Register
#define BAS_CS2_H    0x48 //Touch Debug Registe

#define SEN_CS3_L    0x49 //Touch Debug Register
#define SEN_CS3_H    0x4A //Touch Debug Register
#define BAS_CS3_L    0x4B //Touch Debug Register
#define BAS_CS3_H    0x4C //Touch Debug Registe

#define SEN_CS4_L    0x4D //Touch Debug Register
#define SEN_CS4_H    0x4E //Touch Debug Register
#define BAS_CS4_L    0x4F //Touch Debug Register
#define BAS_CS4_H    0x50 //Touch Debug Registe

#define SEN_CS5_L    0x51 //Touch Debug Register
#define SEN_CS5_H    0x52 //Touch Debug Register
#define BAS_CS5_L    0x53 //Touch Debug Register
#define BAS_CS5_H    0x54 //Touch Debug Registe

#define SEN_CS6_L    0x55 //Touch Debug Register
#define SEN_CS6_H    0x56 //Touch Debug Register
#define BAS_CS6_L    0x57 //Touch Debug Register
#define BAS_CS6_H    0x58 //Touch Debug Registe

#define SEN_CS7_L    0x59 //Touch Debug Register
#define SEN_CS7_H    0x5A //Touch Debug Register
#define BAS_CS7_L    0x5B //Touch Debug Register
#define BAS_CS7_H    0x5C //Touch Debug Registe

#define SEN_CS8_L    0x5D //Touch Debug Register
#define SEN_CS8_H    0x5E //Touch Debug Register
#define BAS_CS8_L    0x5F //Touch Debug Register
#define BAS_CS8_H    0x60 //Touch Debug Registe

#define Error        0x61 /Touch Debug Registe

//------------- Register Write lock/unlock ----------------------------------------
#define Write_Protect    0x62 //Register Lock Enable/Disable
#define Write_Protect2   0x63 //Register Lock Enable/Disable

//------------ IC Debug Mode Setting Register --------------------------------------
#define Debug_Act1   0x05 //Debug I2C Protection
#define Debug_Act2   0x7A //Debug I2C Protection
#define Debug_Act3   0x0D //Debug I2C Protection 
#define Debug_Act4   0x0F //Debug I2C Protection 
//---------------------- ATS08 Register Map. END-------------------------------------

// ============= ATS08 I2C Slave Address ============================//
#define ATS08_ID_VDD    0x5B //0xB2 >>1( 0b1011011 7bit + R/W 1 Bit, 7bit=0x5B, 8bit=0xB6)
#define ATS08_ID_GND    0x5C //0xB8 >>1( 0b1011100 7bit + R/W 1 Bit, 7bit=0x5C, 8bit=0xB8)
#define ATS08_ID_NC     0x5A //0xB4 >>1( 0b1011010 7bit + R/W 1 Bit, 7bit=0x5A, 8bit=0xB4)

#define RESET_PIN 7 //Reset pin
#define EN_PIN    6 //I2C Enable Pin

void  Init_ATS08(void); //Initialize ATS08

byte read_data1 = 0;   //Touch output read data
byte read_data2 = 0;   //Touch output read data
byte checksum1= 0;     //Checksum Data

void Register_Dump()
{
   byte read_data[1] = {0};

   for (int i = 0; i < 256; i += 16)
   {
      for (int j = 0; j <= 15; j++)
      {
         Wire.beginTransmission(ATS08_ID_NC); // sned ic slave address
         Wire.write((i + j));                   // sends register address
         Wire.endTransmission();                // stop transmitting
         Wire.requestFrom(ATS08_ID_NC, 1);    // data read 
         read_data[0] = Wire.read();            //
         print2hex(read_data, 1);               //
      }
      Serial.write(LF);
      Serial.write(CR);
   }
   delay(500);
}

void print2hex(byte *data, byte length) //Print Hex code
{
   Serial.print("0x");
   for (int i = 0; i < length; i++)
   {
      if (data[i] < 0x10)
      {
         Serial.print("0");
      }
      Serial.print(data[i], HEX);
      Serial.write(SPC);
   }
}

void setup()
{
  delay(100); //wait for 200[msec], Power on Reset

  Wire.begin();          // join i2c bus (address optional for master)
  Wire.setClock(200000); // 200Khz (200Kbps)
  Serial.begin(115200);  // start serial for output (Speed)
  //put your setup code here, to run once:

  pinMode(RESET_PIN, OUTPUT);
 
  digitalWrite(RESET_PIN,HIGH);// H/W RESET PIN Control
  delay(5);
  digitalWrite(RESET_PIN,LOW); // H/W RESET PIN Control

  Init_ATS08(); //Initialize ATS08
  delay(200); //wait for 200[msec]
    
}
void loop()
 {

   Wire.beginTransmission(ATS08_ID_NC); // sned ic slave address
   Wire.write(byte(OUTPUT0));           // Touch OUTPUT Register 0x3Ch
   Wire.endTransmission();              // stop transmitting
   Wire.requestFrom(ATS08_ID_NC, 1);    // key data read 
   while ( Wire.available() )
    {
      read_data1 = Wire.read();
   }
   Wire.endTransmission(); // I2C Stop
  
   Serial.print("---- Touch Sensor Output Data (HEX) -- > ");
   Serial.print(read_data1,HEX);
   Serial.write(LF);
   Serial.write(CR);
   
   Wire.beginTransmission(ATS08_ID_NC); // sned ic slave address
   Wire.write(byte(OUTPUT1));           // OUTPUT Checksum Register 0xBCh (0x3Ch | 0x80)
   Wire.endTransmission();              // stop transmitting
   Wire.requestFrom(ATS08_ID_NC, 1);    // key data read 

   while ( Wire.available() )
    {
      read_data2 = Wire.read();
   }
   Wire.endTransmission(); // I2C Stop
   
   checksum1 = (~(read_data1)); //Checksum Factor = 0x3Ch Data ^0xFF 
   Serial.print("------ Output Data Inverting (HEX)---- > ");
   Serial.print(read_data2,HEX);
   Serial.write(LF);
   Serial.write(CR);
      
   if( checksum1 == read_data2 )
   {
   Serial.print(" Output Data PASS ");
   Serial.write(LF);
   Serial.write(CR);
   }else{
   Serial.print(" Output Data N.G ");
   Serial.write(LF);
   Serial.write(CR);
   }
   delay(100);   

}

void  Init_ATS08(void)
{
   
   // ---------------- Register unlock Control Start-------------------------
   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(Write_Protect));     // 0x62h
   Wire.write(0xF0); // 0xF0 : Register unlock 
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(Write_Protect2));     // 0x63h
   Wire.write(0xC3); // 0xC3 : Register unlock 
   Wire.endTransmission(); //
   // ---------------- Register unlock Control End-------------------------
   //------------------ Software Reset ------------------------------------
   //Wire.beginTransmission(ATS08_ID_NC); // 
   //Wire.write(byte(FUNC_OPT)); //  0x10h
   //Wire.write(byte(0x80)); // Software Reset
   //Wire.endTransmission(); //
   //delay(150); // Wait for S/W Reset to complete.(min 150[ms])

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH_EN)); //  0x11h
   Wire.write(byte(0xFF)); // Touch Channel Enable/Disable, 1=Enable, 0=Disable
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(OUT_OPT0)); //  0x12h
   Wire.write(byte(0x00)); // Select state putput ro PWM output. 
   Wire.endTransmission(); // 0=Touch Output, 1= PWM Output

//------------------ LED PWM Setting Register ----------------------
   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(LED_DRV)); //  0x13h
   Wire.write(byte(0x00)); // Touch Output Port Option(0: Touch Output. 1: LED PWM Drive)
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(PWM0)); //  0x14h
   Wire.write(byte(0x00)); // PWM Pulse Low Duty Control(D2,D1 Port)
   Wire.endTransmission(); // 0x0: OFF(all  high),0xF: ON(all Low)

   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(PWM1)); //  0x15h
   Wire.write(byte(0x00));// PWM Pulse Low Duty Control(D4,D3 Port)
   Wire.endTransmission(); // 0x0: OFF(all  high),0xF: ON(all Low)

   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(PWM2)); //  0x16h
   Wire.write(byte(0x00)); // PWM Pulse Low Duty Control(D6,D5 Port)
   Wire.endTransmission(); // 0x0: OFF(all  high),0xF: ON(all Low)

   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(PWM3)); //  0x17h
   Wire.write(byte(0x00)); // PWM Pulse Low Duty Control(D8,D7 Port)
   Wire.endTransmission(); // 0x0: OFF(all  high),0xF: ON(all Low)

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(DEV_OPT)); //  0x18h
   Wire.write(byte(0x43)); // Single/Mulit Mode, Mix Disable,INT Pin Mode, Touch Debounce Count, 
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Cal_opt)); //  0x19h
   Wire.write(byte(0x60)); // Up_Calibration Speed(Sense>Base),
   Wire.endTransmission(); // Down_Calibration Speed(Sense<Base)

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Sense_Time0)); //  0x1Ah
   Wire.write(byte(0xFF)); // single channel Sensing time
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Sense_Time1)); //  0x1Bh
   Wire.write(byte(0x0F)); // single channel Sensing time
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(PreCharge_time)); //  0x1Ch
   Wire.write(byte(0x8B)); // Precharge timebefore sensing Time,
   Wire.endTransmission(); // First Precharge time for new sensien start

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(IT)); //  0x1Dh
   Wire.write(byte(0x02)); //Expire Time , ITI_Enable/Disable, ITAT Time Control
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(BFT)); //  0x1Eh
   Wire.write(byte(0x03)); // AFT Time Set, IT Time Control
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Hysteresis)); //  0x1Fh
   Wire.write(byte(0x02)); // Calibration Hold Set, Hysteresis Set
   Wire.endTransmission(); //

   //------------------ Touch Sensitivity ----------------------
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH1_Threshold)); //  0x20h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); // 

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH2_Threshold)); //  0x21h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
  
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH3_Threshold)); //  0x22h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
      
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH4_Threshold)); //  0x23h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
      
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH5_Threshold)); //  0x24h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
      
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH6_Threshold)); //  0x25h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
      
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH7_Threshold)); //  0x26h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //
      
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(CH8_Threshold)); //  0x27h
   Wire.write(byte(0x14)); // Threshold Count, 0x14 x 4 = 80 Count
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(PRS_SEED0)); //  0x28h
   Wire.write(byte(0xFF)); // Sensing Randomizer start seed value [7:0]
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(PRS_SEED1)); //  0x29h
   Wire.write(byte(0xFF)); // Sensing Randomizer start seed value [15:8]
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(I2C_SLV_ADDR0)); //  0x2Ah
   Wire.write(byte(0xB4)); // Salve Address of I2C
   Wire.endTransmission(); //

   //------------------ Software Reset ------------- ---------------------       
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(FUNC_OPT)); //  0x10h
   Wire.write(byte(0x80)); // Software Reset
   Wire.endTransmission(); //
   // --------- S/W Reset Delay -------------------------------------------
   delay(150); // Wait for S/W Reset to complete.(min. 150[ms])

   //------------------ IC Debugging Control Register ---------------------
   // Touch Debugging Mode Enable
   // Debug I2C Port (D7: Debug_SCL,D8: Debug_SDA)
   /*
   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Debug_Act1)); //  0x05h
   Wire.write(byte(0x5A)); // Debug Mode Enable
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Debug_Act2)); //  0x7Ah
   Wire.write(byte(0xF0)); // Debug Mode Enable
   Wire.endTransmission(); //

   Wire.beginTransmission(ATS08_ID_NC); // 
   Wire.write(byte(Debug_Act3)); //  0x0Dh
   Wire.write(byte(0xC3); // Debug Mode Enable
   Wire.endTransmission(); //
    */
   
   // ---------------- Register Lock Control --------------------------------
   Wire.beginTransmission(ATS08_ID_NC); //
   Wire.write(byte(Write_Protect));     // 0x62h
   Wire.write(0xA5); // 0xA5 : Register Lock Enable
   Wire.endTransmission(); //
   
   //================ END Initialize ATS08 ==================================
   }

// End

