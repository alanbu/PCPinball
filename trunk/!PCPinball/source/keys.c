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
#include "keys.h"
#include <SDL.h>

static boolean space_pressed = FALSE;
static boolean left_flipper_pressed = FALSE;
static boolean right_flipper_pressed = FALSE;

void clear_keys()
{
  space_pressed = FALSE;
  left_flipper_pressed = FALSE;
  right_flipper_pressed = FALSE;
}

void CTRL_shift_keys()
{
  if (left_flipper_pressed)
  {
      if (arm_links_status<5) arm_links_status++;
  } else
      if (arm_links_status>1) arm_links_status--;

  if (right_flipper_pressed)
  {
      if (arm_rechts_status<5) arm_rechts_status++;
  } else
      if (arm_rechts_status>1) arm_rechts_status--;
}

void keyboard()
{
  SDL_Event event;
  ch = 0;

  while (SDL_PollEvent(&event) && ch == 0)
  {
     switch(event.type)
     {
	    case SDL_KEYDOWN:
	      switch(event.key.keysym.sym)
		  {
		  case SDLK_k: ch = 'k'; break;

		  case SDLK_SPACE:
			  ch = ' ';
			  space_pressed = TRUE;
			  break;

		  case SDLK_q:
		  case SDLK_ESCAPE:
			  ch = 'q';
			  break;

		  case SDLK_LCTRL:
		  case SDLK_LSHIFT:
			  left_flipper_pressed = TRUE;
			  break;

		  case SDLK_RCTRL:
		  case SDLK_RSHIFT:
			  right_flipper_pressed = TRUE;
			  break;

		  case SDLK_y: ch = 'y'; break;
		  case SDLK_n: ch = 'n'; break;
		  }
		  break;

		case SDL_KEYUP:
	      switch(event.key.keysym.sym)
		  {
		  case SDLK_SPACE:
			  space_pressed = FALSE;
			  break;

		  case SDLK_LCTRL:
		  case SDLK_LSHIFT:
			  left_flipper_pressed = FALSE;
			  break;

		  case SDLK_RCTRL:
		  case SDLK_RSHIFT:
			  right_flipper_pressed = FALSE;
			  break;

		  default: // Stop GCC warning
			  break;
		  }
		  break;

		case SDL_QUIT:
			ch = 'q';
			break;

		default: // Stop GCC warning
			break;
	 }
  }
}

boolean spacedown()
{
	return space_pressed;
}
