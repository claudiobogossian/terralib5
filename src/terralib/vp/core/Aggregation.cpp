/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Aggregation.h

  \brief Aggregation Vector Processing functions.
*/

//Terralib
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "Aggregation.h"
#include "AggregationDialog.h"
#include "Config.h"
#include "Exception.h"

// STL
#include <map>
#include <vector>

void te::vp::Aggregation(const std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> > inputAttributes
                        , const std::map<std::string, std::vector<te::vp::Attributes> > outputAttributes
                        , const std::string outputLayerName
                        , te::da::DataSourceInfoPtr dsInfo)
{

}