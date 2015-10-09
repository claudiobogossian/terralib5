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
\file terralib/mnt/qt/CreateISolinesDialog.cpp

\brief A dialog for ISolines generation
*/

// terralib
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../mnt/core/TINCreateIsolines.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../raster.h"
#include "../../statistics/core/Utils.h"


#include "CreateIsolinesDialog.h"
#include "ui_CreateIsolinesDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::mnt::CreateIsolinesDialog::CreateIsolinesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::CreateIsolinesDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>()),
  m_min(0.),
  m_max(0.)
{
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_stepFixedradioButton, SIGNAL(toggled(bool)), this, SLOT(onStepFixeEnabled(bool)));
  connect(m_ui->m_stepVariableradioButton, SIGNAL(toggled(bool)), this, SLOT(on_stepVariableraEnabled(bool)));

  connect(m_ui->m_insertpushButton, SIGNAL(clicked()), this, SLOT(oninsertpushButtonClicked()));
  connect(m_ui->m_deletepushButton, SIGNAL(clicked()), this, SLOT(ondeletepushButtonClicked()));
  connect(m_ui->m_deleteallpushButton, SIGNAL(clicked()), this, SLOT(ondeleteallpushButtonClicked()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_stepFixedradioButton->clicked(true);
}


te::mnt::CreateIsolinesDialog::~CreateIsolinesDialog()
{

}


void te::mnt::CreateIsolinesDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasGeom())
    {
      std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));
      te::gm::GeomType gmType = geomProp->getGeometryType();
      if (gmType == te::gm::TINType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
        gmType == te::gm::TINZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType)//TIN
      {
        m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
      }
      geomProp.release();
    }
    if (dsType->hasRaster()) //GRID
    {
      m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    }
    dsType.release();
    ++it;
  }
}

void te::mnt::CreateIsolinesDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      std::auto_ptr<te::da::DataSet> inds = m_inputLayer->getData();
      if (dsType->hasGeom())
      {
        m_inputType = TIN;
        std::vector<double> values1 = te::stat::GetNumericData(inds.get(), "val1");
        std::vector<double> values2 = te::stat::GetNumericData(inds.get(), "val2");
        std::vector<double> values3 = te::stat::GetNumericData(inds.get(), "val3");
        std::sort(values1.begin(), values1.end());
        std::sort(values2.begin(), values2.end());
        std::sort(values3.begin(), values3.end());
        m_min = std::min(*values1.begin(), std::min(*values2.begin(), *values3.begin()));
        m_max = std::max(values1[values1.size() - 1], std::max(values2[values2.size() - 1], values3[values3.size() - 1]));
      }
      if (dsType->hasRaster())
      {
        m_inputType = GRID;
        std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());
        m_min = inputRst.get()->getBand(0)->getMinValue().real();
        m_max = inputRst.get()->getBand(0)->getMaxValue().real();
        inputRst.release();
      }
      dsType.release();
      break;
    }
    it++;
  }
  m_ui->m_vminrasterlineEdit->setText(QString::number(m_min));
  m_ui->m_vmaxrasterlineEdit->setText(QString::number(m_max));

}

void te::mnt::CreateIsolinesDialog::onStepFixeEnabled(bool)
{
  m_ui->m_vmaxlineEdit->setEnabled(true);
  m_ui->m_vminlineEdit->setEnabled(true);
  m_ui->m_steplineEdit->setEnabled(true);
  m_ui->m_valuelineEdit->setEnabled(false);
  m_ui->m_vmaxlabel->setEnabled(true);
  m_ui->m_vminlabel->setEnabled(true);
  m_ui->m_steplabel->setEnabled(true);
  m_ui->m_valuelabel->setEnabled(false);
}

void te::mnt::CreateIsolinesDialog::on_stepVariableraEnabled(bool)
{
  m_ui->m_vmaxlineEdit->setEnabled(false);
  m_ui->m_vminlineEdit->setEnabled(false);
  m_ui->m_steplineEdit->setEnabled(false);
  m_ui->m_valuelineEdit->setEnabled(true);
  m_ui->m_vmaxlabel->setEnabled(false);
  m_ui->m_vminlabel->setEnabled(false);
  m_ui->m_steplabel->setEnabled(false);
  m_ui->m_valuelabel->setEnabled(true);
}

void te::mnt::CreateIsolinesDialog::oninsertpushButtonClicked()
{
  if (m_ui->m_stepFixedradioButton->isChecked())
  {
    double min = m_ui->m_vminlineEdit->text().toDouble();
    double max = m_ui->m_vmaxlineEdit->text().toDouble();
    double step = m_ui->m_steplineEdit->text().toDouble();
    for (double val = min; val <= max; val += step)
    {
      m_ui->m_isolineslistWidget->addItem(QString::number(val));
    }
  }
  else
  {
    double val = m_ui->m_valuelineEdit->text().toDouble();
    if (val < m_min || val > m_max)
    {
      QMessageBox::information(this, tr("Create Isolines"), tr("Value is invalid!"));
      return;
    }
    m_ui->m_isolineslistWidget->addItem(m_ui->m_valuelineEdit->text());
  }
}

