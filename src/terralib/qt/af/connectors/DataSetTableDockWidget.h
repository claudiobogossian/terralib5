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
  \file DataSetTableDockWidget.h

  \brief A dock widget for table view objects.
*/

#ifndef __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEDOCKWIDGET_H
#define __TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEDOCKWIDGET_H

#include "../Config.h"

// Qt
#include <QtGui/QDockWidget>

namespace te
{
  // Forward declarations
  namespace gm
  {
    class Envelope;
  }

  namespace map
  {
    class AbstractLayer;
  }

  namespace da
  {
    class ObjectIdSet;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declarations
      class DataSetTableView;
      class ChartDisplayWidget;
    }
    
    namespace af
    {
      namespace evt
      {
        // Forward declarations
        struct Event;
      }

      /*!
        \class DataSetTableDockWidget

        \brief A dock widget for DataSetTableView objects.
      */
      class TEQTAFEXPORT DataSetTableDockWidget : public QDockWidget
      {
        Q_OBJECT

        public:

          /*!
            \brief Constructor.

            \param parent The widget parent.
          */
          DataSetTableDockWidget(QWidget* parent=0);

          /*!
            \brief Destructor.
          */
          ~DataSetTableDockWidget();

          /*!
            \brief Sets the layer to be showed on view. This DOES NOT takes the ownership of \a layer.

            \param layer The layer to be showed.
          */
          void setLayer(te::map::AbstractLayer* layer);

          /*!
            \brief Returns the layer being used.

            \return Layer being used.
          */
          te::map::AbstractLayer* getLayer() const;

        protected slots:

          /*!
            \brief Broadcasts the creation of a new chartWidget to the application

            \param chartWidget The widget containing the generated chart.

          */
          void chartDisplayCreated(te::qt::widgets::ChartDisplayWidget* chartWidget);

          /*!
            \brief Used for capture events sent by application framework.
          */
          void onApplicationTriggered(te::qt::af::evt::Event* evt);

          /*!
            \brief Used for selection changed on table view.

            \param oids The selected object ids.

            \param add True to add to previous selection, false to discard older selection.

            \param env Bounding box of the last object selected.

            \note This function WILL TAKE the ownership of \a oids. It gives the ownership to the layer.
          */
          void selectionChanged(te::da::ObjectIdSet* oids, const bool& add, te::gm::Envelope* env);

          /*!
            \brief Removes the \a oids from the list of selected in the Layer.

            \param oids Set of objects ids to be removed.

            \note This WILL NOT TAKE the ownership of \a oids.
          */
          void removeSelectedOIds(te::da::ObjectIdSet* oids);

        signals:

          /*!
            \brief Broadcasts the creation of a new chartWidget to the application

            \param chartWidget The widget containing the generated chart.

          */
          void createChartDisplay(te::qt::widgets::ChartDisplayWidget*);

          /*!
            \brief Emitted before this widget was closed.
          */
          void closed(te::qt::af::DataSetTableDockWidget*);

        protected:

          te::qt::widgets::DataSetTableView* m_view;           //!< The table view.

          te::map::AbstractLayer* m_layer;                      //!< Layer being visualized.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_TABLE_INTERNAL_DATASETTABLEDOCKWIDGET_H
