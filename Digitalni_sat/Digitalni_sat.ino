#include "RTC.h"
#include "Wire.h"

int dan, mjesec, sati, minute, sekunde;
int ispravak;
int T=1000;
int Data=2, Clock=3, Latch=4;
int stage;
char Brojevi[11]={126,12,182,158,204,218,250,14,254,222,0};

long unsigned int timer, micro_timer;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  dan=Datum_molit_cu_lijepo();
  mjesec=Mjesec_molit_cu_lijepo();
  sati=Sati_molit_cu_lijepo();
  minute=Minute_molit_cu_lijepo();
  sekunde=Sekunde_molit_cu_lijepo();
  digitalWrite(A0, HIGH);
  digitalWrite(A2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()-timer>=1000-ispravak){
      //Serial.println(ispravak);
      ispravak=millis()-timer-1000+ispravak;
      while(1000-ispravak<0){
        sekunde++;
        ispravak-=1000;
      }
      //Serial.println(ispravak);
      timer=millis();
      sekunde++;
  }
  if(sekunde>=60) { sekunde=0; minute++;}
  if(minute>=60) {
    minute=0; sati++;
    dan=Datum_molit_cu_lijepo();
    mjesec=Mjesec_molit_cu_lijepo();
    sati=Sati_molit_cu_lijepo();
    minute=Minute_molit_cu_lijepo();
    sekunde=Sekunde_molit_cu_lijepo();
  }
  if(sati>=24){sati=0; dan++;}
  
  MOS_driver(5);
  Reset();
  Latchy();
  Shift_In(Brojevi[sati/10]);
  Latchy();
  MOS_driver(1);
  delayMicroseconds(T);

  MOS_driver(5);
  Reset();
  Latchy();
  Shift_In(Brojevi[sati%10]);
  Latchy();
  MOS_driver(2);
  delayMicroseconds(T);
  
  MOS_driver(5);
  Reset();
  Latchy();
  Shift_In(Brojevi[minute/10]);
  Latchy();
  MOS_driver(3);
  delayMicroseconds(T);

  
  MOS_driver(5);
  Reset();
  Latchy();
  Shift_In(Brojevi[minute%10]);
  Latchy();
  MOS_driver(4);
  delayMicroseconds(T);
  
}

void Latchy(){
  digitalWrite(4, HIGH);
  digitalWrite(4, LOW);
}

void Reset(){
  digitalWrite(2, LOW);
  for(int r=0;r<8;r++){
  digitalWrite(3, HIGH);
  digitalWrite(3, LOW);
  }
}

void MOS_driver(int MOS){
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(10, LOW);
  digitalWrite(12, LOW);
  if(MOS==1){
    digitalWrite(7, HIGH);
  }
  if(MOS==2){
    digitalWrite(8, HIGH);
  }
  if(MOS==3){
    digitalWrite(10, HIGH);
  }
  if(MOS==4){
    digitalWrite(12, HIGH);
  }
}

void Shift_In(int broj){
  for(int x=7; x>=0;x--){
     digitalWrite(2, bitRead(broj, x));
     digitalWrite(3, HIGH);
     digitalWrite(3, LOW);
  }
  digitalWrite(2, LOW);
}
