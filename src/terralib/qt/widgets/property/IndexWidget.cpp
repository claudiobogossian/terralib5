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
  \file terralib/qt/widgets/property/IndexWidget.cpp

  \brief This file has the IndexWidget class.
*/

// TerraLib
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/Index.h"
#include "../utils/DoubleListWidget.h"
#include "IndexWidget.h"
#include "ui_DoubleListWidgetForm.h"
#include "ui_IndexWidgetForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>

te::qt::widgets::IndexWidget::IndexWidget(te::da::DataSetType* dsType, QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::IndexWidgetForm)
{
  m_ui->setupUi(this);

  //set index types
  m_ui->m_typeComboBox->addItem(tr("B Tree"), te::da::B_TREE_TYPE);
  m_ui->m_typeComboBox->addItem(tr("R Tree"), te::da::R_TREE_TYPE);
  m_ui->m_typeComboBox->addItem(tr("Quad Tree"), te::da::QUAD_TREE_TYPE);
  m_ui->m_typeComboBox->addItem(tr("Hash"), te::da::HASH_TYPE);


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

te::qt::widgets::IndexWidget::~IndexWidget()
{
}

Ui::IndexWidgetForm* te::qt::widgets::IndexWidget::getForm() const
{
  return m_ui.get();
}

te::da::Index* te::qt::widgets::IndexWidget::getIndex()
{
  //get index name
  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    return 0;
  }

  std::string indexName = m_ui->m_nameLineEdit->text().toStdString();

  //get index type
  int currIndex = m_ui->m_typeComboBox->currentIndex();
  int indexType = m_ui->m_typeComboBox->itemData(currIndex).toInt();

  te::da::IndexType idxType = (te::da::IndexType)indexType;

  //get index properties
  std::vector<std::string> vec = m_doubleListWidget->getOutputValues();

  if(vec.empty())
  {
    return 0;
  }

  //create index
  te::da::Index* idx = new te::da::Index(m_dsType);

  idx->setName(indexName);
  idx->setIndexType(idxType);

  for(size_t t = 0; t < vec.size(); ++t)
  {
    te::dt::Property* p = m_dsType->getProperty(vec[t])->clone();

    idx->add(p);
  }

  return idx;
}

bool te::qt::widgets::IndexWidget::checkParameters()
{
  //get index name
  if(m_ui->m_nameLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Index name not defined."));
    return false;
  }

  //get index properties
  std::vector<std::string> vec = m_doubleListWidget->getOutputValues();

  if(vec.empty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("No property selected."));
    return false;
  }

  return true;
}

