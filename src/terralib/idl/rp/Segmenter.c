#include "Segmenter.h"

IDL_VPTR SegmenterRG(int argc, IDL_VPTR *argv)
{
  IDL_VPTR inputArray = argv[ 0 ];
    
  IDL_ENSURE_ARRAY( inputArray );
  
  IDL_VPTR outputArray;
  IDL_MEMINT dims[] = { 10, 10 };
  double* outputArrayDataPtr = (double*)IDL_MakeTempArray( inputArray->type, 
    inputArray->value.arr->n_dim, inputArray->value.arr->dim,
    IDL_ARR_INI_NOP, &outputArray );
    
  UCHAR* inputPtr = inputArray->value.arr->data;
  UCHAR* outputPtr = outputArray->value.arr->data;
  
  IDL_MEMINT n = 0;
  for ( n = inputArray->value.arr->arr_len ; n--; n )
    *outputPtr++ = *inputPtr++;
  
  return( outputArray );
}
