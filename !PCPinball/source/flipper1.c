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

/*{ FLIPPER1.PAS   - (c) Ansgar Scherp, Joachim Gelhaus
  All rights reserved / vt'95

  1 Parameter = 'abc'

  a = 1-4  ->> set overscan on/off and highres. on/off
  b = 1-2  ->> 1 = SB-Sound   2 = No Sound
  c = 1-6  ->> Players

  }

{*** INCLUDEN ************************************************************** }
*/
#include <SDL.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "video.h"
#include "tisch1.h"
#include "autodra.h"
#include "ledanz.h"
#include "keys.h"
#include "sound1.h"
#include "random.h"
#include "error.h"
#include "common.h"

/*
{$I _RANDOM .PAS} {short random number list}
{$I _VIDEO  .PAS} {all video functions // // and arm_draw}
{$I _LOADPRC.PAS} {all loadingroutines}
{$I _LEDANZ .PAS} {all routines for the led}
{$I _AUTODRA.PAS} {procedure for automatic-draw // chose the right plane}
{$I _KEYS   .PAS}
{$I _SOUND1 .PAS} {soundkit}
*/
// {$I _INI_CLO.PAS} {init_all & close}
#include "init_close.h"
/*{I _INTEGRI.PAS} {check for integrity // read volumelabel of cd-rom}
{$I _CDPLAYR.PAS} {audio-cd-player-routines}
{$I _TISCH1 .PAS}


{$M 65520,0,655360}
{$P+,G+}
uses dos,crt,soundkit,audiotpu;
*/

/* Forward declarations */
void analyse_arms();

/*
{*** FONTS ***************************************************************** }
{$F+}
procedure font; external;
{$L FONTS\BLCKSNSF.OBJ}
{$F-}

*/

void do_fire_stuff()
{
  if (random_to(100) > 60)
  {
     if (temp < 6)
	 {
        temp++;
        display("Temperature is");
        while(led_status) led_anzeige(TRUE);
        SDL_Delay(1500);
        display(":-) DECREASING (-:");
        while(led_status) led_anzeige(TRUE);
        SDL_Delay(1500);
	 }
  } else
  {
      if (temp > 0)
	  {
        temp--;
        display("Temperature is");
        while(led_status) led_anzeige(TRUE);
        SDL_Delay(1500);
        display(":-(  RISING  )-:");
        while(led_status) led_anzeige(TRUE);
        SDL_Delay(1500);
	  }
  }
  thermo(temp);
}



void check_ball(); // forward;


void analyse_arms()
{
  if ((fo > 0) && (ballspeed_y < 0))
  {
      ballspeed_y = abs(ballspeed_y);
      kraft = 0;
      return;
  }
  if (ballx < 142/*{152}*/)
  {
    if (arm_links_old_status < arm_links_status)
	{
        draw_arm_links();
        check_ball();
        ballspeed_y = - abs(ballx+4 - ArmXLinks);
        if (ballspeed_y < - 50) ballspeed_y = -50;
        bally = bally + ballspeed_y;
        kraft = abs(ballspeed_y);
        switch(arm_links_status)
		{
         case 1 : ballspeed_x+=7+random_to(4); break;
         case 2 : ballspeed_x+=5+random_to(4); break;
         case 4 : ballspeed_x-=5+random_to(4); break;
         case 5 : ballspeed_x-=7+random_to(4); break;
		}
	} else
	{
        switch(arm_links_status)
		{
         case 1 : ballspeed_x +=abs(ballspeed_y) / 4; break;
         case 2 : ballspeed_x +=abs(ballspeed_y) / 4; break;
         case 4 : ballspeed_x -=abs(ballspeed_y) / 4; break;
         case 5 : ballspeed_x -=abs(ballspeed_y) / 4; break;
		}
	}
  } else
  {
    if (arm_rechts_old_status < arm_rechts_status)
	{
      draw_arm_rechts();
      check_ball();
      kraft = 50;
      ballspeed_y = - abs(ArmBreiteRechts - (ballx+4/*{+8}*/ - ArmXRechts));
      if (ballspeed_y < - 50) ballspeed_y = -50;
      bally = bally + ballspeed_y;
      kraft = abs(ballspeed_y);
      switch( arm_rechts_status )
	  {
       case 1 : ballspeed_x-=7+random_to(4); break;
       case 2 : ballspeed_x-=5+random_to(4); break;
       case 4 : ballspeed_x+=5+random_to(4); break;
       case 5 : ballspeed_x+=7+random_to(4); break;
	  }
	} else
	{
        switch(arm_rechts_status)
		{
          case 1 : ballspeed_x -=abs(ballspeed_y) / 4; break;
          case 2 : ballspeed_x -=abs(ballspeed_y) / 4; break;
          case 4 : ballspeed_x +=abs(ballspeed_y) / 4; break;
          case 5 : ballspeed_x +=abs(ballspeed_y) / 4; break;
		}
	}
  }
  fl = 0; fr = 0; fu = 0; fo = 0;
}

