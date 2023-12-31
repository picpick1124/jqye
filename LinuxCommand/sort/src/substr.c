#include "sort.h"

void substr(const char *s,char *str )
{
    int i, j, len;
    extern int pos1, pos2;

    len = strlen( s );

    if( pos2 > 0 && len > pos2 )
        len = pos2;
    else if( pos2 > 0 && len < pos2 )
        error( "substr: string too short");

    for( j = 0, i = pos1; i < len; i++, j++ )
        str[ j ] = str[ i ];

    str[ j ] = '\0';
}