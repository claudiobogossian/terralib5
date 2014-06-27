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
  \file terralib/qt/widgets/layer/search/LayerSearchWidget.h

  \brief This file defines the LayerSearchWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIDGET_H

// TerraLib
#include "../../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <list>
#include <memory>

// Qt
#include <QTreeWidgetItem>
#include <QWidget>

namespace Ui { class LayerSearchWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class LayerSearchWidget

        \brief A widget used to search layers over list of layers.

        \ingroup widgets
      */
      class TEQTWIDGETSEXPORT LayerSearchWidget : public QWidget
      {
        Q_OBJECT

        public:

          LayerSearchWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~LayerSearchWidget();

          Ui::LayerSearchWidgetForm* getForm() const;

        public:

          void enableMultiSelection(bool flag);

          std::list<te::map::AbstractLayerPtr> getSelecteds() const;

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

          void filterOnlyByRaster();

          void filterOnlyByGeom();

          void filterAll();

        protected:

          void fillTreeView(std::list<te::map::AbstractLayerPtr>& layerList);

          void filter(const QList<QTreeWidgetItem*>& items);

        private slots:

          void onFilter();

        private:

          std::auto_ptr<Ui::LayerSearchWidgetForm> m_ui;

          std::list<te::map::AbstractLayerPtr> m_layerList;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_SEARCH_INTERNAL_LAYERSEARCHWIDGET_H