// { *** EVENTS ***************************************************************}

void analyse_crash()
{
	byte fg,a,b,c,d;

  if (fr>0) fg=fr;
  if (fl>0) fg=fl;
  if (fo>0) fg=fo;
  if (fu>0) fg=fu;

  switch(fg)
  {
  case 254:
          play(snd2); incscore(100);
          if (ballspeed_x <= 0) ballspeed_x = -10;
          if (ballspeed_x > 0) ballspeed_x = 10;
          ballspeed_y-=4; kraft = 10;
          break;

   case 253:
          if (bally < 300)
		  {
             b = random_to(20)+40;
             c = random_to(10)+50;
             d = random_to(10)+40;
             for (a = 240; a<=248;a++) set_rgb_color(a,b,c,d);
		  }
          retrace();
          play(snd7); incscore(50);
          if (ballspeed_x < 0) ballspeed_x = -8-random_to(4);
          if (ballspeed_x > 0) ballspeed_x = 8+random_to(4);
          if (ballspeed_y < 0) ballspeed_y = -8-random_to(4);
          if (ballspeed_y > 0) ballspeed_y = 8+random_to(4);
          BumpCount = 7;
          kraft = kraft / 2;
          break;

   case 250:
   case 251:
   case 252:
			play(snd4);
			Lichter1(fg) = 1;
			set_rgb_color(fg-250+53,40,20,50);
			display(int_msg("Light %d turned on!", fg-249));
			if (Lichter1(250) == 1 && Lichter1(251)==1
				&& Lichter1(252) == 1)
			{
					display("COMPLETE BONUS 9999!");
					score[ActPlayer]+=9999;
					while (led_status) led_anzeige(TRUE);
					Lichter1(250) = 0;
					Lichter1(251) = 0;
					Lichter1(252) = 0;
					set_rgb_color(53,pal[53].r,pal[53].g,pal[53].b);
					set_rgb_color(54,pal[54].r,pal[54].g,pal[54].b);
					set_rgb_color(55,pal[55].r,pal[55].g,pal[55].b);
			}
			break;

   case 249:
   case 248:
   case 247:
            Lichter2(fg) = 1;
            score[ActPlayer] += 100;
			display(int_msg("Spot %d touched!", fg-246));
			break;

   case 246:
   case 245:
   case 244:
            play(snd5);
            Lichter3(fg) = 1;
            score[ActPlayer] +=1000;
            break;

   case 243:
          play(snd3);  score[ActPlayer]+=50;
          if (ballspeed_x < 0) ballspeed_x = -15;
          if (ballspeed_x > 0) ballspeed_x = 15;
		  break;

   case 242:
          display("Yuppieeee...");
          play(snd6);
          score[ActPlayer]+=500;
          ballspeed_y = -80-random_to(100);
          ballx=8;
          kraft=240;
          ballspeed_x = 3;
          break;

   case 241:
           display(")-: ball  lost :-(");
           while (led_status) led_anzeige(TRUE);
           senk_arms();
           SDL_Delay(1000);
           move_left();
           display("B O N U S");
           while (led_status) led_anzeige(TRUE);
           SDL_Delay(1000);
           move_left();

           // {1000xball}
		   display(int_msg("Balls 1000 x %d", Balls[ActPlayer]));
           while (led_status) led_anzeige(TRUE);
           score[ActPlayer] += Balls[ActPlayer]*1000;
           SDL_Delay(500);
           move_left();

           // {10000x}
           if (Kurven > 0)
		   {
               for ( ; Kurven >= 1; Kurven--)
			   {
                  display(int_msg("Loop 10000 x %d", Kurven));
                  while (led_status) led_anzeige(TRUE);
                  score[ActPlayer]+=Kurven*10000;
                  SDL_Delay(500);
			   }
               move_left();
		   }

           // {PCS special}
           if (PCSspe[1] == 1 && PCSspe[2] == 1 && PCSspe[3] ==1
               && special ==1)
		   {
				display("PCSspecial full!");
				while (led_status) led_anzeige(TRUE);
				SDL_Delay(500);
				move_left();
				score[ActPlayer]+=100000;
				display(">>> 100000 <<<");
				while (led_status) led_anzeige(TRUE);
				SDL_Delay(500);
				move_left();
		   }

           // {total}
           display(int_msg("Total %d", score[ActPlayer]));
		   while (led_status) led_anzeige(TRUE);
		   SDL_Delay(1000);
           move_left();

           Balls[ActPlayer]++;
           if (Balls[ActPlayer] == MaxBalls)
		   {
                 display("> G A M E  O V E R <");
                 while (led_status) led_anzeige(TRUE);
                 SDL_Delay(1000);
                 bende = TRUE;
		   }

           ActPlayer++;
           if (ActPlayer > MaxPlayer) ActPlayer = 1;
           if (Balls[ActPlayer] < MaxBalls)
		   {
             if (MaxPlayer > 1)
			 {
                display(int_msg("Next Player %d", ActPlayer));
                while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                move_left();
                display(int_msg("Ball %d",Balls[ActPlayer]));
                while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                move_left();
                bende = FALSE;
			 }
             init_ball_values();
             Init_Tisch1();
             for (a = 0; a <= 250; a++)
			 {
                CTRL_shift_keys();
                arm_links_old_status=arm_links_status;
                arm_rechts_old_status=arm_rechts_status;
                check_flipper_arms();
			 }
             
             senk_arms();
		   }
		   break;

   case 239:
            senk_arms();
            ballx = 23; bally = 411; move_ball();
			retrace();
            score[ActPlayer]+=10000;
            ballspeed_y = random_to(15) + 15;
            ballspeed_x = random_to(10) + 20;
            display("PREPARE");
            while (led_status) led_anzeige(TRUE);
            SDL_Delay(500);
            display("FOR");
            while (led_status) led_anzeige(TRUE);
            SDL_Delay(500);
            display("BATTLE...");
            while (led_status) led_anzeige(TRUE);
            SDL_Delay(500);
            display("NOW!");
            while(led_status) led_anzeige(TRUE);
            if (random_to(100) > 70)
			{
                SDL_Delay(700);
                display("NO! WAIT...");
                ballspeed_x = 0;
                ballspeed_y = 0;
	            while(led_status) led_anzeige(TRUE);
                SDL_Delay(1200);
                do_fire_stuff();
			}
            break;

   case 238:
            ballx = 102; bally = 74; move_ball(); retrace();
            do_fire_stuff();
            ballspeed_y = - random_to(15) - 15;
            ballspeed_x = - random_to(15) - 15;
            break;

   case 237:
            if (ballx < 160)
			{
                if (PushUp)
				{
                    ballspeed_y = -30-random_to(15);
                    ballspeed_x+=4;
                    kraft = 100;
				}
			} else if (PushUp)
			{
               ballspeed_y = -30-random_to(15);
               ballspeed_x-=4;
               kraft = 100;
			}
			break;

   case 236:
            ballspeed_x = ballspeed_x + ballspeed_y / 4;
            break;
  }
}

