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
#include "../../common/Exception.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../mnt/core/CreateIsolinesCore.h"
#include "../../mnt/core/TINCreateIsolines.h"
#include "../../mnt/core/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../raster.h"
#include "../../statistics/core/Utils.h"
#include "../../mnt/core/Utils.h"

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
  m_max(0.),
  m_toFile(false),
  m_hasDummy(false)
{
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));

  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));
  connect(m_ui->m_dummylineEdit, SIGNAL(editingFinished()), this, SLOT(onDummyLineEditEditingFinished()));

  connect(m_ui->m_stepFixedradioButton, SIGNAL(toggled(bool)), this, SLOT(onStepFixeEnabled(bool)));
  connect(m_ui->m_stepVariableradioButton, SIGNAL(toggled(bool)), this, SLOT(on_stepVariableraEnabled(bool)));

  connect(m_ui->m_insertpushButton, SIGNAL(clicked()), this, SLOT(oninsertpushButtonClicked()));
  connect(m_ui->m_deletepushButton, SIGNAL(clicked()), this, SLOT(ondeletepushButtonClicked()));
  connect(m_ui->m_deleteallpushButton, SIGNAL(clicked()), this, SLOT(ondeleteallpushButtonClicked()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
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
    if (it->get())
    {
      if (it->get()->isValid())
      {
        std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
        if (dsType.get())
        {
          std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
          if (dsType->hasGeom())
          {
            std::vector<te::dt::Property*> props = dsType->getProperties();
            bool hasval = false;
            for (size_t p = 0; p < props.size(); p++)
            {
              if (props[p]->getName().compare("val1") == 0 ||
                props[p]->getName().compare("val2") == 0 ||
                props[p]->getName().compare("val3") ==0)
              {
                hasval = true;
                break;
              }
            }

            if (hasval)
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
          }
          if (dsType->hasRaster()) //GRID
          {
            m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          }
          dsType.release();
        }
      }
    }
    ++it;
  }
}

std::vector<double> GetNumericData(te::da::DataSet* dataSet, std::vector<std::string>& propName)
{
  std::vector<double> result;
  double numval;

  size_t index = 0;
  std::vector<size_t> index_selected;
  std::vector<std::size_t> type;
  for (index = 0; index < dataSet->getNumProperties(); ++index)
    for (size_t p = 0; p < propName.size(); ++p)
      if (dataSet->getPropertyName(index) == propName[p])
      {
        index_selected.push_back(index);
        type.push_back(dataSet->getPropertyDataType(index));
      }

  dataSet->moveFirst();
  do
  {
    for (size_t i = 0; i < index_selected.size(); ++i)
    {
      index = index_selected[i];
      if (!dataSet->isNull(propName[i]))
      {
        if (type[i] == te::dt::INT16_TYPE)
          numval = dataSet->getInt16(index);
        else if (type[i] == te::dt::INT32_TYPE)
          numval = dataSet->getInt32(index);
        else if (type[i] == te::dt::INT64_TYPE)
          numval = (double)dataSet->getInt64(index);
        else if (type[i] == te::dt::FLOAT_TYPE)
          numval = dataSet->getFloat(index);
        else if (type[i] == te::dt::DOUBLE_TYPE)
          numval = dataSet->getDouble(index);
        else if (type[i] == te::dt::NUMERIC_TYPE)
          numval = boost::lexical_cast<double>(dataSet->getNumeric(index));
        result.push_back(numval);
      }
    }
  } while (dataSet->moveNext());

  return result;
}

void te::mnt::CreateIsolinesDialog::getMinMax(te::map::AbstractLayerPtr inputLayer, double &vmin, double &vmax)
{
  std::auto_ptr<te::da::DataSet> dataquery;
  te::da::DataSourcePtr ds = te::da::GetDataSource(inputLayer->getDataSourceId());
  vmin = std::numeric_limits<double>::max();
  vmax = -vmin;
  std::string qry("select min(val1), min(val2), min(val3), max(val1), max(val2), max(val3) from ");
  qry += inputLayer->getTitle();
  //qry += " where val1 is not NULL and val2 is not NULL and val3 is not NULL";
  dataquery = ds->query(qry);
  dataquery->moveFirst();
  vmin = std::min(std::min(dataquery->getDouble(0), dataquery->getDouble(1)), dataquery->getDouble(2));
  vmax = std::max(std::max(dataquery->getDouble(3), dataquery->getDouble(4)), dataquery->getDouble(5));
  dataquery.release();
}

void te::mnt::CreateIsolinesDialog::onInputComboBoxChanged(int index)
{
  this->setCursor(Qt::WaitCursor);
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
        getMinMax(m_inputLayer, m_min, m_max);
        m_ui->m_dummycheckBox->setVisible(false);
        m_ui->m_dummylineEdit->setVisible(false);
      }
      if (dsType->hasRaster())
      {
        m_inputType = GRID;
        std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());
        te::mnt::getMinMax(inputRst.get(), m_min, m_max);
        m_ui->m_dummycheckBox->setVisible(true);
        m_ui->m_dummylineEdit->setVisible(true);
        m_ui->m_dummylineEdit->setText(QString::number(inputRst->getBand(0)->getProperty()->m_noDataValue));
        m_dummy = inputRst->getBand(0)->getProperty()->m_noDataValue;
        inputRst.release();
      }
      dsType.release();

      break;
    }
    it++;
  }
  m_ui->m_vminrasterlineEdit->setText(QString::number(m_min));
  m_ui->m_vmaxrasterlineEdit->setText(QString::number(m_max));

  this->setCursor(Qt::ArrowCursor);

}

