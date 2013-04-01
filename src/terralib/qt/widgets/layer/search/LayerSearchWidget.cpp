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
  \file terralib/qt/widgets/layer/search/LayerSearchWidget.cpp

  \brief This file defines the LayerSearchWidget class.
*/

// TerraLib
#include "ui_LayerSearchWidgetForm.h"
#include "LayerSearchWidget.h"

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::LayerSearchWidget::LayerSearchWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::LayerSearchWidgetForm)
{
// add controls
  m_ui->setupUi(this);

//connect
  connect(m_ui->m_nameLineEdit, SIGNAL(textChanged(const QString&)), SLOT(onNameLineEditTextChanged(const QString&)));
}

te::qt::widgets::LayerSearchWidget::~LayerSearchWidget()
{
}

Ui::LayerSearchWidgetForm* te::qt::widgets::LayerSearchWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::LayerSearchWidget::enableMultiSelection(bool flag)
{
  if(flag == true)
  {
    m_ui->m_treeWidget->setSelectionMode(QAbstractItemView::MultiSelection);
  }
  else
  {
    m_ui->m_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  }
}

std::list<te::map::AbstractLayerPtr> te::qt::widgets::LayerSearchWidget::getSelecteds() const
{
  std::list<te::map::AbstractLayerPtr> list;

  QList<QTreeWidgetItem*> sel = m_ui->m_treeWidget->selectedItems();

  QList<QTreeWidgetItem*>::iterator it = sel.begin();

  while(it != sel.end())
  {
    QTreeWidgetItem* item = *it;

    QVariant v = item->data(0, Qt::UserRole);

    te::map::AbstractLayerPtr l = v.value<te::map::AbstractLayerPtr>();

    list.push_back(l);

    ++it;
  }

  return list;
}

void te::qt::widgets::LayerSearchWidget::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_layerList = layerList;

  fillTreeView(m_layerList);
}

void te::qt::widgets::LayerSearchWidget::fillTreeView(std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    QTreeWidgetItem* item = new QTreeWidgetItem(m_ui->m_treeWidget);
    item->setText(0, l->getTitle().c_str());
    item->setIcon(0, QIcon::fromTheme("layer"));
    item->setData(0, Qt::UserRole, QVariant::fromValue(l));
    item->setText(1, l->getId().c_str());
    item->setText(2, l->getType().c_str());

    ++it;
  }

//fix columns sizes
  m_ui->m_treeWidget->resizeColumnToContents(0);
  m_ui->m_treeWidget->resizeColumnToContents(2);

  m_ui->m_treeWidget->expandAll();
}

void te::qt::widgets::LayerSearchWidget::filter(const QList<QTreeWidgetItem*>& itens)
{
  QTreeWidgetItemIterator it(m_ui->m_treeWidget);

  while(*it)
  {
    QTreeWidgetItem* layerItem = *it;
    bool hide = itens.indexOf(layerItem) == -1;
    layerItem->setHidden(hide);

    ++it;
  }

  update();
}

void te::qt::widgets::LayerSearchWidget::onNameLineEditTextChanged(const QString& text)
{
  QList<QTreeWidgetItem*> itens = m_ui->m_treeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive, 0);

  filter(itens);
}
