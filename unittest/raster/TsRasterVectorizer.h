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
  \file TsRaterVectorizer.h

  \brief A test suit for the Raster Vectorizer Class.
 */

#ifndef __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERVECTORIZER_H
#define __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERVECTORIZER_H

#include <terralib/raster.h>
#include <boost/shared_ptr.hpp>

// cppUnit
#include <cppunit/extensions/HelperMacros.h>

/*!
  \class TsRaterVectorizer

  \brief A test suit for the Raster Vectorizer class interface.

  ....
  <br>
  This test suite will check the Raster Vectorizer class
  </ul>
 */
class TsRasterVectorizer : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( TsRasterVectorizer );

  CPPUNIT_TEST(tcRasterVectorizer);

  CPPUNIT_TEST_SUITE_END();

  protected :

    void tcRasterVectorizer();

};

#endif  // __TERRALIB_UNITTEST_RASTER_INTERNAL_RASTERVECTORIZER_H

