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

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "globals.h"
#include "video.h"
#include "error.h"
#include "loadprc.h"

/**
 * Unscramble bitmaps to be linear
 *
 * Currently they are stored for the vga unchained mode
 * which means they are stored in rows for each plane.
 */

static void vga_unchain_to_flat(byte *data, int width, int height)
{
   int size = width * height;
   byte *tmp = malloc(size);
   byte *from = tmp;
   int run_size = width/4;
   int loop, x, y;

   memcpy(tmp, data, size);

   for (loop = 0; loop < 4; loop++)
   {
      for (y = 0; y < height; y++)
      {
          for (x = 0; x < run_size; x++)
          {
             data[y*width + loop + (x << 2)] = *from++;
          }
      }
   }

   free(tmp);
}

void init_colors()
{
	int x;
	FILE *f;
	char temp[3];
	char fname[256];

	sprintf(fname, DATA_PREFIX "tisch%c/t%c-pal.pal",tnr, tnr);
	f = fopen(fname, "rb");
	if (f == NULL) fatal_error("Can't open palette file '%s'", fname);

	for (x = 0; x <=255;x++)
	{
		fread(temp, 3, 1, f);
		pal[x].r = temp[0];
		pal[x].g = temp[1];
		pal[x].b = temp[2];
	}

	fclose(f);

  set_rgb_color(led_color_1,20,0,0);
  pal[led_color_1].r = 20;
  pal[led_color_1].g = 0;
  pal[led_color_1].b = 0;
  set_rgb_color(led_color_2,255,0,0);
  pal[led_color_2].r = 255;
  pal[led_color_2].g = 0;
  pal[led_color_2].b = 0;

  if (tnr=='1')
  {
    set_rgb_color(0x10,33,33,33);
    pal[0x10].r = 33; pal[0x10].g = 33; pal[0x10].b = 33;
    set_rgb_color(0x11,35,35,35);
    pal[0x11].r = 35; pal[0x11].g = 35; pal[0x11].b = 35;
    set_rgb_color(0x12,37,37,37);
    pal[0x12].r = 37; pal[0x12].g = 37; pal[0x12].b = 37;
    set_rgb_color(0x13,39,39,39);
    pal[0x13].r = 39; pal[0x13].g = 39; pal[0x13].b = 39;
    set_rgb_color(0x14,42,42,42);
    pal[0x14].r = 42; pal[0x14].g = 42; pal[0x14].b = 42;
    set_rgb_color(0x15,52,52,52);
    pal[0x15].r = 52; pal[0x15].g = 52; pal[0x15].b = 52;
    set_rgb_color(0x19,33,33,33);
    pal[0x19].r = 33; pal[0x19].g = 33; pal[0x19].b = 33;
  }

  if (tnr=='2')
  {
    set_rgb_color(170,33,33,33);
    pal[170].r = 33; pal[170].g = 33; pal[170].b = 33;
    set_rgb_color(171,35,35,35);
    pal[171].r = 35; pal[171].g = 35; pal[171].b = 35;
    set_rgb_color(172,37,37,37);
    pal[172].r = 37; pal[172].g = 37; pal[172].b = 37;
    set_rgb_color(173,39,39,39);
    pal[173].r = 39; pal[173].g = 39; pal[173].b = 39;
    set_rgb_color(174,42,42,42);
    pal[174].r = 42; pal[174].g = 42; pal[174].b = 42;
    set_rgb_color(175,52,52,52);
    pal[175].r = 52; pal[175].g = 52; pal[175].b = 52;
    set_rgb_color(176,33,33,33);
    pal[176].r = 33; pal[176].g = 33; pal[176].b = 33;
  }

  for (x = 255; x >= 0; x--) set_rgb_color(x,pal[x].r,pal[x].g,pal[x].b);
}

