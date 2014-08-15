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
  \file terralib/qt/widgets/property/ConstraintsIndexesListWidget.cpp

  \brief This file has the ConstraintsIndexesListWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/Constraint.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/dataset/Index.h"
#include "../../../dataaccess/dataset/PrimaryKey.h"
#include "../../../dataaccess/dataset/UniqueKey.h"
#include "../../../datatype/Property.h"
#include "ConstraintsIndexesListWidget.h"
#include "ConstraintsIndexesPropertyDialog.h"
#include "ui_ConstraintsIndexesListWidgetForm.h"

#define CONSTRAINT_PK_TYPE "Primary Key"
#define CONSTRAINT_UK_TYPE "Unique Key"
#define CONSTRAINT_UNKNOWN_TYPE "Unknown"
#define INDEX_TYPE "Index"


te::qt::widgets::ConstraintsIndexesListWidget::ConstraintsIndexesListWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::ConstraintsIndexesListWidgetForm)
{
  m_ui->setupUi(this);

// button icons
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeToolButton->setIcon(QIcon::fromTheme("list-remove"));
  m_ui->m_editToolButton->setIcon(QIcon::fromTheme("preferences-system"));

  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_removeToolButton, SIGNAL(clicked()), this, SLOT(onRemoveToolButtonClicked()));
  connect(m_ui->m_editToolButton, SIGNAL(clicked()), this, SLOT(onEditToolButtonClicked()));
  connect(m_ui->m_tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(onCellClicked(int, int)));
}

te::qt::widgets::ConstraintsIndexesListWidget::~ConstraintsIndexesListWidget()
{
}

Ui::ConstraintsIndexesListWidgetForm* te::qt::widgets::ConstraintsIndexesListWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::ConstraintsIndexesListWidget::setDataSetType(te::da::DataSetType* dsType)
{
  m_dsType = dsType;

  listDataSetProperties();
}

void te::qt::widgets::ConstraintsIndexesListWidget::onAddToolButtonClicked()
{
  if(m_dsType == 0)
    return;

  te::qt::widgets::ConstraintsIndexesPropertyDialog w(m_dsType, this);

  if(w.exec() == QDialog::Accepted)
  {
    listDataSetProperties();
  }
}

void te::qt::widgets::ConstraintsIndexesListWidget::onRemoveToolButtonClicked()
{
  int row = m_ui->m_tableWidget->currentRow();

  if(row < 0)
    return;

  std::string type = m_ui->m_tableWidget->item(row, 1)->text().toStdString();
  std::string name = m_ui->m_tableWidget->item(row, 0)->text().toStdString();

  if(type == CONSTRAINT_PK_TYPE)
  {
    removePrimaryKey(name);
  }
  else if(type == CONSTRAINT_UK_TYPE)
  {
    removeUniqueKey(name);
  }
  else if(type == INDEX_TYPE)
  {
    removeIndex(name);
  }

  m_ui->m_removeToolButton->setEnabled(false);

  listDataSetProperties();
}

void te::qt::widgets::ConstraintsIndexesListWidget::onEditToolButtonClicked()
{
  if(!m_ui->m_tableWidget->currentItem())
    return;

  std::string constIndxName = m_ui->m_tableWidget->item(m_ui->m_tableWidget->currentRow(), 0)->text().toStdString();

  te::qt::widgets::ConstraintsIndexesPropertyDialog w(m_dsType, this);

  te::da::PrimaryKey* pk = m_dsType->getPrimaryKey();
  te::da::UniqueKey* uk = m_dsType->getUniqueKey(constIndxName);
  te::da::Index* idx = m_dsType->getIndex(constIndxName);

  if(pk->getName() == constIndxName)
  {
    w.setConstraint(m_dsType->getPrimaryKey());
  }
  else if(uk)
  {
    w.setConstraint(uk);
  }
  else if(idx)
  {
    w.setIndex(idx);
  }

  if(w.exec() == QDialog::Accepted)
  {
    listDataSetProperties();
  }

}

void te::qt::widgets::ConstraintsIndexesListWidget::onCellClicked(int row, int col)
{
  m_ui->m_removeToolButton->setEnabled(true);
}

