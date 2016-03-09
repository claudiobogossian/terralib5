#define TEMEMORYEXPORT

AUTO_PTR_TYPEMAPS(te::mem::DataSetItem)

%{
#include "terralib/memory/DataSetItem.h"
#include "terralib/memory/DataSet.h"
%}

#ifdef SWIGLUA
%ignore te::mem::new_clone(const DataSetItem&);
#endif

%nspace te::mem::DataSetItem;
%nspace te::mem::DataSet;

%include "terralib/memory/DataSetItem.h"
%include "terralib/memory/DataSet.h"
