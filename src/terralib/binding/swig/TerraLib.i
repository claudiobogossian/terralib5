/*
 * TerraLib.i
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
#ifdef SWIGJAVA
%include java/TeTypeMaps.i
#endif

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

#include "terralib/binding/swig/QtInitializer.h"

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

// Wrap functions
void Initialize() throw (te::common::Exception);

void Finalize();

%include ObjectId.i
%include DataSet.i
%include DataSource.i
%include Imager.i
%include Layer.i
%include RTree.i

%include "terralib/binding/swig/QtInitializer.h"
