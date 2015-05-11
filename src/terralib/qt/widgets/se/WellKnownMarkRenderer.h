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
  \file terralib/qt/widgets/se/WellKnownMarkRenderer.h

  \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKRENDERER_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKRENDERER_H

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
        \class WellKnownMarkRenderer

        \brief A concrete renderer based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
               It considers basic marks defined on Symbology Enconding Specification: "square", "circle", "triangle", "star", "cross", and "x".

        \sa AbstractMarkRenderer
      */
      class TEQTWIDGETSEXPORT WellKnownMarkRenderer : public te::map::AbstractMarkRenderer
      {
        public:

          /*!
            \enum MarkType

            \brief This enum specifies mark types handle by this renderer.
          */
          enum MarkType
          {
            Square,     /*!< A square mark.      */
            Circle,     /*!< A circle mark.      */
            Triangle,   /*!< A triangle mark.    */
            Star,       /*!< A start mark.       */
            Cross,      /*!< A cross mark.       */
            X,          /*!< A x mark.           */
            Diamond,    /*!< A diamond mark.     */
            Ellipse,    /*!< A ellipse mark.     */
            Semicircle, /*!< A semicircle mark.  */
            Pentagon,   /*!< A pentagon mark.    */
            Hexagon,    /*!< A hegaxon mark.     */
            Octagon     /*!< A octagon mark.     */
          };

          /*! \brief Constructor. */
          WellKnownMarkRenderer();

          /*! \brief Destructor. */
          ~WellKnownMarkRenderer();

          te::color::RGBAColor** render(const te::se::Mark* mark, std::size_t size);

          void getSupportedMarks(std::vector<std::string>& marks) const;

        protected:

          /** @name Internal methods of Qt4 Mark Renderer.
            *  Methods that manipulate Qt4 concepts like QPainter, QImage, QBrush, QPen, QPainterPath, etc.
            */
          //@{

          /*! \brief Builds the internal auxiliary maps to map Symbology Enconding concepts to Qt. */
          void buildMaps();

          /*! \brief Builds the pre-defined marks paths. */
          void buildPaths();

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

          //@}

        private:

          static std::map<std::string, MarkType> sm_markMap;  //!< A map that associates a well-known mark name to the correct mark type.

          QPainter m_painter;                                 //!< The painter used to draw the mark patterns.
          QPen m_pen;                                         //!< The pen used to draw the mark patterns.
          QBrush m_brush;                                     //!< The pen used to draw the mark patterns.

          QPainterPath m_squarePath;                          //!< Pre-defined path to square mark.
          QPainterPath m_circlePath;                          //!< Pre-defined path to circle mark.
          QPainterPath m_trianglePath;                        //!< Pre-defined path to triangle mark.
          QPainterPath m_starPath;                            //!< Pre-defined path to star mark.
          QPainterPath m_crossPath;                           //!< Pre-defined path to cross mark.
          QPainterPath m_xPath;                               //!< Pre-defined path to x mark.
          QPainterPath m_diamondPath;                         //!< Pre-defined path to diamond mark.
          QPainterPath m_ellipsePath;                         //!< Pre-defined path to ellipse mark.
          QPainterPath m_semiCirclePath;                      //!< Pre-defined path to semicircle mark.
          QPainterPath m_pentagonPath;                        //!< Pre-defined path to pentagon mark.
          QPainterPath m_hexagonPath;                         //!< Pre-defined path to hexagon mark.
          QPainterPath m_octagonPath;                         //!< Pre-defined path to octagon mark.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKRENDERER_H
