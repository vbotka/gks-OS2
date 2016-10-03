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

FILE	    *ErrorFile = NULL ;	    /* NOT IMPLEMENTED */
long	    MemBufferArea = 0 ;     /* NOT IMPLEMENTED */
FILE	    *CnfIn ;		    /* CONFIGURATION FILE */
char	    Path[80] ;		    /* GM PATH */

#define	MAXSPEC 	    80

/**************************************************************************\
*
* Module Name: GM.C
*
* Graphic Manager
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
main ( int argc, char *argv[], char *envp[] )
{
int Error, index ;

char Buffer[80] ;
int TypeIndx, FreePos ;

char namFail [MAXSPEC] ;
RESULTCODES resChild ;
char *namGmExe = 0 ;
unsigned offCmd ;


/* LOAD FLOATING POINT SUPPORT */
float FloatingPoint = 1.0 ;

/* OPERATING STATE SEGMENT ALLOCATION */
if (DosGetShrSeg( namGos, (PSEL) &Gos+1)) {
    if ( DosAllocShrSeg( sizeof(struct _GKSOPERSTATE),
			 namGos,
			 (PSEL) &Gos+1)) {
	/* ERROR: Can't allocate Operating State Segment */
	return (1) ;
	}
/* OPERATING STATE SEGMENT INITIALIZATION */
    else {
	DosCreateSem ( 1, &Gos->SemHndlAccess, semGosA ) ;
	DosCreateSem ( 1, &Gos->SemHndlRun,    semGosR ) ;
	DosSemSet ( Gos->SemHndlRun ) ;
	Gos->StateValue = INSTALL ;	 /* GKS is not installed */
	}
    }
else {
/* NOW EXECUTING FROM BACKGROUND */
    if ( Gos->StateValue == INSTALL ) {
	Gos->StateValue = GKCL ;
    /* GET OTHER SEGMENTS AND OPEN SEMAPHORES */
	if ((DosOpenSem ( &Gos->SemHndlRun, semGosR	)) ||
	    (DosOpenSem ( &Gos->SemHndlAccess, semGosA	)) ||
	    (DosGetShrSeg( namGesl, (PSEL) &Gesl+1	)) ||
	    (DosOpenSem ( &Gesl->SemHndlRun, semGeslR	)) ||
	    (DosOpenSem ( &Gesl->SemHndlAccess, semGeslA)) ||
	    (DosGetShrSeg( namGdt,  (PSEL) &Gdt+1	)) ||
	    (DosOpenSem ( &Gdt->SemHndlAccess, semGosA	)) ||
	    (DosGetShrSeg( namGtr,  (PSEL) &Gtr+1	)) ||
	    (DosOpenSem ( &Gtr->SemHndlRun, semGtrR	)) ||
	    (DosOpenSem ( &Gtr->SemHndlAccess, semGtrA	))) {
	    /* ERROR: Can't fined shared segments */
	    Error = 1 ;
	    }
	index = Gdt->WrksTypesAvail ;
	while (index--) {
	    if ((DosGetShrSeg( namWdt[index],(PSEL) &Wdt[index]+1	 )) ||
		(DosOpenSem ( &Wdt[index]->SemHndlAccess, semWdtA[index] ))) {
		/* ERROR: Can't fined shared segments */
		Error = 1 ;
		break ;
		}
	    }
	/* OPEN GKS */
	if ( OpenGks ( ErrorFile, MemBufferArea )) {
	    /* ERROR: Can't open GKS */
	    return(1);
	    }
	/* RELEASE PARENT */
	DosSemClear ( Gos->SemHndlRun ) ;
	/* SERVICE */
	return (GksService ()) ;
	}
    else {
	/* WARRNING: GKS already installed */
	return(1);
	}
    }

/* OPEN CONFIGURATION FILE */
if ((CnfIn = fopen ( "gm.cnf", "r" )) == NULL ) {
     /* ERROR: Can't open configuration file */
     return (1) ;
     }
fscanf ( CnfIn, "%s", Path ) ;

/* ERROR STATE LIST SEGMENT ALLOCATION */
if (DosGetShrSeg( namGesl, (PSEL) &Gesl+1)) {
    if ( DosAllocShrSeg( sizeof(struct _GKSERRSTATELIST),
			 namGesl,
			 (PSEL) &Gesl+1)) {
	/* ERROR: Can't allocate Error State List segment */
	return (1) ;
	}
    else {
	DosCreateSem ( 1, &Gesl->SemHndlAccess, semGeslA ) ;
	DosCreateSem ( 1, &Gesl->SemHndlRun, semGeslR ) ;
	}
    }

