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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL.h>

#include "init_close.h"
#include "globals.h"
#include "sound1.h"

#include "random.h"
#include "video.h"
#include "loadprc.h"
#include "autodra.h"
#include "ledanz.h"

// From flipper1
void calc_page_pos_of_ballpos();

byte FileMode = 0; //TODO: Find out where this comes from

void init_ball_values()
{
//  {ballx := 300; bally := 160;
//  ballspeed_y:=-80; ballspeed_x:=2; kraft := 250;}

  ballx = 300; bally = 500+20;
  get_ground_auto();
  draw_ball_auto();
  bx_old = 300; by_old = 500+20;
  ballspeed_y = 0;
  ballspeed_x = 0;
  kraft = 0;
  play(snd8);

}

void init_globals()
{
	char temp[256];

	MaxPlayer = 1; //TODO: Add multiple players option

//  getfont();
  load_font();
//  nosound();
  init_random();
//  OldHeapSize = memavail;
//  mark(OldHeapLimit);
//  getmem(tableground1,320*200);
//  getmem(tableground2,320*200);
//  getmem(tableground3,320*200);
    led_display = malloc(160*24);
//  getmem(arm_links,ArmBreiteLinks*ArmHoeheLinks*5);
//  getmem(arm_rechts,ArmBreiteRechts*ArmHoeheRechts*5);
//  getmem(arm_links_msk,ArmBreiteLinks*ArmHoeheLinks*5);
//  getmem(arm_rechts_msk,ArmBreiteRechts*ArmHoeheRechts*5);
  ball = malloc(256);
  ground = malloc(256);
  ball_sprite = malloc(256);
  underground = malloc(1024);
  tableground = malloc(9368);
  feder = malloc(480);
//  ledseg=seg(Led_display^);
//  armlinksseg:=seg(arm_links^);
//  armrechtsseg:=seg(arm_rechts^);
//  armlinks_mskseg:=seg(arm_links_msk^);
//  armrechts_mskseg:=seg(arm_rechts_msk^);
//  ballseg:=seg(ball^);
//  groundseg:=seg(ground^);
//  ballspriteseg:=seg(ball_Sprite^);
//  undergroundseg:=seg(underground^);
//  tablegroundseg:=seg(tableground^);
//  federseg:=seg(Feder^);

    led_hoehe=24; led_color_1=129; led_color_2=130; led_funktion=0;
//  {twix-mode 320x400 }
//  vga320x400Overscan(); //{setlinecomp(400-46);}

  load_table_tab();
  load_table_gro();
  load_feder();

  init_colors();

  sprintf(temp,"gfx/feder%c.mpa", tnr);
  load_mini_palette(temp);
  load_arm_links();
  load_arm_rechts();
  load_arm_links_msk();
  load_arm_rechts_msk();
  load_ball();
  
  if (UseSound) init_soundkit();
}

void init_all()
{
	int y;
	word t;

/*  OldFileMode = FileMode;
  FileMode = 0;
  ch = param[0];
  switch(ch)
  {
   case '0' : overscan = FALSE; highres = FALSE; break;
   case '1' : overscan = FALSE; highres = TRUE; break;
   case '2' : overscan = TRUE; highres = FALSE; break;
   case '3' : overscan = TRUE; highres = TRUE; break;
   default:
	   exit(2);
  }
  VideoMode = ch;
  ch = param[1];
  switch(ch)
  {
   case '1' : UseSound=yes; break;
   case '0' : UseSound=no; break;
   default: exit(2);
  }

  ch = param[2];
  if (ch >= '1' && ch <= '6')
  {
	  MaxPlayer = ch - 48;
//      printf(MaxPlayer);
  } else
	  exit(2);
*/
  FederY=FEDER_TOP;
  FederHoehe=40;
  set_feder();
  bally = 50;
  calc_page_pos_of_ballpos();
  
  arm_links_old_status=2;
  arm_rechts_old_status=2;
  arm_links_status=1;
  arm_rechts_status=1;
  for (y=0; y <=255;y++) ground[y]=0;
  ballx=50; bally=260; get_ground_auto(); bx_old=50; by_old=260;

//  {show led}
  LED_anzeige_1();


//  {normal position}
/*  switch(VideoMode)
  {
   case '2' : NormalPos = 400-led_hoehe*2+7; break;
   case '0' : NormalPos = 400-led_hoehe*2+7; break;
   case '3' : NormalPos = 400-led_hoehe+3; break;
   case '1' : NormalPos = 400-led_hoehe+3; break;
  }
*/
  NormalPos = 400-led_hoehe+3;
  CurrentPos = 400;
  bende = FALSE;
  ActPlayer = 1;
  for (t = 1; t <= 6; t++)
  {
     Balls[t] = 1;
     score[t] = 0;
  }
  ruetteln = 0;
//  {**********}
  draw_arm_links(); draw_arm_rechts();
  if (highres)
  {
      for (bally = 50; bally <= 350; bally++)
	  {
           retrace(); calc_page_pos_of_ballpos();
	  }
  } else for (bally = 50; bally <= 560; bally++)
  {	  
      retrace(); calc_page_pos_of_ballpos();
  }

  init_ball_values();
}

void close_all()
{
  nosound();
/*  Release(OldHeapLimit);
  video_mode(3);
  if OldHeapSize <> memavail then begin
      writeln('Achtung Heapmanipulationen waren nicht erfolgreich!');
      readkey;
    end;
*/
  FileMode = OldFileMode;
}
