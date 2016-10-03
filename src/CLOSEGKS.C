#define INCL_PM
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS

#include <os2.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gksdef.h"
#include "gm.h"

/**************************************************************************\
*
* Module Name: CLOSEGKS.C
*
* GKS function Close GKS
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
int CloseGks ()
{
int Error=0, index=0 ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* REQUEST SEGMENTS ACCESS */
if ((DosGetShrSeg( namGos,  (PSEL) &Gos+1 )) ||
    (DosGetShrSeg( namGesl, (PSEL) &Gesl+1)) ||
    (DosGetShrSeg( namGsl,  (PSEL) &Gsl+1))) {
    /* ERROR: Can't fined shared segments */
    Error = 1 ;
    }
else {
    if ( DosOpenSem ( &Gos->SemHndlAccess, semGosA ) ||
	 DosOpenSem ( &Gsl->SemHndlAccess, semGslA )) {
	/* ERROR: Can't open semaphores */
	Error = 2 ;
	}
    else {
	DosSemRequest ( Gos->SemHndlAccess, -1L ) ;
	DosSemRequest ( Gsl->SemHndlAccess, -1L ) ;
	}
    }

if ( Gos->StateValue != GKOP ) {
    /* ERROR: GKS not opened */
    Error = 2 ;
    }
else {
/* SET GKS OPERATING STATE SEGMENT ENTRY INTO CLOSE */
    Gos->StateValue = GKCL ;
/* MAKE UNAVAILABLE GKS STATE LIST */
    DosCloseSem ( Gsl->SemHndlAccess ) ;
    DosCloseSem ( Gsl->SemHndlRun ) ;
    DosFreeSeg ( (SEL) &Gsl+1 ) ;
/* RELEASE ALL BUFFERS & GKS FILES
	  ...			  */
    Error = 0 ;
    }

/* RELEASE SEGMENTS ACCESS */
if ( Error == 2 )
    DosSemClear ( Gsl->SemHndlAccess ) ;
DosSemClear ( Gos->SemHndlAccess ) ;

return (Error) ;
}
