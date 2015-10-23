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
  \file terralib/layout/qt/outside/SVGDialogOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "PDFSettingsOutside.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "ui_PDFSettings.h"

// Qt
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

te::layout::PDFSettingsOutside::PDFSettingsOutside(AbstractOutsideController* controller)
  : QDialog(0),
  AbstractOutsideView(controller),
  m_ui(new Ui::PDFSettings)
{
  m_ui->setupUi(this);

  setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
  
  init();
}

te::layout::PDFSettingsOutside::~PDFSettingsOutside()
{
  
}

void te::layout::PDFSettingsOutside::init()
{
  m_ui->lblDPIInformation->clear();
  m_ui->lblDPIInformation->setText("DPI: 150");

  m_ui->txtEdtFile->setEnabled(false);
  
  if (m_ui->pBtnOK && m_ui->pBtnCancel && m_ui->pBtnSaveAs)
  {
    connect(m_ui->pBtnOK, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
    connect(m_ui->pBtnCancel, SIGNAL(pressed()), this, SLOT(onCancelPushButtonPressed()));
    connect(m_ui->pBtnSaveAs, SIGNAL(clicked()), this, SLOT(onSaveAsClicked()));
    connect(m_ui->cmbDPI, SIGNAL(currentIndexChanged(const QString)), this, SLOT(onCurrentIndexChanged(const QString)));
  }

  QStringList list;
  list.append("150");
  list.append("300");
  list.append("600");
  list.append("1200");

  m_ui->cmbDPI->addItems(list);
}

void te::layout::PDFSettingsOutside::onOkPushButtonPressed()
{
  if (!m_ui->txtEdtFile->text().isEmpty())
  {
    accept();
    return;
  }
  QMessageBox::information(this, tr("PDF Settings"), tr("Please complete the filename."));
}

void te::layout::PDFSettingsOutside::onCancelPushButtonPressed()
{
  reject();
}

void te::layout::PDFSettingsOutside::onSaveAsClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), QDir::currentPath(), tr("PDF Files (*.pdf)"));

  if (!fileName.isEmpty())
  {
    m_ui->txtEdtFile->setText(fileName);
  }
}

void te::layout::PDFSettingsOutside::onCurrentIndexChanged(const QString & text)
{
  QPalette palette = m_ui->lblDPIInformation->palette();
  QString txtCurrentDPI = m_ui->cmbDPI->currentText();
  int dpi = txtCurrentDPI.toInt();
  if (dpi > 300)
  {
    //red text
    QBrush brushRed(QColor(255, 0, 0, 255));
    brushRed.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brushRed);

    m_ui->lblDPIInformation->clear();
    if (dpi < 900)
    {
      m_ui->lblDPIInformation->setText("The DPI chosen is high. Can cause very large files.");
    }
    else
    {
      m_ui->lblDPIInformation->setText("The DPI chosen is too high. Can cause very large files.");
    }
  }
  else
  {
    //black text
    QBrush brushBlack(QColor(0, 0, 0, 255));
    brushBlack.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brushBlack);

    QString txtDPI = "DPI: " + txtCurrentDPI;
    m_ui->lblDPIInformation->clear();
    m_ui->lblDPIInformation->setText(txtDPI);
  }
  m_ui->lblDPIInformation->setPalette(palette);
}

void te::layout::PDFSettingsOutside::setPosition(const double& x, const double& y)
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::PDFSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::string te::layout::PDFSettingsOutside::getFilePath()
{
  QString qTxt = m_ui->txtEdtFile->text();
  if (!qTxt.isEmpty())
  {
    if (!qTxt.endsWith(".pdf", Qt::CaseInsensitive))
    {
      qTxt += ".pdf";
      m_ui->txtEdtFile->clear();
      m_ui->txtEdtFile->setText(qTxt);
    }
  }
  return qTxt.toStdString();
}

int te::layout::PDFSettingsOutside::getDPI()
{
  QString txtDPI = m_ui->cmbDPI->currentText();
  return txtDPI.toInt();
}

void te::layout::PDFSettingsOutside::setCurrentDPI(int dpi)
{
  QString txt = QString::number(dpi);
  m_ui->cmbDPI->setCurrentText(txt);
}

