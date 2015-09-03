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
  \file EditTemplateDock.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT2_INTERNAL_EDIT_TEMPLATE_DOCK_H 
#define __TE_QT_PLUGINS_LAYOUT2_INTERNAL_EDIT_TEMPLATE_DOCK_H

// Qt
#include <QDockWidget>
#include <QTabWidget>

// STL
#include <vector>


namespace te
{
  namespace layout
  {
    class EditTemplateOutside;
  }    
  namespace layout
  {
    class EditTemplateDock : public QDockWidget
    {
      Q_OBJECT //for slots/signals

    public:

      EditTemplateDock( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
      virtual ~EditTemplateDock();

      te::layout::EditTemplateOutside* getEditTemplateOutside();

      virtual bool eventFilter( QObject * watched, QEvent * event );

    protected slots:

      virtual void onCurrentChanged ( int index );

    protected:

      virtual void create();

      virtual int getHeightTabBar();

      virtual void allInstallEventFilter();

      virtual void selectionEditTemplateTab(int index);

      te::layout::EditTemplateOutside* m_editTemplate;
      QTabWidget*                      m_tab;
      int                              m_currentTab;
      double                           m_tabHeight;
      QRectF                           m_editTemplateSize;
    };
  }
}


#endif
