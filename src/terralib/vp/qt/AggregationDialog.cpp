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
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../datatype/Enums.h"
#include "../../datatype/Property.h"
#include "../../maptools/AbstractLayer.h"
#include "../core/Exception.h"
#include "AggregationDialog.h"
#include "LayerTreeModel.h"
#include "ui_AggregationDialogForm.h"
#include "VectorProcessingConfig.h"

// Qt
#include <QtGui/QTreeWidget>
#include <QListWidget>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);
Q_DECLARE_METATYPE(te::dt::Property*);

te::vp::AggregationDialog::AggregationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::AggregationDialogForm)
{
// add controls
  m_ui->setupUi(this);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-aggregation-hint").pixmap(112,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), SLOT(onCancelPushButtonClicked()));   
  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), SLOT(onHelpPushButtonClicked()));
  
  setOperations();
  
  connect(m_ui->m_layerTreeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeViewClicked(const QModelIndex&)));

  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), SLOT(onFilterLineEditTextChanged(const QString&)));

}

te::vp::AggregationDialog::~AggregationDialog()
{
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  m_model = new LayerTreeModel(m_layers, true);

  m_ui->m_layerTreeView->setModel(m_model);
  m_ui->m_layerTreeView->setSelectionMode(QAbstractItemView::NoSelection);
}

void te::vp::AggregationDialog::setSelectedLayers(std::vector<std::string> selectedLayers)
{
  m_selectedLayers = selectedLayers;
}

void te::vp::AggregationDialog::setOperations()
{
  m_ui->m_selectAllComboBox->addItem("");
  m_ui->m_selectAllComboBox->addItem(tr("Minimum value"), MIN_VALUE);
  m_ui->m_selectAllComboBox->addItem(tr("Maximum value"), MAX_VALUE);
  m_ui->m_selectAllComboBox->addItem(tr("Mean"), MEAN);
  m_ui->m_selectAllComboBox->addItem(tr("Sum of values"), SUM);
  m_ui->m_selectAllComboBox->addItem(tr("Total number of values"), COUNT);
  m_ui->m_selectAllComboBox->addItem(tr("Total not null values"), VALID_COUNT);
  m_ui->m_selectAllComboBox->addItem(tr("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_selectAllComboBox->addItem(tr("Kernel"), KERNEL);
  m_ui->m_selectAllComboBox->addItem(tr("Variance"), VARIANCE);
  m_ui->m_selectAllComboBox->addItem(tr("Skewness"), SKEWNESS);
  m_ui->m_selectAllComboBox->addItem(tr("Kurtosis"), KURTOSIS);
  m_ui->m_selectAllComboBox->addItem(tr("Amplitude"), AMPLITUDE);
  m_ui->m_selectAllComboBox->addItem(tr("Median"), MEDIAN);
  m_ui->m_selectAllComboBox->addItem(tr("Coefficient variation"), VAR_COEFF);
  m_ui->m_selectAllComboBox->addItem(tr("Mode"), MODE);

  m_ui->m_rejectAllComboBox->addItem("");
  m_ui->m_rejectAllComboBox->addItem(tr("Minimum value"), MIN_VALUE);
  m_ui->m_rejectAllComboBox->addItem(tr("Maximum value"), MAX_VALUE);
  m_ui->m_rejectAllComboBox->addItem(tr("Mean"), MEAN);
  m_ui->m_rejectAllComboBox->addItem(tr("Sum of values"), SUM);
  m_ui->m_rejectAllComboBox->addItem(tr("Total number of values"), COUNT);
  m_ui->m_rejectAllComboBox->addItem(tr("Total not null values"), VALID_COUNT);
  m_ui->m_rejectAllComboBox->addItem(tr("Standard deviation"), STANDARD_DEVIATION);
  m_ui->m_rejectAllComboBox->addItem(tr("Kernel"), KERNEL);
  m_ui->m_rejectAllComboBox->addItem(tr("Variance"), VARIANCE);
  m_ui->m_rejectAllComboBox->addItem(tr("Skewness"), SKEWNESS);
  m_ui->m_rejectAllComboBox->addItem(tr("Kurtosis"), KURTOSIS);
  m_ui->m_rejectAllComboBox->addItem(tr("Amplitude"), AMPLITUDE);
  m_ui->m_rejectAllComboBox->addItem(tr("Median"), MEDIAN);
  m_ui->m_rejectAllComboBox->addItem(tr("Coefficient variation"), VAR_COEFF);
  m_ui->m_rejectAllComboBox->addItem(tr("Mode"), MODE);

}

void te::vp::AggregationDialog::onLayerTreeViewClicked(QTreeWidgetItem * item, int column)
{ 
}

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{
}

void te::vp::AggregationDialog::onTreeViewClicked(const QModelIndex& index)
{
  QStringList propertyList;
  int propertyType;

  std::map<te::map::AbstractLayerPtr, std::vector<te::dt::Property*>> selected = m_model->getSelected();
  
  if(selected.size() > 0)
  {
    std::vector<te::dt::Property*> properties = selected.begin()->second;
  
    for(size_t i=0; i < properties.size(); i++)
    {
      propertyType = properties[i]->getType();

      if(propertyType == te::dt::STRING_TYPE)
      {  
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MIN_VALUE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MAX_VALUE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : COUNT");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : VALID_COUNT");
        propertyList.append("");
      }
      else
      {
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MIN_VALUE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MAX_VALUE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MEAN");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : SUM");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : COUNT");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : VALID_COUNT");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : STANDARD_DEVIATION");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : KERNEL");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : VARIANCE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : SKEWNESS");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : KURTOSIS");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : AMPLITUDE");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MEDIAN");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : VAR_COEFF");
        propertyList.append(QString(properties[i]->getName().c_str()) + " : MODE");
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


void te::vp::AggregationDialog::onCancelPushButtonClicked()
{
  reject();
}

void te::vp::AggregationDialog::onHelpPushButtonClicked()
{
}
