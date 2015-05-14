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
  \file terralib/qt/widgets/se/GlyphMarkRenderer.h

  \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKRENDERER_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKRENDERER_H

// TerraLib
#include "../../../maptools/AbstractMarkRenderer.h"
#include "../Config.h"

// Qt
#include <QPainter>

// STL
#include <map>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class GlyphMarkRenderer

        \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
               It is a extension that considers marks with well-known name in the format: "ttf://font#code".
               Here, "font" is the name of a TrueType font and "code" is the character code.
               The character code can be enconded in hexadecimal, octonal or decimal.
               e.g.: "ttf://Arial#0x12", "ttf://Times#042", "ttf://Verdana#10".

        \sa AbstractMarkRenderer
      */
      class TEQTWIDGETSEXPORT GlyphMarkRenderer : public te::map::AbstractMarkRenderer
      {
        public:

          /*! \brief Constructor. */
          GlyphMarkRenderer();

          /*! \brief Destructor. */
          ~GlyphMarkRenderer();

          te::color::RGBAColor** render(const te::se::Mark* mark, std::size_t size);

          void getSupportedMarks(std::vector<std::string>& marks) const;

          /*!
            \brief Encodes the given parameters to a format known by this renderer.

            \param font      The font name. e.g. "Times".
            \param charCoder The decimal char code.

            \return A string that represents the format known by this renderer. e.g. "ttf://Times#0x12".

            \note The given char code will be enconded in hexadecimal.
          */
          static QString encode(const QString& font, const int& charCode);

          /*!
            \brief Decodes the given name extracting the parameters known by this renderer.

            \param name      The name dealt by this renderer. e.g. "ttf://Times#0x12".
            \param font      The font name that will be extracted from name.
            \param charCoder The char code that will be extracted from name.
          */
          static void decode(QString& name, QString& font, QChar& charCode);

        protected:

          /** @name Internal methods of Qt4 Mark Renderer.
            *  Methods that manipulate Qt4 concepts like QPainter, QImage, QBrush, QPen, QPainterPath, etc.
            */
          //@{

          /*! \brief Setups the internal QPainter and associates it with the given paint device (QImage). */
          void setup(QImage* img);

          /*! \brief Finalizes the internal QPainter resources. */
          void end();

          /*!
            \brief Draw the given path on the the given image using the internal pen and brush.

            \param img The image where the path will be drawn.
            \param path The path that will be drawn.
          */
          void draw(QImage* img, QPainterPath& path);

          /*!
            \brief Converts the character code to a char.

            \param charCode The character code. It can be enconded in hexadecimal, octonal or decimal.
            \param ch The result char from character code conversion.

            \return It returns true if the character code was converted. Otherwise, it returns false.
          */
          bool getChar(QString& charCode, QChar& ch);

          //@}

        private:

          QPainter m_painter;                 //!< The painter used to draw the mark patterns.
          QPen m_pen;                         //!< The pen used to draw the mark patterns.
          QBrush m_brush;                     //!< The pen used to draw the mark patterns.

          static std::string sm_rendererKey;  //!< The renderer key.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_GLYPHMARKRENDERER_H
