/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

  This file is part of the TerraLib - a Framework for building GIS enabled
  applications.

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
  \file terralib/unittest/core/filesystem/TsFileSystem.cpp

  \brief A test suit for the TerraLib Core FileSystem Module.

  \author Matheus Cavassan Zaglia.
 */

// STL
#include <string>
#include <fstream>

// TerraLib
#include <terralib/core/encoding/CharEncoding.h>
#include <terralib/core/filesystem/FileSystem.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(filesystem)

BOOST_AUTO_TEST_CASE(filesystem_test)
{
  std::string utf8_path =
      te::core::CharEncoding::toUTF8("../unittest/filesystem");

  BOOST_CHECK(te::core::FileSystem::createDirectory(utf8_path));
  BOOST_CHECK(te::core::FileSystem::exists(utf8_path));
  BOOST_CHECK(te::core::FileSystem::isDirectory(utf8_path));
  BOOST_CHECK(te::core::FileSystem::isEmpty(utf8_path));

  // creating files to test some functions
  std::string file1_str = utf8_path + "/file1.txt",
              file2_str = utf8_path + "/file2.sh",
              file3_str = utf8_path + "/file3.txt";
  std::ofstream file1(file1_str);
  std::ofstream file2(file2_str);
  file1.close();
  file2.close();

  BOOST_CHECK(te::core::FileSystem::extension(file1_str) == ".txt");
  BOOST_CHECK(te::core::FileSystem::extension(file2_str) == ".sh");
  BOOST_CHECK_NO_THROW(te::core::FileSystem::rename(file1_str, file3_str));
  BOOST_CHECK_NO_THROW(te::core::FileSystem::copyFile(file3_str, file1_str));

  BOOST_CHECK(te::core::FileSystem::isRegularFile(file3_str));
  std::vector<std::string> files =
      te::core::FileSystem::listDirectory(utf8_path);

  BOOST_CHECK(files.size() == 3);

  for(size_t i = 0; i < files.size(); ++i)
    BOOST_CHECK(te::core::FileSystem::remove(files[i]));

  BOOST_CHECK(te::core::FileSystem::remove(utf8_path));
}

BOOST_AUTO_TEST_SUITE_END()
