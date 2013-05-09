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
  \file terralib/qt/widgets/charts/ChartStyle.h

  \brief A class used to define a chart's style
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H

//TerraLib
#include "../Config.h"

//QT
#include "qcolor.h"

//STL
#include <string.h>

namespace te
{
  namespace se 
  {
    //forward declarations
    class Fill;
    class Stroke;
  }

  namespace qt
  {

    namespace widgets
    {
      class TEQTWIDGETSEXPORT ChartStyle
      {

        public:

          ChartStyle();

          ChartStyle(QString title, te::se::Fill* fill, te::se::Stroke* stroke, bool gridChecked,  QColor color);

          ~ChartStyle();

          QString& getTitle();

          void setTitle(QString newTitle);

          te::se::Fill* getFill();

          void setFill(te::se::Fill* newFill);

          te::se::Stroke* getStroke();

          void setStroke(te::se::Stroke* newStroke);

          QColor& getColor();

          void setColor(QColor newColor);

          bool getGridChecked();

          void setGridChecked(bool newGridChecked);

        private:

          QString          m_title;         //!< The chart's title.
          bool             m_gridChecked;   //!< The boolean used to decide if the chart will have a background grid .
          te::se::Fill*    m_fill;          //!< The chart's fill.
          te::se::Stroke*  m_stroke;        //!< The chart's stroke.
          QColor           m_backColor;     //!< The chart's background color.

      };
    }; //end namespace widgets
  }    //end namespace qt
}      // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTSTYLE_H