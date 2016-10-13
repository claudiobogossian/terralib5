/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/examples/core/filesystem/main.cpp

  \brief Examples for the Terralib File System Module

  \author Matheus Cavassan Zaglia
*/

// STL
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>

// TerraLib
#include <terralib/core/encoding/CharEncoding.h>
#include <terralib/core/filesystem/FileSystem.h>

int main(int argc, char *argv[])
{

  /* Initialize an utf8 string with a path */
  std::string utf8_path =
      te::core::CharEncoding::toUTF8("../example/filesystem");

  /* Create a directory and make some verifications about the path */
  if (te::core::FileSystem::createDirectory(utf8_path))
    std::cout << "Directory created!" << std::endl;

  if (te::core::FileSystem::exists(utf8_path))
    std::cout << "Path exists!" << std::endl;

  if(te::core::FileSystem::isDirectory(utf8_path))
    std::cout << "Path is a directory!" << std::endl;

  if(te::core::FileSystem::isEmpty(utf8_path))
    std::cout << "Path is empty!" << std::endl;

  /* Creating files to test some functions */
  std::string file1_str = utf8_path + "/file1.txt",
              file2_str = utf8_path + "/file2.sh",
              file3_str = utf8_path + "/file3.txt";
  std::ofstream file1(file1_str);
  std::ofstream file2(file2_str);
  file1.close();
  file2.close();

  /* Return the file extension */
  std::string stringTxt = te::core::FileSystem::extension(file1_str);
  std::cout << "File1 has extension: " << stringTxt << std::endl;
  std::string stringSh = te::core::FileSystem::extension(file2_str);
  std::cout << "File1 has extension: " << stringSh << std::endl;

  /* Rename and copy files */
  te::core::FileSystem::rename(file1_str, file3_str);
  te::core::FileSystem::copyFile(file3_str, file1_str);

  /* Check if is a regular file */
  if (te::core::FileSystem::isRegularFile(file3_str))
    std::cout << "File3 is a regular file!" << std::endl;

  /* Create a list with the files in directory, check the list size and remove
   * the files */
  std::vector<std::string> files =
      te::core::FileSystem::listDirectory(utf8_path);

  std::cout << "Files size is " << files.size() << std::endl;

  for(size_t i = 0; i < files.size(); ++i)
    te::core::FileSystem::remove(files[i]);

  if (te::core::FileSystem::remove(utf8_path))
    std::cout << "utf8 path was removed!" << std::endl;

}
