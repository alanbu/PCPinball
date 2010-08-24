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

// Global variables
//

#include "globals.h"

//const {LabelName  : string[9] = 'SPECIAL27';}
byte Bits[10] = {128,64,32,16,8,4,2,1,0,0};
word VSeg = 0xA000;
byte speedmaxy = 100; // {max. bally-speed. (( 45}

char tnr = '1';  // {tablenr}

byte ArmBreiteLinks = 56;
byte ArmHoeheLinks  = 48;
word ArmXLinks = 79;
word ArmYLinks = 400+135;

byte ArmBreiteRechts = 56;
byte ArmHoeheRechts  = 48;
word ArmXRechts = 159;
word ArmYRechts = 400+135;

word FederBreite = 8;
word FederX      = 302;

boolean no = FALSE;
boolean yes = TRUE;
byte rahmen = 255;
byte arm    = 128;


word SetSprite_VGAADR[7] = {258,514,1026,2050,258,514,1026};
word GetSprite_VGAADR[7] = {0x4,0x104,0x204,0x304,0x04,0x104,0x204};

int MaxBalls = 4;


pointer OldHeapLimit;
int OldHeapSize;

word ledseg,
	 armlinksseg,armrechtsseg,
	 armlinks_mskseg,armrechts_mskseg,
     ballseg,
     groundseg,
     ballspriteseg,
     undergroundseg,
     tablegroundseg,federseg;

pointer led_display,
     ball,
     ground,
     ball_sprite,
     underground,
     tableground,feder;

reihe(arm_links);
reihe(arm_rechts);

reihe(arm_links_msk);
reihe(arm_rechts_msk);


//typedef byte ttableground1[320*200]; // =array[0..319,0..199] of byte;
//typedef byte ttableground2[320*200]; // =array[0..319,200..399] of byte;
//typedef byte ttableground3[320*200];  // =array[0..319,400..599] of byte;

byte tableground1a[320 * 200];
byte tableground2a[320 * 200];
byte tableground3a[320 * 200];

char ch;

byte led_hoehe;
byte led_color_1, led_color_2;
word led_funktion, led_parameter,led_timer,led_x,led_y,led_status;
char led_anzeige_text[64]; //TODO: Check length
byte led_f_status_1,led_f_status_2;

//     {SEG und OFS of the FONT}
word Fseg,Fofs;
byte Fdata[4097]; // array [1..4096] of byte;
pointer font;

byte arm_links_status, arm_rechts_status;
byte arm_links_old_status, arm_rechts_old_status;

//     {BALL X and Y Coordinates }
int ballx,bally,bx_old,by_old;
//     {x und y - speed}
int ballspeed_y,ballspeed_x;

//     {a few randoms}
byte ran255[256];
byte ran255z;

//     {counter for the ball-gravitiy left, right, up,down}
byte l1,l2,r1,r2,u1,u2,o1,o2;
byte fu,fo,fl,fr,fm;
byte fh;

int kraft;

boolean overscan, highres;
boolean fullscreen;

boolean UseSound;
pointer sounds[15]; // :array[1..14] of pointer;   { Samples}
word soundlength[15]; // :array[1..14] of word;

int score[7]; // :array [1..6] of longint;

word StartPow;

int NormalPos;
int CurrentPos;

char path[256]; //  : string;

byte MAXfarbe;

byte OldFileMode;

char VideoMode;

boolean bende;// {ende -> true }

struct palette pal[256];

byte ruetteln;

word FederY;
word FederHoehe;

byte hilfsb;

int Balls[7]; //    : array[1..6] of integer;
byte MaxPlayer;
byte ActPlayer;


