/**************************************************************************\
*
* Module Name: GM.H
*
* Graphic Manager Include File
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/

/* GKS FUNCTIONS PROTOTYPES */
int OpenGks ( FILE *, long ) ;
int CloseGks ( void ) ;
int OpenWrks ( char *, PHAB, char * ) ;
int CloseWrks ( char * ) ;
/* LOW LEVEL FUNCTIONS PROTOTYPES */
int RunDrvr ( int, int ) ;
int GksService ( void ) ;


/* SHARED MEMORY SEGMENT NAMES AND STRUCTURES */
static char namGos[] = { "\\SHAREMEM\\GKOS" } ;
static char namGesl[]= { "\\SHAREMEM\\GKESL" } ;
static char namGdt[] = { "\\SHAREMEM\\GKDT" } ;
static char namGsl[] = { "\\SHAREMEM\\GKSL" } ;
static char namGtr[] = { "\\SHAREMEM\\GKTR" } ;
static char namWdt[WRKSMAX][17] = { "\\SHAREMEM\\WDT000" ,
				    "\\SHAREMEM\\WDT001" ,
				    "\\SHAREMEM\\WDT002" } ;
static char namWsl[OWRKSMAX][17] = { "\\SHAREMEM\\WSL000" ,
				     "\\SHAREMEM\\WSL001" ,
				     "\\SHAREMEM\\WSL002" ,
				     "\\SHAREMEM\\WSL003" ,
				     "\\SHAREMEM\\WSL004" } ;



/* SHARED SEMAPHORE NAMES */
static char semGosA[]  = { "\\SEM\\GKOSA" } ;
static char semGosR[]  = { "\\SEM\\GKOSR" } ;
static char semGeslA[] = { "\\SEM\\GKESLA" } ;
static char semGeslR[] = { "\\SEM\\GKESLR" } ;
static char semGdtA[]  = { "\\SEM\\GKDTA" } ;
static char semGslA[]  = { "\\SEM\\GKSLA" } ;
static char semGslR[]  = { "\\SEM\\GKSLR" } ;
static char semGtrA[]  = { "\\SEM\\GKTRA" } ;
static char semGtrR[]  = { "\\SEM\\GKTRR" } ;
static char semWdtA[WRKSMAX][13] = { "\\SEM\\WDTA000" ,
				     "\\SEM\\WDTA001" ,
				     "\\SEM\\WDTA002" } ;
static char semWslA[OWRKSMAX][13] = { "\\SEM\\WSLA000" ,
				      "\\SEM\\WSLA001" ,
				      "\\SEM\\WSLA002" ,
				      "\\SEM\\WSLA003" ,
				      "\\SEM\\WSLA004" } ;
static char semWslR[OWRKSMAX][13] = { "\\SEM\\WSLR000" ,
				      "\\SEM\\WSLR001" ,
				      "\\SEM\\WSLR002" ,
				      "\\SEM\\WSLR003" ,
				      "\\SEM\\WSLR004" } ;

static PGKSOPERSTATE	    Gos = 0 ;
static PGKSERRSTATELIST     Gesl = 0 ;
static PGKSDESCTBL	    Gdt = 0 ;
static PGKSSTATELIST	    Gsl = 0 ;
static PGKSTRANSFER	    Gtr = 0 ;
static PWRKSDESCTBL	    Wdt[WRKSMAX] ;
static PWRKSSTATELIST	    Wsl[WRKSMAX] ;
