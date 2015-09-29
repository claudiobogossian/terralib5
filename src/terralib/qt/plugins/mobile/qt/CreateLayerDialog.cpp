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
\file src/terraMobilePlugin/qt/CreateLayerDialog.cpp

\brief This interface is used to create new layer operation.
*/

// TerraLib
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../dataaccess/datasource/DataSourceManager.h"
#include "../../../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../../../qt/widgets/property/NewPropertyWidget.h"

#include "CreateLayerDialog.h"
#include "ui_CreateLayerDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// Boost
#include <boost/filesystem.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::qt::plugins::terramobile::CreateLayerDialog::CreateLayerDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::CreateLayerDialogForm)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("layer-new").pixmap(50, 50));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  //add new property widget
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  m_newPropWidget = new te::qt::widgets::NewPropertyWidget(m_ui->m_widget);

  layout->addWidget(m_newPropWidget);

  //connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_addPushButton, SIGNAL(clicked()), this, SLOT(onAddPushButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));
}

te::qt::plugins::terramobile::CreateLayerDialog::~CreateLayerDialog()
{

}

void te::qt::plugins::terramobile::CreateLayerDialog::onOkPushButtonClicked()
{
  if (!m_outputDatasource.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Define the data source first."));
    return;
  }

  if (m_props.empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Define the properties first."));
    return;
  }

  te::da::DataSourcePtr outputDataSource = te::da::DataSourceManager::getInstance().get(m_outputDatasource->getId(), m_outputDatasource->getType(), m_outputDatasource->getConnInfo());

  std::string dsTypeName = m_ui->m_newLayerNameLineEdit->text().toStdString();

  std::auto_ptr<te::da::DataSetType> dsType(new te::da::DataSetType(dsTypeName));

  for (std::size_t t = 0; t < m_props.size(); ++t)
  {
    dsType->add(m_props[t]);
  }

  //create
  std::map<std::string, std::string> nopt;

  try
  {
    outputDataSource->createDataSet(dsType.get(), nopt);
  }
  catch (const std::exception& ex)
  {
    QMessageBox::warning(this, tr("Warning"), ex.what());
    return;
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Error creating layer."));
    return;
  }

  accept();
}

void te::qt::plugins::terramobile::CreateLayerDialog::onAddPushButtonClicked()
{
  if (m_newPropWidget->buildProperty())
  {
    //get property
    te::dt::SimpleProperty* sp = m_newPropWidget->getProperty();

    m_props.push_back(sp);

    //set property info into table widget
    int newrow = m_ui->m_tableWidget->rowCount();

    m_ui->m_tableWidget->insertRow(newrow);

    //name
    QTableWidgetItem* itemName = new QTableWidgetItem(sp->getName().c_str());
    m_ui->m_tableWidget->setItem(newrow, 0, itemName);

    //type
    QTableWidgetItem* itemType = new QTableWidgetItem(QString::number(sp->getType()));
    m_ui->m_tableWidget->setItem(newrow, 1, itemType);
  }
}

void te::qt::plugins::terramobile::CreateLayerDialog::onTargetDatasourceToolButtonPressed()
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

  m_newPropWidget->setDataSourceId(m_outputDatasource->getId());

  m_toFile = false;
}

void te::qt::plugins::terramobile::CreateLayerDialog::onTargetFileToolButtonPressed()
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

  m_newPropWidget->setDataSourceId(m_outputDatasource->getId());
}
