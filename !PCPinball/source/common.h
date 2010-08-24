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

// Routines common to both tables

/* Helper to put an integer in a message */
char *int_msg(const char *fmt, int num);
void senk_arms();
void move_left();
void incscore(word points);
void display(const char *t);
void move_ball();
void calc_page_pos_of_ballpos();
void check_flipper_arms();
byte gettablepixel(word x, word y);
void check_ball_oben();
void check_ball_unten();
void check_ball_links();
void check_ball_rechts();
