#ifndef _TYPES_H
#define _TYPES_H

#if 0

#elif defined __GNUC__

// gcc / g++ (the GNU compiler)

typedef unsigned long long int Uint64;
typedef unsigned int           Uint32;
typedef unsigned short int     Uint16;
typedef unsigned char          Uint8;

typedef signed long long int Sint64;
typedef signed int           Sint32;
typedef signed short int     Sint16;
typedef signed char          Sint8;

#elif defined _MSC_VER

//Micosoft Visual C++

typedef unsigned __int64 Uint64;
typedef unsigned __int32 Uint32;
typedef unsigned __int16 Uint16;
typedef unsigned __int8  Uint8;
typedef signed __int64 Sint64;
typedef signed __int32 Sint32;
typedef signed __int16 Sint16;
typedef signed __int8  Sint8;

#else

//unrecognized platform
//will guess type names
//and double-check in types.cpp

typedef unsigned __int64 Uint64;
typedef unsigned __int32 Uint32;
typedef unsigned __int16 Uint16;
typedef unsigned __int8  Uint8;
typedef signed   __int64 Sint64;
typedef signed   __int32 Sint32;
typedef signed   __int16 Sint16;
typedef signed   __int8  Sint8;

#endif
#endif
