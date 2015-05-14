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
  \file terralib/qt/widgets/property/ConstraintWidget.cpp

  \brief This file has the ConstraintWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../utils/DoubleListWidget.h"
#include "ConstraintWidget.h"
#include "ui_ConstraintWidgetForm.h"
#include "ui_DoubleListWidgetForm.h"

// Qt
#include <QMessageBox>

te::qt::widgets::ConstraintWidget::ConstraintWidget(te::da::DataSetType* dsType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ConstraintWidgetForm)
{
  m_ui->setupUi(this);

  //add double list widget to this form
  m_doubleListWidget = new DoubleListWidget(m_ui->m_widget);
  m_doubleListWidget->getForm()->m_leftItemsLabel->setText(tr("Available Properties"));
  m_doubleListWidget->getForm()->m_rightItemsLabel->setText(tr("Selected Properties"));

  QGridLayout* layout = new QGridLayout(m_ui->m_widget);
  layout->addWidget(m_doubleListWidget);

  //set properties names
  m_dsType = dsType;

  if(m_dsType)
  {
    std::vector<std::string> propValues;

    for(size_t t = 0; t < m_dsType->getProperties().size(); ++t)
    {
      propValues.push_back(m_dsType->getProperty(t)->getName());
    }

    m_doubleListWidget->setInputValues(propValues);
  }
}

te::qt::widgets::ConstraintWidget::~ConstraintWidget()
{
}

Ui::ConstraintWidgetForm* te::qt::widgets::ConstraintWidget::getForm() const
{
  return m_ui.get();
}

bool te::qt::widgets::ConstraintWidget::checkParameters()
{
//get index name
  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Constraint name not defined."));
    return false;
  }

  std::string constraintName = m_ui->m_nameLineEdit->text().toStdString();

//get properties
  std::vector<std::string> vec = m_doubleListWidget->getOutputValues();

  if(vec.empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("No property selected."));
    return false;
  }

  return true;
}

