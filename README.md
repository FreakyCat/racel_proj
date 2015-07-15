# racel_proj

 

Zadatak : Realizovati komunikaciju između Raspberija I Arduina pomoću serijske veze .
Slati očitanu temperaturu sa senzora Raspberiju, u momentu kada približimo ruku ultrazvučnom senzoru na rastojanje   manje od 5 cm. Prikazivati rezultate na Nokia 5110 ekranu.

Na slici iznad je prikazana šema povezivanja komponenti. 

Temperaturni senzor
U zadatku se koristi  DS1882  koji koristi napon između 2.7-5.5v. Očitavanje se vrši preko AD konvertora koji već postoji ugrađen u Arduino. Temperatura se dobija po formuli
Napon(mV)= Očitavanje ADC * 5000/1024 (10bitni AD konvertor Arduina) 
Za očitavanje senzora koristi se One Wire protokol. Sličan je SPI protokolu jer postoji jedan master i vise slave uređaja, koji komuniciraju preko jedne žice

Napajanje OneWire uređaja:
1.	Parasite power mode – Kada se radi u ovom modu dve žice su potrebne. Jedna za magistaru i druga za uzemljenje. Tada se jedan otpornik od 4.7k spaja sa magistralom.  stuja je relativno malo oko 1.5mA. U toku obavljanja merenja temperature master bus se mora držati u stanju visoke impedanse, i potrebno je kašnjenje od 750ms.
2.	Normal mode – Sa dodatnim napajanjem postoje koje 3 žice su potrebne : magistrala, ground i napajanje. Magistrala je slobodna za napajanje dok se vrši očitavanje temperature.

Pristupanje One Wire uređajima

Svaki OneWire uređaj ima 64bitnu ROM adresu, koja se sastoji od 8bit koda family, 48-bitnog serijskog protokola i 8-bitnog CRC koda. Kod senzora se proverava integritet CRC kodom tako što se poredi da li je kod 0x10 kod za family DS18S20 senzora. Više informacija o senzoru se mogu naći na
http://datasheets.maximintegrated.com/en/ds/DS18S20.pdf
Posto se skine potrebna One Wire biblioteka sa sajta
http://playground.arduino.cc/Learning/OneWire
Potrebno je da se kreira poseban podirektorijum u folderu Sketchbook i da se kreira novi fajl u kome ce se smestiti biblioteka OneWire, tj unzipovan fajl koji se skida sa sajta. Ona se instalira pomocu Sketch-Import Library- One wire. 
Pre ovoga potrebno je da se instalira AdruinoIDE komandom

sudo apt-get install arduino

Arduino već poseduje biblioteke za serijsku komunikaciju tako da se serijska  podešava pomoću funkcija. Nakon kompajliranja projekta potrebno je da se spusti projekat na Arduino.


Ultrazvučni senzor

Zadatak ultrazvučnog senzora je da detektuje kada se ruka približi uređaju I tada počne da šalje podatke Raspberryju. U zadataku korišćen je senzor HCSR04 koji poseduje 4 pina Vdd, Gnd i Echo i Trig. Echo se koristi za prijem  ultrazvučnog signala sa senzora kada se on odbije o objekat. Triger šalje signal frekvencije 840 kHz. Više informacija o senzoru se može naći u uputstvu na sajtu.
(http://www.micropik.com/PDF/HCSR04.pdf)

Raspberry PI deo

pcd8544 biblioteka

1.Instalirati WiringPI ako vec nije kako je opisano. Nakon ovoga, instalirati biblioteku za ekran

git clone https://github.com/XavierBerger/pcd8544.git
cd pcd8544
./setup.py clean build 
sudo ./setup.py install

Kod za primere se može naći na 
https://github.com/XavierBerger/pcd8544/tree/master/examples

Kompajliranje fajla koji sadrži ovu biblioteku se radi komandama

gcc -o pcd8544_test pcd8544_test.c ../PCD8544.c  -L/usr/local/lib -lwiringPi
gcc -o pcd8544_test2 pcd8544_test2.c ../PCD8544.c  -L/usr/local/lib -lwiringPi
gcc -o pcd8544_rpi pcd8544_rpi.c ../PCD8544.c  -L/usr/local/lib –lwiringPi


gde ../PCD8544 oznacava putanju do fajla.

