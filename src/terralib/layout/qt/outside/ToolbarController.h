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
  \file ToolbarController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_CONTROLLER_H_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_CONTROLLER_H_H

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../../core/Config.h"

// Qt
#include <QObject>

class QAction;

namespace te
{
  namespace layout
  {
    class Scene;
    class AbstractOutsideModel;

    class TELAYOUTEXPORT ToolbarController : public QObject, public AbstractOutsideController
    {
      Q_OBJECT //for slots/signals

      public:

        ToolbarController(AbstractOutsideModel* o);

        virtual ~ToolbarController();

      public slots:

        virtual void onMapTriggered(QAction* action);

        virtual void onMapToolsTriggered(QAction* action);

        virtual void onGeometryTriggered(QAction* action);

        virtual void onViewAreaTriggered(QAction* action);

        virtual void onArrowCursorClicked(bool checked);

        virtual void onItemToolsTriggered(QAction* action);
        
        virtual void onComboZoomActivated();

        virtual void onZoomChanged(int zoom);

        virtual void onBringToFrontClicked(bool checked);

        virtual void onSendToBackClicked(bool checked);

        virtual void onRecomposeClicked(bool checked);

        virtual void onTextToolsTriggered(QAction* action);

        virtual void onAlignLeftClicked(bool checked);

        virtual void onAlignRightClicked(bool checked);

        virtual void onAlignTopClicked(bool checked);

        virtual void onAlignBottomClicked(bool checked);

        virtual void onAlignCenterHorizontalClicked(bool checked);

        virtual void onAlignCenterVerticalClicked(bool checked);

        virtual void onRemoveObjectClicked(bool checked);

        virtual void onDrawMapClicked(bool checked);

        virtual void onObjectToImageClicked(bool checked);

        virtual void onExitClicked(bool checked);

        virtual void onExportToPDFClicked(bool checked);

      protected:

        Scene* getScene();
    };
  }
}

#endif

