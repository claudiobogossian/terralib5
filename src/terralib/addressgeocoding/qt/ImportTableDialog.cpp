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
  \file terralib/addressgeocoding/ImportTableDialog.cpp

  \brief A dialog to import table with address to geocoding operation
*/

// TerraLib
#include "../../common/Logger.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../common/STLUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../../memory/DataSet.h"
#include "../../qt/af/Utils.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/layer/utils/DataSet2Layer.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/property/DataSetAdapterWidget.h"
#include "../../qt/widgets/table/DataSetTableView.h"
#include "../../qt/widgets/Utils.h"
#include "../../statistics/core/Utils.h"
#include "../Config.h"
#include "../Exception.h"
#include "ImportTableDialog.h"
#include "ui_ImportTableDialogForm.h"

// Qt
#include <QCheckBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QMessageBox>

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::addressgeocoding::ImportTableDialog::ImportTableDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::ImportTableDialogForm)
{
// add controls
  m_ui->setupUi(this);

  //Adjusting the dataSetTableView that will be used to display the address data
  m_tblView.reset(new te::qt::widgets::DataSetTableView(m_ui->m_dataPreviewFrame));
  QGridLayout* dataPreviewLayout = new QGridLayout(m_ui->m_dataPreviewFrame);
  dataPreviewLayout->addWidget(m_tblView.get());
  dataPreviewLayout->setContentsMargins(0, 0, 0, 0);

  m_tblView->setAlternatingRowColors(true);
  m_tblView->verticalHeader()->setVisible(false);
  m_tblView->setSelectionMode(QAbstractItemView::NoSelection);


  connect(m_ui->m_filePushButton, SIGNAL(clicked()), this, SLOT(onInputDataToolButtonTriggered()));
  
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

}

te::addressgeocoding::ImportTableDialog::~ImportTableDialog()
{
}

te::da::DataSourcePtr te::addressgeocoding::ImportTableDialog::getDataSource()
{
  return m_dataSource;
}

std::auto_ptr<te::da::DataSet> te::addressgeocoding::ImportTableDialog::getDataSet()
{
  return m_dataSet;
}

void te::addressgeocoding::ImportTableDialog::onInputDataToolButtonTriggered()
{
  try
  {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Textual File"), te::qt::widgets::GetFilePathFromSettings("tabular"), tr("Comma Separated Value (*.csv *.CSV);; dBASE (*.dbf *.DBF)"), 
      0, QFileDialog::ReadOnly);

    if(fileName.isEmpty())
      return;

    QFileInfo info(fileName);
    te::qt::widgets::AddFilePathToSettings(info.absolutePath(), "tabular");

    //Getting the connection info
    std::string ogrInfo("connection_string=" + fileName.toStdString());
    std::map<std::string, std::string> connInfo;
    connInfo["URI"] = fileName.toStdString();

    boost::filesystem::path uri(fileName.toStdString());
    std::string file = uri.stem().string();

    //Creating a DataSource
    static boost::uuids::basic_random_generator<boost::mt19937> gen;
    boost::uuids::uuid u = gen();

    te::da::DataSourceInfoPtr dsInfo(new te::da::DataSourceInfo);
    dsInfo->setConnInfo(connInfo);
    dsInfo->setId(boost::uuids::to_string(u));
    dsInfo->setTitle(fileName.toStdString());
    dsInfo->setDescription("");
    dsInfo->setAccessDriver("OGR");
    dsInfo->setType("OGR");

    te::da::DataSourceInfoManager::getInstance().add(dsInfo);

    m_dataSource = te::da::DataSourceFactory::make(dsInfo->getAccessDriver());
    m_dataSource->setConnectionInfo(dsInfo->getConnInfo());

    m_dataSource->setId(boost::uuids::to_string(u));
    m_dataSource->open();

    //Creating the DataSet and DataType
    m_dataSet = m_dataSource->getDataSet(file);
    std::vector<std::string> datasetNames = m_dataSource->getDataSetNames();

    if(!datasetNames.empty())
      m_dataType = m_dataSource->getDataSetType(datasetNames[0]);
    else
    {
      QMessageBox::critical( this, tr("Error"),tr("The file could not be read!"));
      return;
    }

    m_ui->m_fileLineEdit->setText(fileName);

    //Creating the DataSetConverter 
    m_dsConverter.reset(new te::da::DataSetTypeConverter(m_dataType.get()));

    //Filling the data preview table
    std::vector<std::size_t> properties;
    for(std::size_t i = 0; i < m_dataSet->getNumProperties(); ++i)
      properties.push_back(i);

    //The table will display 10 rows of the data for previewing purposes
    std::auto_ptr<te::mem::DataSet> memFeature((new te::mem::DataSet(*m_dataSet.get(), properties, 10)));

    m_tblView->setDataSet(memFeature.release(), m_dataSource->getEncoding());
    m_tblView->resizeColumnsToContents();
    m_tblView->show();
  }
  catch(const std::exception& e)
  {
    QMessageBox::warning(this, tr("Tabular File"), e.what());
  }
}

void te::addressgeocoding::ImportTableDialog::onHelpPushButtonClicked()
{
  QMessageBox::information(this, "Help", "Under development");
}

void te::addressgeocoding::ImportTableDialog::onOkPushButtonClicked()
{
  this->close();
}

void te::addressgeocoding::ImportTableDialog::onCancelPushButtonClicked()
{
  reject();
}

