/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/st/maptools/AbstractSTDataSetLayer.cpp

  \brief  This file has the AbstractSTDataSetLayer class that provides an abstract interface
          for layers that contains a DataSet with spatiotemporal data. 
*/

//Terralib
#include "AbstractSTDataSetLayer.h"

te::st::AbstractSTDataSetLayer::AbstractSTDataSetLayer(AbstractLayer* parent)
  : AbstractLayer(parent)
{
}

te::st::AbstractSTDataSetLayer::AbstractSTDataSetLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent)
{
}

te::st::AbstractSTDataSetLayer::AbstractSTDataSetLayer(const std::string& id,
                                    const std::string& title,
                                    AbstractLayer* parent)
  : AbstractLayer(id, title, parent)
{
}




