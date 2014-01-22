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
  \file terralib/qt/widgets/classification/ROIManagerWidget.h

  \brief This file has the ROIManagerWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERWIDGET_H
#define __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QtGui/QWidget>

namespace Ui { class ROIManagerWidgetForm; }

namespace te
{
  namespace cl
  {
    class ROISet;
  }

  namespace qt
  {
    namespace widgets
    {
      class ColorPickerToolButton;
      class RasterNavigatorWidget;

      /*!
        \class ROIManagerWidget

        \brief This class is used to manager the ROI acquisition.
      */
      class TEQTWIDGETSEXPORT ROIManagerWidget : public QWidget
      {
        Q_OBJECT

        public:

          ROIManagerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~ROIManagerWidget();

          Ui::ROIManagerWidgetForm* getForm() const;

        public:

          void set(te::map::AbstractLayerPtr layer);

          te::cl::ROISet* getROISet();

        protected:
          
          void drawROISet();

          void updateSamples();

        public slots:

          void onAddROIToolButtonClicked();

          void onRemoveROIToolButtonClicked();

          void onMapDisplayExtentChanged();

          void onGeomAquired(te::gm::Polygon* poly);

        private:

         std::auto_ptr<Ui::ROIManagerWidgetForm> m_ui;

         std::auto_ptr<te::qt::widgets::RasterNavigatorWidget> m_navigator;

         ColorPickerToolButton* m_colorPicker;

         te::cl::ROISet* m_rs;

         int m_sampleCounter;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CLASSIFICATION_INTERNAL_ROIMANAGERWIDGET_H

