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
#include "../../../../common/progress/ProgressManager.h"
#include "../../../../common/progress/TaskProgress.h"
#include "../../../../common/STLUtils.h"
#include "../../../../common/StringUtils.h"
#include "../../../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../../../maptools/DataSetLayer.h"
#include "../../../../dataaccess/dataset/DataSetAdapter.h"
#include "../../../../dataaccess/dataset/DataSetTypeConverter.h"
#include "../../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../../dataaccess/utils/Utils.h"
#include "../../../../gdal/Utils.h"
#include "../../../../geometry/GeometryProperty.h"
#include "../../../../qt/widgets/utils/ScopedCursor.h"
#include "GeoPackageBuilderDialog.h"
#include "ui_GeoPackageBuilderDialogForm.h"

// Qt
#include <QFileDialog>
#include <QListWidgetItem>
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
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
}

te::qt::plugins::terramobile::GeoPackageBuilderDialog::~GeoPackageBuilderDialog()
{

}

void te::qt::plugins::terramobile::GeoPackageBuilderDialog::exportToGPKG(te::map::AbstractLayerPtr layer, te::da::DataSource* dsGPKG)
{
  int srid = layer->getSRID();
  std::auto_ptr<te::da::DataSetType> dsType = layer->getSchema();

  //Checking if the layer contains a raster property
  if (dsType->hasRaster())
  {
    te::map::DataSetLayer* dsLayer = dynamic_cast<te::map::DataSetLayer*>(layer.get());

    // Gets the connection info
    const std::string& id = dsLayer->getDataSourceId();
    te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
    std::string connInfo = info->getConnInfoAsString();

    if (boost::filesystem::is_directory(connInfo))
      connInfo += ("/" + dsLayer->getDataSetName());

    te::gdal::copyToGeopackage(connInfo, m_ui->m_geopackageFileNameLineEdit->text().toStdString());
  }
  else
  {
    te::da::DataSetTypeConverter* converter = new te::da::DataSetTypeConverter(dsType.get(), dsGPKG->getCapabilities(), dsGPKG->getEncoding());

    te::da::DataSetType* dsTypeResult = converter->getResult();

    dsTypeResult->setName(dsType->getName());

    // Check dataset name
    if (!dsGPKG->isDataSetNameValid(dsTypeResult->getName()))
    {
      int r = QMessageBox::question(this, tr("GeoPackage Builder"), tr("Layer name invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if (r == QMessageBox::Yes)
      {
        bool aux;
        std::string newName = te::common::ReplaceSpecialChars(dsTypeResult->getName(), aux);
        dsTypeResult->setName(newName);
      }
      else
      {
        throw te::common::Exception(tr("Layer name invalid for output datasource!").toStdString());
      }
    }

    // Check properties names
    std::vector<te::dt::Property* > props = dsTypeResult->getProperties();
    std::map<std::size_t, std::string> invalidNames;
    for (std::size_t i = 0; i < props.size(); ++i)
    {
      if (!dsGPKG->isPropertyNameValid(props[i]->getName()))
      {
        invalidNames[i] = props[i]->getName();
      }
    }

    if (!invalidNames.empty())
    {
      int r = QMessageBox::question(this, tr("GeoPackage Builder"), tr("Some property name is invalid for output datasource. Would you like to normalize the name?"), QMessageBox::Yes, QMessageBox::No);

      if (r == QMessageBox::Yes)
      {
        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while (it != invalidNames.end())
        {
          bool aux;
          std::string newName = te::common::ReplaceSpecialChars(it->second, aux);

          props[it->first]->setName(newName);

          ++it;
        }
      }
      else
      {
        QString err(tr("Some property name is invalid for output datasource:\n\n"));

        std::map<std::size_t, std::string>::iterator it = invalidNames.begin();

        while (it != invalidNames.end())
        {
          err.append(" - ");
          err.append(it->second.c_str());

          ++it;
        }

        throw te::common::Exception(err.toStdString());
      }
    }

    te::qt::widgets::ScopedCursor c(Qt::WaitCursor);

    //exporting
    std::map<std::string, std::string> nopt;

    std::auto_ptr<te::da::DataSet> dataset = layer->getData();

    std::auto_ptr<te::da::DataSetAdapter> dsAdapter(te::da::CreateAdapter(dataset.get(), converter));

    if (dataset->moveBeforeFirst())
      te::da::Create(dsGPKG, dsTypeResult, dsAdapter.get());
  }
}

std::vector<te::map::AbstractLayerPtr> te::qt::plugins::terramobile::GeoPackageBuilderDialog::getGatheringLayers()
{
    std::vector<te::map::AbstractLayerPtr>  gatheringLayers;

    //Acquiring the gathering layers
    QList<QListWidgetItem*> gatheringItems = m_ui->m_pointLayerListWidget->selectedItems();
    QList<QListWidgetItem*>::iterator gtItB = gatheringItems.begin();
    QList<QListWidgetItem*>::iterator gtItE = gatheringItems.end();

    while (gtItB != gtItE)
    {
        te::map::AbstractLayerPtr layer = qvariant_cast<te::map::AbstractLayerPtr>((*gtItB)->data(Qt::UserRole));
        gatheringLayers.push_back(layer);
        gtItB++;
    }

    return gatheringLayers;
}

std::vector<te::map::AbstractLayerPtr> te::qt::plugins::terramobile::GeoPackageBuilderDialog::getOverlayLayers()
{
  std::vector<te::map::AbstractLayerPtr>  overlayLayers;

  //Acquiring the overlay layers
  QList<QListWidgetItem*> overlayItems = m_ui->m_vecLayerListWidget->selectedItems();
  QList<QListWidgetItem*>::iterator ovItB = overlayItems.begin();
  QList<QListWidgetItem*>::iterator ovItE = overlayItems.end();

  while (ovItB != ovItE)
  {
      te::map::AbstractLayerPtr layer = qvariant_cast<te::map::AbstractLayerPtr>((*ovItB)->data(Qt::UserRole));
      overlayLayers.push_back(layer);
      ovItB++;
  }

  return overlayLayers;
}

std::vector<te::map::AbstractLayerPtr> te::qt::plugins::terramobile::GeoPackageBuilderDialog::getRasterLayers()
{
  std::vector<te::map::AbstractLayerPtr> rasterLayers;

  //Acquiring the raster layers
  QList<QListWidgetItem*> rasterItems = m_ui->m_rasterLayerListWidget->selectedItems();
  QList<QListWidgetItem*>::iterator rstItB = rasterItems.begin();
  QList<QListWidgetItem*>::iterator rstItE = rasterItems.end();

  while (rstItB != rstItE)
  {
      te::map::AbstractLayerPtr layer = qvariant_cast<te::map::AbstractLayerPtr>((*rstItB)->data(Qt::UserRole));
      rasterLayers.push_back(layer);
      rstItB++;
  }

  return rasterLayers;
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

void te::qt::plugins::terramobile::GeoPackageBuilderDialog::onOkPushButtonClicked()
{
  std::string gpkgName = m_ui->m_geopackageFileNameLineEdit->text().toStdString();

  if (gpkgName.empty())
  {
    QMessageBox::warning(this, tr("GeoPackage Builder"), "The name for the output geopackage can not be empty.");
    return;
  }

  te::gdal::createGeopackage(gpkgName);

  //create data source
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = gpkgName;

  std::auto_ptr<te::da::DataSource> dsGPKG = te::da::DataSourceFactory::make("OGR");
  dsGPKG->setConnectionInfo(connInfo);
  dsGPKG->open();

  std::vector<te::map::AbstractLayerPtr> rasterLayers = getRasterLayers();
  std::vector<te::map::AbstractLayerPtr> overlayLayers = getOverlayLayers();
  std::vector<te::map::AbstractLayerPtr> gatheringLayers = getGatheringLayers();

  size_t rstSize, ovSize, gtSize;
  rstSize = rasterLayers.size();
  ovSize = overlayLayers.size();
  gtSize = gatheringLayers.size();

  for (size_t i = 0; i < rstSize; ++i)
  {
    exportToGPKG(rasterLayers[i], dsGPKG.get());
  }

  for (size_t i = 0; i < ovSize; ++i)
  {
    exportToGPKG(overlayLayers[i], dsGPKG.get());
  }

  for (size_t i = 0; i < gtSize; ++i)
  {
    exportToGPKG(gatheringLayers[i], dsGPKG.get());
  }

  if (m_ui->m_visibleAreaCheckBox->isChecked())
  {
    std::string query = "create table if not exists VISIBLE_AREA (LLX real not null, LLY real not null, URX real not null, URY real not null);";
    te::gdal::queryGeopackage(gpkgName, query);
  }

  std::string query = "create table if not exists TM_STYLE (LAYER_NAME text primary key not null, SLD_XML text);";
  te::gdal::queryGeopackage(gpkgName, query);

  this->accept();
}