void te::qt::widgets::ConstraintsIndexesListWidget::listDataSetProperties()
{
  bool isEmpty = true;

  m_ui->m_tableWidget->setRowCount(0);

  //add primary key info
  if(m_dsType->getPrimaryKey())
  {
    addConstraint(m_dsType->getPrimaryKey());
    isEmpty = false;
  }

  //add unique keys info
  size_t size = m_dsType->getNumberOfUniqueKeys();

  for(size_t t = 0; t < size; ++t)
  {
    addConstraint(m_dsType->getUniqueKey(t));
    isEmpty = false;
  }

  //add index info
  size = m_dsType->getNumberOfIndexes();

  for(size_t t = 0; t < size; ++t)
  {
    addIndex(m_dsType->getIndex(t));
    isEmpty = false;
  }

  m_ui->m_tableWidget->resizeColumnsToContents();

  if(!isEmpty)
  {
    m_ui->m_editToolButton->setEnabled(true);
  }
}

void te::qt::widgets::ConstraintsIndexesListWidget::addConstraint(te::da::Constraint* c)
{
  std::string name = c->getName();
  std::string type = "";
  std::string properties = "";

  if(c->getType() == te::da::PRIMARY_KEY)
  {
    type = tr(CONSTRAINT_PK_TYPE).toStdString();
    properties = getPropertiesStr(dynamic_cast<te::da::PrimaryKey*>(c)->getProperties());
  }
  else if(c->getType() == te::da::UNIQUE_KEY)
  {
    type = tr(CONSTRAINT_UK_TYPE).toStdString();
    properties = getPropertiesStr(dynamic_cast<te::da::UniqueKey*>(c)->getProperties());
  }
  else
  {
    type = tr(CONSTRAINT_UNKNOWN_TYPE).toStdString();
  }

  addTableItem(name, type, properties);
}

void te::qt::widgets::ConstraintsIndexesListWidget::addIndex(te::da::Index* i)
{
  std::string name = i->getName();
  std::string type = tr(INDEX_TYPE).toStdString();
  std::string properties = getPropertiesStr(i->getProperties());

  addTableItem(name, type, properties);
}

void te::qt::widgets::ConstraintsIndexesListWidget::removePrimaryKey(const std::string& name)
{
  te::da::PrimaryKey* pk = m_dsType->getPrimaryKey();

  if(pk && pk->getName() == name)
  {
    m_dsType->remove(pk);
  }
}

void te::qt::widgets::ConstraintsIndexesListWidget::removeUniqueKey(const std::string& name)
{
  size_t size = m_dsType->getNumberOfUniqueKeys();

  for(size_t t = 0; t < size; ++t)
  {
    te::da::UniqueKey* uk  = m_dsType->getUniqueKey(t);

    if(uk->getName() == name)
    {
      m_dsType->remove(uk);
      break;
    }
  }
}

void te::qt::widgets::ConstraintsIndexesListWidget::removeIndex(const std::string& name)
{
  size_t size = m_dsType->getNumberOfIndexes();

  for(size_t t = 0; t < size; ++t)
  {
    te::da::Index* i  = m_dsType->getIndex(t);

    if(i->getName() == name)
    {
      m_dsType->remove(i);
      break;
    }
  }
}

void te::qt::widgets::ConstraintsIndexesListWidget::addTableItem(std::string name, std::string type, std::string properties)
{
  //new entry
  int newrow = m_ui->m_tableWidget->rowCount();

  m_ui->m_tableWidget->insertRow(newrow);

  QTableWidgetItem* itemName = new QTableWidgetItem(QString::fromStdString(name));
  m_ui->m_tableWidget->setItem(newrow, 0, itemName);
  itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QTableWidgetItem* itemType = new QTableWidgetItem(QString::fromStdString(type));
  m_ui->m_tableWidget->setItem(newrow, 1, itemType);
  itemType->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

  QTableWidgetItem* itemProp = new QTableWidgetItem(QString::fromStdString(properties));
  m_ui->m_tableWidget->setItem(newrow, 2, itemProp);
  itemProp->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

std::string te::qt::widgets::ConstraintsIndexesListWidget::getPropertiesStr(std::vector<te::dt::Property*> vec)
{
  std::string str = "";

  for(size_t t = 0; t < vec.size(); ++t)
  {
    str += vec[t]->getName();

    str += "; ";
  }

  return str;
}
