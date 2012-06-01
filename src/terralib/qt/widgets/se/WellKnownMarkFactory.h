/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/WellKnownMarkFactory.h

  \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKFACTORY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKFACTORY_H

// TerraLib
#include "../../../maptools/AbstractMarkFactory.h"
#include "../Config.h"

// Qt
#include <QtGui/QPainter>

// STL
#include <map>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class WellKnownMarkFactory

        \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
               It considers basic marks defined on Symbology Enconding Specification: "square", "circle", "triangle", "star", "cross", and "x".

        \sa AbstractMarkFactory, AbstractFactory
      */
      class TEQTWIDGETSEXPORT WellKnownMarkFactory : public te::map::AbstractMarkFactory
      {
        public:

          /*!
            \enum MarkType

            \brief This enum specifies mark types handle by this factory.
          */
          enum MarkType
          {
            Square,    /*!< A square mark.   */
            Circle,    /*!< A circle mark.   */
            Triangle,  /*!< A triangle mark. */
            Star,      /*!< A start mark.    */
            Cross,     /*!< A cross mark.    */
            X          /*!< A x mark.        */
          };

          /** @name Public Initializer
            *  Methods for the control of the factory singleton.
            */
          //@{

          /*! \brief It initializes the factory: the singleton instance will be registered in the abstract factory AbstractMarkFactory. */
          static void initialize();

          /*! \brief It finalizes the factory: the singleton instance will be destroyed and will be unregistered from the abstract factory AbstractMarkFactory. */
          static void finalize();

          //@}

          /** @name Public Destructor
            *  Destructor.
            */
          //@{

          /*!
            \brief Destructor.

            \note It will automatically unregister the factory from the dictionary.
          */
          ~WellKnownMarkFactory();

          //@} 

        protected:

          /** @name Re-implementation of Pure Virtual Method
            *  Concrete factory methods.
            */
          //@{

          te::map::AbstractMarkFactory* build();

          te::color::RGBAColor** create(const te::se::Mark* mark, std::size_t size);

          void getSupportedMarks(std::vector<std::string>& marks) const;

          //@}

          /** @name Internal methods of Qt4 Mark Factory.
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

          /** @name Protected Initializers
            *  Initializers.
            */
          //@{

          /*!
            \brief It creates the factory.

            The key of a WellKnownMarkFactory is a string.

            \param factoryKey The key that identifies the factory.
          */
          WellKnownMarkFactory();

          //@}

        private:

          static std::string sm_factoryKey;                   //!< The factory key.
          static WellKnownMarkFactory* sm_factory;            //!< A pointer to the global factory.

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
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_WELLKNOWNMARKFACTORY_H
