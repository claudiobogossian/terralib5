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
  \file terralib/qt/widgets/query/OrderByWidget.cpp

  \brief This file has the OrderByWidget class.
*/

// TerraLib
#include "../../../dataaccess/query/OrderByItem.h"
#include "../../../dataaccess/query/PropertyName.h"
#include "OrderByWidget.h"
#include "ui_OrderByWidgetForm.h"


// Qt
#include <QIcon>
#include <QMessageBox>

te::qt::widgets::OrderByWidget::OrderByWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::OrderByWidgetForm)
{
  m_ui->setupUi(this);

  //set order types
  m_orderTypes.insert(std::map<std::string, int>::value_type(tr("ASC").toStdString(), te::da::ASC));
  m_orderTypes.insert(std::map<std::string, int>::value_type(tr("DESC").toStdString(), te::da::DESC));

  std::map<std::string, int>::iterator it = m_orderTypes.begin();

  while(it != m_orderTypes.end())
  {
    m_ui->m_orderComboBox->addItem(it->first.c_str());

    ++it;
  }

  // set icons
  m_ui->m_addOrderPushButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_removeOrderPushButton->setIcon(QIcon::fromTheme("list-remove"));

  //connects
  connect(m_ui->m_addOrderPushButton, SIGNAL(clicked()), this, SLOT(onAddOrderPushButton()));
  connect(m_ui->m_removeOrderPushButton, SIGNAL(clicked()), this, SLOT(onRemoveOrderPushButton()));
}

te::qt::widgets::OrderByWidget::~OrderByWidget()
{
}

Ui::OrderByWidgetForm* te::qt::widgets::OrderByWidget::getForm() const
{
  return m_ui.get();
}

te::da::OrderBy* te::qt::widgets::OrderByWidget::gerOrderBy()
{
  std::vector<std::pair<std::string, std::string> > vec;

  getOrderItems(vec);

  if(vec.empty())
    return 0;

  te::da::OrderBy* order = new te::da::OrderBy;

  for(size_t t = 0; t < vec.size(); ++t)
  {
    std::map<std::string, int>::iterator it = m_orderTypes.find(vec[t].second);

    if(it != m_orderTypes.end())
    {
      te::da::OrderByItem* o = new te::da::OrderByItem(vec[t].first, (te::da::SortOrder)it->second);

      order->push_back(o);
    }
  }

  return order;
}

void te::qt::widgets::OrderByWidget::setAttributeList(const std::vector<std::string>& vec)
{
  m_ui->m_orderPropertyComboBox->clear();

  for(size_t t = 0; t <vec.size(); ++t)
  {
    m_ui->m_orderPropertyComboBox->addItem(vec[t].c_str());
  }
}

void te::qt::widgets::OrderByWidget::getOrderItems(std::vector<std::pair<std::string, std::string> >& list)
{
  int row = m_ui->m_orderTableWidget->rowCount();

  //get properties for each data set
  for(int i = 0; i < row; ++i)
  {
    //alias name
    QTableWidgetItem* itemType = m_ui->m_orderTableWidget->item(i, 1);
    std::string type = itemType->text().toStdString();

    //data set name
    QTableWidgetItem* itemName = m_ui->m_orderTableWidget->item(i, 0);
    std::string name = itemName->text().toStdString();

    list.push_back(std::pair<std::string, std::string> (name, type));
  }
}

void te::qt::widgets::OrderByWidget::onAddOrderPushButton()
{
  if(m_ui->m_orderPropertyComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Query Builder"), tr("Property value not defined."));
    return;
  }

  int newrow = m_ui->m_orderTableWidget->rowCount();

  std::string propertyValue = m_ui->m_orderPropertyComboBox->currentText().toStdString();
  std::string order = m_ui->m_orderComboBox->currentText().toStdString();

//new entry
  m_ui->m_orderTableWidget->insertRow(newrow);

  QTableWidgetItem* itemProperty = new QTableWidgetItem(QString::fromStdString(propertyValue));
  m_ui->m_orderTableWidget->setItem(newrow, 0, itemProperty);

  QTableWidgetItem* itemOrder = new QTableWidgetItem(QString::fromStdString(order));
  m_ui->m_orderTableWidget->setItem(newrow, 1, itemOrder);

  m_ui->m_orderTableWidget->resizeColumnsToContents();
}

void te::qt::widgets::OrderByWidget::onRemoveOrderPushButton()
{
  int row = m_ui->m_orderTableWidget->currentRow();

  if(row >= 0)
    m_ui->m_orderTableWidget->removeRow(row);

  m_ui->m_orderTableWidget->resizeColumnsToContents();
}