// { *** ARREAS *************************************************************** }

void analyse_boden()
{
  byte h1,h2;
  byte ll;

  switch(fm)
  {
  case 1:
  case 2:
  case 3:
        set_rgb_color(fm,100,100,0);
        PCSspe[fm] = 1;
        incscore(75);
        if (PCSspe[1] == 1 && PCSspe[2] == 1 && PCSspe[3] ==1
			&& special ==1 )
		{
            display("*** PCS  special ***");
            score[ActPlayer]+=10000;
		}
		break;

  case 4: incscore(1000); break;

  case 5:
        senk_arms();
        play(snd7);
        display("WORM-HOLE:");
        while (led_status) led_anzeige(TRUE);
        SDL_Delay(500);
        move_left();
        h2 = random_to(4);
		ll = random_to(15)+15;
        for (h1 = 8; h1 <= ll;h1++)
		{
            h2++;
            if (h2 > 3) h2 = 0;
            switch(h2)
			{
            case 0: display("ACCESS PERMITTED"); break;
            case 1: display("Icy-Bonus 10000"); break;
            case 2: display("ACCESS DENIED!"); break;
            case 3: display("25000 Bonus"); break;
			}
            while (led_status) led_anzeige(TRUE);
            play(snd4);
            SDL_Delay(h1*7);
		}

        switch(h2)
		{
        case 0:
			do
			{
                retrace();
                calc_page_pos_of_ballpos();
                bally-=2;
			} while (bally >= 85);
            ballx=160;
            bally=85;
			retrace();
            display("Bonus 150000");
            score[ActPlayer]+=150000;
            while (led_status) led_anzeige(TRUE);
            SDL_Delay(500);
            break;
        case 1 : score[ActPlayer] += 10000; break;
        case 3 : score[ActPlayer] += 25000; break;
		}
        ballspeed_x = -2;
		break;

  case 6:
        ballspeed_y = ballspeed_y + random_to(6) - 3;
        ballspeed_x = ballspeed_x + random_to(6) - 3;
        display("ICE SLIDDERING...!");
		break;

  case 7:
        ballspeed_y = ballspeed_y + random_to(6) - 3;
        ballspeed_x = ballspeed_x + random_to(6) - 3;
        display("WARNING: slippery!");
		break;

  case 8:
        incscore(300);
        if (ballspeed_x > 0)
		{
			Kurven++;
			display(int_msg("Loops : %d", Kurven));
			score[ActPlayer] +=Kurven*1000;
			if (Kurven == 11)
			{
				Balls[ActPlayer]--;
				display("/ Loop-Bonus Ball \\");
				while (led_status) led_anzeige(TRUE);
				SDL_Delay(1500);
			}
		} else
		{
           display("Wrong way, dude!");
 		   while (led_status) led_anzeige(FALSE);
		}
		break;

  case 9: incscore(800); break;

  case 10:
        ballspeed_y = ballspeed_y + random_to(6) - 3;
        ballspeed_x = ballspeed_x + random_to(6) - 3;
        display("ICE SLIDDERING...!");
        break;

  case 11:
        incscore(75);
        if (PCSspe[1] == 1 && PCSspe[2] == 1 && PCSspe[3] == 1)
		{
           set_rgb_color(fm,100,100,0); special = 1;
            display("*** PCS  special ***");
            score[ActPlayer]+=10000;
		} else
			display("PCS is incomplete!");
		break;

  case 12:
        if (Balls[ActPlayer] < MaxBalls) display(">Hit SPACE to start<");
		break;

  case 13:
        incscore(1000);
        play(snd4);
		break;
  }
}


