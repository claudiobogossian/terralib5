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
  \file DisplayDock.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_DISPLAY_DOCK_H 
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_DISPLAY_DOCK_H

// Qt
#include <QDockWidget>

class QCloseEvent;

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout
      {
        class DisplayDock : public QDockWidget
        {
	        Q_OBJECT //for slots/signals

        public:

	        DisplayDock(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	        virtual ~DisplayDock();
                
          void setPreviousCentralWidget(QWidget* previous);
      
          virtual void removeDock();

        protected slots:

          virtual void onExit();
      
        protected:

          virtual void	closeEvent ( QCloseEvent * event );

          QWidget* m_previousCentralWidget; /* Previous central display of application */
          bool     m_previousCentralWidgetVisibilite;
        };
      }
    }
  }
}

#endif
