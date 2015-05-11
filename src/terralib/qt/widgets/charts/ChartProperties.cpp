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
  \file terralib/qt/widgets/charts/ChartProperties.cpp

  \brief A dialog used to customize a Chart's parameters, weather it is about it's data or it's visual style
*/
//Terralib

#include "ChartDisplay.h"
#include "ChartDisplayWidget.h"
#include "ChartProperties.h"
#include "ChartStyleFrameFactory.h"
#include "ChartWidget.h"
#include "ChartWidgetFactory.h"
#include "HistogramFrameFactory.h"
#include "ScatterFrameFactory.h"
#include "ui_ChartPropertiesDialogForm.h"

//QWT
#include <qwt_plot_seriesitem.h>

te::qt::widgets::ChartProperties::ChartProperties(te::qt::widgets::ChartDisplayWidget* chartWidget, QWidget* parent)
  : QDialog(parent),
    m_ui(new Ui::ChartPropertiesDialogForm),
    m_curComp(0),
    m_chartWidget(chartWidget)
{
  m_ui->setupUi(this);

  //init factories
  switch (m_chartWidget->getType())
  {
    case(te::qt::widgets::HISTOGRAM_CHART):
      HistogramFrameFactory::initialize();
      ScatterFrameFactory::finalize();
      break;
    case(te::qt::widgets::SCATTER_CHART):
      ScatterFrameFactory::initialize();
      HistogramFrameFactory::finalize();
      break;
  }

  ChartStyleFrameFactory::initialize();

  std::vector<std::string> vec;
  const te::qt::widgets::ChartWidgetFactory::dictionary_type& d = te::qt::widgets::ChartWidgetFactory::getDictionary();
  te::qt::widgets::ChartWidgetFactory::dictionary_type::const_iterator it = d.begin();

  while(it != d.end())
  {
    vec.push_back(it->first);
    ++it;
  }

  for(size_t i = 0; i < vec.size(); i++)
  {
    m_ui->m_componentsListWidget->addItem(vec[i].c_str());
  }
  m_ui->m_componentsListWidget->setFocus();
  m_ui->m_tabWidget->clear();
  m_curComp = te::qt::widgets::ChartWidgetFactory::make("Chart Style", m_ui->m_tabWidget);
  m_ui->m_tabWidget->addTab(m_curComp, QString::fromStdString("Chart Style"));
  m_curComp->setDisplay(m_chartWidget->getDisplay());
  m_ui->m_componentsListWidget->setCurrentRow(0);
  m_ui->m_helpPushButton->setPageReference("widgets/charts/style/style.html");

  connect(m_ui->m_componentsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClicked(QListWidgetItem*)));
  connect(m_ui->m_applyPushButton, SIGNAL(clicked()), this, SLOT(onApplyButtonClicked()));
}

te::qt::widgets::ChartProperties::~ChartProperties()
{
  delete m_curComp;
}

void te::qt::widgets::ChartProperties::onItemClicked(QListWidgetItem * current) 
{
  std::string value = current->text().toStdString();
  delete m_curComp;
  m_curComp = te::qt::widgets::ChartWidgetFactory::make(value, m_ui->m_tabWidget);
  m_curComp->setChart(m_chartWidget->getChart());
  m_curComp->setDisplay(m_chartWidget->getDisplay());
  m_ui->m_tabWidget->clear();
  m_ui->m_tabWidget->addTab(m_curComp, QString::fromStdString(value));
  m_curComp->show();
}

void te::qt::widgets::ChartProperties::onApplyButtonClicked() 
{
  if(m_ui->m_componentsListWidget->currentItem()->text() == "Chart Style")
  {
    m_chartWidget->setDisplay(m_curComp->getDisplay());
  }
  else
  {
    m_chartWidget->setChart(m_curComp->getChart());
  }
  this->close();
}
