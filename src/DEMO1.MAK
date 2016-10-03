PROJ = DEMO1
PROJFILE = DEMO1.MAK
DEBUG = 1

PWBRMAKE  = pwbrmake
NMAKEBSC1  = set
NMAKEBSC2  = nmake
LINKER	= link
ILINK  = ilink
LRF  = echo > NUL
BIND  = bind
RC  = rc
IMPLIB	= implib
LFLAGS_G  =  /NOI /ST:8192  /BATCH
LFLAGS_D  =  /CO /F /PACKC /PACKD  /PMTYPE:PM
LFLAGS_R  = /EXE /FAR /PACKC /PACKD /PMTYPE:PM
MAPFILE_D  = NUL
MAPFILE_R  = NUL
CC  = cl
CFLAGS_G  = /AL /W2 /G2 /Zp /BATCH
CFLAGS_D  = /Od /FPi87 /Zi /Gs
CFLAGS_R  = /Ot /Oi /Ol /Oe /Og /Gs
LLIBS_R  =  /NOD:LLIBCE LLIBCEP
LLIBS_D  =  /NOD:LLIBC7 LLIBCEP
ASM  = masm
AFLAGS_G  = /Mx /T
AFLAGS_D  = /Zi
CVFLAGS  =  /43

OBJS  = DEMO1.obj CLOSWRKS.obj OPENWRKS.obj

all: $(PROJ).exe

.SUFFIXES: .c .obj

DEMO1.obj : DEMO1.C gksdef.h gm.h

CLOSWRKS.obj : CLOSWRKS.C C:\C600\INCLUDE\os2.h C:\C600\INCLUDE\stddef.h\
	C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\string.h C:\C600\INCLUDE\process.h gksdef.h gm.h\
	C:\C600\INCLUDE\os2def.h C:\C600\INCLUDE\bse.h C:\C600\INCLUDE\pm.h\
	C:\C600\INCLUDE\bsedos.h C:\C600\INCLUDE\bsesub.h\
	C:\C600\INCLUDE\bseerr.h C:\C600\INCLUDE\bsedev.h\
	C:\C600\INCLUDE\pmwin.h C:\C600\INCLUDE\pmgpi.h\
	C:\C600\INCLUDE\pmdev.h C:\C600\INCLUDE\pmavio.h\
	C:\C600\INCLUDE\pmord.h C:\C600\INCLUDE\pmbitmap.h\
	C:\C600\INCLUDE\pmfont.h C:\C600\INCLUDE\pmshl.h

OPENWRKS.obj : OPENWRKS.C C:\C600\INCLUDE\os2.h C:\C600\INCLUDE\stddef.h\
	C:\C600\INCLUDE\stdio.h C:\C600\INCLUDE\stdlib.h\
	C:\C600\INCLUDE\string.h C:\C600\INCLUDE\process.h gksdef.h gm.h\
	C:\C600\INCLUDE\os2def.h C:\C600\INCLUDE\bse.h C:\C600\INCLUDE\pm.h\
	C:\C600\INCLUDE\bsedos.h C:\C600\INCLUDE\bsesub.h\
	C:\C600\INCLUDE\bseerr.h C:\C600\INCLUDE\bsedev.h\
	C:\C600\INCLUDE\pmwin.h C:\C600\INCLUDE\pmgpi.h\
	C:\C600\INCLUDE\pmdev.h C:\C600\INCLUDE\pmavio.h\
	C:\C600\INCLUDE\pmord.h C:\C600\INCLUDE\pmbitmap.h\
	C:\C600\INCLUDE\pmfont.h C:\C600\INCLUDE\pmshl.h


$(PROJ).bsc : 

$(PROJ).exe : $(OBJS)
!IF $(DEBUG)
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_D)
$(LLIBS_G: = +^
) +
$(LLIBS_D: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_D);
<<
!ELSE
	$(LRF) @<<$(PROJ).lrf
$(RT_OBJS: = +^
) $(OBJS: = +^
)
$@
$(MAPFILE_R)
$(LLIBS_G: = +^
) +
$(LLIBS_R: = +^
) +
$(LIBS: = +^
)
$(DEF_FILE) $(LFLAGS_G) $(LFLAGS_R);
<<
!ENDIF
!IF $(DEBUG)
	$(LINKER) @$(PROJ).lrf
!ELSE
	$(LINKER) @$(PROJ).lrf
!ENDIF


.c.obj :
!IF $(DEBUG)
	$(CC) /c $(CFLAGS_G) $(CFLAGS_D) /Fo$@ $<
!ELSE
	$(CC) /c $(CFLAGS_G) $(CFLAGS_R) /Fo$@ $<
!ENDIF


run: $(PROJ).exe
	$(PROJ).exe $(RUNFLAGS)

debug: $(PROJ).exe
	CVP $(CVFLAGS) $(PROJ).exe $(RUNFLAGS)
