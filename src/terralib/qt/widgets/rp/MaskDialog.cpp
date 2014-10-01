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
  \file terralib/qt/widgets/rp/MaskDialog.cpp

  \brief This file defines a class for a MaskDialog.
*/

// TerraLib
#include "MaskDialog.h"
#include "ui_MaskDialogForm.h"

// Qt
#include <QGridLayout>
#include <QLineEdit>
#include <QMessageBox>

te::qt::widgets::MaskDialog::MaskDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::MaskDialogForm)
{
//build form
  m_ui->setupUi(this);

//connects
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));
}

te::qt::widgets::MaskDialog::~MaskDialog()
{
  m_matrix.clear();
  m_window.clear();
}

void te::qt::widgets::MaskDialog::setMaskSize(int size, int defaultValue)
{
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  QString defaultValueStr;
  defaultValueStr.setNum(defaultValue);

  for(int i = 0; i < size; ++i)
  {
    std::vector<QLineEdit*> line;

    for(int j = 0; j < size; ++j)
    {
      QLineEdit* lineEdit = new QLineEdit(m_ui->m_widget);
      
      lineEdit->setValidator(new QIntValidator(this));
      lineEdit->setMaximumWidth(30);
      lineEdit->setAlignment(Qt::AlignHCenter);
      lineEdit->setText(defaultValueStr);

      layout->addWidget(lineEdit, i, j);

      line.push_back(lineEdit);
    }

    m_matrix.push_back(line);
  }
}

void te::qt::widgets::MaskDialog::setMaskSize(boost::numeric::ublas::matrix<double> matrix)
{
  QGridLayout* layout = new QGridLayout(m_ui->m_widget);

  for(std::size_t i = 0; i < matrix.size1(); ++i)
  {
    std::vector<QLineEdit*> line;

    for(std::size_t j = 0; j < matrix.size2(); ++j)
    {
      QLineEdit* lineEdit = new QLineEdit(m_ui->m_widget);
      
      lineEdit->setValidator(new QIntValidator(this));
      lineEdit->setMaximumWidth(30);
      lineEdit->setAlignment(Qt::AlignHCenter);

      QString value;
      value.setNum(matrix(i,j));
      lineEdit->setText(value);

      layout->addWidget(lineEdit, i, j);

      line.push_back(lineEdit);
    }

    m_matrix.push_back(line);
  }
}

boost::numeric::ublas::matrix<double> te::qt::widgets::MaskDialog::getMatrix()
{
  return m_window;
}

void te::qt::widgets::MaskDialog::onOkPushButtonClicked()
{
  m_window.resize(m_matrix.size(), m_matrix.size());

  for(std::size_t i = 0; i < m_matrix.size(); ++i)
  {
    for(std::size_t j = 0; j < m_matrix[i].size(); ++j)
    {
      QLineEdit* lineEdit = m_matrix[i][j];
      
      if(lineEdit->text().isEmpty())
      {
        QMessageBox::warning(this, tr("Warning"), tr("Matrix with empty values."));
        return;
      }

      m_window(i,j) = lineEdit->text().toDouble();
    }
  }

  accept();
}
