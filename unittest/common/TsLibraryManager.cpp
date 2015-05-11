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

#include "TsLibraryManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION( TsLibraryManager );

void TsLibraryManager::setUp()
{
  lib1 = new te::common::Library("terralib_common_d");
  lib2 = new te::common::Library("terralib_dataaccess_d");

  ptr_Lib1.reset(lib1);
  ptr_Lib2.reset(lib2);

  te::common::LibraryManager::getInstance().add("1", ptr_Lib1);
  te::common::LibraryManager::getInstance().add("2", ptr_Lib2);
}

void TsLibraryManager::tearDown()
{
}

void TsLibraryManager::tcAdd()
{
  CPPUNIT_ASSERT(ptr_Lib1.get() != 0);
  CPPUNIT_ASSERT(ptr_Lib2.get() != 0);
}

void TsLibraryManager::tcFind()
{
  CPPUNIT_ASSERT(ptr_Lib1 == te::common::LibraryManager::getInstance().find("1"));
  CPPUNIT_ASSERT(ptr_Lib2 == te::common::LibraryManager::getInstance().find("2"));
}