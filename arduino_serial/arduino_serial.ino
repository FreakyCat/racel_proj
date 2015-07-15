#include <OneWire.h>
#include <math.h>
/*makroi  za ultrazvucni senzor*/
#define echoPin 7 //echo  povezan na pin 7
#define trigPin 8 //trig povezna na pin 8
#define LEDPin  13// onboard dioda sa pinom 13

int DS18S20_Pin = 2; //koristimo pin 2 da prikljucimo temperaturni senzor senzor
int flag=0;
int max = 15;//Maksimalan opseg ultrazvucnog senzora, na kojem ce nam vracati temp
int min = 0;//Minimum opsega ultrazvucnog senzora, na kome ce nam vracati temp
long dist;
//Bira se poseban cip za ocitavanje temperature
OneWire ds(DS18S20_Pin); // digitalni pin 2

void setup(void) {
  
  Serial.begin(9600);// postavljamo baudrate
  pinMode(trigPin, OUTPUT);//postavljamo trig pin kao izlazni pin
  pinMode(echoPin, INPUT);// postavljamo echo pin kao ulazni
  pinMode(LEDPin, OUTPUT);// onboard dioda kao izlazni

}

void loop(void) {
  
  dist=getDistance();
  if (dist>=min && dist<=max){
    flag =1;
  }else{
    flag =0;
  }
  
  if (flag){
    float temperature = getTemp();
    Serial.print("+");
    Serial.print(temperature);//serijski stampa temperaturu
    delay(100); //kasnjenje da se moze ocitati temperatura
  };
}


/*Funkcija za trazenje temperature DS18S20, koristi biblioteku One Wire,
Dalas proizvodjac temp senzora zahteva koriscenje 1-Wire protokola. Slican je
SPI protokolu jer postoji jedan master i vise slave, koji komuniciraju preko jedne zice
*/
float getTemp(){
 byte data[12];
 byte addr[8];
// trazi se tamperatura
 if ( !ds.search(addr)) {
   ds.reset_search();
   return -1000;
 }
//ocitava se crc kod porodice kontrolera
 if ( OneWire::crc8( addr, 7) != addr[7]) {
   Serial.println("CRC is not valid!");
   return -1000;
 }
//postoje dve adrese koje prihvata ova porodica a to su 0x10
 if ( addr[0] != 0x10 && addr[0] != 0x28) {
   Serial.print("Device is not recognized");
   return -1000;
 }

 ds.reset();
 ds.select(addr);//odaabrati adresu koja je proverena
 ds.write(0x44,1); // poceti konverziju u parazitnom modu rada

 byte present = ds.reset();
 ds.select(addr);  
 ds.write(0xBE); // Ocitavanje EEPROM

 
 for (int i = 0; i < 9; i++) { // pocetak ad konverzije, koristi se se 9bita
  data[i] = ds.read();
 }
 
 ds.reset_search();//ocitavanje dva vrednosti
 
 byte MSB = data[1];
 byte LSB = data[0];

 float tempRead = ((MSB << 8) | LSB); //using two's compliment
 float TemperatureSum = tempRead / 16;
 
 return TemperatureSum;
 
}
/*Funkcija za ocitavanje ultrazvucnog senzora
Da bi se ocitao potrebno je da se trigeruje senzor na 10us i onda da se vrednost spusti na 0, 
potomm se ceka njegov odziv pomocu echo
*/

long getDistance(){

int maximumRange = 200;//Maksimalan opseg ultrazvucnog senzora
int minimumRange = 0;//Minimum opsega ultrazvucnog senzora
long duration, distance;

digitalWrite(trigPin,LOW);
delayMicroseconds(2);

digitalWrite(trigPin,HIGH);
delayMicroseconds(10);

digitalWrite(trigPin,LOW);
delayMicroseconds(2);

duration=pulseIn(echoPin,HIGH);
//izracunavanje rastojanja
distance=duration/58.2;
if (distance>= maximumRange || distance <=minimumRange){
  Serial.print("-#");
  digitalWrite(LEDPin, HIGH);
  delay(700);
  distance=-1;
}else{
  
  Serial.print(distance);
  digitalWrite(LEDPin, LOW);
  delay(700);
}
delay(50);
return distance;
}

