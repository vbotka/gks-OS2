PROJ = t1
PROJFILE = t1.mak
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

OBJS  = T1.obj
SBRS  = T1.sbr

all: $(PROJ).exe

.SUFFIXES: .c .sbr .obj

T1.obj : T1.C

T1.sbr : T1.C


$(PROJ).bsc : $(SBRS)
	$(PWBRMAKE) @<<
$(BRFLAGS) $(SBRS)
<<

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


.c.sbr :
!IF $(DEBUG)
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_D) /FR$@ $<
!ELSE
	$(CC) /Zs $(CFLAGS_G) $(CFLAGS_R) /FR$@ $<
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
