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

// Video routines

void vga320x400Overscan();
void set_rgb_color(byte co, byte r, byte g, byte b);
void palette_black();
void retrace();
void retrace_whole();

void cls();

void setaddress(word ad);
void put_pixel(word x, word y, byte col);
byte get_pixel(word x, word y);

void set_feder();

void set_sprite(int x, int y, int breite, int hoehe, pointer sprite);
void get_sprite(int x, int y, int breite, int hoehe, pointer sprite);
void put_sprite(int x, int y, int breite, int hoehe, pointer sprite);
void put_sprite2(int x, int y, int breite, int hoehe, pointer sprite);

void put_pixel_led(word x, word y, byte color);

void draw_arm_links();
void draw_arm_rechts();

void flip_led(boolean pause);

pointer get_screen_line(int y);

void load_vga(const char *fname);
void load_vga2(const char *fname);
void load_bmp(const char *fname);

void scroll(word x, word y, word x1, word y1, word b, word h);
void scroll2(word x, word y, word x1, word y1, word b, word h);

void snapshot();
