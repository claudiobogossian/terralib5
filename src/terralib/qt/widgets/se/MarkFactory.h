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
  \file terralib/qt/widgets/se/MarkFactory.h

  \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
*/

#ifndef __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H
#define __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H

// TerraLib
#include "../../../maptools/AbstractMarkFactory.h"
#include "../../../maptools/Enums.h"
#include "../Config.h"

// Qt
#include <QtGui/QPainter>

// STL
#include <map>

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
    class Mark;
    class Stroke;
    class SvgParameter;
  }

  namespace qt
  {
    /*!
      \class MarkFactory

      \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.

      \sa AbstractMarkFactory, AbstractFactory
    */
    class TEQTWIDGETSEXPORT MarkFactory : public te::map::AbstractMarkFactory
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
        ~MarkFactory();

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
          \brief Adjusts the internal pen to use the given color.

          \param color The color that will be used to draw contours.
        */
        void setPenColor(const QColor& color);

        /*!
          \brief Adjusts the internal pen width.

          \param width The width that will be used to draw contours.
        */
        void setPenWidth(const unsigned int& width);

        /*!
          \brief Adjusts the internal pen dash style.

          \param pattern The dash style that will be used to draw contours.
        */
        void setPenStyle(const std::vector<double>& pattern);

        /*!
          \brief Adjusts the internal pen cap style.

          \param cap The cap style that will be used to draw contours.
        */
        void setPenCapStyle(const Qt::PenCapStyle& cap);

        /*!
          \brief Adjusts the internal pen join style.

          \param join The join style that will be used to draw contours.
        */
        void setPenJoinStyle(const Qt::PenJoinStyle& join);

        /*!
          \brief Adjusts the internal brush to use the given color.

          \param color The color that will be used to fill areas.
        */
        void setBrushColor(const QColor& color);

        /*!
          \brief Draw the given path on the the given image using the internal pen and brush.

          \param img The image where the path will be drawn.
          \param path The path that will be drawn.
        */
        void draw(QImage* img, QPainterPath& path);

        /*!
          \brief Auxiliary method that converts the given QImage to a array of te::color::RGBAColor.

          \param img The image that will be converted.
          
          \return A array of te::color::RGBAColor that represents the same image.

          \note The caller will take the ownership of the returned array.
        */
        te::color::RGBAColor** convert(QImage* img);

        //@}

        /** @name Internal methods to Symbology Enconding elements.
          *  Methods that manipulate concepts like te::se::Stroke, te::se::Fill, etc and converts to Qt concepts.
          */
        //@{

        /*!
          \brief Configs the internal pen based on Symbology Enconding Stroke element.

          \param stroke The Symbology Enconding Stroke element.
        */
        void config(const te::se::Stroke* stroke);

        /*!
          \brief Configs the internal brush based on Symbology Enconding Fill element.

          \param fill The Symbology Enconding Fill element.
        */
        void config(const te::se::Fill* fill);

        //@}

        /** @name Protected Initializers
          *  Initializers.
          */
        //@{

        /*!
          \brief It creates the factory.

          The key of a MarkFactory is a string.

          \param factoryKey The key that identifies the factory.
        */
        MarkFactory();

        //@}

      private:

        static std::string sm_markFactoryKey;                            //!< The Qt4 mark factory key.
        static MarkFactory* sm_factory;                                  //!< A pointer to the global Qt4 mark factory.
          
        static std::map<std::string, MarkType> sm_markMap;               //!< A map that associates a mark name to the correct mark type.
        static std::map<std::string, Qt::PenCapStyle> sm_penCapMap;      //!< A map that associates stroke-linecap type to the correct Qt::PenCapStyle.
        static std::map<std::string, Qt::PenJoinStyle> sm_penJoinMap;    //!< A map that associates stroke-linejoin type to the correct Qt::PenJoinStyle.

        QPainter m_painter;                                               //!< The painter used to draw the mark patterns.
        QPen m_pen;                                                       //!< The pen used to draw the mark patterns.
        QBrush m_brush;                                                   //!< The pen used to draw the mark patterns.

        QPainterPath m_squarePath;                                        //!< Pre-defined path to square mark.
        QPainterPath m_circlePath;                                        //!< Pre-defined path to circle mark.
        QPainterPath m_trianglePath;                                      //!< Pre-defined path to triangle mark.
        QPainterPath m_starPath;                                          //!< Pre-defined path to star mark.
        QPainterPath m_crossPath;                                         //!< Pre-defined path to cross mark.
        QPainterPath m_xPath;                                             //!< Pre-defined path to x mark.
    };
  }   // end namespace qt
}     // end namespace te


#endif  // __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H
