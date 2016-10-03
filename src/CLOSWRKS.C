#define INCL_PM
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES

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
* Module Name: CLOSWRKS.C
*
* GKS function Close Workstation
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
int CloseWrks ( char *WrksID )
{
int Error=0, index=0 ;
int WrksIndx ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* REQUEST SEGMENTS ACCESS */
if ((DosGetShrSeg( namGos, (PSEL) &Gos+1))  ||
    (DosGetShrSeg( namGdt, (PSEL) &Gdt+1))  ||
    (DosGetShrSeg( namGtr, (PSEL) &Gtr+1 ))  ||
    (DosGetShrSeg( namGsl, (PSEL) &Gsl+1))) {
    /* ERROR: Can't fined shared segments */
    Error = 1 ;
    }
else {
    if (( DosOpenSem ( &Gos->SemHndlAccess, semGosA ))	||
	( DosOpenSem ( &Gsl->SemHndlAccess, semGslA ))	||
	( DosOpenSem ( &Gtr->SemHndlAccess, semGtrA ))	||
	( DosOpenSem ( &Gsl->SemHndlRun,    semGslR ))	||
	( DosOpenSem ( &Gtr->SemHndlRun,    semGtrR ))) {
	/* ERROR: Can't open semaphores */
	Error = 2 ;
	}
    else {
	DosSemRequest ( Gos->SemHndlAccess, -1L ) ;
	DosSemRequest ( Gsl->SemHndlAccess, -1L ) ;
	DosSemRequest ( Gtr->SemHndlAccess, -1L ) ;
	}
    }

if ( Error == 0 ) {
/* CHECK GKS STATE */
    if ( Gos->StateValue < WSOP ) {
	/* ERROR: No workstation is open */
	Error = 7 ;
	}
    }

if ( Error == 0 ) {
/* GET WORKSTATION INDEX */
    index = 0 ;
    while ( strcmp ( Gsl->WrksOpenSet[index], WrksID )) {
	if ( ++index == Gdt->SimOpenWrksMax ) {
	    /* ERROR: Specified workstation is not open */
	    Error = 25 ;
	    break ;
	    }
	}
    WrksIndx = index ;
    }

if ( Error == 0 ) {
/* REQUEST WORKSTATION STATE LIST ACCESS */
    index = WrksIndx ;
    if (DosGetShrSeg( namWsl[index], (PSEL) &Wsl[index]+1)) {
	/* ERROR: Can't fined shared segment */
	Error = 1 ;
	}
    else {
	if (DosOpenSem ( &Wsl[index]->SemHndlAccess, semWslA[index] )) {
	    /* ERROR: Can't open semaphore */
	    Error = 2 ;
	    }
	else {
	    DosSemRequest ( Wsl[index]->SemHndlAccess, -1L ) ;
	    }
	}
    }

/* CLOSE WORKSTATION DRIVER */

if ( Error == 0 ) {
/* WORKSTATION STATE LIST SEGMENT DEALLOCATION */
    index = WrksIndx ;
    DosCloseSem ( Wsl[index]->SemHndlAccess ) ;
    DosCloseSem ( Wsl[index]->SemHndlRun ) ;
    DosFreeSeg ( (SEL) &Wsl[index]+1 ) ;
/* DELETE WrksID FROM GKS STATE LIST */
    strcpy ( Gsl->WrksOpenSet[index], EMPTYPOSITION ) ;
/* SET GKS OPERATING STATE */
    index = 0 ;
    while ( ! strcmp ( Gsl->WrksOpenSet[index], EMPTYPOSITION )) {
	if ( ++index == Gdt->SimOpenWrksMax ) {
	    Gos->StateValue = GKOP ;
	    break ;
	    }
	}
    }

/* RELEASE SEGMENTS ACCESS */
if ( (Error == 0) || (Error > 2) ) {
    DosSemClear ( Gos->SemHndlAccess ) ;
    DosSemClear ( Gsl->SemHndlAccess ) ;
    DosSemClear ( Gtr->SemHndlAccess ) ;
    }

return (Error) ;
}
