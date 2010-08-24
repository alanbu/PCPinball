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

#include <SDL.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "video.h"
#include "error.h"

// Final display surface
static SDL_Surface *real_screen;
static struct SDL_Surface *screen;

/* Display throttling timer */
static Uint32 last_frame_start = 0;
/* Minimum length of time between frames - 20 = 1000/20 = 50 FPS */
#define FRAME_DELAY 20

// Section of larger internal screen to copy
// to the display
// Low res settings
//static SDL_Rect visible_rect = {0,24,320,200 - 24};
//static SDL_Rect visible_led = {0,0, 320, 24};
static SDL_Rect visible_rect = {0,24,320,400 - 24};
static SDL_Rect visible_led = {0,0, 320, 24};

// LED display area of virtual screen
// Low res settings
//static SDL_Rect led_rect = {0,200-24,320,24};
//static SDL_Rect table_rect = {0,0,320,200-24};
static SDL_Rect led_rect = {0,400-24,320,24};
static SDL_Rect table_rect = {0,0,320,400-24};

//Setting unchained mode where 4 pages of memory are used.
//In this mode plane 0 has pixel 0,4,8 etc
//             plane 1 has pixels 1,5,9 etc etc
//
///* VGA sequence controller */
//#define SC_INDEX      0x03c4
//#define SC_DATA       0x03c5
//
///* VGA CRT controller */
//#define CRTC_INDEX    0x03d4
//#define CRTC_DATA     0x03d5
//
//#define MEMORY_MODE   0x04
//#define UNDERLINE_LOC 0x14
//#define MODE_CONTROL  0x17
//
//...
//
///* turn off chain-4 mode */
//outp(SC_INDEX, MEMORY_MODE);
//outp(SC_DATA, 0x06);
//
///* TODO: Insert code to clear the screen here.
//   (the BIOS only sets every fourth byte
//   to zero -- the rest needs to be set to
//   zero, too) */
//
///* turn off long mode */
//outp(CRTC_INDEX, UNDERLINE_LOC);
//outp(CRTC_DATA, 0x00);
///* turn on byte mode */
//outp(CRTC_INDEX, MODE_CONTROL);
//outp(CRTC_DATA, 0xe3);
//
//**** Selecting the plane
//
//plane = (x&3);
///* select the map mask register */
//outp(0x3c4, 0x02);
///* write 2^plane */
//outp(0x3c5, 1 << plane);


/*procedure video_mode(mode:byte);
begin
      asm
        mov  AH,00
        mov  AL,mode
        int  10h
      end;
end;
*/


void vga320x400Overscan()
{
/* Virtual screen seen by plotting routines */
	screen = SDL_CreateRGBSurface(SDL_SWSURFACE, 320,648,8, 0,0,0,0);
	if (screen == NULL)
	{
		fprintf(stderr, "Unable to create virtual surface");
		exit(3);
	}

/* Actual visible screen */
	if (fullscreen)
	{
		// Table looks better in a larger screen
		real_screen = SDL_SetVideoMode(640,480,8,SDL_HWSURFACE|SDL_FULLSCREEN);

		// Centre table in screen
		led_rect.x = 160; led_rect.y += 40;
		table_rect.x = 160; table_rect.y += 40;
	} else
		real_screen = SDL_SetVideoMode(320,400,8,SDL_SWSURFACE);

	if (real_screen == NULL)
	{
		fprintf(stderr, "Unable to create video surface");
		exit(3);
	}


/*; assembler;
asm
  mov ax,0013h     { mode 13h, 320x200x256x1 }
  int 10h
  mov dx,03c4h
  mov ax,0604h     { unchain stuff: 320x200x256x4 }
  out dx,ax
  mov ax,0f02h
  out dx,ax

  mov cx,320*200
  mov es,vseg
  xor ax,ax
  mov di,ax
  rep stosw
  mov dx,03d4h

{ set 'overscan' mode here! }
  cmp overscan,0
  jz @@weiter1
    mov dx,03c2h
    mov al,10100011b
    out dx,al
  @@weiter1:

  mov dx,03d4h

  cmp highres,0
  jz @@weiter2
    mov ax,4009h
    out dx,ax
  @@weiter2:

  mov ax,0014h
  out dx,ax
  mov ax,0e317h
  out dx,ax{}
end;
*/
}

