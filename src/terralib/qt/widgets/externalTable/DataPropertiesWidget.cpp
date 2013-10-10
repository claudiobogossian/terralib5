/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file  terralib/qt/widgets/externalTable/DataPropertiesWidget.h

  \brief A class used to configure the properties of a new textual file based layer
*/

#include "DataPropertiesWidget.h"
#include "ui_DataPropertiesWidgetForm.h"

te::qt::widgets::DatapPropertiesWidget::DatapPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::dataPropertiesWidgetForm)
{
  m_ui->setupUi(this);

  //Connecting signals and slots
  connect(m_ui->m_inputDataToolButton, SIGNAL(triggered()), this, SLOT(onInputDataToolButtonTriggered()));
}

te::qt::widgets::DatapPropertiesWidget::~DatapPropertiesWidget()
{
}

void te::qt::widgets::DatapPropertiesWidget::onInputDataToolButtonTriggered()
{
    //create data source
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = m_ui->m_inputDataLineEdit->text().toStdString();
    connInfo["DRIVER"] = "OGR";
  
    //std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make(m_outputDataSourceType);
    //dsOGR->setConnectionInfo(connInfo);
    //dsOGR->open();
}