#include "IDLLoad.h" 

#include "rp/Segmenter.h"
 
int IDL_Load(void)
{
  static IDL_SYSFUN_DEF2 SegmenterRG_FunctionAddr[] = { { SegmenterRG, 
    "TERRALIB_RP_SEGMENTER_RG", 1, 1, 0, 0 }, }; 
    
  int returnValue = IDL_SysRtnAdd( SegmenterRG_FunctionAddr, TRUE, 
    IDL_CARRAY_ELTS( SegmenterRG_FunctionAddr ) );
  
  return returnValue;  
}

