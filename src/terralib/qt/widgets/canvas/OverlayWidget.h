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
  \file terralib/qt/widgets/canvas/OverlayWidget.h

  \brief This file has the OverlayWidget class.
*/

#ifndef __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYWIDGET_H
#define __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYWIDGET_H

// TerraLib
#include "../../../maptools/AbstractLayer.h"
#include "../Config.h"

// STL
#include <memory>

// Qt
#include <QWidget>

namespace Ui { class OverlayWidgetForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class MultiThreadMapDisplay;
      class Pan;
      class ZoomWheel;

      /*!
        \class OverlayWidget

        \brief This class is used to define a component for viewing overlaid layers.
      */
      class TEQTWIDGETSEXPORT OverlayWidget : public QWidget
      {
        Q_OBJECT

        public:

          OverlayWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~OverlayWidget();

        public:

          Ui::OverlayWidgetForm* getForm() const;

          void setList(std::list<te::map::AbstractLayerPtr>& layerList);

        protected slots:

          void onApplyToolButtonClicked();

          void onMapDisplayExtentChanged();

          void onOpacityChanged(int value);

          void onVSliderChanged(int value);

          void onHSliderChanged(int value);

          void onHideToolButtonClicked(bool flag);

        protected:

          void drawUpperLayer();

          void updateSliderBarSize();

        private:

         std::auto_ptr<Ui::OverlayWidgetForm> m_ui;

         te::qt::widgets::MultiThreadMapDisplay* m_mapDisplay;
         te::qt::widgets::Pan* m_panTool;                          //!< Pan tool 
         te::qt::widgets::ZoomWheel* m_zoomTool;                   //!< Zoom tool

         QPixmap* m_draft;
         QPixmap* m_draftOriginal;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_CANVAS_INTERNAL_OVERLAYWIDGET_H

