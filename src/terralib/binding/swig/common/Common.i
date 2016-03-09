/*
 * File: DataAccess.i
 *
 * Data Access
 */

#define TECOMMONEXPORT

%{
#include "terralib/common/Enums.h"
#include "terralib/common/PlatformUtils.h"
#include "terralib/common/Version.h"
#include "terralib/common/BoostUtils.h"
%}

//Python doesn't work with namespaces
#ifndef SWIGPYTHON

%nspace te::common::AccessPolicy;
%nspace te::common::TraverseType;
%nspace te::common::LoggerConfigurationType;
%nspace te::common::MeasureType;
%nspace te::common::Hemisphere;
%nspace te::common::MachineByteOrder;
%nspace te::common::Version;

#endif

%include "terralib/common/Enums.h"
%include "terralib/common/PlatformUtils.h"
%include "terralib/common/Version.h"
%include "terralib/common/BoostUtils.h"
