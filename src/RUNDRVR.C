#define INCL_PM
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS

#include <os2.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include "gksdef.h"
#include "gm.h"

#define	MAXSPEC 	    80

/**************************************************************************\
*
* Module Name: RUNDRVR.C
*
* function Run Workstation Driver
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
int RunDrvr ( int TypeIndx, int FreePos )
{
int Error=0, index=0 ;
char namFail [MAXSPEC] ;
RESULTCODES resChild ;
char RunString[100], Parm[100] ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* GET SEGMENTS */
if ((DosGetShrSeg( namGtr, (PSEL) &Gtr+1)) ||
    (DosGetShrSeg( namWdt[TypeIndx], (PSEL) &Wdt[TypeIndx]+1)) ||
    (DosGetShrSeg( namGsl, (PSEL) &Gsl+1))) {
    /* ERROR: Can't fined shared segments */
    Error = 1 ;
    }

if ( Error == 0 ) {
/* EXECUTE WORKSTATION DRIVER */
    sprintf ( Parm, "%d %d\0", TypeIndx, FreePos ) ;
    strcpy ( RunString, Wdt[TypeIndx]->WrksDrvr ) ;
    strcat ( strcat ( RunString, " " ), Parm ) ;
    RunString [strlen(Wdt[TypeIndx]->WrksDrvr)] = '\0' ;
    if ( DosExecPgm ( namFail,
		      MAXSPEC,
		      EXEC_BACKGROUND,
		      RunString,
		      0,
		      &resChild,
		      Wdt[TypeIndx]->WrksDrvr)) {
	/* ERROR: Specified workstation cannot be opened */
	Error = 26 ;
	}
    }
return (Error) ;
}
