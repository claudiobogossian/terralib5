/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ToolbarOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H

// Qt
#include <QToolBar>

// TerraLib
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../../geometry/Envelope.h"
#include "../../core/enum/EnumMode.h"
#include "../../core/Config.h"

// STL
#include <string>

class QGraphicsItem;
class QWidget;
class QGroupBox;
class QAction;
class QToolButton;
class QMenu;
class QPushButton;
class QComboBox;

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT ToolbarOutside : public QToolBar, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    ToolbarOutside(OutsideController* controller, Observable* o);

	    virtual ~ToolbarOutside();

	    virtual void updateObserver(ContextItem context);

	    virtual void setPosition(const double& x, const double& y);

	    virtual te::gm::Coord2D getPosition();
      
    public slots:
      
      virtual void onMapTriggered(QAction* action);

      virtual void onMapToolsTriggered(QAction* action);

      virtual void onGeometryTriggered(QAction* action);

      virtual void onViewAreaTriggered(QAction* action);
      
      virtual void onArrowCursorClicked(bool checked);

      virtual void onItemToolsTriggered(QAction* action);

      virtual void onLineIntersectionMouse(bool checked);

      virtual void onSceneZoomCurrentIndexChanged(int index);

      virtual void onBringToFrontClicked(bool checked);

      virtual void onSendToBackClicked(bool checked);

      virtual void onRecomposeClicked(bool checked);
      
    signals:

      void changeContext(bool change);
      
    protected:

      virtual void createToolbar();

      virtual void createMapToolButton();

      virtual void createMapToolsToolButton();

      virtual void createGeometryToolButton();

      virtual void createViewAreaToolButton();

      virtual void createArrowCursorButton();

      virtual void createItemTools();

      virtual void createLineIntersectionToolButton();

      virtual void createSceneZoomCombobox();

      virtual void createBringToFrontToolButton();

      virtual void createSendToBackToolButton();

      virtual void createRecomposeToolButton();

      virtual void changeAction(LayoutMode mode);

      virtual QToolButton* createToolButton(std::string text, std::string tooltip, std::string icon);

      virtual QPushButton* createPushButton(std::string text, std::string tooltip, std::string icon);
      
      virtual QAction* createAction(std::string text, std::string objName, std::string icon, std::string tooltip = "");

    protected:

      QToolButton* m_btnMap;
      QComboBox* m_comboSceneZoom;
      /* Map Menu */
      std::string m_optionMapDefault;
      std::string m_optionMapGrid;
      std::string m_optionLegendDefault;
      std::string m_optionScale;

      /* Map Tools */
      std::string m_optionMapZoomIn;
      std::string m_optionMapZoomOut;
      std::string m_optionMapPan;

      /* Geometry Menu */
      std::string m_optionRectangle;

      /* View Area */
      std::string m_optionViewPan;
      std::string m_optionViewZoomIn;
      std::string m_optionViewZoomOut;
      
      /* Items Tools */

      std::string m_optionGroup;
      std::string m_optionUngroup;

      std::string m_optionLineIntersectionMouse;

      std::string m_optionSceneZoom;
    };
  }
}


#endif
