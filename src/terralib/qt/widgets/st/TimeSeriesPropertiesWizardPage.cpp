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
  \file terralib/qt/widgets/st/ TimeSeriesPropertiesWIzardPage.cpp

  \brief  A wizardPage used to configure the generaç properties of a new TimeSeries layer.
*/

// TerraLib
#include "../../../st/core/timeseries/TimeSeriesDataSetInfo.h"
#include "TemporalPropertiesWidget.h"
#include "TimeSeriesPropertiesWidget.h"
#include "TimeSeriesPropertiesWizardPage.h"
#include "ui_TemporalPropertiesWidgetForm.h"
#include "ui_TimeSeriesPropertiesWidgetForm.h"

te::qt::widgets:: TimeSeriesPropertiesWizardPage:: TimeSeriesPropertiesWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
  m_propWidget.reset(new TimeSeriesPropertiesWidget(this));
  m_tempPropWidget.reset(new TemporalPropertiesWidget(this));

  // Adjusting...
  QGridLayout* propLayout = new QGridLayout(this);
  propLayout->addWidget(m_propWidget.get());

  // Adjusting the properties widget
  QGridLayout* tempPropLayout = new QGridLayout(m_propWidget->getForm()->m_temporalPropertiesFrame);
  tempPropLayout->setContentsMargins(0, 0, 0, 0);
  tempPropLayout->addWidget(m_tempPropWidget.get());
}

te::qt::widgets:: TimeSeriesPropertiesWizardPage::~ TimeSeriesPropertiesWizardPage()
{
}

te::st::TimeSeriesDataSetInfo* te::qt::widgets::TimeSeriesPropertiesWizardPage::getInfo(const te::da::DataSourceInfoPtr dsInfo)
{
  te::st::TimeSeriesDataSetInfo* timeSeriesInfo;  
  timeSeriesInfo = new te::st::TimeSeriesDataSetInfo(*dsInfo.get(), m_dataType->getName(), m_tempPropWidget->getPhenomenonTime(), m_propWidget->getValuePropName(), m_propWidget->getGeometryPropName(), m_propWidget->getIdPropName(), "");
  return timeSeriesInfo;
}

void te::qt::widgets:: TimeSeriesPropertiesWizardPage::set(const te::da::DataSetTypePtr dataType)
{
  m_dataType = dataType;
  m_tempPropWidget->setUp(dataType);
  m_propWidget->setUp(dataType);
}