void load_table_tab()
{
	FILE *f;
	int y;
	byte temp[320];
	int t;

  for (t = 0; t <= 255; t++) set_rgb_color(t,0,0,0);
  sprintf(temp, DATA_PREFIX "tisch%c/t%c-tab.org", tnr, tnr);
  f = fopen(temp, "rb");

  if (!f) fatal_error("Can't load table file '%s'", temp);

  for (y = 0; y < 600; y++)
  {
	  pointer screen_line = get_screen_line(y);
	  fread(screen_line, 320,1, f);
	  //fread(temp, 320,1, f);
      //for (x = 0; x <= 319; x++) put_pixel(x,y,temp[x]);
  }
  fclose(f);
}

void load_table_gro()
{
	FILE *f;
	word y;
	char fname[256];

  sprintf(fname, DATA_PREFIX "tisch%c/t%c-gro.clb", tnr, tnr);
  f = fopen(fname, "rb");
  if (!f) fatal_error("Can't load table gorund file '%s'", fname);

  for (y=0; y <= 199; y++)
  {
	  fread(tableground1a + 320 * y, 320, 1, f);
  }

  for (y=200; y<=399;y++)
  {
	  fread(tableground2a + 320 * (y-200), 320, 1, f);
  }

  for (y=400; y <= 599; y++)
  {
	  fread(tableground3a + 320 * (y-400), 320, 1, f);
  }
  fclose(f);
}

void load_arm_links()
{
  FILE *f;
  int x;

  f = fopen(DATA_PREFIX "gfx/arml1.gfx", "rb");
  if (!f) fatal_error("Unable to load left flipper graphics 'gfx/arml1.gfx'");

  fread(arm_links, ArmBreiteLinks*ArmHoeheLinks*5, 1, f);
  fclose(f);

  if (tnr=='1')
  {	  
    for (x = 1; x <= 15360; x++)
	{
      switch(arm_links[x])
	  {
		case 0x00 : arm_links[x] = 234; break;
		case 0x05 : arm_links[x] = 31; break;
		case 0x13 : arm_links[x] = 32; break;
		case 0xFF : arm_links[x] = 33; break;
	  }
	}
  }
  if (tnr=='2')
  {
    for (x = 1; x <= 15360; x++)
	{
      switch(arm_links[x])
	  {
		case 0x00 : arm_links[x] = 0; break;
		case 0x05 : arm_links[x] = 0; break;
		case 0x13 : arm_links[x] = 12; break;
		case 0xFF : arm_links[x] = 11; break;
	  }
	}
  }

  for (x = 0; x < 5; x++)
  {
     vga_unchain_to_flat(arm_links + x * ArmBreiteLinks*ArmHoeheLinks, ArmBreiteLinks, ArmHoeheLinks);
  }

}

void load_arm_rechts()
{
  FILE *f;
  int x;

  f = fopen(DATA_PREFIX "gfx/armr1.gfx", "rb");
  if (!f) fatal_error("Unable to load right flipper graphics 'gfx/armr1.gfx'");

  fread(arm_rechts, ArmBreiteRechts*ArmHoeheRechts*5, 1, f);
  fclose(f);

  if (tnr=='1')
  {	  
    for (x = 1; x <= 15360; x++)
	{
      switch(arm_rechts[x])
	  {
		case 0x00 : arm_rechts[x] = 234; break;
		case 0x05 : arm_rechts[x] = 31; break;
		case 0x13 : arm_rechts[x] = 32; break;
		case 0xFF : arm_rechts[x] = 33; break;
	  }
	}
  }
  if (tnr=='2')
  {
    for (x = 1; x <= 15360; x++)
	{
      switch(arm_rechts[x])
	  {
		case 0x00 : arm_rechts[x] = 0; break;
		case 0x05 : arm_rechts[x] = 0; break;
		case 0x13 : arm_rechts[x] = 12; break;
		case 0xFF : arm_rechts[x] = 11; break;
	  }
	}
  }

  for (x = 0; x < 5; x++)
  {
     vga_unchain_to_flat(arm_rechts + x * ArmBreiteRechts*ArmHoeheRechts, ArmBreiteRechts, ArmHoeheRechts);
  }
}

