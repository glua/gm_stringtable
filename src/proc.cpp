#include "proc.h"


#ifdef _WIN32

#include <Windows.h>

#define lib_open(name) ((void *)GetModuleHandleA(name))
#define lib_sym(lib, symname) ((void *)GetProcAddress((HMODULE)lib, symname))

#elif defined(__linux__)

#include <dlfcn.h>

#define lib_open(name) dlopen(name, RTLD_LAZY | RTLD_NOLOAD)
#define lib_sym dlsym

#else
#error "unsupported OS"
#endif

libsym_return libsym_intrnl(void **symbol, const char *libname, const char *interfacename)
{

  void *lib = lib_open(libname);

  if(symbol) *symbol = 0;

  if(!lib)
    return LIBSYM_NODLL;

  void *retval = lib_sym(lib, interfacename);

  if(!retval)
    return LIBSYM_NOSYMBOL;


  if(symbol) *symbol = retval;

  return LIBSYM_SUCCESS;

}
