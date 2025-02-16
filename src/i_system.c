#include <SDL2/SDL.h>
#include <sys/stat.h>
#include <errno.h>
#include "h2stdinc.h"
#include "doomdef.h"
#include "soundst.h"

extern void I_StartupMouse(void);
extern void I_ShutdownGraphics(void);

extern int startepisode;
extern int startmap;


/*
============================================================================

					TIMER INTERRUPT

============================================================================
*/

//--------------------------------------------------------------------------
//
// FUNC I_GetTime
//
// Returns time in 1/35th second tics.
//
//--------------------------------------------------------------------------


/* Stolen from Chocolate Doom */
static Uint32 basetime = 0;

int I_GetTime (void)
{
	Uint32 ticks;

	ticks = SDL_GetTicks();

	if (basetime == 0)
		basetime = ticks;

	ticks -= basetime;

	return (ticks * 35) / 1000;  
}


/*
============================================================================

					JOYSTICK

============================================================================
*/

extern int usejoystick;
boolean joystickpresent;

/*
===============
=
= I_StartupJoystick
=
===============
*/

void I_StartupJoystick (void)
{
// NOTHING HERE YET: TO BE IMPLEMENTED IN i_sdl.c
	joystickpresent = false;
}

/*
===============
=
= I_JoystickEvents
=
===============
*/

void I_JoystickEvents (void)
{
}

/*
===============
=
= I_StartFrame
=
===============
*/

void I_StartFrame (void)
{
	I_JoystickEvents();
}


//==========================================================================


/*
===============
=
= I_Init
=
= hook interrupts and set graphics mode
=
===============
*/

void I_Init (void)
{
	if (SDL_Init(0) < 0)
	{
		I_Error("SDL failed to initialize.");
	}

	I_StartupMouse();
	I_StartupJoystick();
	printf("  S_Init... ");
	S_Init();
	S_Start();
}


/*
===============
=
= I_Shutdown
=
= return to default system state
=
===============
*/

void I_Shutdown (void)
{
	S_ShutDown ();
	I_ShutdownGraphics ();
	SDL_Quit ();
}


/*
================
=
= I_Error
=
================
*/

void I_Error (const char *error, ...)
{
	va_list argptr;

	D_QuitNetGame ();
	I_Shutdown ();
	va_start (argptr, error);
	vfprintf (stderr, error, argptr);
	va_end (argptr);
	fprintf (stderr, "\n");
	exit (1);
}

//--------------------------------------------------------------------------
//
// I_Quit
//
// Shuts down net game, saves defaults, prints the exit text message,
// goes to text mode, and exits.
//
//--------------------------------------------------------------------------

static void put_dos2ansi (byte attrib)
{
	byte	fore, back, blink = 0, intens = 0;
	int	table[] = { 30, 34, 32, 36, 31, 35, 33, 37 };

	fore  = attrib & 15;	// bits 0-3
	back  = attrib & 112;	// bits 4-6
	blink = attrib & 128;	// bit 7

	// Fix background, blink is either on or off.
	back = back >> 4;

	// Fix foreground
	if (fore > 7)
	{
		intens = 1;
		fore -= 8;
	}

	// Convert fore/back
	fore = table[fore];
	back = table[back] + 10;

	// 'Render'
	if (blink)
		printf ("\033[%d;5;%dm\033[%dm", intens, fore, back);
	else
		printf ("\033[%d;25;%dm\033[%dm", intens, fore, back);
}

static void I_ENDTEXT (void)
{
	int i, nl;
	char *scr = (char *) W_CacheLumpName("ENDTEXT", PU_CACHE);
	char *col = getenv("COLUMNS");

	if (col == NULL)
		nl = 1;
	else
	{
		if (atoi(col) > 80)
			nl = 1;
		else	nl = 0;
	}
	for (i = 1; i <= 80*25; scr += 2, i++)
	{
		put_dos2ansi ((byte) scr[1]);
		if (scr[0] < 32)
			putchar('.');
		else
			putchar(scr[0]);
		if (nl && !(i % 80))
			puts("\033[0m");
	}
	printf ("\033[m");	/* Cleanup */
}

void I_Quit (void)
{
	D_QuitNetGame();
	M_SaveDefaults();
	I_Shutdown();
#ifndef _WIN32
	I_ENDTEXT ();
#endif
	exit(0);
}

/*
===============
=
= I_ZoneBase
=
===============
*/

byte *I_ZoneBase (int *size)
{
	byte *ptr;
	int heap = 0x800000;

	ptr = (byte *) malloc (heap);
	if (ptr == NULL)
		I_Error ("I_ZoneBase: Insufficient memory!");

	printf ("0x%x allocated for zone, ", heap);
	printf ("ZoneBase: %p\n", ptr);

	*size = heap;
	return ptr;
}

/*
=============
=
= I_AllocLow
=
=============
*/

byte *I_AllocLow (int length)
{
	byte *ptr;

	ptr = (byte *) malloc (length);
	if (ptr == NULL)
		I_Error ("I_AllocLow: Insufficient memory!");

	return ptr;
}

/*
============================================================================

						NETWORKING

============================================================================
*/

extern	doomcom_t	*doomcom;

/*
====================
=
= I_InitNetwork
=
====================
*/

void I_InitNetwork (void)
{
	int		i;

	i = M_CheckParm ("-net");
	if (!i) {
	//
	// single player game
	//
		doomcom = (doomcom_t *) malloc (sizeof(*doomcom));
		memset (doomcom, 0, sizeof(*doomcom));
		netgame = false;
		doomcom->id = DOOMCOM_ID;
		doomcom->numplayers = doomcom->numnodes = 1;
		doomcom->deathmatch = false;
		doomcom->consoleplayer = 0;
		doomcom->ticdup = 1;
		doomcom->extratics = 0;
		return;
	}
	I_Error ("NET GAME NOT IMPLEMENTED !!!");
}

void I_NetCmd (void)
{
	if (!netgame)
		I_Error ("I_NetCmd when not in netgame");
}

//=========================================================================
//
//		MAIN
//
//=========================================================================

static void CreateBasePath (void)
{
#ifdef _WIN32
	char* pref_dir = SDL_GetBasePath();
#else
	char* pref_dir = SDL_GetPrefPath (H_USERDIR, H_USERDIR);
#endif
	if (pref_dir == NULL)
	{
		I_Error("Unable to determine user directory: %s\n", SDL_GetError());
	}

	basePath = pref_dir;
}

static void InitializeWaddir (void)
{
	const char *_waddir;

	_waddir = getenv(DATA_ENVVAR);
}

int main (int argc, char **argv)
{
	myargc = argc;
	myargv = (const char **) argv;

	CreateBasePath();
	InitializeWaddir();

	D_DoomMain();

	return 0;
}
