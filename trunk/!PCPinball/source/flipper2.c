//{ FLIPPER2.PAS   - (c) Ansgar Scherp, Joachim Gelhaus
//  All rights reserved / vt'95

#include "globals.h"
#include "common.h"
#include "tisch2.h"
#include <SDL.h>


//{*** FONTS **************************************************************** }
//{$F+}
//procedure font; external;
//{$L FONTS\thin8X8.OBJ}
//{$F-}

//{*** INCLUDEN ************************************************************* }
#include "random.h"
#include "video.h"
#include "loadprc.h"
// {I _GRAPH  .PAS} {ball and ground draw routines Kann weg:-) }
#include "ledanz.h"
#include "autodra.h"
#include "keys.h"
#include "sound2.h"
#include "init_close.h"
//{I _INTEGRI.PAS} {check for integrity // read volumelabel of cd-rom}
//{$I _CDPLAYR.PAS} {audio-cd-player-routines}

void check_ball(); // forward;

void analyse_arms()
{
  byte wl;

  if (abs(ballspeed_x) > 4) w = 0; else w = 3;

  if ((fo > 0) && (ballspeed_y < 0))
  {
      ballspeed_y = abs(ballspeed_y);
      kraft = 0;
      return;
  }
  if (ballx < 142 /*{152}*/)
  {
    if (arm_links_old_status < arm_links_status)
	{
        draw_arm_links();
        check_ball();
        ballspeed_y = - abs(Ballx+4 - ArmXLinks);
        if (ballspeed_y < - 50) ballspeed_y = -50;
        bally = bally + ballspeed_y;
        kraft = abs(ballspeed_y)-10;
        switch(arm_links_status)
		{
         case 1 : ballspeed_x+=7+random_to(4)); break;
         case 2 : ballspeed_x+=5+random_to(4)); break;
         case 4 : ballspeed_x-=5+random_to(4)); break;
         case 5 : ballspeed_x-=7+random_to(4)); break;
		}
	} else
	{
        switch(arm_links_status)
		{
         case 1 : ballspeed_x+=(abs(ballspeed_y)+w) / 4); break;
         case 2 : ballspeed_x+=(abs(ballspeed_y)+w) / 4); break;
         case 4 : ballspeed_x-=(abs(ballspeed_y)+w) / 4); break;
         case 5 : ballspeed_x-=(abs(ballspeed_y)+w) / 4); break;
		}
        ballspeed_y+=2;
	}
  } else
  {
    if (arm_rechts_old_status < arm_rechts_status)
	{
        draw_arm_rechts();
        check_ball();
        kraft = 50;
        ballspeed_y = - abs(ArmBreiteRechts - (Ballx+4/*{+8}*/ - ArmXRechts));
        if (ballspeed_y < - 50) ballspeed_y = -50;
        bally = bally + ballspeed_y;
        kraft = abs(ballspeed_y)-10;
        switch(arm_rechts_status)
		{
        case 1 : ballspeed_x -= 7+random_to(4)); break;
        case 2 : ballspeed_x -= 5+random_to(4)); break;
        case 4 : inc(ballspeed_x,5+random_to(4)); break;
        case 5 : inc(ballspeed_x,7+random_to(4)); break;
		}
	} else
	{
        switch(arm_rechts_status)
		{
         case 1 : ballspeed_x-=(abs(ballspeed_y)+w) / 4); break;
         case 2 : ballspeed_x-=(abs(ballspeed_y)+w) / 4); break;
         case 4 : ballspeed_x+=(abs(ballspeed_y)+w) / 4); break;
         case 5 : ballspeed_x+=(abs(ballspeed_y)+w) / 4); break;
		}
        ballspeed_y+=2;
	}
  }
  fl = 0; fr = 0; fu = 0; fo = 0;
}

// { *** EVENTS ***************************************************************}

