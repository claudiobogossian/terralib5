/*  Copyright(C) 2008 National Institute For Space Research(INPE) - Brazil.

This file is part of the TerraLib - a Framework for building GIS enabled applications.

TerraLib is free software : you can redistribute it and / or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License,
or(at your option) any later version.

TerraLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with TerraLib.See COPYING.If not, write to
TerraLib Team at <terralib - team@terralib.org>.
* /

/*!
\file terralib/mnt/qt/VolumeResultDialog.cpp

\brief A dialog Show Calculate Volume Results
*/

#include "VolumeResultDialog.h"
#include "ui_VolumeResultDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

#include <fstream>



te::mnt::VolumeResultDialog::VolumeResultDialog(std::vector<std::string> &polyvec,
                                                std::vector<std::string> &cortevec,
                                                std::vector<std::string> &aterrovec,
                                                std::vector<std::string> &areavec,
                                                std::vector<std::string> &iquotavec,
                                                std::string &attr,
                                                QWidget* parent,
                                                Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::VolumeResultDialogForm)
{
  // add controls
  m_ui->setupUi(this);

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_savePushButton, SIGNAL(clicked()), this, SLOT(onSavePushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_resultTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(QString(attr.c_str())));

  size_t nrows = polyvec.size();
  m_ui->m_resultTableWidget->setRowCount((int)nrows);
  for (size_t i = 0; i < nrows; i++)
  {
    QTableWidgetItem *item1 = new QTableWidgetItem(QString(polyvec[i].c_str()));
    QTableWidgetItem *item2 = new QTableWidgetItem(QString(cortevec[i].c_str()));
    QTableWidgetItem *item3 = new QTableWidgetItem(QString(aterrovec[i].c_str()));
    QTableWidgetItem *item4 = new QTableWidgetItem(QString(areavec[i].c_str()));
    QTableWidgetItem *item5 = new QTableWidgetItem(QString(iquotavec[i].c_str()));
    m_ui->m_resultTableWidget->setItem((int)i, 0, item1);
    m_ui->m_resultTableWidget->setItem((int)i, 1, item2);
    m_ui->m_resultTableWidget->setItem((int)i, 2, item3);
    m_ui->m_resultTableWidget->setItem((int)i, 3, item4);
    m_ui->m_resultTableWidget->setItem((int)i, 4, item5);
  }
}

te::mnt::VolumeResultDialog::~VolumeResultDialog()
{

}

void te::mnt::VolumeResultDialog::onHelpPushButtonClicked()
{
}

void te::mnt::VolumeResultDialog::onSavePushButtonClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("txt (*.txt *.TXT);;"), 0, QFileDialog::DontConfirmOverwrite);

  saveVolume(fileName.toStdString());

  accept();
}

void te::mnt::VolumeResultDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::mnt::VolumeResultDialog::saveVolume(const std::string& path)
{
  int rowCount = m_ui->m_resultTableWidget->rowCount();
  int colCount = m_ui->m_resultTableWidget->columnCount();

  if (rowCount < 1)
    return;

  if (path.empty())
    return;

  std::ofstream result(path.c_str(), std::ofstream::out);
  if (!result.is_open())
  {
    QMessageBox::warning(this, tr("Volume"), tr("File was not create!"));
    return;
  }

  for (int i = 0; i < colCount; i++)
  {
    QTableWidgetItem *item = m_ui->m_resultTableWidget->horizontalHeaderItem(i);
    if (item)
      result << item->text().toStdString() << '\t';
  }

  result << std::endl;

  for (int r = 0; r < rowCount; r++)
  {
    for (int i = 0; i < colCount; i++)
    {
      result << m_ui->m_resultTableWidget->item(r, i)->text().toStdString() << '\t';
    }
    result << std::endl;
  }

  result.close();

  QMessageBox::warning(this, tr("Volume"), tr("File was saved succesfully!"));

}
