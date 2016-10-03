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
* Module Name: OPENGKS.C
*
* GKS function Open GKS
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
int OpenGks ( FILE *ErrorFile, long MemBufferArea )
{
int Error=0, index=0 ;
char Buffer[80] ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* REQUEST SEGMENTS ACCESS */
if ((DosGetShrSeg( namGos,  (PSEL) &Gos+1 )) ||
    (DosGetShrSeg( namGesl, (PSEL) &Gesl+1)) ||
    (DosGetShrSeg( namGdt,  (PSEL) &Gdt+1 ))) {
    /* ERROR: Can't fined shared segments */
    Error = 1 ;
    }
else {
    if ( DosOpenSem ( &Gos->SemHndlAccess, semGosA )) {
	/* ERROR: Can't open semaphore */
	Error = 2 ;
	}
    else {
	DosSemRequest ( Gos->SemHndlAccess, -1L ) ;
	}
    }

if ( Error == 0 ) {
/* GET WORKSTATION DESCRIPTION TABLE SEGMENTS */
    index = Gdt->WrksTypesAvail ;
    while (index--) {
	if ( DosGetShrSeg( namWdt[index],(PSEL) &Wdt[index]+1 )) {
	    /* ERROR: Can't fined shared segments */
	    Error = 1 ;
	    break ;
	    }
	}
    }

if ( Error == 0 ) {
/* CHECK GKS STATE */
    if ( Gos->StateValue != GKCL ) {
	/* ERROR: GKS not closed */
	Error = 1 ;
	}
    }

if ( Error == 0 ) {
    if ( DosGetShrSeg( namGsl, (PSEL) &Gsl+1)) {
/* GKS STATE LIST SEGMENT ALLOCATION */
	if ( DosAllocShrSeg( sizeof(struct _GKSSTATELIST),
			      namGsl,
			      (PSEL) &Gsl+1)) {
	    /* ERROR: Can't allocate GKS State List segment */
	    Error = 1 ;
	    }
	else {
/* GKS STATE LIST SEGMENT INITIALIZATION */
	    DosCreateSem ( 1, &Gsl->SemHndlAccess, semGslA ) ;
	    DosCreateSem ( 1, &Gsl->SemHndlRun, semGslR ) ;
	    DosSemSet ( Gsl->SemHndlAccess ) ;
	    DosSemSet ( Gsl->SemHndlRun ) ;
	    index = Gdt->SimOpenWrksMax ;
	    while ( index-- )
		{
		strcpy ( Gsl->WrksOpenSet[index], EMPTYPOSITION ) ;
		strcpy ( Gsl->WrksActvSet[index], EMPTYPOSITION ) ;
		}
	    Gsl->NmTrCur = 0 ;
	    index = Gdt->NmTrMax ;
	    while ( index-- ) {
		Gsl->NmTr[index].Number = index ;
		Gsl->NmTr[index].W.B.x = 0.0 ;
		Gsl->NmTr[index].W.B.y = 0.0 ;
		Gsl->NmTr[index].W.E.x = 1.0 ;
		Gsl->NmTr[index].W.E.y = 1.0 ;
		Gsl->NmTr[index].V.B.x = 0.0 ;
		Gsl->NmTr[index].V.B.y = 0.0 ;
		Gsl->NmTr[index].V.E.x = 1.0 ;
		Gsl->NmTr[index].V.E.y = 1.0 ;
		Gsl->NmTr[index].Clip = CLIP ;
		}
/* SET ERROR FILE ENTRY IN GKS ERROR STATE LIST */
	    Gesl->ErrorFile = ErrorFile ;
/* SET GKS OPERATING STATE SEGMENT ENTRY INTO OPEN */
	    Gos->StateValue = GKOP ;
/* MAKE AVAILABLE GKS DESCRIPTION TABLE & WORKSTATION DESCRIPTION TABLES */
	    DosOpenSem ( &Gdt->SemHndlAccess, semGdtA ) ;
	    DosSemClear ( Gdt->SemHndlAccess ) ;
	    index = Gdt->WrksTypesAvail ;
	    while ( index-- ) {
		DosOpenSem ( &Wdt[index]->SemHndlAccess, semWdtA[index] ) ;
		DosSemClear ( Wdt[index]->SemHndlAccess ) ;
		}
/* MAKE AVAILABLE GKS STATE LIST */
	    DosSemClear ( Gsl->SemHndlAccess ) ;
	    Error = 0 ;
	    }
	}
    else {
	/* ERROR: Segment already allocated */
	Error = 1 ;
	}
    }

/* RELEASE SEGMENTS ACCESS */
DosSemClear ( Gos->SemHndlAccess ) ;

return (Error) ;
}