/**
 * get pointer to start of screen line.
 */
pointer get_screen_line(int y)
{
	return ((pointer)screen->pixels) + (y+24) * 320;
}

/**
 * Set palette colours.
 *
 * Colours in game are from 0 to 127 (I think)
 */

void set_rgb_color(byte co, byte r, byte g, byte b)
{
	SDL_Color col;
	col.r = (r << 2);
	col.g = (g << 2);
	col.b = (b << 2);

	SDL_SetColors(real_screen, &col, co, 1);
	SDL_SetColors(screen, &col, co, 1);
}

void palette_black()
{
	int x;

	for (x=0; x<=255;x++) set_rgb_color(x,0,0,0);
}

/**
 * Ensure game isn't too fast on fast machines.
 */

void end_of_frame_pause()
{
    Uint32 now = SDL_GetTicks();
    if (now >= last_frame_start && now < last_frame_start + FRAME_DELAY)
    {
		 last_frame_start += FRAME_DELAY;
		 if (last_frame_start < now)
		 {
			 // Time has wrapped so just have one slow frame
			 if (last_frame_start > 0) SDL_Delay(FRAME_DELAY);
		 } else
			 SDL_Delay(last_frame_start - now);
	} else
		last_frame_start = now;
}

/**
 * Use calls to retrace to update the actual screen.
 *
 * (originally used to wait for the vertical sync.
 */
void retrace()
{
	// Led is blitted in flip_led when it changes

	// Table
	SDL_BlitSurface(screen, &visible_rect, real_screen, &table_rect);

	// Update whole screen
	SDL_UpdateRect(real_screen, table_rect.x, table_rect.y, table_rect.w, table_rect.h);

	end_of_frame_pause();
}

/**
 * retrace whole screen from top of back buffer
 */
void retrace_whole()
{
	// Table
	SDL_BlitSurface(screen, NULL, real_screen, NULL);

	// Update whole screen
	SDL_UpdateRect(real_screen, 0,0, 320,400);

	end_of_frame_pause();
}

/**
 * Set top displayed line of screen.
 *
 */
void setaddress(word y)
{
	if (y > 224) y = 224;
	visible_rect.y = y + 24;
}

/* This sets the split screen position
procedure setlinecomp(ad:word); assembler;
asm
  mov dx,3d4h
  mov al,18h
  mov ah,[byte(ad)]
  out dx,ax
  mov al,7
  out dx,al
  inc dx
  in al,dx
  dec dx
  mov ah,[byte(ad)+1]
  and ah,00000001b
  shl ah,4
  and al,11101111b
  or al,ah
  mov ah,al
  mov al,7
  out dx,ax
  mov al,9
  out dx,al
  inc dx
  in al,dx
  dec dx
  mov ah,[byte(ad)+1]
  and ah,00000010b
  shl ah,5
  and al,10111111b
  or al,ah
  mov ah,al
  mov al,9
  out dx,ax
end;

procedure Getfont;

begin
  FSeg := Seg(Font);
  FOfs := Ofs(Font);
end;
*/

void cls()
{
	SDL_FillRect(screen, NULL, 0);
	SDL_FillRect(real_screen,NULL, 0);
	SDL_UpdateRect(real_screen,0,0,0,0);
}

