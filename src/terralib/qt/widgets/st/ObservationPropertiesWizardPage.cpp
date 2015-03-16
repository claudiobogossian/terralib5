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
  \file terralib/qt/widgets/st/ObservationPropertiesWIzardPage.cpp

  \brief  A wizardPage used to configure the properties of a new Observation layer.
*/

// TerraLib
#include "../../../st/core/observation/ObservationDataSetInfo.h"
#include "ObservationPropertiesWidget.h"
#include "ObservationPropertiesWizardPage.h"
#include "ui_ObservationPropertiesWidgetForm.h"

te::qt::widgets::ObservationPropertiesWizardPage::ObservationPropertiesWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
//  m_ui->setupUi(this);
  m_propWidget.reset(new ObservationPropertiesWidget(this));

  // Adjusting...
  QGridLayout* propLayout = new QGridLayout(this);
  propLayout->addWidget(m_propWidget.get());
}

te::qt::widgets::ObservationPropertiesWizardPage::~ObservationPropertiesWizardPage()
{
}

std::list<te::st::ObservationDataSetInfo*> te::qt::widgets::ObservationPropertiesWizardPage::getInfo(const te::da::DataSourceInfoPtr dsInfo)
{
  std::list<te::st::ObservationDataSetInfo*> obsInfos;

  std::list<te::da::DataSetTypePtr>::const_iterator typesItBegin = m_dataTypes.begin();
  std::list<te::da::DataSetTypePtr>::const_iterator typesItEnd = m_dataTypes.end();

  while(typesItBegin != typesItEnd)
  {
    obsInfos.push_back(new te::st::ObservationDataSetInfo(*dsInfo.get(), typesItBegin->get()->getName(), m_propWidget->getTempPropName(), m_propWidget->getTempPropName(), m_propWidget->getGeometryId()));
    typesItBegin++;
  }

  return obsInfos;
}

bool te::qt::widgets::ObservationPropertiesWizardPage::isComplete() const
{
  return true;
}

void te::qt::widgets::ObservationPropertiesWizardPage::set(const std::list<te::da::DataSetTypePtr> dataTypes)
{
  m_dataTypes = dataTypes;
  m_propWidget->setUp(dataTypes.front());
}
