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
\file terralib/qt/widgets/layer/utils/SaveSelectedObjectsWidget.cpp

\brief This interface is used to create a new layer based on a layer with selected objects.
*/

// TerraLib
#include "../../../../common/STLUtils.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../dataaccess/datasource/DataSourceTransactor.h"
#include "../../../../dataaccess/dataset/ObjectIdSet.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../datatype/Utils.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../../../qt/widgets/property/NewPropertyWidget.h"
#include "../../layer/utils/DataSet2Layer.h"
#include "SaveSelectedObjectsWidget.h"
#include "ui_SaveSelectedObjectsWidgetForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::qt::widgets::SaveSelectedObjectsWidget::SaveSelectedObjectsWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
  m_ui(new Ui::SaveSelectedObjectsWidgetForm)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  //connects
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));


}

te::qt::widgets::SaveSelectedObjectsWidget::~SaveSelectedObjectsWidget()
{
}

/*! \brief Set parameters execute the operation. */
void te::qt::widgets::SaveSelectedObjectsWidget::setParameters(te::map::AbstractLayerPtr layer)
{
  //Set layer.
  m_layer = layer;

  //Verify if Layer is valid.
  if (!m_layer)
  {
    QString msg = tr("Invalid layer selected!");
    QMessageBox::warning(this, "Save layer as:", msg);
    return;
  }

  //Set the layer SRID.
  m_srid = m_layer->getSRID();

  //Set the number of selected objects.
  m_oidSet = m_layer->getSelected();
}

/*! \brief Update the components window with current values. */
void te::qt::widgets::SaveSelectedObjectsWidget::updateWindowComponents()
{
  // Set to QLineEdit the layer name.
  m_ui->m_layerNameLineEdit->setText(QString(m_layer->getDataSetName().c_str()));

  //Set to Label the SRID
  m_ui->m_layerSRIDLabel->setText(QString(boost::lexical_cast<std::string>(m_srid).c_str()));

  //Set to Label the number of selected objects.
  std::size_t num = 0;

  if (m_oidSet)
    num = m_oidSet->size();

  m_ui->m_NumSelectedObjLabel->setText(QString(boost::lexical_cast<std::string>(num).c_str()));
}

void te::qt::widgets::SaveSelectedObjectsWidget::save(te::da::DataSource* dataSource, te::da::DataSet* dataSet, te::da::DataSetType* dataSetType)
{

  std::auto_ptr<te::da::DataSourceTransactor> t = dataSource->getTransactor();

  std::map<std::string, std::string> options;

  try
  {
    if (dataSource->getType() == "OGR")
    {
      // create the dataset
      dataSource->createDataSet(dataSetType, options);

      // copy from memory to output datasource
      dataSet->moveBeforeFirst();
      std::string name = dataSetType->getName();
      dataSource->add(dataSetType->getName(), dataSet, options);
    }
    else
    {
      t->begin();

      // create the dataset
      t->createDataSet(dataSetType, options);

      // copy from memory to output datasource
      dataSet->moveBeforeFirst();
      std::string name = dataSetType->getName();
      t->add(dataSetType->getName(), dataSet, options);

      t->commit();
    }

  }
  catch (te::common::Exception& e)
  {
    t->rollBack();
    throw e;
  }
  catch (std::exception& e)
  {
    t->rollBack();
    throw e;
  }
}

te::map::AbstractLayerPtr te::qt::widgets::SaveSelectedObjectsWidget::getLayer()
{
  return m_layerResult;
}

bool te::qt::widgets::SaveSelectedObjectsWidget::execute(std::string& errorMessage)
{
  if (!m_oidSet)
  {
    errorMessage = "Select at least one object from selected layer.";
    return false;
  }

  if (!m_outputDatasource.get())
  {
    errorMessage = "Define the data source first.";
    return false;
  }

  if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
  {
    errorMessage = "Layer name not defined.";
    return false;
  }

  te::da::DataSourcePtr inDataSource = te::da::GetDataSource(m_layer->getDataSourceId(), true);
  if (!inDataSource.get())
  {
    errorMessage = "The selected input data source can not be accessed.";
    return false;
  }

  std::string dsTypeName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(dsTypeName));

  std::auto_ptr<te::da::DataSet> dataSet = inDataSource->getDataSet(m_layer->getTitle(), m_oidSet);
 
  std::map<std::string, std::string> nopt;

  std::auto_ptr<te::map::LayerSchema> schema = m_layer->getSchema();
  
  std::vector<te::dt::Property*> propsVec = schema->getProperties();
  
  for (std::size_t i = 0; i < propsVec.size(); ++i)
    dsType->add(propsVec[i]->clone());

  te::da::DataSourcePtr outputDataSource = te::da::GetDataSource(m_outputDatasource->getId());

  save(outputDataSource.get(), dataSet.get(), dsType.get());

  te::qt::widgets::DataSet2Layer converter(m_outputDatasource->getId());

  te::da::DataSetTypePtr dt(dsType);

  m_layerResult = converter(dt);
  
  return true;
}

void te::qt::widgets::SaveSelectedObjectsWidget::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);

  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if (dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  te::da::DataSourcePtr outputDataSource = te::da::DataSourceManager::getInstance().get(m_outputDatasource->getId(), m_outputDatasource->getType(), m_outputDatasource->getConnInfo());

  outputDataSource->open();

  m_toFile = false;
}

void te::qt::widgets::SaveSelectedObjectsWidget::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Shapefile (*.shp *.SHP);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());

  m_ui->m_repositoryLineEdit->setText(outfile.string().c_str());

  m_ui->m_newLayerNameLineEdit->setText(outfile.leaf().string().c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);

  m_toFile = true;

  //create new data source
  boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

  std::map<std::string, std::string> dsInfo;
  dsInfo["URI"] = uri.string();

  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("OGR");
  ds->setConnectionInfo(dsInfo);
  ds->open();

  boost::uuids::basic_random_generator<boost::mt19937> gen;
  boost::uuids::uuid u = gen();
  std::string id_ds = boost::uuids::to_string(u);

  te::da::DataSourceInfoPtr dsInfoPtr(new te::da::DataSourceInfo);
  dsInfoPtr->setConnInfo(dsInfo);
  dsInfoPtr->setTitle(uri.stem().string());
  dsInfoPtr->setAccessDriver("OGR");
  dsInfoPtr->setType("OGR");
  dsInfoPtr->setDescription(uri.string());
  dsInfoPtr->setId(id_ds);

  te::da::DataSourceInfoManager::getInstance().add(dsInfoPtr);

  m_outputDatasource = dsInfoPtr;

  te::da::DataSourcePtr outputDataSource = te::da::DataSourceManager::getInstance().get(m_outputDatasource->getId(), m_outputDatasource->getType(), m_outputDatasource->getConnInfo());

  outputDataSource->open();
}


