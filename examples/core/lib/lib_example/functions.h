#ifndef __TERRALIB_EXAMPLE_LIBRARY_FUNCTIONS_H__
#define __TERRALIB_EXAMPLE_LIBRARY_FUNCTIONS_H__

// TerraLib
#include <terralib/Defines.h>

#ifdef TEEXEMPCOREDLL
  #define TECOREEXPORT TE_DLL_EXPORT
#else
  #define TECOREEXPORT TE_DLL_IMPORT
#endif

extern "C"
{
  /* Calculates the fatorial of a number */
  TECOREEXPORT int fatorial(int v);
}

#endif //__TERRALIB_EXAMPLE_LIBRARY_FUNCTIONS_H__
