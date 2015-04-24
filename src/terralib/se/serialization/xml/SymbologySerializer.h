/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/serialization/xml/SymbologySerializer.h

  \brief Data serialization for the Symbology Encoder module.
*/

#ifndef __TERRALIB_SE_SERIALIZATION_XML_INTERNAL_SYMBOLOGYSERIALIZER_H
#define __TERRALIB_SE_SERIALIZATION_XML_INTERNAL_SYMBOLOGYSERIALIZER_H

// TerraLib
#include "../../Config.h"
#include "../../RasterSymbolizer.h"

//#ifdef TE_ENABLED_XML

namespace te
{
  namespace xml
  {
    class AbstractWriter;
    class Reader;
  }

  namespace se
  {
    class AnchorPoint;
    class Categorize;
    class ChannelSelection;
    class ColorMap;
    class ContrastEnhancement;
    class Description;
    class ExternalGraphic;
    class Fill;
    class Font;
    class Graphic;
    class GraphicStroke;
    class Halo;
    class ImageOutline;
    class InlineContent;
    class Interpolate;
    class InterpolationPoint;
    class LabelPlacement;
    class LinePlacement;
    class Mark;
    class ParameterValue;
    class PointPlacement;
    class Rule;
    class SelectedChannel;
    class ShadedRelief;
    class Stroke;
    class Style;
    class SvgParameter;
    class Symbolizer;

    namespace serialize
    {
      TESEEXPORT te::se::AnchorPoint* ReadAnchorPoint(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::AnchorPoint* ap, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Categorize* ReadCategorize(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Categorize* c, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ChannelSelection* ReadChannelSelection(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ChannelSelection* cs, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ColorMap* ReadColorMap(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ColorMap* cm, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ContrastEnhancement* ReadContrastEnhancement(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ContrastEnhancement* ce, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Description* ReadDescription(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Description* d, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Displacement* ReadDisplacement(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Displacement* d, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ExternalGraphic* ReadExternalGraphic(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ExternalGraphic* eg, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Fill* ReadFill(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Fill* fill, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Font* ReadFont(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Font* font, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Graphic* ReadGraphic(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Graphic* graphic, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::GraphicStroke* ReadGraphicStroke(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::GraphicStroke* graphicStroke, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Halo* ReadHalo(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Halo* halo, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ImageOutline* ReadImageOutline(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ImageOutline* imageOutline, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::InlineContent* ReadInlineContent(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::InlineContent* ic, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Interpolate* ReadInterpolate(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Interpolate* interpolate, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::InterpolationPoint* ReadInterpolationPoint(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::InterpolationPoint* ip, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::LabelPlacement* ReadLabelPlacement(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::LabelPlacement* lp, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::LinePlacement* ReadLinePlacement(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::LinePlacement* lp, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Mark* ReadMark(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Mark* mark, te::xml::AbstractWriter& writer);

      TESEEXPORT RasterSymbolizer::OverlapBehavior ReadOverlapBehavior(te::xml::Reader& reader);

      TESEEXPORT void Save(const RasterSymbolizer::OverlapBehavior& type, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ParameterValue* ReadParameterValue(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ParameterValue* p, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::PointPlacement* ReadPointPlacement(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::PointPlacement* pp, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Rule* ReadRule(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Rule* rule, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::SelectedChannel* ReadSelectedChannel(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::SelectedChannel* sc, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::ShadedRelief* ReadShadedRelief(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::ShadedRelief* sr, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::Stroke* ReadStroke(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::Stroke* stroke, te::xml::AbstractWriter& writer);

      TESEEXPORT te::se::SvgParameter* ReadSvgParameter(te::xml::Reader& reader);

      TESEEXPORT void Save(const te::se::SvgParameter* p, te::xml::AbstractWriter& writer);

    }  // end namespace serialize
  }    // end namespace se
}      // end namespace te

//#endif // TE_ENABLED_XML

#endif  // __TERRALIB_SE_SERIALIZATION_XML_INTERNAL_SYMBOLOGYSERIALIZER_H