void load_arm_links_msk()
{
	FILE *f = fopen(DATA_PREFIX "msk/arml1.msk", "rb");
	if (!f) fatal_error("Can't open left flipper mask 'msk/arml1.msk'");
	fread(arm_links_msk,ArmBreiteLinks*ArmHoeheLinks*5, 1, f);
    fclose(f);
}

void load_arm_rechts_msk()
{
	FILE *f = fopen(DATA_PREFIX "msk/armr1.msk", "rb");
	if (!f) fatal_error("Can't open right flipper mask 'msk/armr1.msk'");
	fread(arm_rechts_msk,ArmBreiteRechts*ArmHoeheRechts*5, 1, f);
    fclose(f);
}


void load_ball()
{
	char fname[256];
	FILE *f;
	sprintf(fname, DATA_PREFIX "gfx/ball%c.gfx", tnr);
	f = fopen(fname, "rb");
	if (!f) fatal_error("Can't open ball graphics '%s'", fname);
	fread(ball,256,1,f);
	fclose(f);

        vga_unchain_to_flat(ball, 16,16);
}

void load_feder()
{
	FILE *f;
	char fname[256];

	sprintf(fname, DATA_PREFIX "gfx/feder%c.gfx", tnr);
	f = fopen(fname, "rb");
	if (!f) fatal_error("Can't load feder '%s'", fname);

	fread(feder, 480,1, f);
	fclose(f);
}

void load_palette(const char *fname)
{
  FILE *palfile;
  int i;
  char temp[256];

  strcpy(temp, DATA_PREFIX);
  strcat(temp, fname);
  if (strrchr(fname,'.') == NULL) strcat(temp, ".pal");

  palfile = fopen(temp, "rb");
  if (!palfile) fatal_error("Can't load palette '%s'", temp);

  for (i=0; i <= 255;i++)
  {
	  fread(&pal[i].r, 1,1, palfile);
	  fread(&pal[i].g, 1,1, palfile);
	  fread(&pal[i].b, 1,1, palfile);
  }
  fclose(palfile);

//  {kleine eigenm„chtige manipulation}
  set_rgb_color(0,0,0,0);
  for (i=1; i<= 255; i++)
  {
	  set_rgb_color(i, pal[i].r, pal[i].g, pal[i].b);
  }
}

void load_mini_palette(char *fname)
{
	FILE *palfile;
	byte colnr;
	char path[256];
	strcpy(path, DATA_PREFIX);
	strcat(path, fname);

	if (strchr(fname, '.') == 0) strcat(path, ".mpa");
	palfile = fopen(path, "rb");
	if (!palfile) fatal_error("Can't load mini palette file '%s'", path);
	do
	{
		if (!feof(palfile)) fread(&colnr, 1,1, palfile);
		if (!feof(palfile)) fread(&pal[colnr].r, 1,1, palfile);
		if (!feof(palfile)) fread(&pal[colnr].g, 1,1, palfile);
		if (!feof(palfile)) fread(&pal[colnr].b, 1,1, palfile);
		set_rgb_color(colnr,pal[colnr].r,pal[colnr].g,pal[colnr].b);
	} while (!feof(palfile));
	fclose(palfile);
}

/*
 * Load font file
 */
void load_font()
{
	FILE *f = fopen(DATA_PREFIX "FONTS/BLCKSNSF.FNT", "rb");
	int size;

	if (!f) fatal_error("Can't open font file 'FONTS/BLCKSNSF.FNT'");

	fseek(f, 0, SEEK_END);
	size = (int)ftell(f);
	fseek(f, 0, SEEK_SET);

	font = malloc(size);
	fread(font, size, 1, f);
	fclose(f);
}
