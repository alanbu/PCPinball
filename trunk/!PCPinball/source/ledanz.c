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
#include "ledanz.h"
#include "video.h"
#include "random.h"

#include <string.h>

/*function IntToStr(I: Longint): String;
{ Convert any integer type to a string }
var
  S: string[6];
begin
  Str(I, S);
  IntToStr := S;
end;

function IntToStr1(I: Longint): String;
{ Convert any integer type to a string }
var
  S: string[8];
begin
  Str(I:8, S);
  IntToStr1 := S;
end;
*/

void LED_anzeige_1()
{
  int x,y;

  for (y=0; y <= 45/2; y++)
  {
    for (x=0; x <= 319/2; x++)
	{
		put_pixel_led(x,y,led_color_1);
	}
  }
}


void LED_anzeige_2()
{
  if (led_timer & 1) set_rgb_color(led_color_1,255,00,00);
  else set_rgb_color(led_color_1,20,00,00);
}

void LED_anzeige_3()
{
  if (led_timer & 1)
  {
    set_rgb_color(led_color_1,255,00,00);
    set_rgb_color(led_color_2,020,00,00);
  } else
  {
    set_rgb_color(led_color_1,020,00,00);
    set_rgb_color(led_color_2,255,00,00);
  }
}

void LED_anzeige_4()
{
  word x,y;
  ran255z++;
  for (y=0; y <= 45 / 2; y++)
  {
    for (x=0; x <= 319 / 2; x++)
	{
      if (random255()<50)put_pixel_led(x,y,led_color_2);
	  else put_pixel_led(x,y,led_color_1);
	}
  }
}

void LED_anzeige_5_init(byte x, byte y, const char *stg)
{
  led_x =x;
  led_y =y+2;
  strcpy(led_anzeige_text,stg);
  led_status=1;
  led_f_status_1=1;
  led_f_status_2=(byte)strlen(stg);
  led_funktion=5;
}

void LED_anzeige_5()
{
  word i;
  byte char_pos;
  word charac_pos;
  char c1;
  byte color;
  word x;

  x=led_x; //{+led_y*80};
  if (led_f_status_1<led_f_status_2+1)
  {
	c1 = led_anzeige_text[led_f_status_1-1];
    charac_pos = c1*16;
    for (char_pos = 0; char_pos <= 7; char_pos++)
	{
      for (i = 0; i <= 7; i++)
	  {
        if (font[charac_pos+i] & Bits[char_pos]) color = led_color_2;
        else color = led_color_1;
        put_pixel_led(x,i+led_y,color);
	  }
	  x++;
	}

    led_x=x;
    led_f_status_1++;
  } else
  {
    led_status=255;
    led_funktion=0;
  }
}

 /* {Scroll right 2 left} */
void LED_anzeige_6_init()
{
  led_f_status_1=80;
  led_funktion=6;
}

void LED_anzeige_6()
{
  int y;
  word z1,z2;

  for (y=0; y <= 23; y++)
  {
    z1=y*160;
    z2=z1+2;
	memcpy(led_display+ z1, led_display + z2, 158);
    put_pixel_led(158,y,led_color_1);
    put_pixel_led(159,y,led_color_1);
  }
  led_status=255;
  if (led_f_status_1>0) led_f_status_1--;
  else led_funktion =0;
}

void led_anzeige(boolean pause)
{
  led_timer++;
  switch(led_funktion)
  {
  case 1: LED_anzeige_1(); break;
  case 2: LED_anzeige_2(); break;
  case 3: LED_anzeige_3(); break;
  case 4: LED_anzeige_4(); break;
  case 5:
	    LED_anzeige_5();
		LED_anzeige_5();
		break;
  case 6: LED_anzeige_6(); break;
  }
  if (led_status==255) flip_led(pause);
}
