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
  \file terralib/qt/plugins/terralib5/Utils.h

  \brief
*/

#ifndef __TERRALIB_QT_TERRALIB4_INTERNAL_UTILS_H
#define __TERRALIB_QT_TERRALIB4_INTERNAL_UTILS_H

// TerraLib
#include "Config.h"

struct TeColor;
class TeRasterTransform;
class TeTheme;
class TeVisual;

namespace te
{
  namespace color
  {
    class RGBAColor;
  }

  namespace se
  {
    class Fill;
    class LineSymbolizer;
    class PointSymbolizer;
    class PolygonSymbolizer;
    class Style;
    class Stroke;
    class Symbolizer;
  }

  namespace qt
  {
    namespace plugins
    {
      namespace terralib4
      {

        te::color::RGBAColor Convert2TerraLib5(TeColor color);

        std::string GetLineStyle(int type);

        std::string GetPointStyle(int type);

        bool IsHollow(int type);

        te::se::Style* Convert2TerraLib5(int geometryType, TeTheme* theme, bool isRaster = false);

        te::se::PolygonSymbolizer* GetPolygonSymbolizer(TeVisual* visual);

        te::se::LineSymbolizer* GetLineSymbolizer(TeVisual* visual);
        
        te::se::PointSymbolizer* GetPointSymbolizer(TeVisual* visual);

        te::se::RasterSymbolizer* GetRasterSymbolizer(TeRasterTransform* visual);

      } // end namespace terralib4
    } // end namespace plugins
  } // end namespace qt
} // end namespace te

#endif  // __TERRALIB_QT_TERRALIB4_INTERNAL_UTILS_H

