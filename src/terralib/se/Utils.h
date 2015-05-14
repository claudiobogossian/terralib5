/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/Utils.h

  \brief Utility functions for Symbology Enconding module.

  \ingroup se
*/

#ifndef __TERRALIB_SE_INTERNAL_UTILS_H
#define __TERRALIB_SE_INTERNAL_UTILS_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../geometry/Enums.h"
#include "Config.h"
#include "Font.h"

// STL
#include <string>
#include <vector>

namespace te
{
// Forward declaration
  namespace rst { class BandProperty; }

  namespace se
  {
// Forward declarations
    class Description;
    class Fill;
    class Graphic;
    class LineSymbolizer;
    class Mark;
    class ParameterValue;
    class PointSymbolizer;
    class PolygonSymbolizer;
    class Stroke;
    class Style;
    class Symbolizer;
    class RasterSymbolizer;
    class TextSymbolizer;

    /*!
      \brief Creates a stroke.

      \param color The stroke color enconded hexadecimal i.e "#FF0000".
      \param width The stroke width.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width);

    /*!
      \brief Creates a stroke.

      \param color   The stroke color enconded hexadecimal i.e "#FF0000".
      \param width   The stroke width.
      \param opacity The stroke opacity [0,1].

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width, const std::string& opacity);

    /*!
      \brief Creates a stroke.

      \param color     The stroke color enconded hexadecimal i.e "#FF0000".
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width,
                                    const std::string& opacity, const std::string& dasharray);

    /*!
      \brief Creates a stroke.

      \param color     The stroke color enconded hexadecimal i.e "#FF0000".
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.
      \param linecap   The stroke linecap {butt, square, round}.
      \param linejoin  The stroke linejoin {miter, bevel, round}.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(const std::string& color, const std::string& width,
                                    const std::string& opacity, const std::string& dasharray,
                                    const std::string& linecap, const std::string& linejoin);

    /*!
      \brief Creates a stroke.

      \param graphic   The stroke graphic fill.
      \param width     The stroke width.
      \param opacity   The stroke opacity [0,1].
      \param dasharray The stroke dasharray.
      \param linecap   The stroke linecap {butt, square, round}.
      \param linejoin  The stroke linejoin {miter, bevel, round}.

      \return The Symbology Enconding Stroke element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Stroke* CreateStroke(Graphic* graphicFill,
                                    const std::string& width, const std::string& opacity, 
                                    const std::string& dasharray, const std::string& linecap, const std::string& linejoin);

    /*!
      \brief Creates a fill.

      \param color   The fill color enconded hexadecimal i.e "#FF0000".
      \param opacity The fill opacity.

      \return The Symbology Enconding Fill element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Fill* CreateFill(const std::string& color, const std::string& opacity);

    /*!
      \brief Creates a fill.

      \param graphic The graphic fill.

      \return The Symbology Enconding Fill element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Fill* CreateFill(Graphic* graphicFill);

    /*!
      \brief Creates a mark.

      \param wellKnownName The mark well-known name.
      \param stroke        The mark stroke.
      \param fill          The mark fill.

      \return The Symbology Enconding Mark element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Mark* CreateMark(const std::string& wellKnownName, Stroke* stroke, Fill* fill);

    /*!
      \brief Creates a graphic.

      \param mark     The graphic mark.
      \param size     The graphic size.
      \param rotation The graphic rotation.
      \param opacity  The graphic opacity.

      \return The Symbology Enconding Graphic element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Graphic* CreateGraphic(Mark* mark, const std::string& size, const std::string& rotation, const std::string& opacity);

    /*!
      \brief Creates a polygon symbolizer.

      \param stroke The polygon symbolizer stroke.
      \param fill   The polygon symbolizer fill.

      \return The Symbology Enconding PolygonSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT PolygonSymbolizer* CreatePolygonSymbolizer(Stroke* stroke, Fill* fill);

    /*!
      \brief Creates a line symbolizer.

      \param stroke The line symbolizer stroke.

      \return The Symbology Enconding LineSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT LineSymbolizer* CreateLineSymbolizer(Stroke* stroke);

    /*!
      \brief Creates a point symbolizer.

      \param graphic The point symbolizer graphic.

      \return The Symbology Enconding PointSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT PointSymbolizer* CreatePointSymbolizer(Graphic* graphic);

    /*!
      \brief Creates a font.

      \param family The font family.
      \param size The font size.
      \param style The font style.
      \param weight The font weight.

      \return The Symbology Enconding Font element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Font* CreateFont(const std::string& family, const std::string& size,
                                const te::se::Font::FontStyleType& style = te::se::Font::StyleNormal,
                                const te::se::Font::FontWeightType& weight = te::se::Font::WeightNormal);

    /*!
      \brief Creates a text symbolizer.

      \param label The property name that will be labelled.
      \param fill The text symbolizer fill.
      \param font The text symbolizer font.

      \return The Symbology Enconding TextSymbolizer element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT TextSymbolizer* CreateTextSymbolizer(const std::string& label, Fill* fill, Font* font);

    /*!
      \brief Creates a description.

      \param title The title of description.
      \param abst The abstratc of description.

      \return The Symbology Enconding Description element.

      \note The caller of this method will take the ownership of the returned pointer.
    */
    TESEEXPORT Description* CreateDescription(const std::string& title, const std::string& abst);

