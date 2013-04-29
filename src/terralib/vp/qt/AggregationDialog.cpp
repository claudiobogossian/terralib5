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
#include "../../maptools/AbstractLayer.h"
#include "../core/Exception.h"
#include "AggregationDialog.h"
#include "LayerTreeModel.h"
#include "ui_AggregationDialogForm.h"
#include "VectorProcessingConfig.h"

// Qt
#include <QtGui/QTreeWidget>

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

  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), SLOT(onFilterLineEditTextChanged(const QString&)));
}

te::vp::AggregationDialog::~AggregationDialog()
{
}

void te::vp::AggregationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;
  
  LayerTreeModel* model = new LayerTreeModel(m_layers, true);

  m_ui->m_layerTreeView->setModel(model);
}

void te::vp::AggregationDialog::onLayerTreeViewClicked(QTreeWidgetItem * item, int column)
{
  
}

void te::vp::AggregationDialog::setSelectedLayers(std::vector<std::string> selectedLayers)
{
  m_selectedLayers = selectedLayers;
}

void te::vp::AggregationDialog::onFilterLineEditTextChanged(const QString& text)
{

}