void check_ball()
{
  o1 = 0; o2 = 0; u1 = 0; u2 = 0;
  l1 = 0; l2 = 0; r1 = 0; r2 = 0;

  fu = 0; fo = 0; fl = 0; fr = 0;
  if (ballspeed_y < 0)
  {
      check_ball_oben();
      if (o1 + o2 > 0)
	  {
          kraft--;
          if (kraft < 0)
		  {			  
           ballspeed_y = -(ballspeed_y /*{+abs(ballspeed_y)/ 2) / 4}*/ / 2);
		  }
	  }
      if (o1 > 0 && o2 == 0)
	  {
		  if (ballspeed_x < 4) ballspeed_x++;
	  }
      if (o1 == 0 && o2 > 0)
	  {
		  if (ballspeed_x > -4) ballspeed_x--;
	  }
  }
  if (ballspeed_y >= 0)
  {
      check_ball_unten();
      if (u1 + u2 > 0)
	  {
          ballspeed_y = -(ballspeed_y /*{+abs(ballspeed_y)div 2) div 4}*/ / 2);
          kraft = abs(ballspeed_y / 2);
	  }
      if (u1 > 0 && u2 == 0)
	  {
		  if (ballspeed_x < 4)
		  {
           if (random_to(100) > 30) ballspeed_x++; else ballx++;
		  }
	  }
      if (u1 == 0 && u2 > 0)
	  {
		  if (ballspeed_x > -4)
		  {
           if (random_to(100) > 30) ballspeed_x--; else ballx--;
		  }
	  }
  }
  if (ballspeed_x <= 0)
  {
     check_ball_links();
     if (l1 + l2 > 0)
	 {
          ballspeed_x = abs((ballspeed_x+abs(ballspeed_y)/3) / 4) /*{+1}*/;
//          {if ballspeed_x < 4 then inc(ballspeed_x,2);}
	 }
      if (l1 > 0 && l2 == 0) ballspeed_y++;
      if (l1 == 0 && l2 > 0) ballspeed_y--;
  }
  if (ballspeed_x >= 0)
  {
      check_ball_rechts();
      if (r1 + r2 > 0)
	  {
          ballspeed_x = -((ballspeed_x+abs(ballspeed_y)/ 2) / 4) /*{-1}*/;
//          {if ballspeed_x > -4 then dec(ballspeed_x,2);}
	  }
      if (r1 > 0 && r2 == 0) ballspeed_y++;
      if (r1 == 0 && r2 > 0) ballspeed_y--;
  }

  if (l2 > 0 && l1 == 0)
  {
      if (u1 > 0 || u2==0)
	  {
		  if (ballspeed_x <= 0)
		  {
              ballspeed_y++; ballspeed_x++;
		  }
	  }
  }
  if (r2 > 0 && r1 == 0)
  {
      if (u1 == 0 && u2>0)
	  {
		  if (ballspeed_x >= 0)
		  {
			ballspeed_y++; ballspeed_x--;
		  }
	  }
  }

  if (l1 > 0 && l2 == 0)
  {
      if (o1 > 0 || o2==0)
	  {
		  if (ballspeed_x >= 0)
		  {
              /*{inc(bally);}*/ ballx--;
		  }
	  }
  }
  if (r1 > 0 && r2 == 0)
  {
      if (o1 == 0 && o2>0)
	  {
		  if (ballspeed_x <= 0)
		  {
             /* {inc(bally);} */ ballx++;
		  }
	  }
  }

/*{  if kraft < 0 then }*/ ballspeed_y++;
  if (ballspeed_y > speedmaxy) ballspeed_y = speedmaxy;
}


