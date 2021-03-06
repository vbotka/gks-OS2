/* PM p.66 */

#define INCL_PM

#include <os2.h>
#include <stddef.h>

MRESULT EXPENTRY GraphicProc (HWND,USHORT,MPARAM,MPARAM);

main ()
{
static CHAR pszClassName[] = "FirstClass";
HAB	hab;
HMQ	hmq;
HWND	hfrm,hcnt;
QMSG	qmsg;
ULONG	flFrameFlags= FCF_SYSMENU|FCF_TITLEBAR|
		      FCF_SIZEBORDER|FCF_MINMAX;
ULONG	flFrameStyle= WS_VISIBLE;

hab=WinInitialize(0);
hmq=WinCreateMsgQueue(hab,0);
WinRegisterClass(hab,pszClassName,GraphicProc,CS_SIZEREDRAW,0);
hfrm=WinCreateStdWindow(HWND_DESKTOP,flFrameStyle,
		       &flFrameFlags,pszClassName,
		       "Cached-PS PM Graphics",
		       0L,NULL,0,&hcnt);
WinSetWindowPos(hfrm,NULL,10,10,300,300,SWP_SIZE|SWP_MOVE|SWP_SHOW);
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
    case WM_PAINT:
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