/*
procedure clsvir; assembler; asm
  mov es,vseg; mov di,0; mov cx,8000; mov dx,3c4h
  mov ax,0f02h; out dx,ax; xor ax,ax; rep stosw;
end;

procedure flip(src,dst:word); assembler; asm { copy virt scr to visual scr }
  push ds; mov es,[dst]; mov ds,[src]; xor si,si
  xor di,di; mov cx,32000; rep movsw; pop ds;
end;
*/
/*procedure flip_led_1;
var z:byte;
    z1,z2:word;
begin
  for z:=0 to 22 do
  begin
    z1:=z*80;
    z2:=z*160;
    asm
      push si
      push di
      push ds
      push es
      push cx
      push dx

      mov dx,03c4h
      mov ax,258
      out dx,ax
      mov es,[vseg]   {ES:DI}
      mov ds,[ledseg] {DS:SI}
      mov si,z1
      mov di,z2
      mov cx,40
      rep movsw {}

      pop dx
      pop cx
      pop es
      pop ds
      pop di
      pop si
    end;
  end;
end;

procedure flip_led_2;
var z:byte;
    z1,z2:word;
begin
  for z:=0 to 22 do
  begin
    z1:=z*80+1920;
    z2:=z*160;
    asm
      push si
      push di
      push ds
      push es
      push cx
      push dx

      mov dx,03c4h
      mov ax,1026
      out dx,ax
      mov es,[vseg]   {ES:DI}
      mov ds,[ledseg] {DS:SI}
      mov si,z1
      mov di,z2
      mov cx,40
      rep movsw {}

      pop dx
      pop cx
      pop es
      pop ds
      pop di
      pop si
    end;
  end;
end;
*/
void flip_led(boolean pause)
{
  int x,y;
  pointer line = ((byte *)screen->pixels);
  pointer led_line = led_display;

  for (y = 0; y <= 22; y++)
  {
	for (x = 0; x < 160; x++)
	{
	  line[x*2] = led_line[x];
	}
    line += 640;
    led_line += 160;
  }
  led_status =0;

  // Blit led
  SDL_BlitSurface(screen, &visible_led, real_screen, &led_rect);
  SDL_UpdateRect(real_screen, led_rect.x, led_rect.y, led_rect.w, led_rect.h);

  if (pause) end_of_frame_pause();
}

void set_sprite(int x, int y, int breite, int hoehe, pointer sprite)
{
	pointer screen_ptr;
	int row;

	y += 24;
	screen_ptr = (pointer)screen->pixels + ((y << 8) + (y << 6) + x);

	for (row = 0; row < hoehe; row++)
	{
		memcpy(screen_ptr, sprite, breite);
		screen_ptr+=320;
		sprite += breite;
	}
}

void get_sprite(int x, int y, int breite, int hoehe, pointer sprite)
{
	pointer screen_ptr;
	int row;

	y += 24;
	screen_ptr = (pointer)screen->pixels + ((y << 8) + (y << 6) + x);
	for (row = 0; row < hoehe; row++)
	{
		memcpy(sprite, screen_ptr, breite);
		screen_ptr+=320;
		sprite += breite;
	}
}

/**
 * Put sprite to screen and immediately update
 * real screen.
 */
void put_sprite(int x, int y, int breite, int hoehe, pointer sprite)
{
	pointer screen_ptr;
	SDL_Rect pos;
	int row;

	screen_ptr = (pointer)screen->pixels + ((y << 8) + (y << 6) + x);

	for (row = 0; row < hoehe; row++)
	{
		memcpy(screen_ptr, sprite, breite);
		screen_ptr+=320;
		sprite += breite;
	}

	pos.x = x;
	pos.y = y;
	pos.w = breite;
	pos.h = hoehe;

	SDL_BlitSurface(screen, &pos, real_screen, &pos);
	SDL_UpdateRects(real_screen, 1, &pos);
}


/**
 * Put sprite to screen and immediately update
 * real screen.
 * This version double y co-ordinate and height
 */
