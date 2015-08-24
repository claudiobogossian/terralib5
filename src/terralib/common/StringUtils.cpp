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
  \file terralib/common/StringUtils.cpp

  \brief Utility functions for dealing with strings.

  \ingroup common
*/

#include "StringUtils.h"

void te::common::GetAccentuatedUpperVector(std::vector<std::string> & vecUpperIn, std::vector<std::string> & vecUpperOut)
{
  static std::vector<std::string> upperIn;
  static std::vector<std::string> upperOut;

  if (upperIn.empty() == true || upperOut.empty() == true)
  {
    upperIn.clear();
    upperOut.clear();

    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");
    upperIn.push_back("�");


    upperOut.push_back("A");
    upperOut.push_back("E");
    upperOut.push_back("I");
    upperOut.push_back("O");
    upperOut.push_back("U");
    upperOut.push_back("A");
    upperOut.push_back("E");
    upperOut.push_back("I");
    upperOut.push_back("O");
    upperOut.push_back("U");
    upperOut.push_back("A");
    upperOut.push_back("E");
    upperOut.push_back("I");
    upperOut.push_back("O");
    upperOut.push_back("U");
    upperOut.push_back("A");
    upperOut.push_back("E");
    upperOut.push_back("I");
    upperOut.push_back("O");
    upperOut.push_back("U");
    upperOut.push_back("A");
    upperOut.push_back("O");
    upperOut.push_back("C");
  }

  vecUpperIn = upperIn;
  vecUpperOut = upperOut;

}

void te::common::GetAccentuatedLowerVector(std::vector<std::string> & vecLowerIn, std::vector<std::string> & vecLowerOut)
{
  static std::vector<std::string> lowerIn;
  static std::vector<std::string> lowerOut;

  if (lowerIn.empty() == true || lowerOut.empty() == true)
  {
    lowerIn.clear();
    lowerOut.clear();

    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");
    lowerIn.push_back("�");

    std::string str = "�";
    lowerIn.push_back(str);

    lowerOut.push_back("a");
    lowerOut.push_back("e");
    lowerOut.push_back("i");
    lowerOut.push_back("o");
    lowerOut.push_back("u");
    lowerOut.push_back("a");
    lowerOut.push_back("e");
    lowerOut.push_back("i");
    lowerOut.push_back("o");
    lowerOut.push_back("u");
    lowerOut.push_back("a");
    lowerOut.push_back("e");
    lowerOut.push_back("i");
    lowerOut.push_back("o");
    lowerOut.push_back("u");
    lowerOut.push_back("a");
    lowerOut.push_back("e");
    lowerOut.push_back("i");
    lowerOut.push_back("o");
    lowerOut.push_back("u");
    lowerOut.push_back("a");
    lowerOut.push_back("o");
    lowerOut.push_back("c");
  }

  vecLowerIn = lowerIn;
  vecLowerOut = lowerOut;
}

void te::common::GetEspecialCharsFixVector(std::vector<std::string> & especialIn, std::vector<std::string> & especialOut)
{
  especialIn.push_back("�");
  especialIn.push_back("�");
  especialIn.push_back("�");
  especialIn.push_back("�");
  especialIn.push_back("�");

  especialOut.push_back("a");
  especialOut.push_back("o");
  especialOut.push_back("1");
  especialOut.push_back("2");
  especialOut.push_back("3");
}

std::string te::common::ReplaceSpecialChars(const std::string& str, bool& changed)
{

  changed = false;

  std::vector<std::string> upperIn;
  std::vector<std::string> upperOut;
  std::vector<std::string> lowerIn;
  std::vector<std::string> lowerOut;
  std::vector<std::string> especialIn;
  std::vector<std::string> especialOut;

  GetAccentuatedUpperVector(upperIn, upperOut);
  GetAccentuatedLowerVector(lowerIn, lowerOut);
  GetEspecialCharsFixVector(especialIn, especialOut);


  std::string outputStr = str;
  for (unsigned int i = 0; i < outputStr.size(); ++i)
  {
    std::string value = "";
    value += outputStr[i];
    for (unsigned int j = 0; j < upperIn.size(); ++j)
    {
      if (outputStr[i] == upperIn[j][0])
      {
        outputStr[i] = upperOut[j][0];
        changed = true;
        break;
      }
    }

    for (unsigned int j = 0; j < lowerIn.size(); ++j)
    {
      if (value == lowerIn[j])
      {
        outputStr[i] = lowerOut[j][0];
        changed = true;
        break;
      }
    }

    for (unsigned int j = 0; j < especialIn.size(); ++j)
    {
      if (outputStr[i] == especialIn[j][0])
      {
        outputStr[i] = especialOut[j][0];
        changed = true;
        break;
      }
    }

  }

  return outputStr;

}