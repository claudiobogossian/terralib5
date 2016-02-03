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
  \file terralib/qt/widgets/exchanger/PostGIS2SHPDialog.h

  \brief A exchanger dialog from PostGis to SHP operation
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/GeometryProperty.h"
#include "../../../maptools/DataSetLayer.h"
#include "PostGIS2SHPDialog.h"
#include "ui_PostGIS2SHPDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::da::DataSourceInfoPtr);

te::qt::widgets::PostGIS2SHPDialog::PostGIS2SHPDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::PostGis2SHPDialogForm)
{
// setup widget
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("data-exchange-pgis-shp-hint").pixmap(112,48));

//connectors
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_dirToolButton, SIGNAL(clicked()), this, SLOT(onDirToolButtonClicked()));
}

te::qt::widgets::PostGIS2SHPDialog::~PostGIS2SHPDialog()
{
}

void te::qt::widgets::PostGIS2SHPDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin();

  while(it != layers.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::string dsName = l->getDataSourceId();

    te::da::DataSourcePtr dsPtr = te::da::GetDataSource(dsName);

    if(dsPtr->getType() == "POSTGIS") // TENSO
      m_ui->m_inputLayerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

void te::qt::widgets::PostGIS2SHPDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::qt::widgets::PostGIS2SHPDialog::onOkPushButtonClicked()
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

  if(m_ui->m_dataSetLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Output File Name not defined."));
    return;
  }

  
  try
  {
    //create adapter
    std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();
    
    if(dsType->size() == 0)
      te::da::LoadProperties(dsType.get(), layer->getDataSourceId());


    //create data source
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = m_ui->m_dataSetLineEdit->text().toStdString();
    connInfo["DRIVER"] = "ESRI Shapefile"; // MUITO TENSO
  
    std::auto_ptr<te::da::DataSource> dsOGR = te::da::DataSourceFactory::make("OGR"); // TENSO
    dsOGR->setConnectionInfo(connInfo);
    dsOGR->open();

    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsOGR->getCapabilities());

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(m_ui->m_dataSetLineEdit->text().toStdString());

    //exchange
    std::map<std::string,std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    dsOGR->createDataSet(dsTypeResult, nopt);

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if(dataset->moveBeforeFirst())
      dsOGR->add(dsTypeResult->getName(), dsAdapter.get(), dsOGR->getConnectionInfo());

    dsOGR->close();

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

void te::qt::widgets::PostGIS2SHPDialog::onDirToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                                        QString(), tr("Shapefile (*.shp *.SHP);;"),0, QFileDialog::DontConfirmOverwrite);
  
  if (fileName.isEmpty())
    return;
  
  m_ui->m_dataSetLineEdit->setText(fileName);
}
