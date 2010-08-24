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

// Routines common to both tables

#include "globals.h"
#include "common.h"
#include "video.h"
#include "autodra.h"
#include "ledanz.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* Helper to put an integer in a message */
char *int_msg(const char *fmt, int num)
{
	static char msg_text[40];
	sprintf(msg_text, fmt, num);
	return msg_text;
}

void senk_arms()
{
  int t;

  for (t = 5; t>=0; t--)
  {
    if (arm_rechts_status>1) arm_rechts_status--;
    if (arm_links_status>1) arm_links_status--;
    check_flipper_arms();
    arm_links_old_status=arm_links_status;
    arm_rechts_old_status=arm_rechts_status;
  }
  draw_ground_auto();
}

void move_left()
{
  word alt;
  word a;

  alt = led_funktion;
  LED_anzeige_6_init();
  for (a = 0; a <= 160; a++) {led_anzeige(TRUE);}
  led_funktion = alt;
}

void incscore(word points)
{
  char msg[80];
  score[ActPlayer] += points;
  sprintf(msg, "Score %7d Ball %d", score[ActPlayer], Balls[ActPlayer]);
  LED_anzeige_5_init(0,0,msg);
}

void display(const char *t)
{
  int a, len = (int)strlen(t);
  char z[21];

  memset(z, ' ', 20);
  z[20] = 0;
  for (a = 0; a < len; a++) z[a + 10 - len / 2] = t[a];
  LED_anzeige_5_init(0,0,z);
}

void move_ball()
{
  draw_ground_auto(); get_ground_auto(); draw_ball_auto();
  bx_old=ballx; by_old=bally;
}

void calc_page_pos_of_ballpos()
{
  word y2;

  y2 = bally-100;
  if (y2 > 1000) y2 = 1;
  y2=y2+48;
  setaddress(y2);
}

void check_flipper_arms()
{
  //{check if left flipper-arm is moved}
  if (arm_links_old_status != arm_links_status)
  {
	if ((bally+16>ArmYLinks) && (bally<ArmYLinks+ArmHoeheLinks)
		&&(ballx+16>ArmXLinks) && (ballx<ArmXLinks+ArmBreiteLinks))
	{
        draw_ground_auto();
        draw_arm_links();
        get_ground_auto();
        draw_ball_auto();
	} else draw_arm_links();
  }
  // {check if right flipper-arm is moved}
  if (arm_rechts_old_status!=arm_rechts_status)
  {
    if ((bally+16>ArmYRechts) &&(bally<ArmYRechts+ArmHoeheRechts)
       && (ballx+16>ArmXRechts) && (ballx<ArmXRechts+ArmBreiteRechts))
	{
        draw_ground_auto();
        draw_arm_rechts();
        get_ground_auto();
        draw_ball_auto();
	} else draw_arm_rechts();
  }
}

byte gettablepixel(word x, word y)
{
  if (y<200) return tableground1(x,y);
  else if (y<400) return tableground2(x,y);
  else if (y < 600) return tableground3(x,y);

  return 0;
}

void check_ball_oben()
{
  int x,y;
  boolean contact;

  y=bally;
  contact=FALSE;
  do
  {
   // {0&16 / 1&15 / 2&14 / 3&13 / 4&12 / 5&11 / 6&10 / 7&9 / 8&8}
    for (x=ballx+ 4; x<= ballx+12;x++)
	{
        if (gettablepixel(x,y)>127)
		{
           contact=TRUE;
           if (x <= ballx+8) o1++; else o2++;
           fo = gettablepixel(x,y);
		}
	}
    y--;
  } while (y>bally+ballspeed_y / 2 && !contact);
  y++; bally=y;
}

void check_ball_unten()
{
  int x,y;
  boolean contact;

  y=bally;
  contact=FALSE;
  do
  {
    for (x=ballx+ 4; x<= ballx+12;x++)
	{
      if (gettablepixel(x,y+14)>127)
	  {
          contact=TRUE;
          if (x <= ballx+8) u1++; else u2++;
          fu = gettablepixel(x,y+14);
	  }
    }
    y++;
  } while (y<bally+ballspeed_y / 2 && !contact);
  y--; bally=y;
 }

void check_ball_links()
{
  int x,y;
  boolean contact;

  x=ballx;
  contact=FALSE;
  do
  {
    for (y=bally+ 4; y<=bally+12;y++)
	{
      if (gettablepixel(x,y)>127)
	  {
          contact=TRUE;
          if (y <= bally+8) l1++; else l2++;
          fl =  gettablepixel(x,y);
	  }
	}
    x--;
	if (x<0) {x=0; contact=TRUE;}
  } while (x>ballx+ballspeed_x / 2 && !contact);
  x++; ballx=x;
}

void check_ball_rechts()
{
  int x,y;
  boolean contact;

  x=ballx;
  contact=FALSE;
  do
  {
    for (y=bally+ 4;y<=bally+12;y++)
	{
      if (gettablepixel(x+14,y)>127)
	  {
          contact=TRUE;
          if (y <= bally+8) r1++; else r2++;
          fr =  gettablepixel(x,y+14);
	  }
	}
    x++;
	if (x>304) { x=304; contact=TRUE;}

  } while (x<ballx+ballspeed_x / 2  && !contact);
  x--; ballx=x;
}

