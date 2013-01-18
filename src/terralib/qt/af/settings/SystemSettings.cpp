/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/settings/SystemSettings.cpp

  \brief A widget for setting user preferences.
*/

#include "ui_SystemSettingsForm.h"
#include "ComponentWidget.h"
#include "ComponentWidgetFactory.h"
#include "DisplayFrameFactory.h"
#include "GridFrameFactory.h"
#include "ProjectFrameFactory.h"
#include "SystemSettings.h"

te::qt::af::SystemSettings::SystemSettings(QWidget* parent)
  : QDialog(parent),
    m_ui(new Ui::SystemSettingsForm),
    m_curComp(0)
{
  m_ui->setupUi(this);

  //init factories
  DisplayFrameFactory::initialize();
  GridFrameFactory::initialize();
  ProjectFrameFactory::initialize();

  std::vector<std::string> vec;
  const te::qt::af::ComponentWidgetFactory::dictionary_type& d = te::qt::af::ComponentWidgetFactory::getDictionary();
  te::qt::af::ComponentWidgetFactory::dictionary_type::const_iterator it = d.begin();

  while(it != d.end())
  {
    vec.push_back(it->first);
    ++it;
  }

  for(size_t i = 0; i < vec.size(); i++)
  {
    m_ui->m_componentsListWidget->addItem(vec[i].c_str());
  }

  connect(m_ui->m_componentsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(on_itemClicked(QListWidgetItem*)));

}

te::qt::af::SystemSettings::~SystemSettings()
{
  delete m_curComp;
}

void te::qt::af::SystemSettings::on_itemClicked(QListWidgetItem * current) 
{
  std::string value = current->text().toStdString();
  delete m_curComp;
  m_curComp = te::qt::af::ComponentWidgetFactory::make(value, m_ui->m_scrollArea);
  m_ui->m_scrollArea->setWidget(m_curComp);
  m_curComp->show();
}
