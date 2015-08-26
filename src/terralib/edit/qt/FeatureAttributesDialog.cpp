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
  \file terralib/edit/qt/FeatureAttributesDialog.cpp

  \brief A widget used to show and setup feature attributes.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/dataset/ObjectId.h"
#include "../../dataaccess/dataset/ObjectIdSet.h"
#include "../../datatype/AbstractData.h"
#include "../../edit/Feature.h"
#include "FeatureAttributesDialog.h"
#include "ui_FeatureAttributesDialogForm.h"

// Qt
#include <QTreeWidgetItem>

// STL
#include <cassert>

te::edit::FeatureAttributesDialog::FeatureAttributesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::FeatureAttributesDialogForm),
    m_type(0),
    m_feature(0)
{
  m_ui->setupUi(this);

  // Setup the widget that will be used to show the attributes
  m_ui->m_attributesTreeWidget->setAlternatingRowColors(true);
  m_ui->m_attributesTreeWidget->setColumnCount(2);

  QStringList labels;
  labels << tr("Property") << tr("Value");
  m_ui->m_attributesTreeWidget->setHeaderLabels(labels);

  // Signals & slots
  connect(m_ui->m_okPushButton, SIGNAL(pressed()), this, SLOT(onOkPushButtonPressed()));
  connect(m_ui->m_attributesTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem*, int)));
}

te::edit::FeatureAttributesDialog::~FeatureAttributesDialog()
{
}

void te::edit::FeatureAttributesDialog::set(te::da::DataSetType* type, Feature* f)
{
  m_type = type;
  m_feature = f;

  initialize();
}

void te::edit::FeatureAttributesDialog::initialize()
{
  const std::vector<te::dt::Property*>& properties = m_type->getProperties();

  const std::map<std::size_t, te::dt::AbstractData*> data = m_feature->getData();

  for (std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = data.begin(); it != data.end(); ++it)
  {
    te::dt::Property* p = properties[it->first];

    QTreeWidgetItem* propertyItem = new QTreeWidgetItem;
    propertyItem->setText(0, p->getName().c_str());
    propertyItem->setText(1, QString(it->second->toString().c_str()));

    m_ui->m_attributesTreeWidget->addTopLevelItem(propertyItem);
  }

}

void te::edit::FeatureAttributesDialog::onOkPushButtonPressed()
{
  close();
}

// Hack from http://stackoverflow.com/a/13374558 to making only one column of a QTreeWidgetItem editable
void te::edit::FeatureAttributesDialog::onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column)
{
  Qt::ItemFlags tmp = item->flags();
  if(column == 1)
    item->setFlags(tmp | Qt::ItemIsEditable);
  else if(tmp & Qt::ItemIsEditable)
    item->setFlags(tmp ^ Qt::ItemIsEditable);
}
