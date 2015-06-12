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
  \file terralib/qt/widgets/se/OverlapBehaviorWidget.cpp

  \brief A dialog used to build a OverlapBehavior element.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "OverlapBehaviorWidget.h"
#include "ui_OverlapBehaviorWidgetForm.h"
#include "../../../se.h"

// Qt


// STL


te::qt::widgets::OverlapBehaviorWidget::OverlapBehaviorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::OverlapBehaviorWidgetForm)
{
  m_ui->setupUi(this);

  initialize();

  // Signals & slots
  connect(m_ui->m_comboBox, SIGNAL(activated(QString)), SLOT(onValueChanged(QString)));
}

te::qt::widgets::OverlapBehaviorWidget::~OverlapBehaviorWidget()
{
  m_obNames.clear();
}

void te::qt::widgets::OverlapBehaviorWidget::setOverlapBehavior(te::se::RasterSymbolizer::OverlapBehavior value)
{
  m_ob = value;

  updateUi();
}

te::se::RasterSymbolizer::OverlapBehavior te::qt::widgets::OverlapBehaviorWidget::getOverlapBehavior() const
{
  return m_ob;
}

void te::qt::widgets::OverlapBehaviorWidget::initialize()
{
  //define the Overlap Behavior names
  m_obNames.clear();

  m_obNames.insert(std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::value_type
    (te::se::RasterSymbolizer::LATEST_ON_TOP, tr("Latest on Top")));
  m_obNames.insert(std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::value_type
    (te::se::RasterSymbolizer::EARLIEST_ON_TOP, tr("Earliest on Top")));
  m_obNames.insert(std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::value_type
    (te::se::RasterSymbolizer::AVERAGE, tr("Average")));
  m_obNames.insert(std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::value_type
    (te::se::RasterSymbolizer::RANDOM, tr("Random")));
  m_obNames.insert(std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::value_type
    (te::se::RasterSymbolizer::NO_BEHAVIOR, tr("No Behavior")));


  //fill the combo box with this names
  std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::iterator it = m_obNames.begin();

  while(it != m_obNames.end())
  {
    m_ui->m_comboBox->addItem(it->second);

    ++it;
  }

  //set default value
  m_ob = te::se::RasterSymbolizer::NO_BEHAVIOR;

  updateUi();
}

void te::qt::widgets::OverlapBehaviorWidget::updateUi()
{
  m_ui->m_comboBox->setCurrentIndex(m_ui->m_comboBox->findText(m_obNames[m_ob]));
}

void te::qt::widgets::OverlapBehaviorWidget::onValueChanged(QString value)
{
  std::map<te::se::RasterSymbolizer::OverlapBehavior, QString>::iterator it = m_obNames.begin();

  while(it != m_obNames.end())
  {
    if(it->second == value)
    {
      m_ob = it->first;
      break;
    }

    ++it;
  }
}