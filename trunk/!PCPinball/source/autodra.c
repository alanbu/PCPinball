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
#include "video.h"
#include "autodra.h"

void draw_ground_auto()
{
  set_sprite(bx_old,by_old-1,16,16,ground);
}

void get_ground_auto()
{
	get_sprite(ballx,bally-1,16,16,ground);
}

void mix_ballground()
{
  word z;
  byte b1,b2;

  for (z=0; z <= 255; z++)
  {
    b1=ground[z];
    b2=ball[z];
    if ((b2==0) || (b1 > MAXfarbe))
      ball_sprite[z]=b1;
    else
      ball_sprite[z]=b2;
  }
}

void draw_ball_auto()
{
  mix_ballground();
  set_sprite(ballx,bally-1,16,16,ball_sprite);
}
