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
  \file terralib/qt/widgets/raster/MultiResolutionWidget.cpp

  \brief A widget to create multi resolution over a raster.
*/

// TerraLib
#include "../../../common/Translator.h"
#include "../../../dataaccess/datasource/DataSource.h"
#include "../../../dataaccess/datasource/DataSourceFactory.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../raster/Interpolator.h"
#include "../../../raster/Raster.h"
#include "../../../raster/RasterFactory.h"
#include "../Exception.h"
#include "MultiResolutionWidget.h"
#include "ui_MultiResolutionWidgetForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

Q_DECLARE_METATYPE(te::map::AbstractLayerPtr);

te::qt::widgets::MultiResolutionWidget::MultiResolutionWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::MultiResolutionWidgetForm)
{
// add controls
  m_ui->setupUi(this);

// set button icons
  m_ui->m_fileToolButton->setIcon(QIcon::fromTheme("file-raster"));
  m_ui->m_folderToolButton->setIcon(QIcon::fromTheme("folder-open"));

//connects
  connect(m_ui->m_fileToolButton, SIGNAL(clicked()), this, SLOT(onFileToolButtonClicked()));
  connect(m_ui->m_folderToolButton, SIGNAL(clicked()), this, SLOT(onFolderToolButtonClicked()));

  fillInterpolatorTypes();
 }

te::qt::widgets::MultiResolutionWidget::~MultiResolutionWidget()
{
}

Ui::MultiResolutionWidgetForm* te::qt::widgets::MultiResolutionWidget::getForm() const
{
  return m_ui.get();
}

bool te::qt::widgets::MultiResolutionWidget::createMultiResolution()
{
  if(m_ui->m_layerRadioButton->isChecked())
    return fromLayer();
  else if(m_ui->m_fileRadioButton->isChecked())
    return fromFile();
  else if(m_ui->m_folderRadioButton->isChecked())
    return fromFolder();

  return false;
}

void te::qt::widgets::MultiResolutionWidget::setLayerList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  m_ui->m_layerComboBox->clear();

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();

  while(it != layerList.end())
  {
    te::map::AbstractLayerPtr l = *it;

    std::auto_ptr<te::da::DataSetType> dsType = l->getSchema();

    if(dsType->hasRaster())
      m_ui->m_layerComboBox->addItem(l->getTitle().c_str(), QVariant::fromValue(l));

    ++it;
  }
}

bool te::qt::widgets::MultiResolutionWidget::fromLayer()
{
  if(m_ui->m_layerComboBox->currentText().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Layer not selected."));
    return false;
  }

  int idx = m_ui->m_layerComboBox->currentIndex();
  QVariant varLayer = m_ui->m_layerComboBox->itemData(idx, Qt::UserRole);
  te::map::AbstractLayerPtr layer = varLayer.value<te::map::AbstractLayerPtr>();

  if(!layer.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid layer selected."));
    return false;
  }

  std::auto_ptr<te::da::DataSet> ds = layer->getData();
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rst = ds->getRaster(rpos);

  if(!rst.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Selected layer does not have raster representation."));
    return false;
  }

  bool res = createMultiRes(rst.get());

  if(!res)
  {
      QMessageBox::warning(this, tr("Warning"), tr("Error creating multi resolution."));
  }

  QMessageBox::information(this, tr("Information"), tr("Multi resolution created."));

  return res;
}

bool te::qt::widgets::MultiResolutionWidget::fromFile()
{
  if(m_ui->m_fileLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("File not selected."));
    return false;
  }

  std::map<std::string, std::string> rinfo;
  rinfo["URI"] = m_ui->m_fileLineEdit->text().toStdString();

  std::auto_ptr<te::rst::Raster> rst(te::rst::RasterFactory::open(rinfo));

  if(!rst.get())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid file selected."));
    return false;
  }

  bool res = createMultiRes(rst.get());

  if(!res)
  {
      QMessageBox::warning(this, tr("Warning"), tr("Error creating multi resolution."));
  }

  QMessageBox::information(this, tr("Information"), tr("Multi resolution created."));

  return res;
}

bool te::qt::widgets::MultiResolutionWidget::fromFolder()
{
  if(m_ui->m_folderLineEdit->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("Folder not selected."));
    return false;
  }

  std::map<std::string, std::string> dsInfo;
  dsInfo["SOURCE"] = m_ui->m_folderLineEdit->text().toStdString();

  std::auto_ptr<te::da::DataSource> ds = te::da::DataSourceFactory::make("GDAL");

  ds->setConnectionInfo(dsInfo);

  ds->open();

  std::vector<std::string> rasterNames = ds->getDataSetNames();

  for(std::size_t t = 0; t < rasterNames.size(); ++t)
  {
    std::auto_ptr<te::da::DataSet> dataSet = ds->getDataSet(rasterNames[t]);

    if(!dataSet.get())
      continue;

    std::size_t rpos = te::da::GetFirstPropertyPos(dataSet.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> rst = dataSet->getRaster(rpos);

    if(rst.get())
    {
      bool res = createMultiRes(rst.get());

      if(!res)
      {
        std::string errorMsg = "Error creating multi resolution. File: " + rasterNames[t] + ".";
        QMessageBox::warning(this, tr("Warning"), errorMsg.c_str());
        return false;
      }
    }
  }

  QMessageBox::information(this, tr("Information"), tr("Multi resolution created."));

  return true;
}

bool te::qt::widgets::MultiResolutionWidget::createMultiRes(te::rst::Raster* raster)
{
  if(!raster || raster->getAccessPolicy() == te::common::NoAccess)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Invalid Raster."));
    return false;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    unsigned int nLevels = m_ui->m_levelsSpinBox->value();
   
    int interpolationIdx = m_ui->m_interpolatorComboBox->currentIndex();
    te::rst::Interpolator::Method interpolator = (te::rst::Interpolator::Method)m_ui->m_interpolatorComboBox->itemData(interpolationIdx).toInt();

    raster->createMultiResolution(nLevels, interpolator);
  }
  catch(...)
  {
    QApplication::restoreOverrideCursor();

    return false;
  }

  QApplication::restoreOverrideCursor();

  return true;
}

void te::qt::widgets::MultiResolutionWidget::fillInterpolatorTypes()
{
  m_ui->m_interpolatorComboBox->clear();

  m_ui->m_interpolatorComboBox->addItem(tr("Nearest Neighbor"), te::rst::NearestNeighbor);
  m_ui->m_interpolatorComboBox->addItem(tr("Bilinear"), te::rst::Bilinear);
  m_ui->m_interpolatorComboBox->addItem(tr("Bicubic"), te::rst::Bicubic);
}

void te::qt::widgets::MultiResolutionWidget::onFileToolButtonClicked()
{
  QString filter = tr("Image File (*.png *.jpg *.jpeg *.tif *.tiff *.geotif *.geotiff);; Web Map Service - WMS (*.xml *.wms);; Web Coverage Service - WCS (*.xml *.wcs);; All Files (*.*)");

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Raster File"), "", filter);

  if(fileName.isEmpty())
    return;

  m_ui->m_fileLineEdit->setText(fileName);
}

void te::qt::widgets::MultiResolutionWidget::onFolderToolButtonClicked()
{
  QString folderName = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "");

  if(folderName.isEmpty())
    return;

  m_ui->m_folderLineEdit->setText(folderName);
}
