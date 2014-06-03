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
  \file  terralib/qt/widgets/charts/ChartDisplay.h

  \brief A class to represent a chart display
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H

//TerraLib
#include "../Config.h"

//QWT
#include <qwt_plot.h>

//STL
#include <string>

//QWT Forward declarations
class QwtPlotGrid;
class QwtPlotPanner;
class QwtPlotPicker;
class QwtPlotMagnifier;

namespace te
{
  namespace color { class RGBAColor; }
  namespace da    { class ObjectIdSet; }
  namespace se    { class Stroke; class Fill; class Font; }

  namespace qt
  {
    namespace widgets
    {

      //Forward declaration
      class ChartStyle;

      /*!
        \class ChartDisplay

        \brief A class to represent a chart display

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT ChartDisplay : public QwtPlot
      {

        Q_OBJECT

        public:

          /*!
            \brief Constructor

            It constructs a ChartDisplay with the given title and style.

            \param parent this widget's parent
            \param title this widget's title.

            \note it will take the ownership of the pointer "style". If style is null, the display will be constructed with a default style.
          */
          ChartDisplay(QWidget* parent=NULL, QString title = "", ChartStyle* style = 0);

          /*!
            \brief Destructor
          */
          ~ChartDisplay();

          /*!
            \brief Returns a pointer to the display's style

            \return A ChartStyle type pointer to the display's style.
            \note The caller will not take ownership of the returned pointer. 
          */
          te::qt::widgets::ChartStyle* getStyle();

          /*!
            \brief It sets the display's style

            \param newStyle The new style.
            \note It will take ownership of the given pointer 
          */
          void setStyle(te::qt::widgets::ChartStyle* newStyle);

          /*!
            \brief Highlights the objects identified by \a oids

            \param oids The identifiers of plotitems to be highlighted.
          */
          void highlightOIds(const te::da::ObjectIdSet* oids);

          /*!
            \brief color used to hgihlight selected objects on this display.

            \param selColor The color used to hgihlight selected objects on this display.

          */
          void setSelectionColor(QColor selColor);

          /*!
            \brief Updates the general display settings according to the ChartStyle.
                   The adjusted properties are:
                      Title;
                      Axis titles (x and Y);
                      Weather to display the grid or not;
                      Background color;
          */
          void adjustDisplay();

        protected slots:

          /*!
            \brief Called when the user selects an area of the canvas.
                   Will highlight the data, if possible, depending on the type of chart being displayed (f.e. histogram, scatter, etc).
          */
          void onRectPicked(const QRectF &rect);

        signals:

          /*!
            \brief Emmit when objects were selected.
          */
          void selected(te::da::ObjectIdSet*, const bool&);

         private:

          ChartStyle*       m_chartStyle;   //!< The display's style.
          QwtPlotGrid*      m_grid;         //!< The display's grid
          QwtPlotPanner*    m_panner;       //!< The display's panner.
          QwtPlotPicker*    m_leftPicker;   //!< The display's left button picker.
          QwtPlotMagnifier* m_magnifier;     //!< The display's magnifinifier.
          QwtPlotPicker*    m_ctrlPicker;   //!< The display's control button picker.
          QwtPlotPicker*    m_shiftPicker;  //!< The display's shift button picker.
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAY_H

