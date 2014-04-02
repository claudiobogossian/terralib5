/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/vp/qt/BasicGeopraphicOpWizardPage.cpp

  \brief This file defines a class for a Basic Geographic Operation Wizard Page.
*/

// TerraLib
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSet.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/utils/DoubleListWidget.h"
#include "BasicGeographicOpWizardPage.h"
#include "ui_BasicGeographicOpWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QIntValidator>
#include <QtGui/QMessageBox>

// STL
#include <memory>


te::vp::BasicGeographicOpWizardPage::BasicGeographicOpWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::BasicGeographicOpWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  m_widget.reset(new te::qt::widgets::DoubleListWidget(m_ui->m_widget));

  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

// configure page
  this->setTitle(tr("Output Layer Attributes"));
  this->setSubTitle(tr("Choose the attributes that compose the output layer."));

  m_widget->setLeftLabel("Available Properties");
  m_widget->setRightLabel("Used Properties");

}

te::vp::BasicGeographicOpWizardPage::~BasicGeographicOpWizardPage()
{
}

void te::vp::BasicGeographicOpWizardPage::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

// load attributes of the selected layer in the left list widget.
  loadAttributes();
}

te::map::AbstractLayerPtr te::vp::BasicGeographicOpWizardPage::getLayer()
{
  return m_layer;
}

std::vector<std::string> te::vp::BasicGeographicOpWizardPage::getSelectedProps()
{
  return m_widget->getOutputValues();
}

void te::vp::BasicGeographicOpWizardPage::loadAttributes()
{
  std::auto_ptr<te::da::DataSetType> dsType = m_layer->getSchema();
  std::vector<te::dt::Property*> vecProps = dsType->getProperties();
  std::vector<std::string> vecPropNames;

  for(std::size_t i = 0; i < vecProps.size(); ++i)
  {
    if(vecProps[i]->getType() != te::dt::GEOMETRY_TYPE)
      m_props.push_back(vecProps[i]->getName());
  }

  m_widget->setInputValues(m_props);

}

