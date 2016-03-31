#pragma once

/* the user must access only these
 * two functions for safety however getAt(...)
 * can colide with other headers so use with caution
 * or warp when using C++
 * */

unsigned int                split(const char*, char);
const unsigned int          getSize(void);
char*                       getAt(int);
