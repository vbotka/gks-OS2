/**************************************************************************\
*
* Module Name: GKSDEF.H
*
* GKS Common Definitions file
*
* Copyright (c) 1990,				     . All rights reserved.
*
\**************************************************************************/

#define WRKSMAX        3
#define OWRKSMAX       5
#define AWRKSMAX       5
#define SEGMAX	       1
#define NMTRMAX	      10
#define NAMELENGHT    13
#define DRVRLENGHT    80
#define EMPTYPOSITION "@"

/* IMPLEMENTATION DEPENDENT TYPES */
typedef FILE		GKSFILE;	/* FILE structure defined in stdio.h  */
typedef HSEM		GKSSEMAPHOR;	/* OS/2 semaphor  defined in os2def.h */
/*	HAB		Anchor Block handle		  defined in os2def.h */

typedef struct _GKSOPERSTATE {	    /* GKS OPERATING STATE */
    GKSSEMAPHOR     SemHndlAccess;
    GKSSEMAPHOR     SemHndlRun;
    unsigned short  StateValue;
} GKSOPERSTATE;
typedef GKSOPERSTATE far *PGKSOPERSTATE;

/* StateValue		*/
#define INSTALL	    0xff	/* GKS INSTALLATION	*/
#define GKCL	    0x00	/* GKS CLOSED		*/
#define GKOP	    0x01	/* GKS OPEN		*/
#define WSOP	    0x02	/* WORKSTATION OPEN	*/
#define WSAC	    0x03	/* WORKSTATION ACTIVE	*/
#define SGOP	    0x04	/* SEGMENT OPEN 	*/

typedef struct _GKSERRSTATELIST {   /* GKS ERROR STATE LIST */
    GKSSEMAPHOR     SemHndlAccess;
    GKSSEMAPHOR     SemHndlRun;
    unsigned short  ErrorState;
    GKSFILE	    *ErrorFile;
    unsigned short  WrksID;	    /* queue overflow */
    unsigned short  InClass;
    unsigned short  DeviceNum;
} GKSERRSTATELIST;
typedef GKSERRSTATELIST far *PGKSERRSTATELIST;
					    /* ErrorState */
#define OFF	    0x00
#define ON	    0x01

typedef struct _GKSDESCTBL {	    /* GKS DESCRIPTION TABLE */
    GKSSEMAPHOR     SemHndlAccess;
    unsigned short  Level;
    unsigned short  WrksTypesAvail;
    char	    WrksTypeList [WRKSMAX] [NAMELENGHT];
    unsigned short  SimOpenWrksMax;
    unsigned short  SimActvWrksMax;
    unsigned short  WrksAssocSegMax;
    unsigned short  NmTrMax;
} GKSDESCTBL;
typedef GKSDESCTBL far *PGKSDESCTBL;

/* Level */
#define a0	    0x00
#define b0	    0x01
#define c0	    0x02
#define a1	    0x03
#define b1	    0x04
#define c1	    0x05
#define a2	    0x06
#define b2	    0x07
#define c2	    0x08

typedef struct _POINTF {
    double	x;  /* x coordinate */
    double	y;  /* y coordinate */
} POINTF;
typedef POINTF far *PPOINTF;

typedef struct _LINEF {
    POINTF	B;  /* begin point */
    POINTF	E;  /* end point   */
} LINEF;
typedef LINEF far *PLINEF;

typedef struct _LINEL {
    POINTL	B;  /* begin point */
    POINTL	E;  /* end point   */
} LINEL;
typedef LINEL far *PLINEL;

typedef struct _NORMTRANS {
    unsigned short  Number;
    LINEF	    W;
    LINEF	    V;
    unsigned short  Clip;
} NORMTRANS;
typedef NORMTRANS far *PNORMTRANS;

#define NOCLIP	    0x00
#define CLIP	    0x01

typedef struct _GKSSTATELIST {	  /* GKS STATE LIST - not completed */
    GKSSEMAPHOR     SemHndlAccess;
    GKSSEMAPHOR	    SemHndlRun;
    char	    WrksOpenSet [OWRKSMAX] [NAMELENGHT];
    char	    WrksActvSet [AWRKSMAX] [NAMELENGHT];
    unsigned short  NmTrCur;
    NORMTRANS	    NmTr [NMTRMAX];
    /*		.
		.		*/
} GKSSTATELIST;
typedef GKSSTATELIST far *PGKSSTATELIST;


typedef struct _WRKSDESCTBL { /* WORKSTATION DESCRIPTION TABLE - not completed */
    GKSSEMAPHOR     SemHndlAccess;
    char	    WrksType [NAMELENGHT];
    char	    WrksDrvr [DRVRLENGHT];
    unsigned short  WrksCategory;
    unsigned short  DevCoordUnits;
    POINTF	    MaxLenUn;
    POINTL	    MaxDevUn;
    /*		.
		.		*/
} WRKSDESCTBL;
typedef WRKSDESCTBL far *PWRKSDESCTBL;

/* WrksCategory */
#define OUTPUT	    0x00
#define INPUT	    0x01
#define OUTIN	    0x02
#define WISS	    0x03
#define MO	    0x04
#define MI	    0x05

/* DevCoordUnit */
#define METRES	    0x00
#define OTHER	    0x01

typedef struct _WRKSSTATELIST { /* WORKSTATION STATE LIST - not completed */
    GKSSEMAPHOR     SemHndlAccess;
    GKSSEMAPHOR     SemHndlRun;
    char	    WrksID [NAMELENGHT];
    HAB 	    ConnectID;
    char	    WrksType [NAMELENGHT];
    unsigned short  WrksState;
    char	    StoredSgmtList [SEGMAX] [NAMELENGHT];
    /* COLOUR TABLE */
    short	    ColourIndex;
    /* WORKSTATION TRANSFORMATION */
    unsigned short  WrksTransfUpdtSt;
    LINEF	    WrksWinReq;
    LINEF	    WrksWinCurrent;
    LINEF	    WrksViewReq;
    LINEF	    WrksViewCurrent;
    /* MISCELLANEOUS */
    POINTL	    GrCursorPos;
} WRKSSTATELIST;
typedef WRKSSTATELIST far *PWRKSSTATELIST;

/* WrksState */
#define INACTIVE    0x00
#define ACTIVE	    0x01

/* WrksTransfUpdtSt */
#define NOTPENDING  0x00
#define PENDING     0x01

typedef struct _GKSTRANSFER {
    GKSSEMAPHOR     SemHndlAccess;
    GKSSEMAPHOR     SemHndlRun;
    unsigned short  SrvNr;
    int 	    p1;
    int 	    p2;
    int 	    p3;
    POINTF	    pf;
    POINTL	    pl;
    PPOINTL	    ppl;
    PPOINTF	    ppf;
    LINEF	    lf;
    LINEL	    ll;
} GKSTRANSFER;
typedef GKSTRANSFER far *PGKSTRANSFER;

/* SrvNr */
#define GM_CLOSEGKS 0x00
#define GM_OPENWRKS 0x01
#define GM_CLOSWRKS 0x02
#define GM_ACTVWRKS 0x03
#define GM_DEACWRKS 0x04

/* User Defined Window Messages */
#define WM_USER_PAINT	0x1000
