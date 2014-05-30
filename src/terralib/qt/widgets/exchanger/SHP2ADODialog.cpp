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
  \file terralib/qt/widgets/exchanger/SHP2ADODialog.h

  \brief A exchanger dialog from SHP to ADO operation
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "SHP2ADODialog.h"
#include "ui_SHP2ADODialogForm.h"

// Qt
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::da::DataSourceInfoPtr);

te::qt::widgets::SHP2ADODialog::SHP2ADODialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::SHP2ADODialogForm)
{
// setup widget
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("data-exchange-shp-ado-hint").pixmap(112,48));

//connectors
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));

//set list of data sources
  setDataSources();
}

te::qt::widgets::SHP2ADODialog::~SHP2ADODialog()
{
}

void te::qt::widgets::SHP2ADODialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(l.get());

    if(!dsLayer)
      continue;

    std::string dsName = dsLayer->getDataSourceId();

    te::da::DataSourcePtr dsPtr = te::da::GetDataSource(dsName);

    if(dsPtr->getType() == "OGR") // TENSO
      m_ui->m_inputLayerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }

  if(m_ui->m_inputLayerComboBox->count() > 0)
  {
    QString s = m_ui->m_inputLayerComboBox->currentText();

    m_ui->m_dataSetLineEdit->setText(s);
  }
}

void te::qt::widgets::SHP2ADODialog::setDataSources()
{
  m_ui->m_outputDataSourceComboBox->clear();

  std::vector<te::da::DataSourceInfoPtr> datasources;

  te::da::DataSourceInfoManager::getInstance().getByType("ADO", datasources); // TENSO

  for(std::size_t i = 0; i < datasources.size(); ++i)
  {
    const te::da::DataSourceInfoPtr& datasource = datasources[i];

    if(datasource.get() == 0)
      continue;

    const std::string& title = datasource->getTitle();

    m_ui->m_outputDataSourceComboBox->addItem(title.c_str(), QVariant::fromValue(datasource));
  }
}

void te::qt::widgets::SHP2ADODialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::qt::widgets::SHP2ADODialog::onOkPushButtonClicked()
{
  int idxLayer = m_ui->m_inputLayerComboBox->currentIndex();

  if(idxLayer == -1)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Input layer not selected."));
    return;
  }

  QVariant varLayer = m_ui->m_inputLayerComboBox->itemData(idxLayer, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if(!layer.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected layer."));
    return;
  }

  int idxDataSource = m_ui->m_outputDataSourceComboBox->currentIndex();

  if(idxLayer == -1)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output data source not selected."));
    return;
  }

  QVariant varDataSource = m_ui->m_outputDataSourceComboBox->itemData(idxDataSource, Qt::UserRole);
  te::da::DataSourceInfoPtr dsInfo = varDataSource.value<te::da::DataSourceInfoPtr>();

  if(!dsInfo.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error getting selected data source."));
    return;
  }

  if(m_ui->m_dataSetLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Data Set name not defined."));
    return;
  }

  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    if(dsType->size() == 0)
      te::da::LoadProperties(dsType.get(), dsLayer->getDataSourceId());

    te::da::DataSourcePtr targetDSPtr = te::da::DataSourceManager::getInstance().get(dsInfo->getId(), dsInfo->getType(), dsInfo->getConnInfo()); 

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), targetDSPtr->getCapabilities());

    te::da::DataSetType* dsTypeResult = converter->getResult();
    
    dsTypeResult->setName(m_ui->m_dataSetLineEdit->text().toStdString());

    //exchange
    std::map<std::string,std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    targetDSPtr->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

     if(dataset->moveBeforeFirst())
       targetDSPtr->add(dsTypeResult->getName(), dsAdapter.get(), targetDSPtr->getConnectionInfo());

    QMessageBox::information(this, tr("Exchanger"), tr("Layer exported successfully."));
  }
  catch(const std::exception& e)
  {
    QString errMsg(tr("Error during exchanger. The reported error is: %1"));

    errMsg = errMsg.arg(e.what());

    QMessageBox::information(this, tr("Exchanger"), errMsg);
  }

  accept();
}