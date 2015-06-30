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
  \file terralib/qt/af/BaseApplication.h

  \brief A QMainWindow to be used as the basis for TerraLib applications.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
#define __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H

#include "Config.h"

// TerraLib
#include "../../maptools/AbstractLayer.h"


#include <QMainWindow>

//#include <ui_BaseApplicationForm.h>

namespace Ui {
  class BaseApplicationForm;
}

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class LayerExplorer;
    }

    namespace af
    {      
      namespace evt
      {
        // Forward declaration
        struct Event;
      }

      class ApplicationController;
      class MapDisplay;
      class LayerExplorer;

      class TEQTAFEXPORT BaseApplication : public QMainWindow
      {
        Q_OBJECT

      public:

        BaseApplication(QWidget* parent = 0);

        virtual ~BaseApplication();

        virtual void init(const QString& cfgFile);

        te::qt::widgets::LayerExplorer* getLayerExplorer();

      public slots:

        void onAddLayerTriggered();

        void onDrawTriggered();

        void onFitLayersTriggered();

        void onZoomInTriggered(bool s);

        void onPanTriggered(bool s);

        void onSelectionTriggered(bool s);

        virtual void onApplicationTriggered(te::qt::af::evt::Event* e);

      protected slots:

        void onLayerSelectionChanged(const te::map::AbstractLayerPtr& layer);

      signals:

        void triggered(te::qt::af::evt::Event* e);

      protected:

        virtual void initFramework();

        QMenu* getMenuFile();

        QToolBar* getToolbar(const QString& barName);

        ApplicationController* m_app;
        MapDisplay* m_display;
        LayerExplorer* m_layerExplorer;

      private:
        Ui::BaseApplicationForm* m_ui;
      };
    }
  }
}

#endif // __TERRALIB_QT_AF_INTERNAL_BASEAPPLICATION_H
