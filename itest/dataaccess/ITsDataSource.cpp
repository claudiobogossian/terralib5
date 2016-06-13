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
  \file terralib/itest/dataaccess/ITsDataSource.cpp

  \brief A integration test suit for the TerraLib Data Access Module.

  \author Carolina Galvão dos Santos
*/

// TerraLib
#include <terralib/dataaccess.h>

// STL
#include <string>

// GTest and GMock
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::AtLeast;
using ::testing::_;
using ::testing::ReturnRef;
using ::testing::InSequence;

void CallDataSourceInfoMethods(te::da::DataSourceInfo* dataset, const std::string& id)
{
  dataset->setId(id);
  dataset->getId();
}

class MockClass : public te::da::DataSourceInfo
{
public:
  MOCK_CONST_METHOD0(getId, std::string& ());
  MOCK_METHOD1(setId, void (const std::string& id));
};

TEST(DataAccessTest, CanCallDataSourceInfoMethods) {

  MockClass mock;

  std::string id = "1234";

  {
    InSequence dummy;

    EXPECT_CALL(mock, setId(_))
      .Times(AtLeast(1));

    EXPECT_CALL(mock, getId())
      .Times(AtLeast(1))
      .WillOnce(ReturnRef(id));
  }

  CallDataSourceInfoMethods(&mock, id);
}
