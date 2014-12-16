/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/sa/qt/Utils.h

  \brief Utilitary interface function for spatial analysis module.
*/

#ifndef __TERRALIB_SA_INTERNAL_UTILSQT_H
#define __TERRALIB_SA_INTERNAL_UTILSQT_H

// TerraLib
#include "../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <string>

namespace te
{
  // Forward declaration
  namespace color { class ColorBar; }
  namespace da    { class DataSource; }

  namespace sa
  {
    // Forward declaration

    TESAEXPORT te::da::DataSourcePtr CreateOGRDataSource(std::string repository);

    TESAEXPORT te::da::DataSourcePtr CreateOGRDataSource(std::string path, std::string dataSetName);

    TESAEXPORT te::da::DataSourcePtr CreateGDALDataSource(std::string path, std::string dataSetName);

    TESAEXPORT te::map::AbstractLayerPtr CreateLayer(te::da::DataSourcePtr ds, std::string dataSetName);

    TESAEXPORT void CreateBayesGrouping(te::map::AbstractLayerPtr layer);

    TESAEXPORT void CreateKernelGrouping(te::map::AbstractLayerPtr layer, std::string kernelAttr);

    TESAEXPORT void CreateKernelColorMaping(te::map::AbstractLayerPtr layer);

    TESAEXPORT void CreateSampleGeneratorStratifiedGrouping(te::map::AbstractLayerPtr layer, std::vector<std::string> strVec);
    
    TESAEXPORT void CreateSkaterGrouping(te::map::AbstractLayerPtr layer, int nClasses);

    TESAEXPORT te::color::ColorBar* GetColorBar(std::string catalog, std::string group, std::string schema);
  } // end namespace sa
}   // end namespace te

#endif  // __TERRALIB_SA_INTERNAL_UTILS_H

