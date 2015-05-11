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
  \file terralib/maptools/CanvasConfigurer.h
  
  \brief A Symbology Enconding visitor that configures a given canvas based on symbolizers elements.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_CANVASCONFIGURER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_CANVASCONFIGURER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../se/Visitor.h"
#include "Config.h"
#include "Enums.h"

// STL
#include <map>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace se
  {
    class Fill;
    class Graphic;
    class GraphicStroke;
    class ParameterValue;
    class Stroke;
  }

  namespace map
  {
// Forward declarations
    class Canvas;

    /*!
      \class CanvasConfigurer
      
      \brief A Symbology Enconding visitor that configures a given canvas based on symbolizers elements.
             If you want to use this visitor, use a command like:
             <code>
             te::map::CanvasConfigurer cc(canvas);
             cc.config(symbolizer);
             </code>

      \ingroup map

      \sa te::se::Visitor
    */
    class TEMAPEXPORT CanvasConfigurer : public te::se::Visitor, public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         */
        //@{

        /*! 
          \brief Constructor. 
          
          \param canvas A pointer to the canvas that will be configured.

          \note This class will not take the ownership of the given canvas.
        */
        CanvasConfigurer(Canvas* canvas);

        /*! \brief Destructor. */
        ~CanvasConfigurer();

        //@}

        /** @name Config Method
         *  Method that can be used to configure a canvas based on Symbology Enconding symbolizer elements.
         */
        //@{

        /*!
          \brief It configs the canvas based on given symbolizer.

          \param symbolizer The symbolizer that will be used to config the canvas.
        */
        void config(const te::se::Symbolizer* symbolizer);

        //@}

        /** @name Visitor Methods
         *  All concrete visitors must implement these methods.
         */
        //@{

        void visit(const te::se::Style& visited);
        void visit(const te::se::FeatureTypeStyle& visited);
        void visit(const te::se::CoverageStyle& visited);
        void visit(const te::se::Symbolizer& visited);
        void visit(const te::se::PolygonSymbolizer& visited);
        void visit(const te::se::LineSymbolizer& visited);
        void visit(const te::se::PointSymbolizer& visited);
        void visit(const te::se::TextSymbolizer& visited);
        void visit(const te::se::RasterSymbolizer& visited);

        //@}

      private:

        /*!
          \enum ConfigStyle

          \brief Internal enum that indicates which style that should be configured.
        */
        enum ConfigStyle
        {
          Point,   /*!< Adjust parameters associated with point.           */
          Line,    /*!< Adjust parameters associated with line.            */
          Contour, /*!< Adjust parameters associated with contour.         */
          Fill     /*!< Adjust parameters associated with fill.            */
        };

        /*!
          \brief Internal method that configs the canvas based on given Stroke element. 
          
          \param stroke             The Stroke element that will be used to config the canvas.
          \param fromLineSymbolizer A flag that indicates if the given stroke come from LineSymbolizer element.
        */
        void config(const te::se::Stroke* stroke, const bool& fromLineSymbolizer = true);

        /*!
          \brief Internal method that configs the canvas based on given Fill element. 
          
          \param fill The Fill element that will be used to config the canvas.
        */
        void config(const te::se::Fill* fill);

        /*!
          \brief Internal method that configs the canvas based on given Graphic element. 
          
          \param graphic     The Graphic element that will be used to config the canvas.
          \param configStyle Enumerated value that indicates which style that should be configured (for Point, Line, Contour, or Fill).
        */
        void config(const te::se::Graphic* graphic, ConfigStyle configStyle);

        /*! \brief Configs the canvas with default values for polygon styles. */
        void configDefaultPolygon();

        /*! \brief Configs the canvas with default values for line styles. */
        void configDefaultLine();
        
        /*! \brief Configs the canvas with default values for point styles. */
        void configDefaultPoint();

        /*! \brief Configs the canvas with default values for text styles. */
        void configDefaultText();

      private:

        Canvas* m_canvas;                                                    //!< A pointer to the canvas that will be configured.

        static std::map<std::string, te::map::LineCapStyle> sm_lineCapMap;   //!< A map that associates stroke-linecap value to the correct LineCapStyle.
        static std::map<std::string, te::map::LineJoinStyle> sm_lineJoinMap; //!< A map that associates stroke-linejoin value to the correct LineJoinStyle.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_CANVASCONFIGURER_H
