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
#include "../../edit/Feature.h"
#include "../../edit/RepositoryManager.h"
#include "FeatureAttributesDialog.h"
#include "ui_FeatureAttributesDialogForm.h"

// Qt
#include <QTreeWidgetItem>

// STL
#include <cassert>

te::edit::FeatureAttributesDialog::FeatureAttributesDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::FeatureAttributesDialogForm),
    m_feature(0),
    m_layer(0),
    m_restrictivePropertyPos(0)
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
{}

void te::edit::FeatureAttributesDialog::set(te::da::DataSet* dataset, Feature* f, const te::map::AbstractLayerPtr& layer)
{
  m_feature = f;
  m_layer = layer;
  m_dataset = dataset;
  m_data = f->getData();

  initialize();
}

void te::edit::FeatureAttributesDialog::initialize()
{
  // Get the property pos that compose the object id
  te::da::GetOIDPropertyPos(m_layer->getSchema().get(), m_restrictivePropertyPos);

  // Get the geometry property position
  m_restrictivePropertyPos.push_back(te::da::GetFirstSpatialPropertyPos(m_dataset));

  if (m_feature->getData().size() == 0)
  {
    // Fills the QTreeWidgetItem
    while (m_dataset->moveNext())
    {
      for (std::size_t i = 0; i < m_dataset->getNumProperties(); ++i)
      {
        QTreeWidgetItem* propertyItem = new QTreeWidgetItem;

        int pos = te::da::GetPropertyPos(m_layer->getSchema().get(), m_dataset->getPropertyName(i));
        propertyItem->setText(0, m_dataset->getPropertyName(pos).c_str());

        if (m_dataset->getPropertyDataType(pos) == te::dt::GEOMETRY_TYPE)
          propertyItem->setIcon(0, QIcon::fromTheme("geometry"));

        if (!m_dataset->isNull(pos))
        {
          QString qvalue;

          if (m_dataset->getPropertyDataType(pos) == te::dt::STRING_TYPE)
          {
            std::string value = m_dataset->getString(pos);
            te::common::CharEncoding encoding = m_dataset->getPropertyCharEncoding(pos);
            qvalue = te::qt::widgets::Convert2Qt(value, encoding);
          }
          else
            qvalue = m_dataset->getAsString(pos, 3).c_str();

          propertyItem->setText(1, qvalue);
        }
        else // property null value!
          propertyItem->setText(1, "");

        m_ui->m_attributesTreeWidget->addTopLevelItem(propertyItem);

        // fill the m_data
        std::auto_ptr<te::dt::AbstractData> data(m_dataset->getValue(pos));
        m_data[pos] = data.release();
      }
    }
  }
  else
  {
    while (m_dataset->moveNext())
    {
      std::map<std::size_t, te::dt::AbstractData* > ::iterator it;

      for (std::size_t i = 0; i < m_dataset->getNumProperties(); ++i)
      {
        QTreeWidgetItem* propertyItem = new QTreeWidgetItem;

        int pos = te::da::GetPropertyPos(m_layer->getSchema().get(), m_dataset->getPropertyName(i));
        propertyItem->setText(0, m_dataset->getPropertyName(pos).c_str());

        it = m_data.find(pos);
        if (it != m_data.end())
          propertyItem->setText(1, QString(it->second->toString().c_str()));
        else
        {
          if (m_dataset->getPropertyDataType(pos) == te::dt::GEOMETRY_TYPE)
            propertyItem->setIcon(0, QIcon::fromTheme("geometry"));

          propertyItem->setText(1, QString(m_dataset->getAsString(pos, 3).c_str()));

        }

        m_ui->m_attributesTreeWidget->addTopLevelItem(propertyItem);
      }
    }
  }

}

void te::edit::FeatureAttributesDialog::onOkPushButtonPressed()
{
  std::size_t level_item = 0;

  m_dataset->moveBeforeFirst();

  while (m_dataset->moveNext())
  {
    for (std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
    {
      QTreeWidgetItem* propertyItem = m_ui->m_attributesTreeWidget->topLevelItem(level_item);

      std::auto_ptr<te::dt::AbstractData> data(getValue(m_dataset->getPropertyDataType(it->first), propertyItem->text(1)));

      m_data[it->first] = data.release();

      m_dataset->getValue(it->first);

      level_item++;
    }
  }

  m_feature->setData(m_data);
  m_feature->setOperation(te::edit::GEOMETRY_UPDATE_ATTRIBUTES);

  RepositoryManager::getInstance().addFeature(m_layer->getId(), m_feature->clone());

  close();
}

// Hack from http://stackoverflow.com/a/13374558 to making only one column of a QTreeWidgetItem editable
void te::edit::FeatureAttributesDialog::onAttributesTreeWidgetItemDoubleClicked(QTreeWidgetItem* item, int column)
{
  bool isrestrictive = false;

  for (std::size_t i = 0; i < m_restrictivePropertyPos.size(); i++)
  {
    if (m_restrictivePropertyPos[i] == m_ui->m_attributesTreeWidget->currentIndex().row())
    {
      isrestrictive = true;
      break;
    }
  }

  Qt::ItemFlags tmp = item->flags();
  if (column == 1 && !isrestrictive)
    item->setFlags(tmp | Qt::ItemIsEditable);
  else if (tmp & Qt::ItemIsEditable)
    item->setFlags(tmp ^ Qt::ItemIsEditable);

}

std::auto_ptr<te::dt::AbstractData> te::edit::FeatureAttributesDialog::getValue(int type, QString value) const
{
  switch (type)
  {
    case te::dt::INT16_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Int16(atoi(value.toStdString().c_str())));

    case te::dt::INT32_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Int32(atoi(value.toStdString().c_str())));

    case te::dt::INT64_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Int64(atoi(value.toStdString().c_str())));

    case te::dt::FLOAT_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Float(atof(value.toStdString().c_str())));

    case te::dt::DOUBLE_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Double(atof(value.toStdString().c_str())));

    case te::dt::NUMERIC_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::Numeric(value.toStdString().c_str()));

    case te::dt::STRING_TYPE:
      return std::auto_ptr<te::dt::AbstractData>(new te::dt::String(value.toStdString().c_str()));

    default:
      return std::auto_ptr<te::dt::AbstractData>(0);
  }
}