#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <process.h>

#define MAXSPEC 80

char parm[] = { "NAME.EXE\0PARM1 PARM2 PARM3" } ;
char namExe[] = { "D:\\USERS\\BOTKA\\GM\\TEST2.EXE" } ;
char namFail [MAXSPEC] ;
RESULTCODES resChild ;

main ()
{
if ( DosExecPgm ( namFail,
		  MAXSPEC,
		  EXEC_BACKGROUND,
		  parm,
		  0,
		  &resChild,
		  namExe )) {
    printf ( "Can't execute\n" ) ;
    }
exit (0) ;
}
