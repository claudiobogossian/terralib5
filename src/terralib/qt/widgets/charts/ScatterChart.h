/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file  terralib/qt/widgets/charts/ScatterChart.h

  \brief A class to represent a scatter's chart. 
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCHART_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCHART_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot_curve.h>

//STL
#include <vector>

namespace te
{

  namespace da    { class ObjectIdSet; }
  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class Scatter;
      class ScatterStyle;

      /*!
        \class ScatterChart

        \brief A class to represent a scatter chart.
      */
      class TEQTWIDGETSEXPORT ScatterChart : public QwtPlotCurve
      {
        public:

          /*!
            \brief Constructor

            It constructs a scatter chart with a default mark and size.

            \note It will not take the ownership of the pointers.
          */
          ScatterChart(Scatter* data);
          
          /*!
            \brief Constructor

            \note It will take the ownership of the pointer "symbol".
            \note It will not take the ownership of the pointer "data".
          */
          ScatterChart(Scatter* data, ScatterStyle* style, size_t size);

          /*! \brief Destructor. */
          ~ScatterChart();

          /*! \brief Returns the chart's type. */
          virtual int rtti() const;

          te::qt::widgets::Scatter* getScatter();

          void setScatter(te::qt::widgets::Scatter* newScatter);

          ScatterStyle* getScatterStyle();

          void setScatterStyle(ScatterStyle* newStyle);

          /*!
            \brief Highlights the objects identified by \a oids

            \param oids The identifiers of plotitems to be highlighted.
            \param items The plotitems to be highlighted.
          */
          void highlight(const te::da::ObjectIdSet* oids);

        private:

          void setData();

        private:
          Scatter*       m_scatter;       //!< The scatter that will be shown on this chart.
          ScatterStyle*  m_scatterStyle;  //!< The symbol that defines the look of a scatter's point.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_SCATTERCHART_H

