#define INCL_PM
#define INCL_DOSMEMMGR
#define INCL_DOSSEMAPHORES

#include <os2.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#define WM_USER_PAINT	0x1000

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

FILE *out ;

/* CREATE GRAPHICS WINDOW */
hab=WinInitialize(0);
hmq=WinCreateMsgQueue(hab,0);
WinRegisterClass(hab,pszClassName,GraphicProc,CS_SIZEREDRAW,0);
hfrm=WinCreateStdWindow(HWND_DESKTOP,flFrameStyle,
		       &flFrameFlags,pszClassName,
		       "Cached-PS PM Graphics",
		       0L,NULL,0,&hcnt);
WinSetWindowPos(hfrm,NULL,10,10,300,300,SWP_SIZE|SWP_MOVE|SWP_SHOW);

out = fopen ( "info", "w" ) ;
fprintf ( out, " hab:%p\n", hab ) ;
fprintf ( out, " hmq:%p\n", hmq ) ;
fprintf ( out, "hfrm:%p\n", hfrm ) ;
fprintf ( out, "hcnt:%p\n", hcnt ) ;
fclose (out) ;

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
