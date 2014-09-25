/*
 * File: TerraLib.i
 *
 */

%module TerraLib

%warnfilter(401) Singleton;

/*
 * STL
 */
%include std_map.i
%include std_pair.i
%include std_string.i
%include std_vector.i
%include typemaps.i

/*
 * Te Type Maps
 */
%include java/TeTypeMaps.i

#define TEDATAACCESSEXPORT
#define TECOMMONEXPORT
#define TEPLUGINEXPORT

SWIG_JAVABODY_METHODS(protected, protected, SWIGTYPE)
%pragma(java) jniclassclassmodifiers = "class"

namespace std {
 %template(Str2StrMap) map<string, string>;
 %template(StrVector) vector<string>;
}

%{
// TerraLib includes
#include "terralib/common/TerraLib.h"
#include "terralib/plugin/PluginManager.h"

#include "terralib/binding/swig/java/QtInitializer.h"

static void Initialize() throw (te::common::Exception)
{
	TerraLib::getInstance().initialize();
  
	te::plugin::PluginManager::getInstance().loadAll();
} 
static void Finalize()
{
	te::plugin::PluginManager::getInstance().shutdownAll();
  te::plugin::PluginManager::getInstance().unloadAll();
	te::plugin::PluginManager::getInstance().clear();

	TerraLib::getInstance().finalize();
} 
%} 

// ---------------------------------
// Java Documentation
// ---------------------------------

%javamethodmodifiers Initialize() "
/**
 * Initializes the TerraLib modules.
 *
 * Make sure to call this function BEFORE using any resource of the TerraLib module.
 *
 * @throws java.io.IOException Any problem on modules initialization will raise an java.io.IOException.
 */
  public";
  
%javamethodmodifiers Finalize() "
/**
 * Finalizes the TerraLib modules.
 *
 * Make sure to call this function BEFORE finalize the execution of your application.
 */
  public";
 
%typemap(javaimports) TerraLib "
/** 
 * The main class of TerraLib module. Remember to call <i>Initialize</i> and <i>Finalize</i> methods, on the initialization and finalization of the execution 
 * of your application respectively.
 */"

// Wrap functions
void Initialize() throw (te::common::Exception);

void Finalize();

%include java/ObjectId.i
%include java/DataSet.i
%include java/DataSource.i
%include java/Imager.i
%include java/Layer.i
%include java/RTree.i

%include "java/QtInitializer.h"
