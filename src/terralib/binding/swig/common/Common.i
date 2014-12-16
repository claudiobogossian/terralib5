/*
 * File: DataAccess.i
 *
 * Data Access
 */

#define TECOMMONEXPORT
 
%{
  #include "terralib/common/Enums.h"
%}
 
%nspace te::common::AccessPolicy;
%nspace te::common::TraverseType;
%nspace te::common::LoggerConfigurationType;
%nspace te::common::MeasureType;
%nspace te::common::Hemisphere;
%nspace te::common::MachineByteOrder;

%include "terralib/common/Enums.h"
