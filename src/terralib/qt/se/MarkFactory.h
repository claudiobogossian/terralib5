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
  \file MarkFactory.h

  \brief A concrete factory based on Qt4 for conversion of Symbology Enconding Mark elements to an image pattern.
 */

#ifndef __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H
#define __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H

// TerraLib
#include "../widgets/Config.h"
#include "../../maptools/AbstractMarkFactory.h"
#include "../../maptools/Enums.h"

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

        void buildMaps();
        void buildPaths();

        void setup(QImage* img);
        void end();

        void setPenColor(const QColor& color);
        void setPenWidth(const unsigned int& width);
        void setPenStyle(const std::vector<double>& pattern);
        void setPenCapStyle(const Qt::PenCapStyle& cap);
        void setPenJoinStyle(const Qt::PenJoinStyle& join);

        void setBrushColor(const QColor& color);

        void draw(QImage* img, QPainterPath& path);

        te::color::RGBAColor** convert(QImage* img);

        //@}

        /** @name Internal methods to Symbology Enconding elements.
          *  Methods that manipulate Qt4 concepts like se::Stroke, se::Fill, etc.
          */
        //@{

        void config(const te::se::Stroke* stroke);
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

        static std::string sm_markFactoryKey; //!< The Qt4 mark factory key.
        static MarkFactory* sm_factory;       //!< A pointer to the global Qt4 mark factory.
          
        static std::map<std::string, MarkType> sm_markMap;               //!< A map that associates a mark name to the correct mark type.
        static std::map<std::string, Qt::PenCapStyle> sm_penCapMap;      //!< A map that associates stroke-linecap type to the correct Qt::PenCapStyle.
        static std::map<std::string, Qt::PenJoinStyle> sm_penJoinMap;    //!< A map that associates stroke-linejoin type to the correct Qt::PenJoinStyle.

        QPainter m_painter;                   //!< The painter used to draw the mark patterns.
        QPen m_pen;                           //!< The pen used to draw the mark patterns.
        QBrush m_brush;                       //!< The pen used to draw the mark patterns.

        QPainterPath m_squarePath;            //!< Pre-defined path to square mark.
        QPainterPath m_circlePath;            //!< Pre-defined path to circle mark.
        QPainterPath m_trianglePath;          //!< Pre-defined path to triangle mark.
        QPainterPath m_starPath;              //!< Pre-defined path to star mark.
        QPainterPath m_crossPath;             //!< Pre-defined path to cross mark.
        QPainterPath m_xPath;                 //!< Pre-defined path to x mark.
    };
  }   // end namespace qt
}     // end namespace te


#endif  // __TERRALIB_QT_SE_INTERNAL_MARKFACTORY_H
