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

typedef unsigned char byte;
typedef unsigned short word;
typedef byte *pointer;
typedef byte boolean;

#ifndef DATA_PREFIX
#ifdef __riscos__
#define DATA_PREFIX "/<PCPinball$Dir>/"
#else
#define DATA_PREFIX ""
#endif
#endif

#define TRUE 1
#define FALSE 0

#define N1 " PCS-PINBALL  - Version 1.1 written by A.Scherp and J.Gelhaus ";
#define N2 " (c)opyrights reserved by PC Spiel and vIRTUAL tECHNOLOGIES GbR";

//const {LabelName  : string[9] = 'SPECIAL27';}
extern byte Bits[10];
extern word VSeg;
extern byte speedmaxy; // {max. bally-speed. (( 45}

extern char tnr;  // {tablenr}

extern byte ArmBreiteLinks;
extern byte ArmHoeheLinks;
extern word ArmXLinks;
extern word ArmYLinks;

extern byte ArmBreiteRechts;
extern byte ArmHoeheRechts;
extern word ArmXRechts;
extern word ArmYRechts;

extern word FederBreite;
extern word FederX;

extern boolean no;
extern boolean yes;
extern byte rahmen;
extern byte arm;

// Sounds common to both
#define snd1 1
#define snd2 2
#define snd3 3
#define snd4 4
#define snd5 5
#define snd6 6
#define snd7 7
#define snd8 8

// Sounds additional to table 2
#define snd9 9
#define snd10 10
#define snd11 11
#define snd12 12
#define snd13 13
#define snd14 14

extern word SetSprite_VGAADR[7];
extern word GetSprite_VGAADR[7];

extern int MaxBalls;

extern pointer OldHeapLimit;
extern int OldHeapSize;

extern word ledseg,
	 armlinksseg,armrechtsseg,
	 armlinks_mskseg,armrechts_mskseg,
     ballseg,
     groundseg,
     ballspriteseg,
     undergroundseg,
     tablegroundseg,federseg;

extern pointer led_display,
     ball,
     ground,
     ball_sprite,
     underground,
     tableground,feder;

// {typedeclaration for the mask}
//typedef byte[15361] reihe;
#define reihe(name) byte name[15361]

extern reihe(arm_links);
extern reihe(arm_rechts);

extern reihe(arm_links_msk);
extern reihe(arm_rechts_msk);


extern byte tableground1a[320 * 200];
extern byte tableground2a[320 * 200];
extern byte tableground3a[320 * 200];

#define tableground1(x, y) tableground1a[y * 320 + x]
#define tableground2(x, y) tableground2a[(y-200) * 320 + x]
#define tableground3(x, y) tableground3a[(y-400) * 320 + x]

extern char ch;

extern byte led_hoehe;
extern byte led_color_1, led_color_2;
extern word led_funktion, led_parameter,led_timer,led_x,led_y,led_status;
extern char led_anzeige_text[64]; //TODO: Check length
extern byte led_f_status_1,led_f_status_2;

//     {SEG und OFS of the FONT}
extern word Fseg,Fofs;
extern byte Fdata[4097]; // array [1..4096] of byte;
extern pointer font;

extern byte arm_links_status, arm_rechts_status;
extern byte arm_links_old_status, arm_rechts_old_status;

//     {BALL X and Y Coordinates }
extern int ballx,bally,bx_old,by_old;
//     {x und y - speed}
extern int ballspeed_y,ballspeed_x;

//     {a few randoms}
extern byte ran255[256];
extern byte ran255z;

//     {counter for the ball-gravitiy left, right, up,down}
extern byte l1,l2,r1,r2,u1,u2,o1,o2;
extern byte fu,fo,fl,fr,fm;
extern byte fh;

extern int kraft;

extern boolean overscan, highres;
extern boolean fullscreen;

extern boolean UseSound;
extern pointer sounds[15]; // :array[1..14] of pointer;   { Samples}
extern word soundlength[15]; // :array[1..14] of word;

extern int score[7]; // :array [1..6] of longint;

extern word StartPow;

extern int NormalPos;
extern int CurrentPos;

extern char path[256]; //  : string;

extern byte MAXfarbe;

extern byte OldFileMode;

extern char VideoMode;

extern boolean bende;// {ende -> true }

struct palette
{
	byte r;
	byte g;
	byte b;
};

extern struct palette pal[256];

extern byte ruetteln;

#define FEDER_TOP (400 + 181)
extern word FederY;
extern word FederHoehe;

extern byte hilfsb;

extern int Balls[7]; //    : array[1..6] of integer;
extern byte MaxPlayer;
extern byte ActPlayer;

