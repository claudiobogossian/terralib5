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
  \file terralib/qt/widgets/rp/ClassifierWizardPage.cpp

  \brief This file defines a class for a Classifier Wizard page.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../raster/Raster.h"
#include "ClassifierWizardPage.h"
#include "MapDisplay.h"
#include "ui_ClassifierWizardPageForm.h"

// Qt
#include <QGridLayout>
#include <QtGui/QMessageBox>

te::qt::widgets::ClassifierWizardPage::ClassifierWizardPage(QWidget* parent)
  : QWizardPage(parent),
    m_ui(new Ui::ClassifierWizardPageForm),
    m_layer(0)
{
// setup controls
  m_ui->setupUi(this);

  fillClassifierTypes();

//build form
  QGridLayout* displayLayout = new QGridLayout(m_ui->m_frame);
  m_mapDisplay.reset( new te::qt::widgets::MapDisplay(m_ui->m_frame->size(), m_ui->m_frame));
  displayLayout->addWidget(m_mapDisplay.get());

//configure page
  this->setTitle(tr("Classifier"));
  this->setSubTitle(tr("Select the type of classifier and set their specific parameters."));
}

te::qt::widgets::ClassifierWizardPage::~ClassifierWizardPage()
{
}

void te::qt::widgets::ClassifierWizardPage::set(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  std::list<te::map::AbstractLayerPtr> list;

  list.push_back(m_layer);

  te::gm::Envelope e = calculateExtent();

  m_mapDisplay->setLayerList(list);
  m_mapDisplay->setSRID(m_layer->getSRID(), false);
  m_mapDisplay->setExtent(e, false);

  listBands();
}

te::rp::Classifier::InputParameters te::qt::widgets::ClassifierWizardPage::getInputParams()
{
  te::rp::Classifier::InputParameters algoInputParams;

  return algoInputParams;
}

void te::qt::widgets::ClassifierWizardPage::apply()
{
  //get input raster
  te::da::DataSet* ds = m_layer->getData();
  te::rst::Raster* inputRst = ds->getRaster();

  //set contrast parameters
  te::rp::Classifier::InputParameters algoInputParams = getInputParams();



  te::rp::Classifier::OutputParameters algoOutputParams;


  //run contrast
  te::rp::Classifier algorithmInstance;

  if(algorithmInstance.initialize(algoInputParams))
  {
    if(algorithmInstance.execute(algoOutputParams))
    {

      QMessageBox::information(this, tr("Classifier"), tr("Classifier ended sucessfully"));
    }
    else
    {
      QMessageBox::critical(this, tr("Classifier"), tr("Classifier execution error"));
    }
  }
  else
  {
    QMessageBox::critical(this, tr("Classifier"), tr("Classifier initialization error"));
  }

  //delete input raster dataset
  delete ds;
}

void te::qt::widgets::ClassifierWizardPage::preview()
{
  if(m_ui->m_previewGroupBox->isChecked() && m_layer)
  {
    m_mapDisplay->refresh();
  }
}

te::gm::Envelope te::qt::widgets::ClassifierWizardPage::calculateExtent()
{
  if(m_layer.get())
  {
    te::gm::Envelope extent(m_layer->getExtent());

    return extent;
  }

  return te::gm::Envelope();
}

void te::qt::widgets::ClassifierWizardPage::fillClassifierTypes()
{
  m_ui->m_classifierTypeComboBox->clear();

  m_ui->m_classifierTypeComboBox->addItem(tr("ISOSeg"));
}

void te::qt::widgets::ClassifierWizardPage::listBands()
{
  assert(m_layer.get());

  //get input raster
  te::da::DataSet* ds = m_layer->getData();

  if(ds)
  {
    te::rst::Raster* inputRst = ds->getRaster();

    if(inputRst)
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_inputRasterBandsListWidget->addItem(QString::number(i));
      }
    }
  }

  delete ds;
}
