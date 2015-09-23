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
 \file AlgorithmParams.cpp
 */

#include "../dataaccess/datasource/DataSource.h"
#include "AlgorithmParams.h"

te::vp::AlgorithmParams::AlgorithmParams():
  m_inputParams()
{
}

te::vp::AlgorithmParams::AlgorithmParams( std::vector<const te::vp::InputParams> inputParams,
                                          te::da::DataSource* outputDataSource,
                                          const std::string& outputDataSetName,
                                          const std::map<std::string, te::dt::AbstractData*>& specificParams)
  : m_inputParams(inputParams),
  m_outputDataSource(outputDataSource),
  m_outputDataSetName(outputDataSetName),
  m_specificParams(specificParams)
{
}

te::vp::AlgorithmParams::~AlgorithmParams()
{
}


std::vector<const te::vp::InputParams> te::vp::AlgorithmParams::getInputParams()
{
  return m_inputParams;
}

void te::vp::AlgorithmParams::setInputParams(std::vector<const te::vp::InputParams> inputParams)
{
  m_inputParams = inputParams;
}


te::da::DataSource* te::vp::AlgorithmParams::getOutputDataSource()
{
  return m_outputDataSource;
}

void te::vp::AlgorithmParams::setOutputDataSource(te::da::DataSource* outputDataSource)
{
  m_outputDataSource = outputDataSource;
}


const std::string& te::vp::AlgorithmParams::getOutputDataSetName()
{
  return m_outputDataSetName;
}

void te::vp::AlgorithmParams::setOutputDataSetName(const std::string& outputDataSetName)
{
  m_outputDataSetName = outputDataSetName;
}


const std::map<std::string, te::dt::AbstractData*>& te::vp::AlgorithmParams::getSpecificParams()
{
  return m_specificParams;
}

void te::vp::AlgorithmParams::setSpecificParams(const std::map<std::string, te::dt::AbstractData*>& specificParams)
{
  m_specificParams = specificParams;
}