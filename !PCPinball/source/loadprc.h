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

// Data loading routines

void init_colors();
void load_table_tab();
void load_table_gro();
void load_feder();
void load_arm_links();
void load_arm_rechts();
void load_arm_links_msk();
void load_arm_rechts_msk();
void load_ball();

void load_palette(const char *fname);
void load_mini_palette(char *fname);
void load_font();