void put_sprite2(int x, int y, int breite, int hoehe, pointer sprite)
{
	pointer screen_ptr;
	SDL_Rect pos;
	int row;

	y <<=1;
	screen_ptr = (pointer)screen->pixels + ((y << 8) + (y << 6) + x);

	for (row = 0; row < hoehe; row++)
	{
		memcpy(screen_ptr, sprite, breite);
		screen_ptr+=320;
		memcpy(screen_ptr, sprite, breite);
		screen_ptr+=320;
		sprite += breite;
	}

	pos.x = x;
	pos.y = y;
	pos.w = breite;
	pos.h = hoehe<<1;

	SDL_BlitSurface(screen, &pos, real_screen, &pos);
	SDL_UpdateRects(real_screen, 1, &pos);
}

void draw_arm_links_msk()
{
  word z1,z2,z5;
  pointer z6,z7;
  int x,y;
  word z8,z9,z10;

  z8=ArmBreiteLinks*ArmHoeheLinks;
  z9=(ArmBreiteLinks / 8) * 2;
  z10=ArmHoeheLinks;
  z5=(arm_links_status-1)*z8;
  z6=tableground3a;
  z7=arm_links_msk;

  for (x=0; x <= ArmBreiteLinks-1;x++)
  {
//	  int ax = (x + ArmXLinks) * 200;
//      z1 = ax + ArmYLinks - 399;
	  z1 = (ArmYLinks - 399) * 320 + ArmXLinks + x;
	  z2 = x * ArmHoeheLinks + z5;
	  for (y = 0; y < ArmHoeheLinks; y++)
	  {
		  *(z6+z1+y*320) = *(z7 + z2 + y);
	  }
  }

}

void draw_arm_rechts_msk()
{
  word z1,z2,z5;
  pointer z6,z7;
  int x, y;
  word z8,z9,z10;

  z8=ArmBreiteRechts*ArmHoeheRechts;
  z9=(ArmBreiteRechts / 8) * 2;
  z10=ArmHoeheRechts;
  z5=(arm_rechts_status-1)*z8;
  z6=tableground3a;
  z7=arm_rechts_msk;

  for (x=0; x <= ArmBreiteRechts-1;x++)
  {
//	  int ax = (x + ArmXRechts) * 200;
//      z1 = ax + ArmYRechts - 399;
	  z1 = (ArmYRechts - 399) * 320 + ArmXRechts+ x;
	  z2 = x * ArmHoeheRechts + z5;
	  for (y = 0; y < ArmHoeheLinks; y++)
	  {
		  *(z6+z1+y*320) = *(z7 + z2 + y);
	  }
  }

 /*TODO: Remove this
 {
	  static int save = 1;
	  if (save)
	  {
		  SDL_Surface *test1 = SDL_CreateRGBSurface(SDL_SWSURFACE, 320,200,8, 0,0,0,0);
		  SDL_Surface *test = SDL_DisplayFormat(test1);
		  memcpy(test->pixels, tableground3a, 320 * 200);

		  SDL_SaveBMP(test, "c:\\tb.bmp");
		  SDL_FreeSurface(test);
		  SDL_FreeSurface(test1);
		  save = 0;
	  }
  }
*/
}

void draw_arm_links()
{
	word SpriteOffs;

	SpriteOffs=(arm_links_status-1)*ArmBreiteLinks*ArmHoeheLinks;
	set_sprite(ArmXLinks,ArmYLinks,ArmBreiteLinks,ArmHoeheLinks, arm_links + SpriteOffs);

    draw_arm_links_msk();
}

void draw_arm_rechts()
{
  word SpriteOffs;

  SpriteOffs=(arm_rechts_status-1)*ArmBreiteRechts*ArmHoeheRechts;
  set_sprite(ArmXRechts,ArmYRechts,ArmBreiteRechts,ArmHoeheRechts,arm_rechts + SpriteOffs);
  draw_arm_rechts_msk();
}

/**
 * put pixel on virtual screen.
 */
 
void put_pixel(word x, word y, byte col)
{
	*((byte *)screen->pixels + (y << 8) + (y << 6) + x) = col;
}

