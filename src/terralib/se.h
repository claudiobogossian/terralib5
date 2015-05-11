/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file se.h
  
  \brief This file contains include headers for TerraLib Symbology Encoding module.
*/

#ifndef __TERRALIB_INTERNAL_SE_H
#define __TERRALIB_INTERNAL_SE_H

// TerraLib
#include "se/AnchorPoint.h"
#include "se/Categorize.h"
#include "se/ChangeCase.h"
#include "se/ChannelSelection.h"
#include "se/ColorMap.h"
#include "se/Concatenate.h"
#include "se/Config.h"
#include "se/ContrastEnhancement.h"
#include "se/CoverageStyle.h"
#include "se/Description.h"
#include "se/Displacement.h"
#include "se/Enums.h"
#include "se/ExternalGraphic.h"
#include "se/FeatureTypeStyle.h"
#include "se/Fill.h"
#include "se/Font.h"
#include "se/FormatDate.h"
#include "se/FormatNumber.h"
#include "se/Function.h"
#include "se/Graphic.h"
#include "se/GraphicStroke.h"
#include "se/Halo.h"
#include "se/ImageOutline.h"
#include "se/InlineContent.h"
#include "se/Interpolate.h"
#include "se/InterpolationPoint.h"
#include "se/LabelPlacement.h"
#include "se/LinePlacement.h"
#include "se/LineSymbolizer.h"
#include "se/MapItem.h"
#include "se/Mark.h"
#include "se/ParameterValue.h"
#include "se/PointPlacement.h"
#include "se/PointSymbolizer.h"
#include "se/PolygonSymbolizer.h"
#include "se/RasterSymbolizer.h"
#include "se/Recode.h"
#include "se/Rule.h"
#include "se/SelectedChannel.h"
#include "se/ShadedRelief.h"
#include "se/StringLength.h"
#include "se/StringPosition.h"
#include "se/Stroke.h"
#include "se/Substring.h"
#include "se/SvgParameter.h"
#include "se/Symbolizer.h"
#include "se/SymbologyManager.h"
#include "se/TextSymbolizer.h"
#include "se/Trim.h"
#include "se/Utils.h"

/*!
  \defgroup se Symbology Encoding

  \brief The symbology encoding module can be used for styling information that can be applied to Features and Coverages.

    It implements the OGC Symbology Encoding Implementation Specification.
    Most of the documentation used in Symbology classes were extracted
    from the specificaton available at: http://www.opengeospatial.org/standards/symbol.

    The main classes/concepts in this module are listed here. The namespace associated to the Symbolig Encoding module is te::se.
    To know more about it, see the te::se namespace documentation.
*/

namespace te
{
  /*!
    \brief Namespace for the Symbology Encoding module of TerraLib.
  */
  namespace se
  {
  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_INTERNAL_SE_H

