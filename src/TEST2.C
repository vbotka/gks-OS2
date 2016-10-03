#define INCL_DOSPROCESS
#include <os2.h>
#include <stdio.h>
#include <process.h>

main ( int argc, char *argv[] )
{
while ( argc-- )
    printf ( "%d %s\n", argc, argv[argc] ) ;
exit (0) ;
}
