#define INCL_PM
#define INCL_SUB
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#define INCL_DOSINFOSEG

#include <os2.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <process.h>
#include "gksdef.h"
#include "gm.h"

/**************************************************************************\
*
* Module Name: GKSSERV.C
*
* Graphic Manager Service Function
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
/* Service routine waits and process service requirements from application
 * programs.
 *
 */

int GksService ()
{
int TypeIndx, FreePos ;
int Error, index ;

/* GET GKS STATE LIST & TRANSFER SEGMENTS */
if ((DosGetShrSeg( namGsl, (PSEL) &Gsl+1))  ||
    (DosGetShrSeg( namGtr, (PSEL) &Gtr+1))) {
    /* ERROR: Can't fined shared segment */
    return (1) ;
    }
else {
    if (( DosOpenSem ( &Gsl->SemHndlAccess, semGslA ))	||
	( DosOpenSem ( &Gsl->SemHndlRun,    semGslR ))) {
	/* ERROR: Can't open semaphores */
	return (1) ;
	}
    }

/* SERVICE */
while ( Gtr->SrvNr ) {
    DosSemWait ( Gsl->SemHndlRun, -1L ) ;
    DosSemSet ( Gsl->SemHndlRun ) ;
    switch ( Gtr->SrvNr )
	{
	case GM_OPENWRKS :
	    TypeIndx = Gtr->p1 ;
	    FreePos  = Gtr->p2 ;
	    if (( Error = RunDrvr ( TypeIndx, FreePos ))) {
		/* ERROR: Can't execute workstation driver */
		}
	    break ;
	case GM_CLOSWRKS :
	    break ;
	case GM_CLOSEGKS :
	    if ( CloseGks ()) {
		/* ERROR: Can't close GKS */
		Gtr->SrvNr = -1 ;
		}
	    break ;
	}
    }
return (0) ;
}
