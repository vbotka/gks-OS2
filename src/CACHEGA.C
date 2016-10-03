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

MRESULT EXPENTRY GraphicProc (HWND,USHORT,MPARAM,MPARAM);

main ( int argc, char * *argv)
{
static CHAR pszClassName[] = "FirstClass";
HAB	hab;
HMQ	hmq;
HWND	hfrm,hcnt;
QMSG	qmsg;
ULONG	flFrameFlags= FCF_SYSMENU|FCF_TITLEBAR|
		      FCF_SIZEBORDER|FCF_MINMAX;
ULONG	flFrameStyle= WS_VISIBLE;
int Type, Index, Error ;
char *endptr ;

/* GET SHARED SEGMENTS */
Type  = strtol (  argv[1], &endptr, 10 ) ;
Index = strtol (  argv[2], &endptr, 10 ) ;
if ((DosGetShrSeg( namWdt[Type ], (PSEL) &Wdt[Type ]+1)) ||
    (DosGetShrSeg( namGtr	, (PSEL) &Gtr	    +1)) ||
    (DosGetShrSeg( namWsl[Index], (PSEL) &Wsl[Index]+1))) {
    /* ERROR: Can't fined shared segments */
    Error = 1 ;
    }

/* OPEN SEMAPHORE */
if ( DosOpenSem ( &Gtr->SemHndlRun, semGtrR )) {
    /* ERROR: Can't open semaphores */
    Error = 2 ;
    }

/* CREATE GRAPHICS WINDOW */
hab=WinInitialize(0);
hmq=WinCreateMsgQueue(hab,0);
WinRegisterClass(hab,pszClassName,GraphicProc,CS_SIZEREDRAW,0);
hfrm=WinCreateStdWindow(HWND_DESKTOP,flFrameStyle,
		       &flFrameFlags,pszClassName,
		       "Cached-PS PM Graphics",
		       0L,NULL,0,&hcnt);
WinSetWindowPos(hfrm,NULL,10,10,300,300,SWP_SIZE|SWP_MOVE|SWP_SHOW);

/* STORE HAB AS CONNECT ID AND CLEAR SEMAPHORE */
Wsl[Index]->ConnectID = hab ;
DosSemClear ( Gtr->SemHndlRun ) ;

/* WAIT AND DISPATCH MESSAGES */
while (WinGetMsg(hab,&qmsg,NULL,0,0))
    WinDispatchMsg(hab,&qmsg);

WinDestroyWindow(hfrm);
WinDestroyMsgQueue(hmq);
WinTerminate(hab);
return 0;
}

MRESULT EXPENTRY GraphicProc(hwnd,messg,parm1,parm2)
HWND	hwnd;
USHORT	messg;
MPARAM	parm1,parm2;
{
HPS	hps;
RECTL	rcl;
POINTL	ptl1,ptl2;
switch (messg)
    {
    case WM_USER_PAINT:
    hps=WinBeginPaint(hwnd,NULL,NULL);
    GpiErase(hps);
    WinQueryWindowRect(hwnd,&rcl);
    WinFillRect(hps,&rcl,CLR_YELLOW);

    ptl1.x=20;
    ptl1.y=50;
    ptl2.x=100;
    ptl2.y=100;
    GpiMove(hps,&ptl1);
    GpiLine(hps,&ptl2);

    WinEndPaint(hps);
    break;

    default:
    return WinDefWindowProc(hwnd,messg,parm1,parm2);
    }
return 0;
}
