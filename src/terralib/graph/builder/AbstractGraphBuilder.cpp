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
  \file AbstractGraphBuilder.cpp

  \brief This abstract class provides the common functions for graph
    builder classes. Each builder strategy has to implement only
    a public function called build.

*/

// TerraLib Includes
#include "../core/AbstractGraph.h"
#include "AbstractGraphBuilder.h"

te::graph::AbstractGraphBuilder::AbstractGraphBuilder()
{
  m_graph.reset();
  m_errorMessage = "";
}

te::graph::AbstractGraphBuilder::~AbstractGraphBuilder()
{
}

std::string te::graph::AbstractGraphBuilder::getErrorMessage()
{
  return m_errorMessage;
}
       
boost::shared_ptr<te::graph::AbstractGraph> te::graph::AbstractGraphBuilder::getGraph()
{
  return m_graph;
}