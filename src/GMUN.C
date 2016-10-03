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

/**************************************************************************\
*
* Module Name: GMUN.C
*
* Uninstal Graphic Manager
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
main ( int argc, char *argv[], char *envp[] )
{
int Error, index ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* GET GKS STATE LIST & TRANSFER SEGMENTS */
if ((DosGetShrSeg( namGsl, (PSEL) &Gsl+1))  ||
    (DosGetShrSeg( namGtr, (PSEL) &Gtr+1))) {
    printf ( " ERROR: Can't fined shared segment\n" ) ;
    return (1) ;
    }
else {
    if (( DosOpenSem ( &Gtr->SemHndlRun,    semGtrR )) ||
	( DosOpenSem ( &Gsl->SemHndlRun,    semGslR ))) {
	printf ( " ERROR: Can't open semaphores\n" ) ;
	return (1) ;
	}
    }

/* CLOSE GKS */
Gtr->SrvNr = GM_CLOSEGKS ;
DosSemSet ( Gtr->SemHndlRun ) ;
DosSemClear ( Gsl->SemHndlRun ) ;
/*  DosSemWait ( Gtr->SemHndlRun, -1L ) ;*/
printf ( " GM uninstaled\n" ) ;

return (0) ;
}
