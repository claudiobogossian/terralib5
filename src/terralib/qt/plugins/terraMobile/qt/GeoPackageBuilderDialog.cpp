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
\file src/terraMobilePlugin/qt/GeoPackageBuilderDialog.cpp

\brief This interface is used to get the input parameters for GeoPackage Builder operation.
*/

// TerraLib
#include <terralib/common/progress/ProgressManager.h>
#include <terralib/common/progress/TaskProgress.h>
#include <terralib/common/STLUtils.h>
#include <terralib/dataaccess/utils/Utils.h>
#include <terralib/geometry/GeometryProperty.h>
#include "GeoPackageBuilderDialog.h"
#include "ui_GeoPackageBuilderDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::plugins::terramobile::GeoPackageBuilderDialog::GeoPackageBuilderDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::GeoPackageBuilderDialogForm)
{
  // add controls
  m_ui->setupUi(this);
  m_ui->m_imgLabel->setPixmap(QIcon::fromTheme("terraMobile-hint").pixmap(100, 50));

  connect(m_ui->m_dirToolButton, SIGNAL(pressed()), this, SLOT(onDirToolButtonPressed()));

}

te::qt::plugins::terramobile::GeoPackageBuilderDialog::~GeoPackageBuilderDialog()
{

}

void te::qt::plugins::terramobile::GeoPackageBuilderDialog::setLayerList(std::list<te::map::AbstractLayerPtr> list)
{
  //clear lists
  m_ui->m_rasterLayerListWidget->clear();
  m_ui->m_vecLayerListWidget->clear();
  m_ui->m_pointLayerListWidget->clear();

  //fill combos
  std::list<te::map::AbstractLayerPtr>::iterator it = list.begin();

  while (it != list.end())
  {
    te::map::AbstractLayerPtr l = *it;

    if (l->isValid())
    {
      std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

      if (dsType->hasGeom())
      {
        QListWidgetItem* itemVec = new QListWidgetItem(m_ui->m_vecLayerListWidget);
        itemVec->setText(it->get()->getTitle().c_str());
        itemVec->setData(Qt::UserRole, QVariant::fromValue(l));

        m_ui->m_vecLayerListWidget->addItem(itemVec);

        //if this layer has point as geom type, add this layer to gatherer list
        te::gm::GeometryProperty* geomProp = te::da::GetFirstGeomProperty(dsType.get());

        if (geomProp && (geomProp->getGeometryType() == te::gm::PointType || geomProp->getGeometryType() == te::gm::MultiPointType))
        {
          QListWidgetItem* itemPoint = new QListWidgetItem(m_ui->m_pointLayerListWidget);
          itemPoint->setText(it->get()->getTitle().c_str());
          itemPoint->setData(Qt::UserRole, QVariant::fromValue(l));

          m_ui->m_pointLayerListWidget->addItem(itemPoint);
        }
      }
      else if (dsType->hasRaster())
      {
        QListWidgetItem* item = new QListWidgetItem(m_ui->m_rasterLayerListWidget);
        item->setText(it->get()->getTitle().c_str());
        item->setData(Qt::UserRole, QVariant::fromValue(l));

        m_ui->m_rasterLayerListWidget->addItem(item);
      }
    }
    ++it;
  }
}

void te::qt::plugins::terramobile::GeoPackageBuilderDialog::onDirToolButtonPressed()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(), tr("Geopackage (*.gpkg *.GPKG);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  m_ui->m_geopackageFileNameLineEdit->setText(fileName);
}
