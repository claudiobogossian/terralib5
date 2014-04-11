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
#include <QDockWidget>

// TerraLib
#include "OutsideObserver.h"
#include "../../../../geometry/Envelope.h"

class QGraphicsItem;
class QWidget;
class QToolBar;
class QGroupBox;

namespace te
{
  namespace layout
  {
    class ToolbarOutside : public QDockWidget, public OutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    ToolbarOutside(OutsideController* controller, Observable* o);
	    virtual ~ToolbarOutside();

	    virtual void updateObserver(ContextItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();
      
    public slots:

      virtual void onClickPanTool(bool toggled);
      virtual void onClickZoomInTool(bool toggled);
      virtual void onClickZoomOutTool(bool toggled);
      virtual void onIndexChanged(int index);
      virtual void onClickMapTool(bool toggled);
      virtual void onClickRectangleTool(bool toggled);
      virtual void onClickSalvePropsTool(bool toggled);

    signals:

      void changeContext(bool change);
      
    protected:

      virtual void createToolbar();

      QToolBar*  m_toolbar;
    };
  }
}


#endif
