#ifndef CONV_H
#define CONV_H

#ifndef _WIN32

#define __cdecl __attribute__((cdecl))
#define __stdcall __attribute__((stdcall))
#define __fastcall __attribute__((fastcall))
#define __thiscall __attribute__((thiscall))

#define _cdecl __attribute__((cdecl))
#define _stdcall __attribute__((stdcall))
#define _fastcall __attribute__((fastcall))
#define _thiscall __attribute__((thiscall))

#endif

#endif // CONV_H
