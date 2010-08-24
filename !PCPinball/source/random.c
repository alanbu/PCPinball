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


#include "globals.h"
#include <stdlib.h>
#include <time.h>

byte random255()
{
	return ran255[ran255z++];
}
void init_random()
{
	int j;
	srand((unsigned int)time(0));
    for (j =0; j <= 255; j++)
	{
      ran255[j] = (byte)(rand() & 0xFF);
	}
}

/**
 * Return a random number from 0 to val -1
 */
int random_to(int val)
{
	return rand() % val;
}
