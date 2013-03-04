/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/VisualDockWidget.h

  \brief A dock widget used control the vectorial visual using se elements
      and a property browser to show its properties.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_VISUALDOCKWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_VISUALDOCKWIDGET_H

// TerraLib
#include "../Config.h"

#include "../../../se.h"

// Qt
#include <QtGui/QDockWidget>
#include <QtGui/QTabWidget>

namespace te
{
  namespace map {class AbstractLayer; }

  namespace se { class Symbolizer; }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class PolygonSymbolizerProperty;
      class LineSymbolizerProperty;
      class PointSymbolizerProperty;
      class RasterVisualWidget;
      class StyleControllerWidget;

      /*!
        \class VisualDockWidget

        \brief A dock widget used control the vectorial visual using se elements
            and a property browser to show its properties.
      */
      class TEQTWIDGETSEXPORT VisualDockWidget : public QDockWidget
      {
        Q_OBJECT

        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a RasterVisualDockWidget. */
          VisualDockWidget(const QString & title, QWidget * parent = 0, Qt::WindowFlags flags = 0);

          /*! \brief Destructor. */
          ~VisualDockWidget();

          //@}


        public:
          /*!
            \brief Sets a style element to this widget.

            \param style A valid style element.

            \param layer A valid layer element.

            \note The widget will NOT take the ownership of the given fill.
            \note The widget will be update based on given style parameters.
          */
          void setStyle(te::se::Style* style, te::map::AbstractLayer* layer);

          void setTabStatus(bool status);

        protected:

          /*! \brief Updates the widget form based on internal mark element. */
          void updateUi();

          QWidget* buildUi();

        protected slots:

          void onSymbolizerSelected(te::se::Symbolizer* s);

          void onPolygonSymbolizerChanged();
          void onLineSymbolizerChanged();
          void onPointSymbolizerChanged();
          void onRasterSymbolizerChanged();

        signals:

          void symbolizerChanged(te::se::Symbolizer* s);

        private:

          QTabWidget* m_tabWidget;

          te::qt::widgets::PolygonSymbolizerProperty* m_polyWidget;   //!< Visual widget.
          te::qt::widgets::LineSymbolizerProperty* m_lineWidget;      //!< Visual widget.
          te::qt::widgets::PointSymbolizerProperty* m_pointWidget;    //!< Visual widget.
          te::qt::widgets::RasterVisualWidget* m_visualWidget;        //!< Visual widget.
          te::qt::widgets::StyleControllerWidget* m_styleController;

          te::map::AbstractLayer* m_currentLayer;                     //!< Current Layer pointer (used for raster symbolizer information)
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_VISUALDOCKWIDGET_H
