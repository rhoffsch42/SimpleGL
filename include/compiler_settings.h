#pragma once
#pragma warning( disable : 4244 ) 
#ifndef __clang__
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define Stringize( L )     #L 
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" $Line ") : Reminder: "
