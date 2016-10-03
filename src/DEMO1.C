#define  INCL_PM

#include <os2def.h>
#include <stdio.h>
#include <process.h>
#include "gksdef.h"
#include "gm.h"

char WrksType[] = { "Cached-PS-1" } ;
HAB  ConnectID ;
char WrksID[] = { "DEVICE 01" }	;

/**************************************************************************\
*
* Module Name: DEMO1.C
*
* Graphic Demo No.1
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/
main ( int argc, char *argv[], char *envp[] )
{
int Error ;

if ( Error = OpenWrks ( WrksID, &ConnectID, WrksType )) {
    printf ( "ERROR: Can't open workstation\n" ) ;
    Error = 1 ;
    }

if ( Error = CloseWrks ( WrksID )) {
    printf ( "ERROR: Can't close workstation\n" ) ;
    Error = 1 ;
    }

exit ( Error ) ;
}
