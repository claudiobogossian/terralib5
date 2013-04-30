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
  \file terralib/vp/IntersectionDialog.cpp

  \brief A dialog for intersection operation
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../dataaccess/dataset/DataSetType.h"
#include "../../dataaccess/datasource/DataSourceInfo.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../maptools/AbstractLayer.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../core/Exception.h"
#include "IntersectionDialog.h"
#include "LayerTreeModel.h"
#include "ui_IntersectionDialogForm.h"
#include "VectorProcessingConfig.h"

// Qt
#include <QtGui/QTreeWidget>
#include <QtGui/QFileDialog>

te::vp::IntersectionDialog::IntersectionDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::IntersectionDialogForm)
{
// add controls
  m_ui->setupUi(this);

  //m_ui->m_layerTreeView->header()->setStretchLastSection(false);

  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme(VP_IMAGES"/vp-intersection-hint").pixmap(48,48));
  m_ui->m_targetDatasourceToolButton->setIcon(QIcon::fromTheme("datasource"));

  connect(m_ui->m_filterLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onFilterLineEditTextChanged(const QString&)));
  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this,  SLOT(onTargetFileToolButtonPressed()));
}

te::vp::IntersectionDialog::~IntersectionDialog()
{
  delete m_model;
}

void te::vp::IntersectionDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  m_model = new LayerTreeModel(m_layers);

  m_ui->m_layerTreeView->setModel(m_model);
}

void te::vp::IntersectionDialog::onLayerTreeViewClicked(QTreeWidgetItem * item, int column)
{
  
}

void te::vp::IntersectionDialog::setSelectedLayers(std::vector<std::string> selectedLayers)
{
  m_selectedLayers = selectedLayers;
}

void te::vp::IntersectionDialog::onFilterLineEditTextChanged(const QString& text)
{
  
}

int te::vp::IntersectionDialog::getMemoryUse()
{
  if(m_ui->m_wholeMemRadioButton->isChecked())
    return WHOLE_MEM;
  else if(m_ui->m_partiallyMemRadioButton->isChecked())
    return PARTIALLY_MEM;
  else
    return LOW_MEM;
}

void te::vp::IntersectionDialog::onTargetDatasourceToolButtonPressed()
{
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if(dsPtrList.size() <= 0)
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;
}

void te::vp::IntersectionDialog::onTargetFileToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Open Feature File"), QString(""), tr("Common Formats (*.shp *.SHP *.kml *.KML *.geojson *.GEOJSON *.gml *.GML);; Shapefile (*.shp *.SHP);; GML (*.gml *.GML);; Web Feature Service - WFS (*.xml *.XML *.wfs *.WFS);; All Files (*.*)"), 0, QFileDialog::ReadOnly);
  
  if(fileName.isEmpty())
    return;

  m_ui->m_repositoryLineEdit->setText(fileName);

  std::vector<te::da::DataSourceInfoPtr> datasources;
  te::da::DataSourceInfoManager::getInstance().getByType("OGR", datasources);

  m_outputDatasource = datasources[0];
}