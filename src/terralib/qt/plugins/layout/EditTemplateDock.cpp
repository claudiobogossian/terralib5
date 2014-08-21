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
  \file EditTemplateDock.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EditTemplateDock.h"
#include "../../../layout/core/pattern/mvc/OutsideObserver.h"
#include "../../../layout/outside/EditTemplateModel.h"
#include "../../../layout/outside/EditTemplateController.h"
#include "../../../layout/qt/outside/EditTemplateOutside.h"

// Qt
#include <QTabBar>
#include <QEvent>
#include <QSizePolicy>
#include <QString>

te::qt::plugins::layout::EditTemplateDock::EditTemplateDock( QWidget * parent, Qt::WindowFlags flags ) :
	QDockWidget(parent, flags),
  m_editTemplate(0),
  m_tab(0),
  m_currentTab(-1),
  m_tabHeight(0)
{
  setVisible(false);
  setAllowedAreas(Qt::RightDockWidgetArea);
  setFeatures(QDockWidget::DockWidgetVerticalTitleBar);
    
	create();
  
  m_editTemplateSize.setWidth(m_editTemplate->geometry().width());
  m_editTemplateSize.setHeight(m_editTemplate->geometry().height());

  m_tab = new QTabWidget(this);
  m_tab->addTab(m_editTemplate, "Edit Template");
  m_tab->setTabPosition(QTabWidget::East);
  
  connect(m_tab, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

  setWidget(m_tab);

  int h = getHeightTabBar();
  m_tab->setFixedWidth(h);

  m_tabHeight = h;

  allInstallEventFilter();
}

te::qt::plugins::layout::EditTemplateDock::~EditTemplateDock()
{

}

void te::qt::plugins::layout::EditTemplateDock::create()
{
  //Use the Property Browser Framework for create Property Window
  te::layout::EditTemplateModel* model = new te::layout::EditTemplateModel();		 
  te::layout::EditTemplateController* controller = new te::layout::EditTemplateController(model);
  te::layout::OutsideObserver* outside = (te::layout::OutsideObserver*)controller->getView();
  m_editTemplate = dynamic_cast<te::layout::EditTemplateOutside*>(outside);   
}

te::layout::EditTemplateOutside* te::qt::plugins::layout::EditTemplateDock::getEditTemplateOutside()
{
  return m_editTemplate;
}

void te::qt::plugins::layout::EditTemplateDock::onCurrentChanged( int index )
{
  
}

int te::qt::plugins::layout::EditTemplateDock::getHeightTabBar()
{
  QList<QTabBar*> tabBar = m_tab->findChildren<QTabBar*>();
  tabBar.at(0)->setCursor(Qt::PointingHandCursor);
  int h = tabBar.at(0)->size().height();

  return h;
}

void te::qt::plugins::layout::EditTemplateDock::allInstallEventFilter()
{
  QList<QTabBar*> tabBar = m_tab->findChildren<QTabBar*>();
  for(int i = 0 ; i < tabBar.count() ; ++i)
    tabBar.at(i)->installEventFilter(this);
}

bool te::qt::plugins::layout::EditTemplateDock::eventFilter( QObject * watched, QEvent * event )
{
  QList<QTabBar*> tabBar = m_tab->findChildren<QTabBar*>();
  for(int i = 0 ; i < tabBar.count() ; ++i)
  {
    if (watched == tabBar.at(i) && event->type() == QEvent::MouseButtonPress) 
    {  
      if(m_editTemplate == m_tab->widget(i))
      {
        selectionEditTemplateTab(i);
        break;
      }
    }
  }
    
  return QDockWidget::eventFilter(watched, event);
}

void te::qt::plugins::layout::EditTemplateDock::selectionEditTemplateTab( int index )
{
  if(m_currentTab == index)
  {
    /* fixed size */
    m_tab->setFixedWidth(m_tabHeight);
    m_currentTab = -1;
    return;
  }

  m_currentTab = index;

  /* Remove fixed size */
  m_tab->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX); 
  int w = m_editTemplateSize.width();
  int h = m_editTemplateSize.height();
  m_tab->setMinimumSize(w,h);
}
