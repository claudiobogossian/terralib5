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
  \file terralib/unittest/ogc-wcs/MockCurlWrapper.h

  \brief A mock class to simulate the use of Lib Curl.

  \author Emerson Moraes
 */

#include <terralib/ws/core/CurlWrapper.h>

// GMock
#include <gmock/gmock.h>

class MockCurlWrapper : public te::ws::core::CurlWrapper
{
public:

  MockCurlWrapper() = default;

  virtual ~MockCurlWrapper() = default;

  MOCK_METHOD2(downloadFile, void(const std::string&, const std::string&));

  MOCK_METHOD2(downloadFile, void(const std::string&, std::FILE*));
};
