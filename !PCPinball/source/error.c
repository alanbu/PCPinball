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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#endif

static void show_error(const char *msg)
{
#ifdef _WIN32
	MessageBox(NULL, msg, "ERROR", MB_ICONHAND);
#else
	fprintf(stderr, msg);
#endif
	exit(4);
}


void fatal_error(const char *fmt, ...)
{
   char temp[2048];
   va_list marker;

   va_start( marker, fmt );     /* Initialize variable arguments. */
   vsprintf(temp, fmt, marker);
   va_end(marker);
   show_error(temp);
}
