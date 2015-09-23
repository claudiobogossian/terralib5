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
  \file terralib/vp/InputParams.cpp
 
  \brief A structure to hold the input parameters of vector processing.
*/

// TerraLib
#include "InputParams.h"

te::vp::InputParams::InputParams()
  :
  m_inputDataSetName(""),
  m_inputDataSet(0),
  m_inputDataSetType(0),
  m_inputRestriction(0)
{
}

te::vp::InputParams::InputParams(const InputParams& rhs)
  :
  m_inputDataSetName(""),
  m_inputDataSet(0),
  m_inputDataSetType(0),
  m_inputRestriction(0)
{
  m_inputDataSource = rhs.m_inputDataSource;
  m_inputDataSetName = rhs.m_inputDataSetName;
  m_inputDataSet = rhs.m_inputDataSet;
  m_inputDataSetType = rhs.m_inputDataSetType;
  m_inputRestriction = rhs.m_inputRestriction;
}

te::vp::InputParams::~InputParams()
{
  clear();
}

te::vp::InputParams& te::vp::InputParams::operator=(const InputParams& rhs)
{
  if(this != &rhs)
  {
    clear();

    m_inputDataSource = rhs.m_inputDataSource;
    m_inputDataSetName = rhs.m_inputDataSetName;
    m_inputDataSet = rhs.m_inputDataSet;
    m_inputDataSetType = rhs.m_inputDataSetType;
    m_inputRestriction = rhs.m_inputRestriction;
  }

  return *this;
}

void te::vp::InputParams::clear()
{
  m_inputDataSource.reset();
  m_inputDataSetName = "";
  m_inputDataSet = 0;
  m_inputDataSetType = 0;
  m_inputRestriction = 0;
}
