/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/layer/utils/DataSet2Layer.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_DATASET2LAYER_H
#define __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_DATASET2LAYER_H

// TerraLib
#include "../../../../maptools/DataSetLayer.h"
#include "../../Config.h"

// STL
#include <string>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      struct TEQTWIDGETSEXPORT DataSet2Layer
      {
        std::string m_datasourceId;

        DataSet2Layer(const std::string& datasourceId);

        te::map::DataSetLayerPtr operator()(const te::da::DataSetTypePtr& dataset) const;

        te::map::DataSetLayerPtr operator()(const te::da::DataSetTypePtr& dataset, const std::string& geomPropertyName) const;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_UTILS_INTERNAL_DATASET2LAYER_H

