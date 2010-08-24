
#include "globals.h"
#include "tisch2.h"

// {*** TABLE2 *************************************************************** }

byte RATa[26 - 24 + 1] ; // array[24..26] of byte; {R, A, T}
byte PHASERa[238 - 233 + 1]; // array[233..238] of byte; {P, H, A, S, E, R}
boolean RL; // {RELAUNCH}
byte PFEIL; // {2x,4x,6x}
byte Felder1; // {5k,3k,1k top left}
byte Felder2; // {500,1000,1500}
byte Felder3; // {10.000,20.000,30.000,50.000}
byte Locked;  // { 0 - > no Ball locked // 1 - > Ball locked //
              //        2 - > locked ball already released!!! }
byte Counter;

byte TischGrad = 2;
byte grad;



void Init_Tisch2()
{
  int t;

  for (t=0; t <= 255;t++) set_rgb_color(t,pal[t].r,pal[t].g,pal[t].b );
  
   RAT[24] := 0; RAT[25] := 0; RAT[26] := 0;
   PHASER[233] := 0; PHASER[234] := 0; PHASER[235] := 0; PHASER[236] := 0;
   PHASER[237] := 0; PHASER[238] := 0;
   RL := true;
   PFEIL := 0;
   Felder1 := 0;
   Felder2 := 0;
   Felder3 := 0;
   Locked := 0;
   Counter := 0;

   MAXFarbe := 150; {235-255}

  // Common variables with different values depending on table
  tnr  = '2';
  ArmXLinks = 79+5;
  ArmYLinks = 400+135+14;

  ArmXRechts = 159-4;
  ArmYRechts = 400+135+14;
}

procedure HilfsProc1;
begin
             ballx := 125;
             bally := 25{+10};
             move_ball;
             inc(bally,10);
             delay(500);
             ballspeed_y := 0;
             ballspeed_x := - 5 + random(10);
end;

procedure HilfsProc2;
begin
          if ballspeed_x < 0 then ballspeed_x := -8-random(4);
          if ballspeed_x > 0 then ballspeed_x := 8+random(4);
          if ballspeed_y < 0 then ballspeed_y := -8-random(4);
          if ballspeed_y > 0 then ballspeed_y := 8+random(4);
          counter := 10; set_rgb_color(98,60,60,60);
          incscore(50);
end;

