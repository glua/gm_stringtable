#include "proc.h"


#ifdef _WIN32

#include <Windows.h>

#elif defined(__linux__)

#include <dlfcn.h>

#define GetModuleHandleA(name) dlopen(name, RTLD_LAZY | RTLD_NOLOAD)
#define GetProcAddress dlsym

#else
#error "unsupported OS"
#endif

libsym_return libsym_intrnl(void **symbol, const char *libname, const char *interfacename)
{

  void *lib = GetModuleHandleA(libname);

  if(symbol) *symbol = 0;

  if(!lib)
    return LIBSYM_NODLL;

  void *retval = GetProcAddress(lib, interfacename);

  if(!retval)
    return LIBSYM_NOSYMBOL;


  if(symbol) *symbol = retval;

  return LIBSYM_SUCCESS;

}