void analyse_crash()
{
  byte fg,a,b,c,d,w;

  if (fr>0) fg=fr;
  if (fl>0) fg=fl;
  if (fo>0) fg=fo;
  if (fu>0) fg=fu;
  switch(fg)
  {
    case 254:
            incscore(100);
            if (ballspeed_y > 0) ballspeed_x = abs(ballspeed_y);
            else ballspeed_x = -abs(ballspeed_y);
            if (ballspeed_x > 0) ballspeed_y = abs(ballspeed_x) / 2
                                        + random_to(5);
            else ballspeed_y = -abs(ballspeed_x) / 2;
            play(snd14);
          break;
    case 253:
          //{ ballspeed_x := ballspeed_x - 10;}
            if (abs(ballspeed_x) > 4) w = 0; else w = 1;
            ballspeed_x-=(abs(ballspeed_y)+w) / 4);
            ballspeed_y+=2;
          break;
    case 252:
           //{ballspeed_x := ballspeed_x + 10;}
            if (abs(ballspeed_x) > 4) w = 0; else w = 1;
            ballspeed_x+=(abs(ballspeed_y)+w) / 4);
            ballspeed_y+=2;
          break;
    case 250,251:
            if (ballx < 160)
			{
                if (RL)
				{
                    play(snd12);
                    ballspeed_y = -30-random_to(15);
                    ballspeed_x+=6;
                    kraft = 100;
				}
			} else if (RL)
			{
                     play(snd12);
                     ballspeed_y = -30-random_to(15);
                     ballspeed_x+=4+random_to(3);
                     kraft = 100;
			}
          break;
    case 249:
            incscore(100);
            if (ballspeed_y < 0) ballspeed_x = abs(ballspeed_y);
            else ballspeed_x = -abs(ballspeed_y);
            if (ballspeed_x < 0) ballspeed_y = abs(ballspeed_x) / 2;
            else ballspeed_y = -abs(ballspeed_x) / 2;
            play(snd14);
          break;
    case 248:
            incscore(100);
            ballspeed_y = ballspeed_y / 2;
            ballspeed_x-=ballspeed_y;
            play(snd1);
            break;
    case 247:
          play(snd4);
          HilfsProc2();
          counter = 10; set_rgb_color(247,60,30,20);
          break;
    case 246:
          play(snd4);
          HilfsProc2();
          counter = 10; set_rgb_color(246,60,30,20);
          break;
    case 245:
          play(snd4);
          HilfsProc2();
          counter = 10; set_rgb_color(245,60,30,20);
          break;
    case 244:
          play(snd4);
          HilfsProc2();
          counter = 10; set_rgb_color(244,60,30,20);
          break;
    case 243:
          incscore(50);
          // {if ballspeed_x < 0 then ballspeed_x := -15;
          //if (ballspeed_x > 0) ballspeed_x = 15;}
          if (ballspeed_x < 0) ballspeed_x = -8-random_to(4);
          if (ballspeed_x > 0) ballspeed_x = 8+random_to(4);
          if (ballspeed_y < 0) ballspeed_y = -8-random_to(4);
          if (ballspeed_y > 0) ballspeed_y = 8+random_to(4);
          if (ballx < 160)
		  {
              counter = 10; set_rgb_color(96,60,30,20); play(snd3);
		  } else
		  {
              counter = 10; set_rgb_color(97,60,30,20); play(snd13);
		  }
          break;
    case 242:
           display(">-BALL-<>-LOST-<");
           play(snd9);
           while (led_status) led_anzeige(TRUE);
           senk_arms();
           SDL_Delay(1000);
           move_left();
           //{LOCKED BALL RELEASEN WHEN EXISTANCE}
           if (locked == 1)
		   {
               display("RELEASE LOCKED BALL!");

	           while (led_status) led_anzeige(TRUE);
               draw_ground_auto();
               SDL_Delay(1000);
               move_left();
               init_ball_values();
               for (a = 99; a<=101;a++)
                   set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
               PFEIL = 0;
               locked = 0;
               return;
		   }

           // {RAT-Bonus}
           if ((RAT(24) == 1) && (RAT(25) == 1) && (RAT(26) ==1))
		   {
                display("RAT-BONUS: 50000");
                while (led_status) led_anzeige(TRUE);
                SDL_Delay(500);
                move_left();
                score[actplayer]+=50000;
		   }

           // {phaser bonus}
           if (felder3 > 0)
		   {
                display("PHASER - BONUS");
		        while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                score[actplayer]+=felder3*10000;
                display(int_msg("< < < %d > > >',felder3*10000));
		        while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                move_left();
		   }

           // {Rampen-Bonus}
           if (felder1 > 0)
		   {
               display("RAMP-HIT : 25000");
               score[actplayer]+=25000;
	           while (led_status) led_anzeige(TRUE);
               SDL_Delay(1000);
               move_left();
		   }

           // {death-bonus}
           if (felder2 > 0)
		   {
                display(int_msg("DEATH BONUS %dx5',felder2*500));
	            while (led_status) led_anzeige(TRUE);
                score[actplayer]+=felder2*500*5;
                SDL_Delay(1000);
                move_left();
		   }

           //{total}
           display(int_msg("Total %d", score[actplayer]));
           while (led_status) led_anzeige(TRUE);
           SDL_Delay(1000);
           move_left();

           balls[Actplayer]++;
           if (balls[actplayer] == MaxBalls)
		   {
               display("* G A M E  O V E R *");
	           while (led_status) led_anzeige(TRUE);
               SDL_Delay(1000);
               bende = TRUE;
		   }
           actplayer++;
           if (actplayer > Maxplayer) actplayer = 1;
           if (balls[actplayer] < MaxBalls)
		   {
             if (MaxPlayer > 1)
			 {
                display(int_msg("Next Player %d", actplayer);
	            while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                move_left();
                display(int_msg("Ball %d", balls[actplayer]));
	            while (led_status) led_anzeige(TRUE);
                SDL_Delay(1000);
                move_left();
                bende = FALSE;
			 }
             init_ball_values();
             Init_Tisch2();
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

    case 241:
            ballx-=2; ballspeed_x = -1;
            ballspeed_y = abs(ballspeed_y);
          break;

    case 240:
            //{if ballspeed_x < 4 then inc(ballspeed_x,3);}
            ballspeed_x = 4;
           //{ play(snd1);}
            ballx++;
            ballspeed_y--;
          break;

	case 233:
	case 234:
	case 235:
	case 236:
	case 237:
	case 238:
        if (PHASER(fg) == 0)
		{
            play(snd11);
            PHASER(fg) = 1;
            set_rgb_color(238-fg+109,50,30,20);
            ballspeed_x+=20;
            score[actplayer]+=1000;

            if ((PHASER(233) == 1)&&(PHASER(234) == 1)&&(PHASER(235) == 1) &&
               (PHASER(236) == 1)&&(PHASER(237) == 1)&&(PHASER(238) == 1))
			{
                switch(felder3)
				{
                 case 0:
                        felder3 = 1;
                        display(">>> 10000  BONUS <<<");
                        while (led_status) led_anzeige(TRUE);
                        score[actplayer]+=10000;
                        set_rgb_color(105,60,30,20);
                      break;
                  case 1:
                        felder3 = 2;
                        display(">>> 20000  BONUS <<<");
                        while (led_status) led_anzeige(TRUE);
                        score[actplayer]+=20000;
                        set_rgb_color(106,60,30,20);
                      break;
                  case 2:
                        felder3 = 3;
                        display(">>> 30000  BONUS <<<");
                        while (led_status) led_anzeige(TRUE);
                        score[actplayer]+=30000;
                        set_rgb_color(107,60,30,20);
                      break;
                  case 3:
                        felder3 = 5;
                        display(">>> 50000  BONUS <<<");
                        while (led_status) led_anzeige(TRUE);
                        score[actplayer]+=50000;
                        set_rgb_color(108,60,30,20);
                      break;
				}
                if (felder3 < 5)
				{
                    for (fg = 109; fg <=114; fg++)
					{
                        set_rgb_color(fg,pal[fg].r,pal[fg].g,pal[fg].b);
					}
                    PHASER(233) = 0; PHASER(234) = 0; PHASER(235) = 0;
                    PHASER(236) = 0; PHASER(237) = 0; PHASER(238) = 0;
				}
			}
		}
        break;
	}
}

// { *** AREAS *************************************************************** }

void analyse_boden()
{
  byte fg;
  byte h1,h2;

  switch(fm)
  {
      case 1:
             play(snd7);
             display("Sure Shot 25000");
             while(led_status) led_anzeige(TRUE);
             draw_ground_auto();
             do 
			 {
               bally+=2;
               calc_page_pos_of_ballpos(); retrace();
			 } while (bally <= 320);
             ballx = 5;
             ballspeed_x = 0; ballspeed_y = 0; kraft = 0;
             SDL_Delay(350);
             score[actplayer]+=25000;
			 break;
       case 2:
             ballspeed_y+=4;
             break;
       case 3:
             play(snd7);
             switch(pfeil)
			 {
              case 0:
                     pfeil = 2;
                     set_rgb_color(99,63,20,0);
                   break;
              case 2:
                     pfeil = 4;
                     set_rgb_color(100,63,20,0);
                   break;
              case 4:
                     pfeil = 6;
                     set_rgb_color(101,63,20,0);
                   break;
              case 6:
                     if (locked == 0)
					 {
                         locked = 1;
                         display("BALL LOCKED!");
                         while(led_status) led_anzeige(TRUE);
                         draw_ground_auto();
                         SDL_Delay(1000);
                         init_ball_values();
                         return;
					 }
                     display("BALL ALREADY LOCKED!");
		             while(led_status) led_anzeige(TRUE);
                     SDL_Delay(500);
                   break;
			 }
             score[actplayer]+=pfeil*2000;
             display(int_msg("Lock door : %dx2000", pfeil));
             while(led_status) led_anzeige(TRUE);
             SDL_Delay(1000);
           break;
      case 5:
             display("KILL THE RAT!");
           break;
      case 6:
             play(snd7);
             h1 = MAXFarbe;
             MAXFarbe = 209;
             display("RAMP BONUS:");
             play(snd12);
             while(led_status) led_anzeige(TRUE);
             SDL_Delay(500);
             draw_ground_auto();
             SDL_Delay(1000);
             switch(felder1)
			 {
              case 0:
                     set_rgb_color(95, 63,20,0);
                     felder1 = 1;
                   break;
              case 1:
                     set_rgb_color(94, 63,20,0);
                     felder1 = 3;
                   break;
              case 3:
                     set_rgb_color(93, 63,20,0);
                     felder1 = 5;
                   break;
			 }
             display(int_msg("-._ %d000 * 5 _.-",felder1));
             while(led_status) led_anzeige(TRUE);
             play(snd11);
             ballx = 15; bally = 106;
             score[actplayer]+=felder1*1000*5;
             ballspeed_x = 0; ballspeed_y = 0;
             for ( ;bally <=136; bally++)
			 {
                 retrace(); move_ball();
                 calc_page_pos_of_ballpos();// {readkey;}
			 }
             for ( ;bally <=145; bally++)
			 {
                 retrace(); move_ball();
                 calc_page_pos_of_ballpos(); // {readkey;}
                 ballx++;
			 }
             for ( ; bally < 223+random_to(3); bally++)
			 {
                 retrace(); move_ball();
                 calc_page_pos_of_ballpos();// {readkey;}
                 if ((bally % 6) == 0) ballx++; else ballx+=2;
			 }
             do
			 {
               retrace(); move_ball();
               calc_page_pos_of_ballpos();// {readkey;}
               bally+=2;
			 } while (bally <= 250);
             draw_ground_auto();
             SDL_Delay(500);
             MAXFarbe = h1;
             // {zufall}
             h1 = random(4)+13;
             ballspeed_y = 0;
             ballspeed_x = 0;
             switch(h1)
			 {
              case 13:
                      ballx = 170; bally = 233;
                      ballspeed_y = -random_to(15)-15;
                    break;
              case 14:
                      ballx = 189; bally = 250;
                      ballspeed_x = random_to(15)+15;
                    break;
              case 15:
                      ballx = 170; bally = 269;
                      ballspeed_y = random_to(15)+15;
                    break;
               case 16:
                      ballx = 151; bally = 250;
                      ballspeed_x = -random_to(15)-15;
                    break;
			 }
           break;
       case 7:
             display("NO RAMP!");
           break;
       case 8:
             play(snd7);
             display("RAT HOLE 3000");
             while(led_status) led_anzeige(TRUE);
             draw_ground_auto();
             score[actplayer]+=3000;
             for ( ;bally >= 40; bally--)
			 {
                 retrace(); calc_page_pos_of_ballpos();
			 }
             HilfsProc1();
           break;
      case 10:
            if (ballspeed_y > 0)
			{
                play(snd10);
                rl = !rl;
                if (rl) set_rgb_color(115,pal[115].r,pal[115].g,pal[115].b);
                else set_rgb_color(115,20,10,10);
                if (rl) display("RELAUNCH ON!")
                else display("RELAUNCH OFF!");
			}
            if (felder2 < 3)
			{
                felder2++;
                display(int_msg("DEATH BONUS %d", felder2*500));
                set_rgb_color(102+felder2-1,20,30,60);
			}
          break;
     case 11:
            play(snd10);
            if (ballspeed_y < 0)
			{
                ballspeed_y = ballspeed_y / 4;
                ballspeed_x = 8;
			}
          break;
      case 12:
              if (balls[actplayer] < maxballs) display(">Hit SPACE to start<");
			  break;
	  case 13:
	  case 14:
	  case 15:
	  case 16:
		  do { h1 = random_to(4)+13; } while (h1 == fm);
          // {delay(150);}
                 draw_ground_auto();
                 SDL_Delay(400);
                 ballspeed_y = 0;
                 ballspeed_x = 0;
                 switch(h1)
				 {
                  case 13:
                          ballx = 170; bally = 233;
                          ballspeed_y = -random_to(15)-15;
                        break;
                  case 14:
                          ballx = 189; bally = 250;
                          ballspeed_x = random_to(15)+15;
                        break;
                  case 15:
                          ballx = 170; bally = 269;
                          ballspeed_y = random_to(15)+15;
                        break;
                  case 16:
                          ballx = 151; bally = 250;
                          ballspeed_x = -random_to(15)-15;
                        break;
				 }
                 move_ball();
                 //{delay(500);}
                 score[actplayer]+=1000;
                 play(snd1);
           break;
     case 17:
		  //{inc(ballspeed_x);}
          break;
     case 23:
             ballspeed_x--;
           break;
	 case 24:
	 case 25:
	 case 26:
          if (ballspeed_y < 0) return;
          play(snd10);
          if (RAT(fm) == 0)
		  {
              set_rgb_color(fm-24+90,63,20,0);
              RAT(fm) = 1;
              score[actplayer]+=1000;
		  }
          if ((RAT(24) == 0) || (RAT(25) == 0) || (RAT(26) == 0)
		  {
            display("RAT is incomplete");
		  } else
		  {
                display("RAT is COMPLETE!");
                score[actplayer]+=10000;
		  }
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
            ballspeed_y = -(ballspeed_y /*{+abs(ballspeed_y)div 2) div 4}*/ / 2);
	  }        
      if ((o1 > 0) && (o2 == 0)) if (ballspeed_x < 4) ballspeed_x++;
      if ((o1 == 0) && (o2 > 0)) if (ballspeed_x > -4) ballspeed_x--;
  }
  if (ballspeed_y >= 0)
  {
      check_ball_unten();
      if (u1 + u2 > 0)
	  {
          ballspeed_y = -(ballspeed_y /*{+abs(ballspeed_y)div 2) div 4}*/ / 2);
          kraft = abs(ballspeed_y / 2);
	  }
      if ((u1 > 0) && (u2 == 0) && ballspeed_x < 4)
	  {
          if (random_to(100) > 30) ballspeed_x++; else ballx++;
	  }
      if ((u1 == 0) && (u2 > 0) && ballspeed_x > -4)
	  {
          if (random_to(100) > 30) ballspeed_x--; else ballx--;
	  }
  }
  if (ballspeed_x <= 0)
  {
      check_ball_links();
      if (l1 + l2 > 0)
	  {
          ballspeed_x = abs((ballspeed_x+abs(ballspeed_y) / 3) / 4);
          //{if ballspeed_x < 4 then inc(ballspeed_x,2);}
	  }
      if ((l1 > 0) && (l2 == 0)) ballspeed_y++;
      if ((l1 == 0) && (l2 > 0)) ballspeed_y--;
  }

  if (ballspeed_x >= 0)
  {
      check_ball_rechts();
      if (r1 + r2 > 0)
	  {
		ballspeed_x = -((ballspeed_x+abs(ballspeed_y)/ 2) / 4);
        //{if ballspeed_x > -4 then dec(ballspeed_x,2);}
	  }
      if ((r1 > 0) && (r2 == 0)) ballspeed_y++;
      if ((r1 == 0) && (r2 > 0)) ballspeed_y--;
  }

  if ((l2 > 0) && (l1 == 0))
  {
      if (((u1 > 0) || (u2==0)) && ballspeed_x <= 0)
	  {
          ballspeed_y++; ballspeed_x++;
	  }
  }
  if ((r2 > 0) && (r1 == 0))
  {
      if ((u1 == 0) && (u2>0) && ballspeed_x >= 0)
	  {
          ballspeed_y++; ballspeed_x--;
	  }
  }
  if ((l1 > 0) && (l2 == 0)
  {
      if (((o1 > 0) || (o2==0)) && ballspeed_x >= 0)
	  {
              /*{inc(bally);} */ ballx--;
	  }
  }
  if ((r1 > 0) &&(r2 == 0)
  {
      if ((o1 == 0) && (o2>0) && ballspeed_x <= 0)
	  {
        /* {inc(bally);} */ ballx++;
	  }
  }

  if (grad == TischGrad) grad = 0;
  else
  {
	ballspeed_y++; grad++;
  }
//{  inc(ballspeed_y); {}
  if (ballspeed_y > speedmaxy) ballspeed_y = speedmaxy;
}


void flipper2()
{
  int a;
  byte b,c,d;

  checkbreak = false;
//  if (paramcount <> 1) or (length(paramstr(1)) <> 3) then halt(0);
//  {detect soundblaster and initialize the values}
//  textcolor(black);
//  textbackground(black);
//  detect_soundblaster;

  init_all();
  Init_Tisch2();

//  {main-loop}
  while (!bende)
  {
    // {get key}
	keyboard();
	switch(ch)
	{
      case 'k':
              ballspeed_y = ballspeed_y - 10;
              ballspeed_x = ballspeed_x - 6 + random_to(12);
              ruetteln++;
              display(int_msg("DER %d.RšTTLER!",ruetteln));
              if (ruetteln == 5)
			  {
                  bende = true;
                  display("T I L T !");
			  }
              while (led_status) led_anzeige(TRUE);
              SDL_Delay(200);
            break;
//      'P' : StartCDPlayer;
     case 'q':
          if (NormalPos ==  CurrentPos)
		  {
              Display("Are you a coward ?");
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

          ballspeed_y := -StartPow / 4 - 30 - 19; kraft = 90;
          display("GET THE SPACE-RAT");
          while (led_status) led_anzeige(FALSE);
		 
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
	   
       if (bally > 581) fu = 242;
       if ((fr>0) || (fl>0) || (fu>0) || (fo>0)) analyse_crash();
       if (bende) break;
       fh = gettablepixel(ballx+8,bally+8);
       if (fh <> fm)
	   {
          if ((fh>0) && (fh<128))
		  {
			  fm = fh;
			  analyse_boden();
		  } else if (fh == 0) fm = 0;
	   }
	}

    led_anzeige(FALSE);

    switch(counter)
	{
     case 0: break;
     case 1:
             for (a = 96; a <=98; a++)
                 set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
             for (a = 116; a <=119;a++)
				 set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
             counter--;
           break;
	 default:
        counter--;
	}
  }  // End of game loop

  for (b = 0; b <=63; b++)
    for (a = 0; a <=255; a++)
	{
        if (pal[a].r > 0) pal[a].r--;
        if (pal[a].g > 0) pal[a].g--;
        if (pal[a].b > 0) pal[a].b--;
        set_rgb_color(a,pal[a].r,pal[a].g,pal[a].b);
	}
/*
  Close_All;
  asm sti end;
*/
	}
