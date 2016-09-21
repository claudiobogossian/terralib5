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
  \file terralib/edit/qt/NewShapeFileDialog.cpp

  \brief A dialog used to create a shapefile layer.
*/

// TerraLib
#include "../../dataaccess/dataset/DataSet.h"
#include "../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../srs/SpatialReferenceSystemManager.h"
#include "../Snap.h"
#include "../SnapManager.h"
#include "NewShapeFileDialog.h"
#include "ui_NewShapeFileDialogForm.h"
#include "Utils.h"

// Qt
#include <QCheckBox>
#include <QComboBox>
#include <QTableWidgetItem>

// STL
#include <cassert>



Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::edit::NewShapeFileDialog::NewShapeFileDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
    m_ui(new Ui::NewShapeFileDialogForm),
    m_layerExplorer(0),
    m_srid(4326)
{
  m_ui->setupUi(this);

  // Signals & slots
  connect(m_ui->m_buttonBox, SIGNAL(accepted()), this, SLOT(onOkPushButtonPressed()));
  connect(m_ui->m_specifySRSButton, SIGNAL(clicked()), SLOT(onMapSRIDTriggered()));
}

te::edit::NewShapeFileDialog::~NewShapeFileDialog()
{
}

void te::edit::NewShapeFileDialog::setLayerExplorer(te::qt::widgets::LayerItemView* layerExplorer)
{
  m_layerExplorer = layerExplorer;
}

void te::edit::NewShapeFileDialog::setSRID(const int& srid)
{
  m_srid = srid;

  std::string srsName = te::srs::SpatialReferenceSystemManager::getInstance().getName(m_srid);

  QString srs = QString("%1, SRID: %2").arg(srsName.c_str()).arg(m_srid);

  m_ui->m_srsEdit->setText(srs);
}

void te::edit::NewShapeFileDialog::onOkPushButtonPressed()
{
  te::gm::GeomType type;

  if (m_ui->m_radioBtnPoint->isChecked())
    type = te::gm::MultiPointType;
  else if (m_ui->m_radioBtnLine->isChecked())
    type = te::gm::MultiLineStringType;
  else if (m_ui->m_radioBtnPolygon->isChecked())
    type = te::gm::MultiPolygonType;

  te::map::AbstractLayerPtr layer = te::edit::CreateShapeFileLayer(type, m_srid);

  std::list<te::map::AbstractLayerPtr> list;
  list.push_back(layer);

  //insert in tree
  m_layerExplorer->addLayers(list, QModelIndex());

  close();
}

void te::edit::NewShapeFileDialog::onMapSRIDTriggered()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

  std::string srsName = te::srs::SpatialReferenceSystemManager::getInstance().getName(srid.first);

  QString srs = QString("%1, SRID: %2").arg(srsName.c_str()).arg(srid.first);

  m_ui->m_srsEdit->setText(srs);

  m_srid = srid.first;
}