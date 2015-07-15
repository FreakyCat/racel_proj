/*void DisplayTemp(const char *temp[]
=================================================================================
 Name        : pcd8544_rpi.c
Kod za  PCD8544 LCD (Nokia3310/5110)  dimenzija 84x84
Pinovi povezivanje
	 LCD pins      Raspberry Pi
	 LCD1 - GND    P06  - GND
	 LCD2 - VCC    P01 - 3.3V
	 LCD3 - CLK    P11 - GPIO0
	 LCD4 - Din    P12 - GPIO1
	 LCD5 - D/C    P13 - GPIO2
	 LCD6 - CS     P15 - GPIO3
	 LCD7 - RST    P16 - GPIO4
	 LCD8 - LED    P01 - 3.3V 
==============================================================================
*/

#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//dodati relativnu putanju
#include "/home/pi/proj_racel/PCD8544.h"

void DisplayTemp(char temp[]);


// podesavanje pinova
int _din = 1;
int _sclk = 0;
int _dc = 2;
int _rst = 4;
int _cs = 3;  
// podesavanje kontrasta
int contrast = 55;
//promenjive
char off[2];
char temperature[7];
char check;
char second;
char *buffer;


int main ()
{
  int fd ;
  int count ;
  unsigned int nextTime ;//broji clanove u baferu
/*=========================================================================
int serialOpen (char *device, int baud) 
	Provera da li je device otvoren i podesavanje baudrate.U slucaju greske vraca se -1
===========================================================================
int wiringPiSetup()
	Podesava pinout tako da se koristi WiringPi pinout. U slucaju greske -1 se vraca
===========================================================================
*/
  if ((fd = serialOpen ("/dev/ttyUSB0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
/*=========================================================================
void LCDInit(uint8_t SCLK, uint8_t DIN, uint8_t DC, uint8_t CS, uint8_t RST, uint8_t contrast)
	Podesavaju se svi pinovi, i kontrast,kao i x,y koodrinara se postavlja na 0;
===========================================================================
 LCDshowLogo();
	RaspberryPIlogo
==========================================================================*/
  LCDInit(_sclk, _din, _dc, _cs, _rst, contrast);
  LCDclear();
  LCDshowLogo(1);
  delay(200);
  LCDclear();	

/*----------------------------------------------------------------------------------------------------------------*/

 nextTime = millis () + 300 ;

  for (count = 0 ; count < 256 ; )
  {
    if (millis () > nextTime)

    delay (3) ;
/*=========================================================================
int   serialDataAvail (int fd) ;	
	vraca broj karaktera u baferu, u slucaju da nema karatktera vraca 0, A u slucaju da je 
	nastupila greska vraca -#
 
int serialGetchar (int fd) ;
	poziva blok na svakih 10 sekundi i vraca -1 ako nemo karaktera koji su dostupni
===========================================================================
*/
        while (serialDataAvail (fd))
        {
		check=serialGetchar(fd);
		printf("check je %c\n ", check);
		if ((check=='-') || (check=='#'))
                {	
			off[0]=check;
		//	fflush(stdin);
			LCDclear();
			LCDshowLogo(2);
			delay(50);
			LCDclear();
			char second;
			second=serialGetchar(fd);
		}
                else
                {
			if(check!='+')
                        {
			 check=serialGetchar(fd);

			}
                        else
                        {
			  int i;
			  for(i=0; i<6; i++)
                          {
			    temperature[i]=serialGetchar(fd);	
			   // printf("Temp je %c",temperature[i]);
			  }
			  temperature[6]='\0';
		          DisplayTemp(temperature);	
			 fflush (stdout);
		        }
		  }
	}

// praznimo bafer da bi bili sigurni da ce sledeci podatak biti primljen
   }
  return 0 ;
  
}
/*===========================================================================
void LCDdrawstring(uint8_t x, uint8_t line, char *c);
	iscrtava string na koordinati x, i koloni(line), gde se *c pokazivac na string
===========================================================================
void LCDdisplay()
	ispisuje sve na ekranu,ide piksel po piksel
===========================================================================
*/
void DisplayTemp(char temp[]){

	LCDclear();
	LCDshowLogo(3);
	LCDdrawstring(0, 0, "Raspberry Pi:");
	LCDdrawline(0, 10, 83, 10, BLACK);
	LCDdrawstring(0, 12, temp);
	LCDdisplay();
    delay(100);

}
