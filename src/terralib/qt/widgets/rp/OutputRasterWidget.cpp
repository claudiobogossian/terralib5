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
  \file terralib/qt/widgets/rp/OutputRasterWidget.cpp

  \brief This file has the OutputRasterWidget class.
*/

// TerraLib
#include "../../../dataaccess/datasource/DataSourceInfo.h"
#include "../datasource/selector/DataSourceSelectorDialog.h"
#include "OutputRasterWidget.h"
#include "ui_OutputRasterWidgetForm.h"

// BOOST Includes
#include <boost/filesystem.hpp>

// QT
#include <QFileDialog>


te::qt::widgets::OutputRasterWidget::OutputRasterWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
  m_ui(new Ui::OutputRasterWidgetForm)
{
  m_ui->setupUi(this);

  //add icons
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_targetFileToolButton, SIGNAL(clicked()), this, SLOT(onTargetFileToolButtonClicked()));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(clicked()), this, SLOT(onTargetDatasourceToolButtonClicked()));
}

te::qt::widgets::OutputRasterWidget::~OutputRasterWidget()
{
}

Ui::OutputRasterWidgetForm* te::qt::widgets::OutputRasterWidget::getForm() const
{
  return m_ui.get();
}

std::string te::qt::widgets::OutputRasterWidget::getOutputRaster()
{
  std::string outputRaster = "";
  if (!m_ui->m_repositoryLineEdit->text().isEmpty())
    outputRaster = m_ui->m_repositoryLineEdit->text().toStdString();

  return outputRaster;
}

bool te::qt::widgets::OutputRasterWidget::fileExists() const
{
  boost::filesystem::path path(m_ui->m_repositoryLineEdit->text().toStdString());
  return boost::filesystem::exists(path);
}

std::string te::qt::widgets::OutputRasterWidget::getType() const
{
  return "GDAL";
}

std::map<std::string, std::string> te::qt::widgets::OutputRasterWidget::getInfo() const
{
  std::map<std::string, std::string> rinfo;

  rinfo["URI"] = m_ui->m_repositoryLineEdit->text().toStdString();

  return rinfo;
}

void te::qt::widgets::OutputRasterWidget::onTargetFileToolButtonClicked()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("Image File(*.tif *.tiff);; "), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::qt::widgets::OutputRasterWidget::onTargetDatasourceToolButtonClicked()
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
}
