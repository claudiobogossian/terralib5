/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// TerraLib
#include "TsLibrary.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsLibrary);

void TsLibrary::setUp()
{
  lib1 = new te::common::Library("terralib_mod_common");
  lib2 = new te::common::Library("terralib_mod_postgis");
  lib3 = new te::common::Library("terralib_mod_ogr");
  //lib1 = new te::common::Library("terralib_common_d");
  //lib2 = new te::common::Library("terralib_postgis_d");
  //lib3 = new te::common::Library("terralib_ogr_d");
}

void TsLibrary::tearDown()
{
  delete lib1;
  delete lib2;
  delete lib3;
}

void TsLibrary::tcLoad()
{
  lib1->load();
  CPPUNIT_ASSERT(lib1->isLoaded());
  lib2->load();
  CPPUNIT_ASSERT(lib2->isLoaded());
}

void TsLibrary::tcUnload()
{
  lib2->load();
  CPPUNIT_ASSERT(lib2->isLoaded());
  lib2->unload();
  CPPUNIT_ASSERT(!(lib2->isLoaded()));
}

void TsLibrary::tcGetFileName()
{
  CPPUNIT_ASSERT(lib3->getFileName() == "terralib_ogr_d");
}

void TsLibrary::tcGetNativeName()
{
  #if TE_PLATFORM == TE_PLATFORMCODE_LINUX
    CPPUNIT_ASSERT(lib3->getNativeName(lib3->getFileName()) =="libterralib_ogr_d.so");
  #elif TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    CPPUNIT_ASSERT(lib3->getNativeName(lib3->getFileName()) == "terralib_ogr_d.dll");
  #elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
    CPPUNIT_ASSERT(lib3->getNativeName(lib3->getFileName()) == "terralib_ogr_d.dylib");
  #endif
}

void TsLibrary::tcAddSearchDir()
{
  std::string path1= lib3->getSearchPath();
  lib3->addSearchDir("TESTE");
  CPPUNIT_ASSERT(path1 != lib3->getSearchPath());
}

void TsLibrary::tcResetSearchPath()
{
  std::string originalPath= lib3->getSearchPath();
  lib3->addSearchDir("TESTE");
  std::string modifiedPath= lib3->getSearchPath();
  lib3->resetSearchPath();
  std::string restoredPath= lib3->getSearchPath();

  //check to see if the path was successfully modified
  CPPUNIT_ASSERT(!(originalPath.compare(modifiedPath)));
  //check to see if the path was successfully restored
  CPPUNIT_ASSERT(originalPath.compare(restoredPath));  
}

void TsLibrary::tcGetAddress()
{
  lib2->load();
  void* libFunction = lib2->getAddress("CppPluginGetInstance");
  CPPUNIT_ASSERT(libFunction != NULL);
}