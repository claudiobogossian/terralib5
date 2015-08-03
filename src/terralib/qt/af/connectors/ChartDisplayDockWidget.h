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
  \file ChartDisplayDockWidget.h

  \brief A dock widget for chart display objects.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYDOCKWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYDOCKWIDGET_H

#include "../Config.h"

// Qt
#include <QDockWidget>

namespace te
{
  namespace map { class AbstractLayer; }

  namespace da
  {
    class ObjectIdSet;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class ChartDisplayWidget;
    }
    
    namespace af
    {
      class ApplicationController;

      namespace evt
      {
        // Forward declarations
        struct Event;
      }

      /*!
        \class ChartDisplayDockWidget

        \brief A dock widget for ChartDisplayWidget objects.
      */
      class TEQTAFEXPORT ChartDisplayDockWidget : public QDockWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

          \param display The widget's chart display.
          \param parent The widget's parent.
          */
          ChartDisplayDockWidget(te::qt::widgets::ChartDisplayWidget* displayWidget, QWidget* parent=0);

          /*!
            \brief Destructor.
          */
          ~ChartDisplayDockWidget();

          /*!
          \brief Sets the app controller to emit signal to app. This DOES NOT takes the ownership of \a app controller.

          \param app The app controller instance from application.
          */
          void setAppController(ApplicationController* app);

          /*!
            \brief Sets the layer to be showed on view. This DOES NOT takes the ownership of \a layer.

            \param layer The layer to be showed.
          */
          void setLayer(te::map::AbstractLayer* layer);

          /*!
            \brief color used to hgihlight selected objects on this display.

            \param selColor The color used to hgihlight selected objects on this display.

          */
          void setSelectionColor(QColor selColor);

          /*!
            \brief Returns the layer being used.

            \return Layer being used.
          */
          te::map::AbstractLayer* getLayer() const;

        protected slots:

          /*!
            \brief Used to capture events sent by the application framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* evt);

          /*!
            \brief Used to handle a selection change.

            \param oids The selected object ids.

            \param add True to add to previous selection, false to discard older selection.

            \note This function WILL TAKE the ownership of \a oids. It gives the ownership to the layer.
          */
          void selectionChanged(te::da::ObjectIdSet* oids, const bool& add);

        signals:

          /*!
            \brief Emitted before this widget was closed.
          */
          void closed(te::qt::af::ChartDisplayDockWidget*);

        protected:

          te::qt::widgets::ChartDisplayWidget* m_displayWidget;  //!< The widget containing the display

          te::map::AbstractLayer* m_layer;                       //!< Layer being visualized.

          ApplicationController* m_app;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_CHARTDISPLAYDOCKWIDGET_H
