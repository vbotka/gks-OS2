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

main ( int argc, char * *argv)
{
HAB	hab;
HMQ	hmq;
HWND	hfrm,hcnt;
QMSG	qmsg;
MPARAM	parm1,parm2;

WinSendMsg ( hfrm, WM_USER_PAINT, 0, 0 ) ;

}
