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
  \file terralib/qt/widgets/charts/ChartRenderer.h

  \brief A concrete chart renderer based on Qt4.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTRENDERER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTRENDERER_H

// TerraLib
#include "../../../maptools/AbstractChartRenderer.h"
#include "../Config.h"

// Qt
#include <QImage>
#include <QPainter>

// STL
#include <vector>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class ChartRenderer

        \brief A concrete chart renderer based on Qt4.

        \sa AbstractChartRenderer
      */
      class TEQTWIDGETSEXPORT ChartRenderer : public te::map::AbstractChartRenderer
      {
        public:

          /*! \brief Default constructor. */
          ChartRenderer();

          /*! \brief Destructor. */
          ~ChartRenderer();

          te::color::RGBAColor** render(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width);

          te::color::RGBAColor** render(const te::map::Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width);

        protected:

          /** @name Internal methods of Qt4 Chart Renderer.
            *  Methods that manipulate Qt4 concepts like QPainter, QImage, QBrush, QPen, QPainterPath, etc.
            */
          //@{

          /*! \brief Setups the internal QPainter and associates it with the given paint device (QImage). */
          void setup(QImage* img);

          /*! \brief Finalizes the internal QPainter resources. */
          void end();

           //@}

          QImage* drawPies(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width);
          QImage* drawPies(const te::map::Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width);

          QImage* drawBars(const te::map::Chart* chart, const te::da::DataSet* dataset, std::size_t& width);
          QImage* drawBars(const te::map::Chart* chart, const std::map<std::string, double>& chartValue, std::size_t& width);

          void getValues(const te::map::Chart* chart, const te::da::DataSet* dataset, std::vector<double>& values);
          void getValues(const te::map::Chart* chart, const std::map<std::string, double>& chartValue, std::vector<double>& values);

          double computeSum(const std::vector<double>& values);

          double getMaxValue(const std::vector<double>& values);

          //@}

        private:

          QPainter m_painter; //!< The painter used to draw the chart.
          QPen m_pen;         //!< The pen used to draw the chart.
          QBrush m_brush;     //!< The pen used to draw the chart.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTRENDERER_H
