/*{ FLIPPERM.PAS   - (c) Ansgar Scherp, Joachim Gelhaus
  All right reserved / vt'95

  MAIN MENU

  }

  This code is no longer used - see main.c instead
*/
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "loadprc.h"
#include "video.h"
#include "error.h"
#include <SDL.h>

// tables
extern void flipper1();

byte rahmen1 = 35;
byte rahmen2 = 36;

// first entry of following is a dummy value to match
// the pascal addressing of the array starting at 1
byte menucolor[7] = {0, 95,96,97,98,99,100};
byte playercolor[7] = {0, 101,102,103,104,105,106};
byte kreuz[4] = {0, 35,36,107};

byte auswahl;
boolean ende;
byte auswahlcolor,auswahlrichtung;
byte tables,highres,overscan,music;
pointer minis[3]; // :array[1..2] of pointer;
word I;
byte player;
char path[256];
byte hilfsb;
char params[8];
int code;

/*
{$I _NORMVGA.PAS}
*/

void SET_PAL0()
{
	set_rgb_color(0, pal[0].r, pal[0].g, pal[0].b);
}

/*
procedure decode_paras;
var i:byte;
begin
  val(params,i,code);
  if I and 64 = 64 then tables:=2   else tables:=1;
  if i and 32 = 32 then music:=1    else music:=0;
  if i and 16 = 16 then highres:=1  else highres:=0;
  if i and  8 =  8 then overscan:=1 else overscan:=0;
  player:=i and 7;
end;

procedure DISPLAY_ANSI;
var f:file;
    buffer:array[1..4000] of byte;
    b:byte;
    z:word;
    mfm:word;
begin
  mfm:=filemode;
  filemode:=0;
  assign(f,'menu\menu.end');
  reset(f,1);
  blockread(f,buffer,4000,z);
  close(f);
  filemode:=mfm;
  for z:=1 to 4000 do mem[$b800:z-1]:=buffer[z];
end;

function IntToStr(I: Longint): String;
var
  Stri: string[11];
begin
  Str(I, Stri);
  IntToStr := Stri;
end;

procedure Go_out_of_here;
begin
  for i:=1 to 2 do FreeMem(minis[i], 82*151);
  video_mode(3);
{STANDART-DOS-FONT}
 (* asm
  push ax
  push es
  push bp
  {}
  mov ax,1130h; mov bh,1; int 10h; {mov Fseg,es; mov Fofs,bp;}
  push es
  push bp
  {}
  mov ax,1100h
  mov bx,1000h
  mov cx,0100h
  mov dx,0000h
  pop es
  pop bp
  lea bp,[es:bp]
  int 10h
  {}
  pop bp
  pop es
  pop ax
  end;*)
  Display_ansi;
  gotoxy(1,16);
end;

*/
void set_menu_color()
{
  int i;
  retrace_whole();
  for (i=1; i <= 6;i++) set_rgb_color(menucolor[i],37,0,0);

  if (highres==1) set_rgb_color(kreuz[1],23,40,10);
  else set_rgb_color(kreuz[1],0,0,32);

  if (overscan==1) set_rgb_color(kreuz[2],23,40,10);
  else set_rgb_color(kreuz[2],0,0,32);

  if (music==1) set_rgb_color(kreuz[3],23,40,10);
  else set_rgb_color(kreuz[3],0,0,32);

  for (i=1; i<= 6;i++) set_rgb_color(playercolor[i],20,0,0);
  set_rgb_color(playercolor[player],37,0,0);
  if (auswahl == 5) set_rgb_color(playercolor[player],23,40,10);
}

void load_mini(byte mininr, char *fname)
{
	char temp[256];
	FILE *f;

	strcpy(temp, DATA_PREFIX);
	strcat(temp, fname);
	strcat(temp, ".MIN");

	f = fopen(temp, "rb");
	if (!f) fatal_error("Unable to load file '%s'", fname);

	fread(minis[mininr], 82 * 151, 1, f);
	fclose(f);
}

void show_menu1()
{
  palette_black();
  load_palette("MENU/menu1");
  load_mini_palette("MENU/menu2");
  set_menu_color();
  set_rgb_color(menucolor[auswahl],0,60,0);
  SET_PAL0();
  load_vga2("MENU\\MENU1");
  put_sprite2(212,24,82,150,minis[tables]);
}