void te::mnt::CreateIsolinesDialog::onDummyLineEditEditingFinished()
{
  m_dummy = m_ui->m_dummylineEdit->text().toDouble();
  if (m_inputType == GRID)
  {
    std::auto_ptr<te::da::DataSet> inds = m_inputLayer->getData();
    std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());
    inputRst->getBand(0)->getProperty()->m_noDataValue = m_dummy;
    m_min = inputRst.get()->getBand(0)->getMinValue(true, 0, 0, inputRst->getNumberOfRows() - 1, inputRst->getNumberOfColumns() - 1).real();
    m_max = inputRst.get()->getBand(0)->getMaxValue(true, 0, 0, inputRst->getNumberOfRows() - 1, inputRst->getNumberOfColumns() - 1).real();
    m_ui->m_vminrasterlineEdit->setText(QString::number(m_min));
    m_ui->m_vmaxrasterlineEdit->setText(QString::number(m_max));
    inputRst.release();
  }
  m_hasDummy = true;
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
  m_ui->m_guidelinescheckBox->setEnabled(true);
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
  m_ui->m_guidelinescheckBox->setChecked(false);
  m_ui->m_guidelinescheckBox->setEnabled(false);
}

void te::mnt::CreateIsolinesDialog::oninsertpushButtonClicked()
{
  if (m_ui->m_stepFixedradioButton->isChecked())
  {
    double min = m_ui->m_vminlineEdit->text().toDouble();
    double max = m_ui->m_vmaxlineEdit->text().toDouble();
    double step = m_ui->m_steplineEdit->text().toDouble();
    if (step <= 0)
    {
      QMessageBox::information(this, tr("Create Isolines"), tr("Step value is invalid!"));
      return;
    }

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
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!m_inputLayer.get())
      throw te::common::Exception(TE_TR("Select an input layer!"));

    te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
    if (!indsLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer!"));

    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
    if (!inDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed!"));

    std::string inDsetName = indsLayer->getDataSetName();
    std::auto_ptr<te::da::DataSetType> inDsetType(inDataSource->getDataSetType(inDsetName));

  // Checking consistency of output paramenters
    if (m_ui->m_repositoryLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Select a repository for the resulting layer."));

    if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Define a name for the resulting layer."));

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
    std::map<std::string, std::string> outdsinfo;
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    bool result = false;

    if (m_toFile)
    {
      if (boost::filesystem::exists(uri))
        throw te::common::Exception(TE_TR("Output file already exists! Remove it or select a new name and try again."));

      std::size_t idx = outputdataset.find(".");
      if (idx != std::string::npos)
        outputdataset = outputdataset.substr(0, idx);

      outdsinfo["URI"] = uri.string();
    }

    std::vector<double> val;
    std::vector<double> guideval;
    bool ok;
    double step = m_ui->m_steplineEdit->text().toDouble(&ok);
    if (!ok)
      step = 0;

    double gLineValue = m_ui->m_isolineslistWidget->item(0)->text().toDouble() + step * 5;

    for (int i = 0; i < m_ui->m_isolineslistWidget->count(); i++)
    {
      val.push_back(m_ui->m_isolineslistWidget->item(i)->text().toDouble());
      if (m_ui->m_guidelinescheckBox->isChecked())
      {
        if (val[i] == gLineValue)
        {
          guideval.push_back(gLineValue);
          gLineValue += (step * 5);
        }
      }
    }

    if (m_inputType == GRID)
    {
      CreateIsolines* ci = new CreateIsolines();
      ci->setInput(inDataSource, inDsetName, inDsetType);
      if (m_toFile)
      {
        te::da::DataSourcePtr dsOGR(te::da::DataSourceFactory::make("OGR").release());
        dsOGR->setConnectionInfo(outdsinfo);
        dsOGR->open();

        if (dsOGR->dataSetExists(outputdataset))
          throw te::common::Exception(TE_TR("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));

        ci->setOutput(dsOGR, outputdataset);
      }
      else
      {
        te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
        if (!aux)
          throw te::common::Exception(TE_TR("The selected output datasource can not be accessed."));

        if (aux->dataSetExists(outputdataset))
          throw te::common::Exception(TE_TR("Dataset already exists. Remove it or select a new name and try again. "));
        ci->setOutput(aux, outputdataset);
      }

      if (m_ui->m_dummycheckBox->isChecked() == true)
      {
        m_hasDummy = true;
      }

      ci->setParams(val, guideval, m_max, m_min, m_dummy, m_hasDummy);
      std::auto_ptr<te::rst::Raster> raster = ci->getPrepareRaster();
      result = ci->run(raster);

      delete ci;
    }
    else //TIN
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
          throw te::common::Exception(TE_TR("There is already a dataset with the requested name in the output data source. Remove it or select a new name and try again."));

        Tin->setOutput(dsOGR, outputdataset);
      }
      else
      {
        te::da::DataSourcePtr aux = te::da::GetDataSource(m_outputDatasource->getId());
        if (!aux)
          throw te::common::Exception(TE_TR("The selected output datasource can not be accessed."));
        if (aux->dataSetExists(outputdataset))
          throw te::common::Exception(TE_TR("Dataset already exists. Remove it or select a new name and try again. "));

        Tin->setOutput(aux, outputdataset);
      }

      Tin->setSRID(m_inputLayer->getSRID());
      Tin->setParams(val, guideval, tol);

      result = Tin->run();
      delete Tin;
    }

    if (result)
    {
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
  }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::information(this, tr("Create Isolines "), e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  te::common::ProgressManager::getInstance().removeViewer(id);
  accept();

}

