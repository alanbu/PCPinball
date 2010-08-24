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

/* Main entry point and between game processing
 *
 * Original code by Alan Buckley - Feb 2008
 *
 * Replace original menu by going straight into
 * table and giving Play Again message when game
 * is finished
 *
 ************************************************/

#include "globals.h"
#include "loadprc.h"
#include "video.h"
#include "error.h"
#include "common.h"
#include "ledanz.h"
#include "init_close.h"
#include "keys.h"

#include <SDL.h>

// tables
extern void flipper1();

int main(int argc, char *argv[])
{
	SDL_Event event;
    int again = 1;
	int doinput;
	int j;
	SDL_Surface *endQ;
	SDL_Rect endPos;
	int hiscore = 0;

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

	SDL_WM_SetCaption("PC Spiel Pinball", NULL);

	SDL_ShowCursor(0);

	endQ = SDL_LoadBMP(DATA_PREFIX "GFX/end.bmp");

	endPos.x = (SDL_GetVideoSurface()->w - endQ->w)/2;
	endPos.y = (SDL_GetVideoSurface()->h - endQ->h)/2;

    init_globals();

	// Main loop
	do
	{
		int dispStage = 2;
		int dispStart = 2;
		int pauseCount = 0;
		Uint32 ticks = SDL_GetTicks();

		flipper1();

		// Bring the arms down
		clear_keys();
		senk_arms();
		retrace();

		if (score[1] > hiscore)
		{
			dispStage = 0;
			dispStart = 0;
			hiscore = score[1];
		}

		SDL_BlitSurface(endQ, NULL, SDL_GetVideoSurface(), &endPos);
		SDL_Flip(SDL_GetVideoSurface());

		doinput = 1;
		while (doinput)
		{
			if (SDL_PollEvent(&event))
			{
				doinput = 1;
				switch(event.type)
				{
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym)
						{
						case SDLK_y:
							doinput = 0;
							break;

						case SDLK_n:
						case SDLK_q:
						case SDLK_ESCAPE:
							again = 0;
							doinput = 0;
							break;
						}
						break;

					case SDL_QUIT:
						again = 0;
						doinput = 0;
						break;

					default:
						break;
				}
			}

			if (SDL_GetTicks() - ticks > 100)
			{
				ticks = SDL_GetTicks();

				if (led_status) led_anzeige(TRUE);
				else
				{
					switch(dispStage)
					{
					case 0:
						display("A NEW HIGH SCORE!!!");
						dispStage++;
						pauseCount = 20;
						break;

					case 1:
						if (--pauseCount == 0) dispStage++;
						break;

					case 2:
						display(int_msg("SCORE %d", score[1]));
						dispStage++;
						pauseCount = 20;
						break;

					case 3:
						if (--pauseCount == 0) dispStage++;
						break;

					case 4:
						display(int_msg("HIGH SCORE %d", hiscore));
						dispStage++;
						pauseCount = 20;
						break;

					case 5:
						if (--pauseCount == 0) dispStage=dispStart;
						break;
					}
				}
			}
		}

		if (again)
		{
			display("RESTARTING");
			while (led_status) led_anzeige(TRUE);
		}
		
	} while(again);

	SDL_FreeSurface(endQ);
	
	return 0;
}
