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
  \file terralib/qt/widgets/rp/OutputRasterParametersWidget.cpp

  \brief This file has the OutputRasterParametersWidget class.
*/

// TerraLib
#include "OutputRasterParametersWidget.h"
#include "../raster/RasterInfoDialog.h"
#include "../raster/RasterInfoWidget.h"
#include "ui_OutputRasterParametersWidgetForm.h"

//QT
#include <QFileDialog>

te::qt::widgets::OutputRasterParametersWidget::OutputRasterParametersWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::OutputRasterParametersWidgetForm),
    m_fileGDAL(false),
    m_dataSource(false)
{
//build form
  m_ui->setupUi(this);

  m_ui->m_fileToolButton->setIcon(QIcon::fromTheme("folder"));
  m_ui->m_sourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  m_rasterInfoDlg.reset( new te::qt::widgets::RasterInfoDialog(true, this, 0));

//connects
  connect(m_ui->m_sourceToolButton, SIGNAL(clicked()), this, SLOT(onSourceToolButtonClicked()));
  connect(m_ui->m_fileToolButton, SIGNAL(clicked()), this, SLOT(onFileToolButtonClicked()));
}

te::qt::widgets::OutputRasterParametersWidget::~OutputRasterParametersWidget()
{
}

Ui::OutputRasterParametersWidgetForm* te::qt::widgets::OutputRasterParametersWidget::getForm() const
{
  return m_ui.get();
}

std::string te::qt::widgets::OutputRasterParametersWidget::getType() const
{
  if(m_dataSource)
    return m_rasterInfoDlg->getWidget()->getType();

  if(m_fileGDAL)
    return "GDAL";

  return "";
}

std::map<std::string, std::string> te::qt::widgets::OutputRasterParametersWidget::getInfo() const
{
  if(m_dataSource)
    return m_rasterInfoDlg->getWidget()->getInfo();

  if(m_fileGDAL)
  {
    std::map<std::string, std::string> rinfo;
    std::string name = m_ui->m_outputLineEdit->text().toStdString();
    rinfo["URI"] = name;

    return rinfo;
  }

  return std::map<std::string, std::string>();
}

std::string te::qt::widgets::OutputRasterParametersWidget::getName() const
{
  if(m_dataSource)
    return m_rasterInfoDlg->getWidget()->getName();

  if(m_fileGDAL && !m_ui->m_outputLineEdit->text().isEmpty())
    return m_ui->m_outputLineEdit->text().toStdString();

  return "";
}

void te::qt::widgets::OutputRasterParametersWidget::onSourceToolButtonClicked()
{
  if(m_rasterInfoDlg->exec() == QDialog::Accepted)
  {
    std::string name = m_rasterInfoDlg->getWidget()->getName();

    m_ui->m_outputLineEdit->setText(name.c_str());

    m_dataSource = true;
    m_fileGDAL = false;
  }
}

void te::qt::widgets::OutputRasterParametersWidget::onFileToolButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Raster to File"), "", tr("GDAL File (*.tif *.TIF)"));

  if (fileName.isEmpty())
    return;

  QFileInfo file(fileName);

  if(file.suffix().isEmpty())
    fileName.append(".tif");

  m_ui->m_outputLineEdit->setText(fileName);

  m_dataSource = false;
  m_fileGDAL = true;
}
