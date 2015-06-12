/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file TsMapDisplay.cpp
 
  \brief Test suite for the Qt Map Display widget.
 */


// Unit-Test TerraLib
#include "TsMapDisplay.h"

// TerraLib
#include <terralib/color.h>
#include <terralib/common.h>
#include <terralib/dataaccess.h>
#include <terralib/geometry.h>
#include <terralib/maptools.h>
#include <terralib/postgis.h>
#include <terralib/qt/widgets.h>

// Qt
#include <QtGui/QApplication>

// STL
#include <ctime>

CPPUNIT_TEST_SUITE_REGISTRATION( TsMapDisplay );

void TsMapDisplay::setUp()
{
}

void TsMapDisplay::tearDown()
{
}

void TsMapDisplay::tcSetExtent()
{
#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);
  return;
#endif
}

void TsMapDisplay::tcShowLayer()
{
#ifdef TE_COMPILE_ALL
  int argc = 0;
  QApplication app(argc, 0);
  return;
#endif
}