/* GKS DESCRIPTION TABLE SEGMENT ALLOCATION */
if (DosGetShrSeg( namGdt, (PSEL) &Gdt+1)) {
    if ( DosAllocShrSeg( sizeof(struct _GKSDESCTBL),
			 namGdt,
			 (PSEL) &Gdt+1)) {
	/* ERROR: Can't allocate GKS Description Table segment */
	return (1) ;
	}
    else {
/* GKS DESCRIPTION TABLE SEGMENT INITIALIZATION */
	fscanf ( CnfIn, "%s", Buffer ) ;
	DosCreateSem ( 1, &Gdt->SemHndlAccess, semGdtA ) ;
	DosSemSet ( Gdt->SemHndlAccess ) ;
	fscanf ( CnfIn, "%x", &Gdt->Level ) ;
	fscanf ( CnfIn, "%u", &Gdt->WrksTypesAvail ) ;
	index = Gdt->WrksTypesAvail ;
	while ( index-- )
	    fscanf ( CnfIn, "%s", Gdt->WrksTypeList[index] ) ;
	fscanf ( CnfIn, "%u", &Gdt->SimOpenWrksMax ) ;
	fscanf ( CnfIn, "%u", &Gdt->SimActvWrksMax ) ;
	fscanf ( CnfIn, "%u", &Gdt->WrksAssocSegMax ) ;
	fscanf ( CnfIn, "%u", &Gdt->NmTrMax ) ;
	}
    }

index = Gdt->WrksTypesAvail ;
while ( index-- ) {
/* WORKSTATION DESCRIPTION TABLE SEGMENT ALLOCATION */
    if (DosGetShrSeg( namWdt[index], (PSEL) &Wdt[index]+1)) {
	Wdt[index] = 0 ;
	if ( DosAllocShrSeg( sizeof(struct _WRKSDESCTBL),
			     namWdt[index],
			     (PSEL) &Wdt[index]+1)) {
	    /* ERROR: Can't allocate Workstation Description Table segment */
	    return (1) ;
	    }
	else {
/* WORKSTATION DESCRIPTION TABLE SEGMENT INITIALIZATION */
	    fscanf ( CnfIn, "%s", Buffer ) ;
	    DosCreateSem ( 1, &Wdt[index]->SemHndlAccess, semWdtA[index] ) ;
	    DosSemSet ( Wdt[index]->SemHndlAccess ) ;
	    fscanf ( CnfIn, "%s", Wdt[index]->WrksType ) ;
	    fscanf ( CnfIn, "%s", Buffer ) ;
	    strcpy ( Wdt[index]->WrksDrvr, Path ) ;
	    strcat ( Wdt[index]->WrksDrvr, Buffer ) ;
	    fscanf ( CnfIn, "%x", &Wdt[index]->WrksCategory ) ;
	    fscanf ( CnfIn, "%x", &Wdt[index]->DevCoordUnits ) ;
	    fscanf ( CnfIn, "%le", &Wdt[index]->MaxLenUn.x ) ;
	    fscanf ( CnfIn, "%le", &Wdt[index]->MaxLenUn.y ) ;
	    fscanf ( CnfIn, "%ld", &Wdt[index]->MaxDevUn.x ) ;
	    fscanf ( CnfIn, "%ld", &Wdt[index]->MaxDevUn.y ) ;
	    }
	}
    }
fclose ( CnfIn ) ;


/* TRANSFER SEGMENT ALLOCATION */
if (DosGetShrSeg( namGtr, (PSEL) &Gtr+1)) {
    if ( DosAllocShrSeg( sizeof(struct _GKSTRANSFER),
			 namGtr,
			 (PSEL) &Gtr+1)) {
	/* ERROR: Can't allocate Transfer segment */
	return (1) ;
	}
    else {
/* TRANSFER SEGMENT INITIALIZATION */
	DosCreateSem ( 1, &Gtr->SemHndlAccess, semGtrA ) ;
	DosCreateSem ( 1, &Gtr->SemHndlRun, semGtrR ) ;
	Gtr->SrvNr = -1 ;
	}
    }

/* START BACKGROUND PROCESS */
DosGetEnv ((PSEL)&namGmExe + 1, &offCmd ) ;
while ( *namGmExe++ || *namGmExe )
    ;
++namGmExe ;
if ( DosExecPgm ( namFail,
		  MAXSPEC,
		  EXEC_BACKGROUND,
		  NULL,
		  0,
		  &resChild,
		  namGmExe)) {
    /* ERROR: Can't start background process */
    return (1);
    }

/* WAIT FOR BACKGOUND PROCESS TO CLEAR SEMAPHORE */
DosSemWait ( Gos->SemHndlRun, -1L ) ;

}