void flipper1()
{
	int a;

	// asm cli end;
//  checkbreak = false;
//  if (paramcount <> 1) or (length(paramstr(1)) <> 3) then halt(0);
//  {search for right cd-rom // canceld here!}
//  {CheckCDROM;}
//  {detect soundblaster and initialize the values}
//  textcolor(black);
//  textbackground(black);
//  detect_soundblaster;
//  {initialise}
  init_all();
  Init_Tisch1();

  FederY=FEDER_TOP+(StartPow/5);
  FederHoehe=40-(StartPow / 5);
  set_feder();
  move_ball();

//  {main-loop}
  while (!bende)
  {
    // {get key}
	keyboard();
	switch(ch)
	{
	  case 'k': // Shake table
            ballspeed_y = ballspeed_y - 10;
            ballspeed_x = ballspeed_x - 6 + random_to(12);
            ruetteln++;
            display(int_msg("DER %d.RšTTLER!",ruetteln));
            if (ruetteln == 5)
            {
                bende = TRUE;
                display("T I L T !");
            }
            while (led_status) led_anzeige(TRUE);
            SDL_Delay(200);
	    break;
	    
//      'P' : StartCDPlayer;

	  case 'q':
	    if (NormalPos == CurrentPos)
		{
            display("Really quit ?");
            while (led_status) led_anzeige(TRUE);
			do
			{
				keyboard();
			} while(ch != 'y' && ch != 'n' && ch != 'q');
            if (ch == 'n')
			{
                bende = FALSE;
                display("No!");
			} else
			{
				bende = TRUE;
                display("Yes!");
			}				
            while (led_status) led_anzeige(TRUE);
		}
		break;

	  case ' ':
		if ((fm == 12) && (NormalPos == CurrentPos) && (Balls[ActPlayer] < MaxBalls))
		{
          display("Release to start!");
          while (led_status) led_anzeige(FALSE);
          StartPow = 0;
          do
		  {
            if (StartPow < 75)
			{
				StartPow+=2;
                FederY=FEDER_TOP+StartPow / 5;
                FederHoehe=40-StartPow / 5;
                retrace();
                set_feder();
                move_ball();
			} else play(snd1);
            check_flipper_arms();
            keyboard();
		  } while (spacedown());

          do
		  {
            FederY -= 2;
            if (FederHoehe<40) FederHoehe+=2;
            else FederHoehe = 40;
            set_feder();
            move_ball();
            retrace();
		  } while (FederY > FEDER_TOP);

          ballspeed_y = -StartPow; kraft = 200;
          display("\" GO FOR THE ICE \"");
          while (led_status) led_anzeige(FALSE);
	    }
		break;
	}

//    {get extended key} - process flippers
    CTRL_shift_keys();

//    {arms}
    check_flipper_arms();
//    {calc_new_ball_pos // check border etc. // main proc}
    check_ball();
    if ((fr==arm) || (fl==arm) || (fu==arm) || (fo==arm)
        || (fm==arm) || (fh==arm))
	   analyse_arms();

    arm_links_old_status=arm_links_status;
    arm_rechts_old_status=arm_rechts_status;
//    {final check routine}
    if (CurrentPos > NormalPos)
	{
      if ((ballx == bx_old) && (bally == by_old)) retrace();
      CurrentPos--;//TODO: check SetLineComp(CurrentPos);
	}
    
    if ((ballx != bx_old) || (bally != by_old))
	{
       retrace();
//        {set ball}
       calc_page_pos_of_ballpos();
       move_ball();

       if ((fr>0) || (fl>0) || (fu>0) || (fo>0)) analyse_crash();
       if (bende) break;
       fh = gettablepixel(ballx+8,bally+8);
       if (fh != fm)
	   {
         if ((fh>0) && (fh<128))
		 {
			 fm = fh;
			 analyse_boden();
		 } else if (fh == 0) fm = 0;
	   }
	}

    led_anzeige(FALSE);

	switch(BumpCount)
	{
	case 0: break;
	case 1:
      for (a = 240; a <= 248; a++)
        set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
      BumpCount = 0;
	  break;
	default:
	  BumpCount--;
	  break;
	}

//  {fireflacker}
    if (PushUp == TRUE) flameflacker();

  } // End of game loop

/*
  for b = 0 to 63 do
    for a = 0 to 255 do begin
        if pal[a].r > 0 then dec(pal[a].r);
        if pal[a].g > 0 then dec(pal[a].g);
        if pal[a].b > 0 then dec(pal[a].b);
        set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
      end;
  Close_All;
  asm sti end;
  halt(1);
end.
*/
}
