/*
 * File: DataAccess.i
 *
 * Data Access
 */

#define TECOMMONEXPORT

%{
  #include "terralib/common/Enums.h"
%}

//Python doesn't work with namespaces
#ifndef SWIGPYTHON

%nspace te::common::AccessPolicy;
%nspace te::common::TraverseType;
%nspace te::common::LoggerConfigurationType;
%nspace te::common::MeasureType;
%nspace te::common::Hemisphere;
%nspace te::common::MachineByteOrder;

#endif

%include "terralib/common/Enums.h"
