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
  \file terralib/qt/widgets/layer/utils/CharEncodingMenuWidget.cpp

  \brief This file defines a class for a CharEncodingMenuWidget.
*/

// TerraLib
#include "../../../../dataaccess/utils/Utils.h"
#include "CharEncodingMenuWidget.h"

// Qt
#include <QActionGroup>

te::qt::widgets::CharEncodingMenuWidget::CharEncodingMenuWidget(QWidget* parent, Qt::WindowFlags f):QWidget(parent, f)
{
  //create menu
  m_menu = new QMenu(tr("Char Encoding"), this);

  //create action group
  m_actGroup = new QActionGroup(this);

  std::vector<std::string> etNames = te::core::CharEncoding::getEncodingList();

  for (std::size_t t = 0; t < etNames.size(); ++t)
  {
    //create actions
    QAction* act = new QAction(etNames[t].c_str(), this);
    
    act->setData(QVariant(etNames[t].c_str()));
    act->setCheckable(true);

    //add actions to menu
    m_menu->addAction(act);
    m_actGroup->addAction(act);

    //connections
    connect(act, SIGNAL(triggered()), this, SLOT(onEncodingClicked()));
  }
}

te::qt::widgets::CharEncodingMenuWidget::~CharEncodingMenuWidget()
{
}

void te::qt::widgets::CharEncodingMenuWidget::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::string etName = te::core::CharEncoding::getEncodingName(m_layer->getEncoding());

  QList<QAction*> actions = m_actGroup->actions();
  QList<QAction*>::iterator it = actions.begin();

  while (it != actions.end())
  {
    std::string etCurName = (*it)->data().toString().toStdString();

    if (etCurName == etName)
    {
      (*it)->setChecked(true);

      break;
    }

    ++it;
  }

  checkLayerDataSourceType();
}

QMenu* te::qt::widgets::CharEncodingMenuWidget::getMenu()
{
  return m_menu;
}

void te::qt::widgets::CharEncodingMenuWidget::checkLayerDataSourceType()
{
  te::da::DataSourcePtr ds = te::da::GetDataSource(m_layer->getDataSourceId());

  QList<QAction*> actions = m_actGroup->actions();
  QList<QAction*>::iterator it = actions.begin();

  while (it != actions.end())
  {
    if (ds->getType() == "OGR")
      (*it)->setEnabled(true);
    else
      (*it)->setEnabled(false);

    ++it;
  }
}

void te::qt::widgets::CharEncodingMenuWidget::onEncodingClicked()
{
  QAction* act = dynamic_cast<QAction*>(sender());

  if (act)
  {
    std::string etName = act->data().toString().toStdString();

    std::vector<std::string> etNames = te::core::CharEncoding::getEncodingList();

    for (std::size_t t = 0; t < etNames.size(); ++t)
    {
      if (etName == etNames[t])
      {
        m_layer->setEncoding(te::core::CharEncoding::getEncodingType(etName));
      }
    }
  }
}
