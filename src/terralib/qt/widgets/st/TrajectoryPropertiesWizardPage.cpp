/*  Copyright (C) 2010-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/st/TrajectoryPropertiesWIzardPage.cpp

  \brief  A wizardPage used to configure the generaç properties of a new Trajectory layer.
*/

// TerraLib
#include "../../../st/core/trajectory/TrajectoryDataSetInfo.h"
#include "TemporalPropertiesWidget.h"
#include "TrajectoryPropertiesWidget.h"
#include "TrajectoryPropertiesWizardPage.h"
#include "ui_TrajectoryPropertiesWidgetForm.h"
#include "ui_TemporalPropertiesWidgetForm.h"

te::qt::widgets::TrajectoryPropertiesWizardPage::TrajectoryPropertiesWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
//  m_ui->setupUi(this);
  m_propWidget.reset(new TrajectoryPropertiesWidget(this));
  m_genPropWidget.reset(new TemporalPropertiesWidget(this));

  // Adjusting...
  QGridLayout* propLayout = new QGridLayout(this);
  propLayout->addWidget(m_propWidget.get());

  // Adjusting the properties widget
  QGridLayout* genPropLayout = new QGridLayout(m_propWidget->getForm()->m_generalPropertiesFrame);
  genPropLayout->setContentsMargins(0, 0, 0, 0);
  genPropLayout->addWidget(m_genPropWidget.get());
}

te::qt::widgets::TrajectoryPropertiesWizardPage::~TrajectoryPropertiesWizardPage()
{
}

std::list<te::st::TrajectoryDataSetInfo*> te::qt::widgets::TrajectoryPropertiesWizardPage::getInfo(const te::da::DataSourceInfoPtr dsInfo)
{
  std::list<te::st::TrajectoryDataSetInfo*> trajInfos;

  std::list<te::da::DataSetTypePtr>::const_iterator typesItBegin = m_dataTypes.begin();
  std::list<te::da::DataSetTypePtr>::const_iterator typesItEnd = m_dataTypes.end();

  while(typesItBegin != typesItEnd)
  {
    trajInfos.push_back(new te::st::TrajectoryDataSetInfo(*dsInfo.get(), typesItBegin->get()->getName(), m_genPropWidget->getPhenomenonTime(), m_propWidget->getGeometryId(), -1, typesItBegin->get()->getName()));
    typesItBegin++;
  }

  return trajInfos;
}

bool te::qt::widgets::TrajectoryPropertiesWizardPage::isComplete() const
{
  return true;
}

void te::qt::widgets::TrajectoryPropertiesWizardPage::set(const std::list<te::da::DataSetTypePtr> dataTypes)
{
  m_dataTypes = dataTypes;
  m_genPropWidget->setUp(dataTypes.front());
  m_propWidget->setUp(dataTypes.front());
}
