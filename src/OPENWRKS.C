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
* Module Name: OPENWRKS.C
*
* GKS function Open Workstation
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
int OpenWrks ( char *WrksID, HAB *ConnectID, char *WrksType )
{
int Error=0, index=0 ;
int FreePos, TypeIndx ;

/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* REQUEST SEGMENTS ACCESS */
if ((DosGetShrSeg( namGos, (PSEL) &Gos+1 ))  ||
    (DosGetShrSeg( namGdt, (PSEL) &Gdt+1 ))  ||
    (DosGetShrSeg( namGtr, (PSEL) &Gtr+1 ))  ||
    (DosGetShrSeg( namGsl, (PSEL) &Gsl+1 ))) {
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
    if ( Gos->StateValue == GKCL ) {
	/* ERROR: GKS is closed */
	Error = 8 ;
	}
    }

if ( Error == 0 ) {
/* CHECK WORKSTATION TYPE */
    index = 0 ;
    while ( strcmp ( Gdt->WrksTypeList[index], WrksType)) {
	if ( ++index == Gdt->WrksTypesAvail ) {
	    /* ERROR: Specified workstation type does not exist */
	    Error = 23 ;
	    break ;
	    }
	}
    TypeIndx = index ;
    }

if ( Error == 0 ) {
/* FIND POSITION IN GKS STATE LIST TABLE TO PLACE NEW WORKSTATION */
    index = 0 ;
    while ( strcmp ( Gsl->WrksOpenSet[index], EMPTYPOSITION )) {
	if ( ++index == Gdt->SimOpenWrksMax ) {
	    /* ERROR: Specified workstation cannot be opened */
	    Error = 26 ;
	    break ;
	    }
	}
    FreePos = index ;
    }

if ( Error == 0 ) {
/* WORKSTATION STATE LIST SEGMENT ALLOCATION */
    index = FreePos ;
    if ( DosGetShrSeg( namWsl[index], (PSEL) &Wsl[index]+1)) {
	if ( DosAllocShrSeg( sizeof(struct _WRKSSTATELIST),
			     namWsl[index],
			     (PSEL) &Wsl[index]+1)) {
	    /* ERROR: Can't allocate Workstation State List segment */
	    Error = 26 ;
	    }
	else {
/* WORKSTATION STATE LIST SEGMENT INITIALIZATION */
	    DosCreateSem ( 1, &Wsl[index]->SemHndlAccess, semWslA[index] ) ;
	    DosCreateSem ( 1, &Wsl[index]->SemHndlRun, semWslR[index] ) ;
	    DosSemSet ( Wsl[index]->SemHndlAccess ) ;
	    DosSemSet ( Wsl[index]->SemHndlRun ) ;
	    strcpy ( Wsl[index]->WrksID, WrksID ) ;
	  /*Wsl[index]->ConnectID = ConnectID ;*/
	    strcpy ( Wsl[index]->WrksType, WrksType ) ;
	    Wsl[index]->WrksState = INACTIVE ;
	    /* ... */
	    Wsl[index]->ColourIndex = 15 ;
	    /* ... */
	    Wsl[index]->WrksTransfUpdtSt = NOTPENDING ;
	    Wsl[index]->WrksWinReq.B.x = 0.0 ;
	    Wsl[index]->WrksWinReq.B.y = 0.0 ;
	    Wsl[index]->WrksWinReq.E.x = 1.0 ;
	    Wsl[index]->WrksWinReq.E.y = 1.0 ;
	    Wsl[index]->WrksWinCurrent.B.x = 0.0 ;
	    Wsl[index]->WrksWinCurrent.B.y = 0.0 ;
	    Wsl[index]->WrksWinCurrent.E.x = 1.0 ;
	    Wsl[index]->WrksWinCurrent.E.y = 1.0 ;
	    Wsl[index]->WrksViewReq.B.x = 0.0 ;
	    Wsl[index]->WrksViewReq.B.y = 0.0 ;
	    Wsl[index]->WrksViewReq.E.x = 1.0 ;
	    Wsl[index]->WrksViewReq.E.y = 1.0 ;
	    Wsl[index]->WrksViewCurrent.B.x = 0.0 ;
	    Wsl[index]->WrksViewCurrent.B.y = 0.0 ;
	    Wsl[index]->WrksViewCurrent.E.x = 1.0 ;
	    Wsl[index]->WrksViewCurrent.E.y = 1.0 ;
	    /* MISCELLANEOUS */
	    Wsl[index]->GrCursorPos.x = 0 ;
	    Wsl[index]->GrCursorPos.y = 0 ;
/* MAKE AVAILABLE WORKSTATION STATE LIST */
	    DosSemClear ( Wsl[index]->SemHndlAccess ) ;
/* RELEASE WORKSTATION */
	    DosSemClear ( Wsl[index]->SemHndlRun ) ;
	    }
	}
    else {
	/* ERROR: Segment already allocated */
	Error = 24 ;
	}
    }
if ( Error == 0 ) {
/* EXECUTE WORKSTATION DRIVER */
    Gtr->SrvNr = GM_OPENWRKS ;
    Gtr->p1 = TypeIndx ;
    Gtr->p2 = FreePos  ;
    DosSemSet ( Gtr->SemHndlRun ) ;
    DosSemClear ( Gsl->SemHndlRun ) ;
    DosSemWait ( Gtr->SemHndlRun, -1L ) ;
    }

if ( Error == 0 ) {
/* CHECK ANCHOR BLOCK HANDLE */
    }

if ( Error == 0 ) {
/* SET GKS STATE LIST */
    strcpy ( Gsl->WrksOpenSet[FreePos], WrksID ) ;
/* SET GKS OPERATING STATE */
    if ( Gos->StateValue == GKOP )
	Gos->StateValue = WSOP ;
    }

/* RELEASE SEGMENTS ACCESS */
if ( (Error == 0) || (Error > 2) ) {
    DosSemClear ( Gos->SemHndlAccess ) ;
    DosSemClear ( Gsl->SemHndlAccess ) ;
    DosSemClear ( Gtr->SemHndlAccess ) ;
    }

return (Error) ;
}
