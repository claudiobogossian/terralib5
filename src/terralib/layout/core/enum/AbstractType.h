/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutAbstractType.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TYPE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TYPE_H

#include "../Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Enum LayoutRulerOrientation. This is the enumeration of the ruler orientation types
    */
    enum TELAYOUTEXPORT LayoutRulerOrientation
    {
      TypeRulerVertical = 0,
      TypeRulerHorizontal = 90
    };

    enum TELAYOUTEXPORT PrinterScene
    {
      PreviewScene,
      PrintingScene,
      NoPrinter
    };

    /*!
    \brief Enum LayoutGridType. This is the enumeration of the grid types
    */
    enum TELAYOUTEXPORT LayoutGridType
    {
      TypePlanar = 0,
      TypeGeodesic = 1
    };

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    enum LayoutAbstractPaperType
    {     
      A0,
      A1,
      A2,
      A3,
      A4,
      A5,
      A6,
      A7,
      A8,
      A9,
      Letter,
      Legal,
      Executive,
      Custom
    };

    enum LayoutOrientationType
    {
      Portrait,	//!< portrait orientation
      Landscape	//!< landScape orientation
    };

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    enum LayoutAlign
    {
      TPTopLeft,
      TPTopRight,
      TPLowerLeft,
      TPLowerRight,
      TPCenter,
      TPCenterRight,
      TPCenterLeft,
      TPNoneSide
    };

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    enum TELAYOUTEXPORT LayoutUnitsMetrics
    {
      TPMillimeter = 0,
      TPInch = 1
    };
    
    //typedef enum
    //{
    //  StyleNone = 1,
    //  StyleContinuous = 2,	//! continuos line grid
    //  StyleCross = 3				//! cross grid
    //} LayoutGridStyle;

    enum LayoutUnit
    {
      StyleMeter=1,
      StyleKilometer=1000
    };

    enum SystematicScaleType
    {
      S250000,		
      S100000,
      S50000,
      S25000,
      S0
    };
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TYPE_H