    /*!
      \brief Try creates an appropriate symbolizer based on given geometry type.

      \param geomType The geometry type.

      \return A symbolizer based on given geometry type.

      \note Random colors will be generated.
      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default symbolizer could not be created.
    */
    TESEEXPORT Symbolizer* CreateSymbolizer(const te::gm::GeomType& geomType);

    /*!
      \brief Try creates an appropriate symbolizer based on given geometry type and a default color.

      \param geomType The geometry type.
      \param color     The fill color enconded hexadecimal i.e "#FF0000".

      \return A symbolizer based on given geometry type.

      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default symbolizer could not be created.
    */
    TESEEXPORT Symbolizer* CreateSymbolizer(const te::gm::GeomType& geomType, const std::string& color);

    /*!
      \brief Try creates an appropriate feature type style based on given geometry type.

      \param geomType The geometry type.

      \return A feature type style based on given geometry type.

      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default style could not be created.
    */
    TESEEXPORT Style* CreateFeatureTypeStyle(const te::gm::GeomType& geomType);

    /*!
      \brief Try creates an appropriate coverage style based on given band properties.

      \param properties The band properties.

      \return A coverage style based on given geometry type.

      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default style could not be created.
      
      \todo Review this method in order to extract the maximum information about the given band properties.
    */
    TESEEXPORT Style* CreateCoverageStyle(const std::vector<te::rst::BandProperty*>& properties);

    /*!
      \brief Try creates an appropriate coverage style based on given number of bands.

      \param nBands The number of bands.

      \return A coverage style based on given geometry type.

      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default style could not be created.
    */
    TESEEXPORT Style* CreateCoverageStyle(const std::size_t& nBands);

    /*!
      \brief Try creates an appropriate raster symbolizer style based on given number of bands.

      \param nBands The number of bands.

      \return A raster symbolizer based on number of bands.

      \note The caller of this method will take the ownership of the returned pointer.
      \note The method will return a NULL pointer if a default raster symbolizer could not be created.
    */
    TESEEXPORT RasterSymbolizer* CreateRasterSymbolizer(const std::size_t& nBands);

    /*!
      \brief Try to get raster symbolizer from a style.

      \param s Style.

      \return A raster symbolizer.

      \note The method will return a NULL pointer if the symbolizer does not have a  raster symbolizer.
    */
    TESEEXPORT RasterSymbolizer* GetRasterSymbolizer(Style* s);

    /*!
      \brief Creates a random RGB color encoded using two hexadecimal digits per primary-color component prefixed with a hash (#) sign.

      \return A random color encoded using two hexadecimal digits per primary-color component prefixed with a hash (#) sign.
    */
    TESEEXPORT std::string GenerateRandomColor();


    /*!
      \brief It gets the RGBA color from the Stroke element.

      \param stroke The Symbology Enconding Stroke element.
      \param color  A pre-created color that will be filled with the values extracted from Stroke.

      \note The given color will be altered based on "stroke" and "stroke-opacity" attributes, if availables.
    */
    TESEEXPORT void GetColor(const te::se::Stroke* stroke, te::color::RGBAColor& color);

    /*!
      \brief It gets the RGBA color from the Fill element.

      \param fill  The Symbology Enconding Fill element.
      \param color A pre-created color that will be filled with the values extracted from Fill.

      \note The given color will be altered based on "fill" and "fill-opacity" attributes, if availables.
    */
    TESEEXPORT void GetColor(const te::se::Fill* fill, te::color::RGBAColor& color);

    /*!
      \brief It gets the RGBA color from the parameter values.

      \param color   The Symbology Enconding Parameter Value element that represents the color.
      \param opacity The Symbology Enconding Parameter Value element that represents the opacity.
      \param color A pre-created color that will be filled with the values extracted from color and opacity.

      \note The given color will be altered based on "color" and "opacity" parameters, if availables.
      \note Warning: It considers that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TESEEXPORT void GetColor(const te::se::ParameterValue* color, const te::se::ParameterValue* opacity, te::color::RGBAColor& rgba);

    /*!
      \brief It gets the parameter value as a RGBA color.

      \param param The Symbology Enconding Parameter Value element.

      \return The RGBA color.

      \note Warning: It considers that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TESEEXPORT te::color::RGBAColor GetColor(const te::se::ParameterValue* param);

    /*!
      \brief It gets the parameter value as an integer.

      \param param The Symbology Enconding Parameter Value element.

      \return The integer value.

      \note Warning: It considers that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TESEEXPORT int GetInt(const te::se::ParameterValue* param);

    /*!
      \brief It gets the parameter value as a double.

      \param param The Symbology Encoding Parameter Value element.

      \return The double value.

      \note Warning: It considers that the paramater value is enconded using a Literal OGC Filter expression.
    */
    TESEEXPORT double GetDouble(const te::se::ParameterValue* param);

    /*!
      \brief It gets the parameter value as a string.

      \param param The Symbology Encoding Parameter Value element.

      \return The string value.

      \note Warning: It considers that the paramater value is encoded using a Literal OGC Filter expression.
    */
    TESEEXPORT std::string GetString(const te::se::ParameterValue* param);


  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_UTILS_H
