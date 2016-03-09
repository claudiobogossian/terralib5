/*
 * File: AttributeFill.i
 *
 * AttributeFill module specific to Lua
 */

#define TEATTRIBUTEFILLEXPORT

%{
#include "terralib/attributefill/VectorToVectorOp.h"
#include "terralib/attributefill/VectorToVectorMemory.h"
#include "terralib/attributefill/RasterToVector.h"
%}

%nspace te::attributefill::VectorToVectorOp;
%nspace te::attributefill::VectorToVectorMemory;
%nspace te::attributefill::RasterToVector;

#ifdef SWIGLUA
%{
#include <boost/make_shared.hpp>	
%}
// TODO: ITS IS MADE BECAUSE ENUM 'te::attributefill::OperationType' THERE IS A KEY REDEFINED (REVIEW)
%rename (VALUE_OPERATION) te::attributefill::VALUE;

// TODO: THIS METHOD IS IGNORED BECAUSE SWIG DO NOT KNOW THIS TYPE 'std::map<std::string, std::vector<te::attributefill::OperationType> >'
%ignore te::attributefill::VectorToVectorOp::setParams(const std::map<std::string, std::vector<te::attributefill::OperationType> >&, std::vector<std::string>);

// TODO: THIS 3 METHODS ARE IGNORED BECAUSE THERE IS NOT THEIR IMPLEMENTATION IN CONCRETE CLASS (REVIEW)
%ignore te::attributefill::VectorToVectorMemory::setInput(te::map::AbstractLayerPtr, te::map::AbstractLayerPtr);
%ignore te::attributefill::VectorToVectorMemory::setParams(const std::map<te::dt::Property*, std::vector<std::string> >&);
%ignore te::attributefill::VectorToVectorMemory::setOutput(te::da::DataSourcePtr, std::string);

#endif //SWIGLUA

%include "terralib/attributefill/Enums.h"
%include "terralib/attributefill/VectorToVectorOp.h"
%include "terralib/attributefill/VectorToVectorMemory.h"
%include "terralib/attributefill/RasterToVector.h"