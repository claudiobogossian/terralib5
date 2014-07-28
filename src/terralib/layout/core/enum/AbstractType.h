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
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    typedef TELAYOUTEXPORT enum
    {
      TPRetangleItem = 1,
      TPMapItem = 2,
      TPMapGridItem = 3,
      TPPaperItem = 4,
      TPLegendItem = 5,
      TPPropertiesWindow = 6,
      TPDisplayWindow = 7,
      TPObjectInspectorWindow = 8,
      TPToolbar = 9,
      TPGridSettings = 10,
      TPGrid = 11,
      TPGridPlanar = 12,
      TPGridGeodesic = 13,
      TPHorizontalRuler = 14,
      TPVerticalRuler = 15,
      TPText = 16,
      TPItemGroup = 17,
      TPScaleItem = 18,
      TPPageSetup = 19,
      TPObjectUnknown = 0
    } LayoutAbstractObjectType;

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    typedef enum
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
    } LayoutAbstractPaperType;

    typedef enum _TdkOrientation
    {
      Portrait,	//!< portrait orientation
      Landscape	//!< landScape orientation
    } LayoutOrientationType;

    typedef enum AlignmentFlags
    {
      AlignFlagsAuto             = 0x0000,      // text alignment
      AlignFlagsLeft             = 0x0001,
      AlignFlagsRight            = 0x0002,
      AlignFlagsHCenter          = 0x0004,
      AlignFlagsJustify          = 0x0008,
      AlignFlagsHorizontal_Mask   = AlignFlagsLeft | AlignFlagsRight | AlignFlagsHCenter | AlignFlagsJustify,
      AlignFlagsTop              = 0x0010,
      AlignFlagsBottom           = 0x0020,
      AlignFlagsVCenter          = 0x0040,
      AlignFlagsVertical_Mask     = AlignFlagsTop | AlignFlagsBottom | AlignFlagsVCenter,
      AlignFlagsCenter           = AlignFlagsVCenter | AlignFlagsHCenter
    } LayoutAlignmentType;

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    typedef enum
    {
      TPTopLeft,
      TPTopRight,
      TPLowerLeft,
      TPLowerRight,
      TPCenter,
      TPCenterRight,
      TPCenterLeft,
      TPNoneSide
    } LayoutSides;

    /*!
    \brief Enum TdkAbstractComponentType. This is the enumeration of the components types
    */
    typedef TELAYOUTEXPORT enum
    {
      TPMillimeter = 0,
      TPInch = 1
    } LayoutUnitsMetrics;

    /*!
    \brief Enum LayoutPropertyDataType. 
    */
    typedef TELAYOUTEXPORT enum
    {
      DataTypeInt = 1,
      DataTypeDouble = 2,
      DataTypeLong = 3,
      DataTypeFloat = 4,
      DataTypeString = 5,
      DataTypeBool = 6,
      DataTypeColor = 7,
      DataTypeGridSettings = 8,
      DataTypeStringList = 9,
      DataTypeNone = 10
    } LayoutPropertyDataType;

    /*!
    \brief Enum LayoutTemplateType.
    */
    typedef TELAYOUTEXPORT enum
    {
      TPJSONTemplate = 1
    } LayoutTemplateType;

    typedef enum
    {
      StyleNone = 1,
      StyleContinuous = 2,	//! continuos line grid
      StyleCross = 3				//! cross grid
    } LayoutGridStyle;

    typedef enum
    {
      StyleMeter=1,
      StyleKilometer=1000
    } LayoutUnit;
  }
}

#endif // __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_TYPE_H