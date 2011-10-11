/* Part of HGEPP project, a HGE-rewrite https://github.com/kvakvs/hgepp
 * Based on Haaf's Game Engine 1.8.1 (C) 2003-2007, Relish Games http://hge.relishgames.com
 */
#pragma once

#define HGE_VERSION 0x180

#ifdef _UNICODE
    #error Please change CHARACTER SET in project settings to MULTIBYTE, not UNICODE!
#endif

// CMake adds PROJECTNAME_EXPORTS when compiles DLL
#ifdef hgepp_EXPORTS
    #define HGEDLL
#endif
//------

#ifdef HGEDLL
    #ifdef HGEDLL_EXPORT
        #define HGE_EXPORT  __declspec(dllexport)
    #else
        #define HGE_EXPORT  __declspec(dllimport)
    #endif
#else
    #define HGE_EXPORT
#endif

#define HGE_CALL  __stdcall

#if defined(_IA64)||defined(_M_IA64)||defined(_M_X64)||defined(__amd64__)
    #define HGE_64BIT
#endif
