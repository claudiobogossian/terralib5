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
  \file QDisplayWindowOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_QDISPLAY_WINDOWOUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_QDISPLAY_WINDOWOUTSIDE_H

// Qt
#include <QDockWidget>

// TerraLib
#include "LayoutOutsideObserver.h"
#include "../../../../geometry/Envelope.h"

class QGraphicsItem;
class QCloseEvent;

namespace te
{
  namespace layout
  {
    class QDisplayWindowOutside : public QDockWidget, public LayoutOutsideObserver
    {
	    Q_OBJECT //for slots/signals

    public:

	    QDisplayWindowOutside(LayoutOutsideController* controller, LayoutObservable* o);
	    virtual ~QDisplayWindowOutside();

	    virtual void updateObserver(ContextLayoutItem context);
	    virtual void setPosition(const double& x, const double& y);
	    virtual te::gm::Coord2D getPosition();
      void setPreviousCentralWidget(QWidget* previous);

    protected:

      virtual void	closeEvent ( QCloseEvent * event );

      QWidget* m_previousCentralWidget; /* Previous central display of application */
      bool     m_previousCentralWidgetVisibilite;
    };
  }
}


#endif
