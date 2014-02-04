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
  \file terralib/qt/widgets/layer/search/LayerSelectorWidget.h

  \brief This file has the LayerSelectorWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERSELECTORWIDGET_H
#define __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERSELECTORWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../../Config.h"

// STL
#include <memory>
#include <list>

// Qt
#include <QtGui/QWidget>

namespace Ui { class LayerSelectorWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayerSearchDialog;

      /*!
        \class LayerSelectorWidget

        \brief This class represents a component used to quickly select a layer

        \sa LayerSearchWidget
      */
      class TEQTWIDGETSEXPORT LayerSelectorWidget : public QWidget
      {
        Q_OBJECT
        
        public:

          enum FilterOptions
          {
            FILTER_BY_RASTER,
            FILTER_BY_GEOM,
            NO_FILTER
          } ;

        public:

          LayerSelectorWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~LayerSelectorWidget();

          Ui::LayerSelectorWidgetForm* getForm() const;

        public:

          void setList(std::list<te::map::AbstractLayerPtr>& layerList, FilterOptions fo = NO_FILTER);

        protected:

          void filterOnlyByRaster();

          void filterOnlyByGeom();

          void filterAll();

        public slots:

          void onLayerSearchToolButtonClicked();

          void onLayerComboBoxActivated(int index);

        signals:

          void layerSelected(te::map::AbstractLayerPtr l);

        private:

         std::auto_ptr<Ui::LayerSelectorWidgetForm> m_ui;
         std::auto_ptr<LayerSearchDialog> m_layerSearchDlg;

         std::list<te::map::AbstractLayerPtr> m_layerList;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_INTERNAL_LAYERSELECTORWIDGET_H