/**
 * get pixel from virtual screen.
 */
byte get_pixel(word x, word y)
{
	return *(((byte *)screen->pixels + (y << 8) + (y << 6) + x));
}

void put_pixel_led(word x, word y, byte color)
{
	led_display[x + y * 160]=color;
}

void set_feder()
{
  word x,y,z;
  z = 0;

  for (y=400+205; y <= 400+205+(StartPow / 5); y++)
  {
    for (x =FederX; x < FederX+FederBreite;x++)
      put_pixel(x,y,0);
  }
  for (y=FederY; y < FederY+FederHoehe; y++)
  {
    for (x=FederX; x <FederX+FederBreite;x++)
	{
      put_pixel(x,y,feder[z]);
      z++;
	}
  }
}


void load_vga(const char *fname)
{
	FILE *f;
	char temp[256];
	strcpy(temp, DATA_PREFIX);
	strcat(temp, fname);
	strcat(temp, ".VGA");

	f = fopen(temp, "rb");
	if (!f) fatal_error("Can't load vga file '%s'", temp);

	fread(screen->pixels, 64000,1, f);
	fclose(f);

	retrace_whole();
}

void load_vga2(const char *fname)
{
	FILE *f;
	char temp[256];
	char *start = screen->pixels;
	char *to = start + 320 * 400;
	char *from = start + 320 * 200;

	strcpy(temp, DATA_PREFIX);
	strcat(temp, fname);
	strcat(temp, ".VGA");

	f = fopen(temp, "rb");
	if (!f) fatal_error("Can't load vga file '%s'", temp);

	fread(screen->pixels, 64000,1, f);
	fclose(f);
	
	do
	{
		to -= 320;
		from -= 320;
		memcpy(to, from, 320);
		to-=320;
		memcpy(to, from, 320);
	} while (from != start);

	retrace_whole();
}

void load_bmp(const char *fname)
{
	SDL_Surface *bmp;
	char temp[256];
	strcpy(temp, DATA_PREFIX);
	strcat(temp, fname);
	strcat(temp, ".bmp");

	bmp = SDL_LoadBMP(temp);
	if (!bmp) fatal_error("Can't load bitmap file '%s'", temp);

	SDL_BlitSurface(bmp, NULL, screen, NULL);
	retrace_whole();
}

void scroll(word x, word y, word x1, word y1, word b, word h)
{
	SDL_Rect from, to;

	from.x = x1;
	from.y = y1;
	from.w = b;
	from.h = h;
	to.x = x;
	to.y = y;
	to.w = b;
	to.h = h;

	SDL_BlitSurface(screen,&from,screen,&to);
	SDL_BlitSurface(real_screen, &from, real_screen, &to);

	if (x1 < x) {from.x = x1; from.w += (x - x1);}
	else from.w += (x1 - x);
	if (y1 < y) {from.y = y1; from.h += (y - y1);}
	else from.h += y1 - y;

	SDL_UpdateRects(real_screen, 1, &from);
}

/**
 * As scroll by y and height is doubled.
 */
void scroll2(word x, word y, word x1, word y1, word b, word h)
{
	SDL_Rect from, to;

	from.x = x1;
	from.y = y1<<1;
	from.w = b;
	from.h = h<<1;
	to.x = x;
	to.y = y<<1;
	to.w = b;
	to.h = h<<1;

	SDL_BlitSurface(screen,&from,screen,&to);
	SDL_BlitSurface(real_screen, &from, real_screen, &to);

	if (x1 < x) {from.x = x1; from.w += (x - x1);}
	else from.w += (x1 - x);
	if (y1 < y) {from.y = y1<<1; from.h += (y - y1)<<1;}
	else from.h += (y1 - y)<<1;

	SDL_UpdateRects(real_screen, 1, &from);
}

void snapshot()
{
	SDL_SaveBMP(real_screen, DATA_PREFIX "screen.bmp");
}
