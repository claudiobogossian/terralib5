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
  \file terralib/qt/widgets/charts/ChartConfigurer.h
  
  \brief A Symbology Enconding visitor that configures a given chart based on symbolizer elements.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTCONFIGURER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTCONFIGURER_H

// TerraLib
#include "../../../se/Visitor.h"
#include "../Config.h"
#include "ChartDisplayWidget.h"
#include "Enums.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  // Forward declarations
  namespace map { class AbstractLayer; }

  namespace se
  {
    class Fill;
    class Graphic;
    class GraphicStroke;
    class Stroke;
  }

  namespace qt
  {
    namespace widgets
    {

    /*!
      \class ChartConfigurer
      
      \brief A Symbology Enconding visitor that configures a given chart based on symbolizers elements.
             If you want to use this visitor, use a command like:
             <code>
             te::qt::widgets::ChartConfigurer cc(chart);
             cc.config(symbolizer);
             </code>

      \sa te::se::Visitor
    */
    class TEQTWIDGETSEXPORT ChartConfigurer : public te::se::Visitor, public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         */
        //@{

        /*! 
          \brief Constructor. 
          
          \param chart A pointer to the chart that will be configured.

          \note This class will not take the ownership of the given chart.
        */
        ChartConfigurer(ChartDisplayWidget* chart);

        /*! \brief Destructor. */
        ~ChartConfigurer();

        //@}

        /** @name Config Method
         *  Method that can be used to configure a chart based on Symbology Enconding symbolizer elements.
         */
        //@{

        /*!
          \brief It configs the chart based on given symbolizer.

          \param symbolizer The symbolizer that will be used to config the chart.
        */
        void config(const te::se::Symbolizer* symbolizer);

        /*!
          \brief It configs the chart based on given layer.

          \param layer The layer that will be used to config the chart.

          \note This function will simply use the first rule and symbolizer of the given layer.
         */
        void config(te::map::AbstractLayer* layer);

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
          \brief Internal method that configs the chart based on given Stroke element. 
          
          \param stroke  The Stroke element that will be used to config the chart.
        */
        void config(const te::se::Stroke* stroke);

        /*!
          \brief Internal method that configs the chart based on given Fill element. 
          
          \param fill The Fill element that will be used to config the chart.
        */
        void config(const te::se::Fill* fill);

        /*!
          \brief Internal method that configs the chart based on given Graphic element. 
          
          \param graphic     The Graphic element that will be used to config the chart.
        */
        void config(const te::se::Graphic* graphic);

      private:

        ChartDisplayWidget* m_chart;  //!< A pointer to the chart that will be configured.

    };

    } // end namespace widgets
  }   // end namespace qt
}   // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTCONFIGURER_H