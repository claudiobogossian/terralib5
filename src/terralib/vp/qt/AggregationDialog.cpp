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
  \file terralib/vp/AggregationDialog.cpp

  \brief A dialog for intersection operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../common/StringUtils.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Config.h"
#include "../core/Exception.h"
#include "AggregationDialog.h"
#include "LayerTreeModel.h"
#include "ui_AggregationDialogForm.h"
#include "VectorProcessingConfig.h"
#include "Utils.h"

// Qt
#include <QtCore/QList>
#include <QtGui/QTreeWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>

te::vp::AggregationDialog::AggregationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::AggregationDialogForm),
    m_layers(std::list<te::map::AbstractLayerPtr>()),
    m_model(0)
{
// add controls
  m_ui->setupUi(this);

// add icons
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-aggregation-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  setAttributes();
  setAttributesNameMap();

  connect(m_ui->m_layerTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeViewClicked(const QModelIndex&)));
  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onFilterLineEditTextChanged(const QString&)));
  connect(m_ui->m_outputListWidget, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(onOutputListWidgetClicked(QListWidgetItem *)));
  connect(m_ui->m_selectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectAllComboBoxChanged(int)));
  connect(m_ui->m_rejectAllComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRejectAllComboBoxChanged(int)));

  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked()));
}

te::vp::AggregationDialog::~AggregationDialog()
{
  delete m_model;
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  if(m_model != 0)
      delete m_model;

  m_model = new LayerTreeModel(m_layers, true);

  m_ui->m_layerTreeView->setModel(m_model);
  m_ui->m_layerTreeView->setSelectionMode(QAbstractItemView::NoSelection);
}

int te::vp::AggregationDialog::getMemoryUse()
{
  if(m_ui->m_wholeMemRadioButton->isChecked())
    return WHOLE_MEM;
  else if(m_ui->m_partiallyMemRadioButton->isChecked())
    return PARTIALLY_MEM;
  else
    return LOW_MEM;
}

void te::vp::AggregationDialog::setAttributes()
{
  m_ui->m_selectAllComboBox->addItem("");
  m_ui->m_selectAllComboBox->addItem(TR_VP("Minimum value"), MIN_VALUE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Maximum value"), MAX_VALUE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Mean"), MEAN);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Sum of values"), SUM);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Total number of values"), COUNT);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Total not null values"), VALID_COUNT);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Kernel"), KERNEL);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Variance"), VARIANCE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Skewness"), SKEWNESS);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Kurtosis"), KURTOSIS);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Amplitude"), AMPLITUDE);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Median"), MEDIAN);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Coefficient variation"), VAR_COEFF);
  m_ui->m_selectAllComboBox->addItem(TR_VP("Mode"), MODE);

  m_ui->m_rejectAllComboBox->addItem("");
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Minimum value"), MIN_VALUE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Maximum value"), MAX_VALUE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Mean"), MEAN);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Sum of values"), SUM);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Total number of values"), COUNT);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Total not null values"), VALID_COUNT);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Kernel"), KERNEL);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Variance"), VARIANCE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Skewness"), SKEWNESS);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Kurtosis"), KURTOSIS);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Amplitude"), AMPLITUDE);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Median"), MEDIAN);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Coefficient variation"), VAR_COEFF);
  m_ui->m_rejectAllComboBox->addItem(TR_VP("Mode"), MODE);

}

void te::vp::AggregationDialog::setAttributesNameMap()
{
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MIN_VALUE, TR_VP("Minimum value")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MAX_VALUE, TR_VP("Maximum value")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MEAN, TR_VP("Mean")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(SUM, TR_VP("Sum of values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(COUNT, TR_VP("Total number of values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VALID_COUNT, TR_VP("Total not null values")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(STANDARD_DEVIATION, TR_VP("Standard deviation")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(KERNEL, TR_VP("Kernel")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VARIANCE, TR_VP("Variance")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(SKEWNESS, TR_VP("Skewness")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(KURTOSIS, TR_VP("Kurtosis")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(AMPLITUDE, TR_VP("Amplitude")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MEDIAN, TR_VP("Median")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(VAR_COEFF, TR_VP("Coefficient variation")));
  m_attributeNameMap.insert(std::map<Attributes, std::string>::value_type(MODE, TR_VP("Mode")));
}

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{
  std::list<te::map::AbstractLayerPtr> filteredLayers = te::vp::GetFilteredLayers(text.toStdString(), m_layers);

  delete m_model;
  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
  m_ui->m_outputListWidget->clear();

  if(text == "")
    filteredLayers = m_layers;

  m_model = new LayerTreeModel(filteredLayers);

  m_ui->m_layerTreeView->setModel(m_model);
}

void te::vp::AggregationDialog::onTreeViewClicked(const QModelIndex& index)
{
  QStringList propertyList;
  int propertyType;

//set both combobox at first position
  m_ui->m_selectAllComboBox->setCurrentIndex(0);
  m_ui->m_rejectAllComboBox->setCurrentIndex(0);

  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*> > selected = m_model->getSelected();
  
  if(selected.size() > 0)
  {
    std::vector<te::dt::Property*> properties = selected.begin()->second;
  
    for(size_t i=0; i < properties.size(); ++i)
    {
      propertyType = properties[i]->getType();

      if(propertyType == te::dt::STRING_TYPE)
      {  
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MIN_VALUE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MAX_VALUE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[COUNT].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VALID_COUNT].c_str());

        propertyList.append("");
      }
      else
      {
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MIN_VALUE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MAX_VALUE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MEAN].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[SUM].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[COUNT].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VALID_COUNT].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[STANDARD_DEVIATION].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[KERNEL].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VARIANCE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[SKEWNESS].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[KURTOSIS].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[AMPLITUDE].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MEDIAN].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[VAR_COEFF].c_str());
        propertyList.append(QString(properties[i]->getName().c_str()) + " : " + m_attributeNameMap[MODE].c_str());
     
        propertyList.append("");
      }
    }
  
    m_ui->m_outputListWidget->clear();
    m_ui->m_outputListWidget->addItems(propertyList);
  }
  else
  {
    m_ui->m_outputListWidget->clear();
  }
}

void te::vp::AggregationDialog::onSelectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text=="")
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_outputListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
    listFound.at(i)->setSelected(true);

  m_ui->m_rejectAllComboBox->setCurrentIndex(0);
}

void te::vp::AggregationDialog::onRejectAllComboBoxChanged(int index)
{
  QString text = m_ui->m_selectAllComboBox->itemText(index);
  Qt::MatchFlags flag = Qt::MatchEndsWith; //The search term matches the end of the item.
  
  if(text=="")
    return;

  QList<QListWidgetItem *> listFound;
  listFound = m_ui->m_outputListWidget->findItems(text, flag);
  
  for(int i=0; i < listFound.size(); ++i)
    listFound.at(i)->setSelected(false);

  m_ui->m_selectAllComboBox->setCurrentIndex(0);
}

void te::vp::AggregationDialog::onOutputListWidgetClicked(QListWidgetItem * item)
{
  if(item->text()=="")
    item->setSelected(false);
}

void te::vp::AggregationDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::vp::AggregationDialog::onHelpPushButtonClicked()
{
}
