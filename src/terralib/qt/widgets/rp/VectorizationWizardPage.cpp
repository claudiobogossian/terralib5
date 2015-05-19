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


//Terralib
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "VectorizationWizardPage.h"
#include "ui_VectorizationWizardPageForm.h"

// Qt
#include <QCheckBox>
#include <QFileDialog>
#include <QIntValidator>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

te::qt::widgets::VectorizationWizardPage::VectorizationWizardPage(QWidget *parent) :
  QWizardPage(parent)
{
  m_ui.reset(new Ui::VectorizationWizardPageForm);

  m_ui->setupUi(this);

//configure page
  this->setTitle(tr("Vectorization"));
  this->setSubTitle(tr("Define the vectorization parameters."));

  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));

  m_ui->m_maxGeomLineEdit->setValidator(new QIntValidator(this));
}

te::qt::widgets::VectorizationWizardPage::~VectorizationWizardPage()
{

}

bool te::qt::widgets::VectorizationWizardPage::isComplete() const
{
  return true;
}

void te::qt::widgets::VectorizationWizardPage::setLayer(te::map::AbstractLayerPtr layer)
{
  m_layer = layer;

  m_ui->m_layerLineEdit->setText(m_layer->getTitle().c_str());

  //list bands
  m_ui->m_bandComboBox->clear();

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_bandComboBox->addItem(QString::number(i));
      }
    }
  }
}

std::auto_ptr<te::rst::Raster> te::qt::widgets::VectorizationWizardPage::getRaster()
{
  assert(m_layer.get());

   //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  return ds->getRaster(rpos);
}

unsigned int te::qt::widgets::VectorizationWizardPage::getBand()
{
  return m_ui->m_bandComboBox->currentText().toUInt();
}

bool te::qt::widgets::VectorizationWizardPage::hasMaxGeom(unsigned int& maxGeom)
{
  if(!m_ui->m_maxGeomLineEdit->text().isEmpty())
    maxGeom = m_ui->m_maxGeomLineEdit->text().toInt();

  return m_ui->m_maxGeomCheckBox->isChecked();
}

bool te::qt::widgets::VectorizationWizardPage::outputDataSourceToFile()
{
  return m_toFile;
}

te::da::DataSourceInfoPtr te::qt::widgets::VectorizationWizardPage::getDataSourceInfo()
{
  return m_outputDatasource;
}

std::string te::qt::widgets::VectorizationWizardPage::getLayerName()
{
  if(m_ui->m_newLayerNameLineEdit->text().isEmpty())
    return "";

  return m_ui->m_newLayerNameLineEdit->text().toStdString();
}

std::string te::qt::widgets::VectorizationWizardPage::getRepositoryName()
{
  if(m_ui->m_repositoryLineEdit->text().isEmpty())
    return "";

  return m_ui->m_repositoryLineEdit->text().toStdString();
}

void te::qt::widgets::VectorizationWizardPage::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  m_toFile = false;
}

void te::qt::widgets::VectorizationWizardPage::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                      QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);

  if(fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());
  
  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}


