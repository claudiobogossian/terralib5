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
  \file terralib/qt/widgets/rp/InputLayerWidget.h

  \brief This file has the InputLayerWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_INPUTLAYERWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_INPUTLAYERWIDGET_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../maptools/AbstractLayer.h"
#endif
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class InputLayerWidgetForm; }

namespace te
{
  namespace rst { class Raster; }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class InputLayerWidget

        \brief This class is used to set input layer.
      */
      class TEQTWIDGETSEXPORT InputLayerWidget : public QWidget
      {
        Q_OBJECT

        public:

          InputLayerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~InputLayerWidget();

          Ui::InputLayerWidgetForm* getForm() const;

          te::map::AbstractLayerPtr getInputLayer();

          std::string getType() const;

          std::map<std::string, std::string> getInfo() const;

          void setInputLayer(te::map::AbstractLayerPtr layer);

          void setLayerList(std::list<te::map::AbstractLayerPtr> list);

        protected slots:

          void onSearchLayerToolButtonClicked();

          void onLayersComboBoxActivated(int index);

        signals:

          void layerChanged();

        private:

          std::auto_ptr<Ui::InputLayerWidgetForm> m_ui;

          te::map::AbstractLayerPtr m_layer;
          std::list<te::map::AbstractLayerPtr> m_layerList;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_INPUTLAYERWIDGET_H

