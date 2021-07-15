

#include "DebugPrint.h"

template <class T>

void _DebugPrint( std::vector<T> list, bool newLine )
{
    typename std::vector<T>::iterator iter = list.begin();

    for( int size = list.size(); size > 1; size-- )
        Serial.print(*++iter);

    if (newLine)
        Serial.println(*++iter);
    else
        Serial.print(*++iter);
}