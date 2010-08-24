/* PC Spiel Pinball
 *
 * (c) vIRTUAL tECHNOLOGIES 1993-1996 Ansgar Scherp, Joachim Gelhaus
 *
 * Original Pascal source released 1996
 *
 * SDL/C conversion Alan Buckley 2008
 *
 * See COPYRIGHT file for copyright details.
 *
 ********************************************************************/


#include "globals.h"
#include "tisch1.h"
#include "random.h"
#include "video.h"
#include "ledanz.h"

#include <SDL.h>

// From flipper1
extern void display(const char *t);

// Table one specific variables

word Kurven;
byte Lichter1a[252 - 250 + 1]; // : array[250..252] of byte;
byte Lichter2a[249 - 247 + 1]; // : array[247..249] of byte;
byte Lichter3a[246 - 244 + 1]; //  : array[244..246] of byte;
byte Licht4;
boolean PushUp;
byte Bonus;
byte PCSspe[4]; //  : array[1..3] of byte;
byte special;
byte temp;
byte BumpCount;


void Init_Tisch1()
{
  int t;

  for (t=0; t <= 255;t++) set_rgb_color(t,pal[t].r,pal[t].g,pal[t].b );
  
  Kurven = 0;
  Lichter1(250) = 0;  Lichter1(251) = 0;  Lichter1(252) = 0;
  Lichter2(247) = 0;  Lichter2(248) = 0;  Lichter2(249) = 0;
  Lichter3(244) = 0;  Lichter3(245) = 0;  Lichter3(246) = 0;
  Licht4   = 0;
  PushUp = TRUE;
  Bonus    = 0;
  MAXfarbe = 234; // {235-255}
  temp = 3;
  thermo(temp);
  PCSspe[1] = 0;
  PCSspe[2] = 0;
  PCSspe[3] = 0;
  special = 0;
  BumpCount = 0;

  // Common variables with different values depending on table
  tnr = '1';  // {tablenr}
  ArmXLinks = 79;
  ArmYLinks = 400+135;
  ArmXRechts = 159;
  ArmYRechts = 400+135;
}

// {z :-> 0-6 }
void thermo(byte tmp)
{
  byte z;

  for (z = 1; z <= 6;z++)
  {
    if (z <= tmp)
	{
		set_rgb_color(55+z, 0,0,47);
	} else
		set_rgb_color(55+z, 50, 0, 0);
  }

  if (temp == 0)
  {
      display("ATTENTION:");
	  do { led_anzeige(TRUE); } while (led_status);
      SDL_Delay(2000);
      display("TEMP. OVERHEAT!");
	  do {led_anzeige(TRUE); } while (led_status);
      SDL_Delay(2000);
      display("TURNED FIRE OFF!");
	  do {led_anzeige(TRUE); } while (led_status);
      SDL_Delay(2000);
      PushUp = FALSE;
      set_rgb_color(40,10,10,60);
      set_rgb_color(41,10,10,50);
      set_rgb_color(42,10,10,40);
      set_rgb_color(43,10,10,30);
  }
}

void flameflacker()
{
  set_rgb_color(40, 45+random_to(15), 0, 0);
  set_rgb_color(41, 45+random_to(15), 8+random_to(15), 0);
  set_rgb_color(42, 45+random_to(15), 22+random_to(15), 0);
  set_rgb_color(43, 45+random_to(15), 38+random_to(13), 0);
}