void te::mnt::CreateIsolinesDialog::ondeletepushButtonClicked()
{
}

void te::mnt::CreateIsolinesDialog::ondeleteallpushButtonClicked()
{
  m_ui->m_isolineslistWidget->clear();
}

void te::mnt::CreateIsolinesDialog::onCancelPushButtonClicked()
{
  reject();
}

te::map::AbstractLayerPtr te::mnt::CreateIsolinesDialog::getLayer()
{
  return m_outputLayer;
}


void te::mnt::CreateIsolinesDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if (dsPtrList.empty())
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  m_toFile = false;
}

void te::mnt::CreateIsolinesDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("Shapefile (*.shp *.SHP);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::mnt::CreateIsolinesDialog::onHelpPushButtonClicked()
{
}

void te::mnt::CreateIsolinesDialog::onOkPushButtonClicked()
{
  
  if (!m_inputLayer.get())
  {
    QMessageBox::information(this, tr("Create Isolines"), tr("Select an input layer!"));
    return;
  }

  te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
  if (!indsLayer)
  {
    QMessageBox::information(this, tr("Create Isolines"), tr("Can not execute this operation on this type of layer!"));
    return;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    QMessageBox::information(this, tr("Create Isolines"), tr("The selected input data source can not be accessed!"));
    return;
  }

  std::string inDsetName = indsLayer->getDataSetName();
  std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

  std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
  if (outputdataset.empty())
  {
    QMessageBox::information(this, tr("Create Isolines"), tr("The selected output datasource is empty!"));
    return;
  }
  std::map<std::string, std::string> outdsinfo;
  boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

  if (m_toFile)
  {
    if (boost::filesystem::exists(uri))
    {
      QMessageBox::information(this, tr("Create Isolines"), tr("Output file already exists! Remove it or select a new name and try again."));
      return;
    }

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();
  }

  std::vector<double> val;
  for (int i = 0; i < m_ui->m_isolineslistWidget->count(); i++)
  {
    val.push_back(m_ui->m_isolineslistWidget->item(i)->text().toDouble());
  }

  if (m_inputType == GRID)
  {

  }
  else
  {
    double tol = m_inputLayer->getExtent().getHeight() / 1.e9;

    TINCreateIsolines *Tin = new te::mnt::TINCreateIsolines();

    Tin->setInput(inDataSource, inDsetName, inDsetType);

    if (m_toFile)
    {
      te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
      dsOGR->setConnectionInfo(outdsinfo);
      dsOGR->open();

      if (dsOGR->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, tr("TIN Generation"), tr("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));
        return;
      }
 
      Tin->setOutput(dsOGR, outputdataset);
    }
    else
    {
      te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
      if (!aux)
      {
        QMessageBox::information(this, tr("Create Isolines"), tr("The selected output datasource can not be accessed."));
        return;
      }
      if (aux->dataSetExists(outputdataset))
      {
        QMessageBox::information(this, tr("Create Isolines"), tr("Dataset already exists. Remove it or select a new name and try again. "));
        return;
      }
      Tin->setOutput(aux, outputdataset);

    }

    Tin->setSRID(m_inputLayer->getSRID());
    Tin->setParams(val, tol);

    bool result = Tin->run();
    if (m_toFile)
    {
      // let's include the new datasource in the managers
      boost::uuids::basic_random_generator<boost::mt19937> gen;
      boost::uuids::uuid u = gen();
      std::string id = boost::uuids::to_string(u);

      te::da::DataSourceInfoPtr ds(new te::da::DataSourceInfo);
      ds->setConnInfo(outdsinfo);
      ds->setTitle(uri.stem().string());
      ds->setAccessDriver("OGR");
      ds->setType("OGR");
      ds->setDescription(uri.string());
      ds->setId(id);

      te::da::DataSourcePtr newds = te::da::DataSourceManager::getInstance().get(id, "OGR", ds->getConnInfo());
      newds->open();
      te::da::DataSourceInfoManager::getInstance().add(ds);
      m_outputDatasource = ds;
    }

    // creating a layer for the result
    te::da::DataSourcePtr outDataSource = te::da::GetDataSource(m_outputDatasource->getId());

    te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

    te::da::DataSetTypePtr dt(outDataSource->getDataSetType(outputdataset).release());
    m_outputLayer = converter(dt);
  }

  accept();

}

