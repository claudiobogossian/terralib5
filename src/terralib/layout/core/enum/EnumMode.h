/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutMode.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MODE_H
#define __TERRALIB_LAYOUT_INTERNAL_MODE_H

#include "../Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Enum LayoutMode. This is the enumeration of the components types
    */
    typedef TELAYOUTEXPORT enum
    {
      TypeSelectByBox = 1,
      TypeSelect = 2,
      TypeArrowCursor = 3,
      TypePan = 4,
      TypeZoomIn = 5,
      TypeZoomOut = 6,
      TypeMapPan = 7,
      TypeMapZoomIn = 8,
      TypeMapZoomOut = 9,
      TypeCreate = 10,
      TypeCreateMap = 11,
      TypeCreateMapGrid = 12,
      TypeCreateLegend = 13,
      TypeCreateRectangle = 14,
      TypeCreateText = 15,
      TypeCreateItemGroup = 16,
      TypeUnitsMetricsChange = 17,
      TypeExportPropsJSON = 18,
      TypeImportJSONProps = 19,
      TypeNewTemplate = 20,
      TypeCreateScale = 21,
      TypeGroup = 22,
      TypeUngroup = 23,
      TypeActiveLinesIntersectionMouse = 24,
      TypeOffLinesIntersectionMouse = 25,
      TypePrinter = 26,
      TypeExit = 27,
      TypeSceneZoom = 28,
      TypeBringToFront = 29,
      TypeSendToBack = 30,
      TypeRecompose = 31,
      TypeCreateImage = 32,
      TypeNone = 0
    } LayoutMode;

    /*!
    \brief Enum LayoutRulerOrientation. This is the enumeration of the ruler orientation types
    */
    typedef TELAYOUTEXPORT enum
    {
      TypeRulerVertical = 0,
      TypeRulerHorizontal = 90
    } LayoutRulerOrientation;

    /*!
    \brief Enum LayoutGridType. This is the enumeration of the grid types
    */
    typedef TELAYOUTEXPORT enum
    {
      TypePlanar = 0,
      TypeGeodesic = 1
    } LayoutGridType;
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_MODE_H