void init_alles()
{
  tables = 1;
  minis[1] = malloc(82*151);
  minis[2] = malloc(82*151);
  load_mini(1,"MENU/MENU1");
  load_mini(2,"MENU/MENU2");
  ende = FALSE;
//  video_mode($13);
  show_menu1();
//  palette_black();
//  load_palette("MENU/menu1");
//  load_mini_palette("MENU/menu2");
  set_rgb_color(rahmen1,0,0,0);
  set_rgb_color(rahmen2,0,0,0);
  set_rgb_color(kreuz[1],0,0,32);
  set_rgb_color(kreuz[2],63,63,0);
  set_rgb_color(kreuz[3],63,63,0);
//  SET_PAL0();
//  load_vga2("MENU/menu1");
  auswahl=1;
//  params:=paramstr(2);
//  decode_paras;
/*{  tables:=0;
  highres:=0;
  music:=1;
*/
  player=1;
/*
  overscan:=1;}*/
  auswahl=1;
//  params:=paramstr(2);
//  decode_paras;
//  set_menu_color();
//  set_rgb_color(menucolor[auswahl],0,60,0);
//  put_sprite2(212,24,82,150,minis[tables]);
}



void starte_flipper()
{
	cls();

	switch(tables)
	{
	case 1: flipper1(); break;
//	case 2: flipper2(); break;
	}

	cls();
	show_menu1();
}
/*
var paras:string;
begin
  for i:=1 to 2 do FreeMem(minis[i], 82*151);
  video_mode(3);

  i:=0;
  if tables  = 2 then i:=i or 64;
  if music   = 1 then i:=i or 32;
  if highres = 1 then i:=i or 16;
  if overscan= 1 then I:=i or  8;
  i:=I or player;
  halt(i);
end;
*/

void display_help()
{
  SDL_Event event;
  int wait = 1;
  int i;

  palette_black();
  load_palette("MENU\\MENU3");
  load_vga2("MENU\\MENU3");

  while(wait)
  {
	  if (SDL_PollEvent(&event)
		  && (event.type == SDL_KEYDOWN || event.type == SDL_QUIT)
		  ) wait = 0;
  }
  
  show_menu1();
}

int main(int argc, char *argv[])
{
	int a;
	int j;
	SDL_Event event;

	/* Initialize SDL */
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	// Always use these settings
	highres = TRUE;
	overscan = FALSE;

	UseSound = TRUE;
	fullscreen = FALSE;

	for (j = 1; j < argc; j++)
	{
		if (strcmp(argv[j],"-fullscreen") == 0) fullscreen = TRUE;
		else if (strcmp(argv[j],"-nosound") == 0) UseSound = FALSE;
	}

	// Set video mode
    vga320x400Overscan();

	init_alles();

	snapshot();

	while (!ende)
	{
	  do
	  {
         set_menu_color();
         set_rgb_color(menucolor[auswahl],23,40,10);

		  ch = 0;
		  while (ch == 0)
		  {
			if (SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym)
						{
						case SDLK_q:
						case SDLK_ESCAPE:
							ch = 27;
							break;

						case SDLK_SPACE:
							ch = 32;
							break;

						case SDLK_RETURN:
							ch = 13;
							break;

						case SDLK_F1:
							ch = 59;
							break;

						case SDLK_UP:
							ch = 72;
							break;

						case SDLK_DOWN:
							ch = 80;
							break;

						default:
							break;
						}
						break;

					case SDL_QUIT:
						ch = 27;
						break;

					default:
						break;
				}
			}
		  }
          if (ch==59) display_help();
          if (ch==80)
		  {
			  if (auswahl<6) auswahl++; else auswahl=1;
		  }
          if (ch==72)
		  {
			  if (auswahl>1) auswahl--; else auswahl=6;
		  }

		  if (ch == 27) ende = TRUE;
	  } while (!ende && !(ch == 32 || ch == 13));

      if (!ende)
	  {
        if (auswahl==1)
		{
			int i;
			tables = tables ^ 3;
			for (i = 174; i>=24; i--)
			{
				retrace_whole();
//				scroll2(212,24,212,27,82, i-24);
				scroll2(212,24,212,29,82, i-24);
				put_sprite2(212,i,82,174-i,minis[tables]);
				i -=4;
			}
		}
        if (auswahl==2) highres=highres ^ 1;
        if (auswahl==3) overscan=overscan ^ 1;
        if (auswahl==4) music=music ^ 1;
        if (auswahl==5)
		{
			if (player<6) player++;
			else player=1;
		}
        if (auswahl==6) starte_flipper();
	  }
	}

//	go_out_of_here;

	return 0;
}

