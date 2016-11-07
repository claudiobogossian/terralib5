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
\file terralib/mnt/qt/ImageGenerationDialog.cpp

\brief A dialog for Image generation
*/

//terralib
#include "../../core/filesystem/FileSystem.h"
#include "../../core/logger/Logger.h"
#include "../../core/translator/Translator.h"
#include "../../common/Exception.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../maptools/Utils.h"
#include "../../mnt/core/Shadow.h"
#include "../../qt/widgets/canvas/Canvas.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/rp/RasterNavigatorWidget.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../qt/widgets/srs/SRSManagerDialog.h"
#include "../../qt/widgets/tools/ExtentAcquire.h"
#include "../../raster.h"
#include "../../raster/Interpolator.h"
#include "../../raster/RasterFactory.h"
#include "../../se/CoverageStyle.h"
#include "../../se/Utils.h"
#include "../../srs/SpatialReferenceSystemManager.h"

#include "../core/CalculateGrid.h"
#include "../core/SplineGrass.h"
#include "../core/SplineGrassMitasova.h"
#include "../core/TINCalculateGrid.h"
#include "../core/Utils.h"

#include "LayerSearchDialog.h"
#include "ImageGenerationDialog.h"
#include "ui_ImageGenerateDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


te::mnt::ImageGenerationDialog::ImageGenerationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::ImageGenerationDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>()),
  m_inputLayer(0),
  m_mapDisplay(0),
  m_canvas(0),
  m_outputLayer(0),
  m_inDataSource(0)
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onlayerSearchToolButtonClicked()));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_previewToolButton, SIGNAL(clicked()), this, SLOT(onPreviewPushButtonClicked()));

  m_ui->m_dimLLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_dimCLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));

  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_HelpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_OKPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_CancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  m_ui->m_srsToolButton->setIcon(QIcon::fromTheme("srs"));
  connect(m_ui->m_srsToolButton, SIGNAL(clicked()), this, SLOT(onSrsToolButtonClicked()));

  m_outsrid = 0;
}

te::mnt::ImageGenerationDialog::~ImageGenerationDialog()
{
}

void te::mnt::ImageGenerationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    if (it->get())
    {
      if (it->get()->isValid())
      {
        std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
        if (dsType.get())
        {
          std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
          mntType type = getMNTType(dsType.get());

          if (type == GRID)
          {
            std::auto_ptr<te::da::DataSet> inds = it->get()->getData();
            std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
            if (inds->getRaster(rpos)->getNumberOfBands() == 1)
              m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
          }

          dsType.release();
        }
      }
    }
    ++it;
  }
}

te::map::AbstractLayerPtr te::mnt::ImageGenerationDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::ImageGenerationDialog::onInputComboBoxChanged(int index)
{
  m_inputLayer = 0;
  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toUtf8().data();

  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();

      setSRID(m_inputLayer->getSRID());

      std::auto_ptr<te::da::DataSetType> dsType (m_inputLayer->getSchema());
      std::auto_ptr<te::da::DataSet> inds = m_inputLayer->getData();

      if (dsType->hasRaster())
      {
        std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());

        te::mnt::getMinMax(inputRst.get(), m_min, m_max);
        m_ui->m_dummycheckBox->setVisible(true);
        m_ui->m_dummylineEdit->setVisible(true);
        m_ui->m_dummylineEdit->setText(QString::number(inputRst->getBand(0)->getProperty()->m_noDataValue));
        m_ui->m_vminRasterLineEdit->setText(QString::number(m_min));
        m_ui->m_vmaxRasterLineEdit->setText(QString::number(m_max));
        m_ui->m_vminLineEdit->setText(QString::number(m_min));
        m_ui->m_vmaxLineEdit->setText(QString::number(m_max));
        m_ui->m_resXLineEdit->setText(QString::number(inputRst->getResolutionX()));
        m_ui->m_resYLineEdit->setText(QString::number(inputRst->getResolutionY()));
        onResXLineEditEditingFinished();
        onResYLineEditEditingFinished();
        m_dummy = inputRst->getBand(0)->getProperty()->m_noDataValue;
        inputRst.release();
      }
      dsType.release();
      break;
    }
    it++;
  }
}

void te::mnt::ImageGenerationDialog::onlayerSearchToolButtonClicked()
{
  LayerSearchDialog search(this->parentWidget());
  search.setLayers(m_layers);

  if (search.exec() != QDialog::Accepted)
  {
    return;
  }

  m_ui->m_layersComboBox->setCurrentIndex(search.getLayerIndex());
}

void te::mnt::ImageGenerationDialog::onResXLineEditEditingFinished()
{
  double resX = m_ui->m_resXLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Image Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxCols = (int)ceil((env.m_urx - env.m_llx) / resX);

  m_ui->m_dimCLineEdit->setText(QString::number(maxCols));
}

void te::mnt::ImageGenerationDialog::onResYLineEditEditingFinished()
{
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("Image Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxRows = (int)ceil((env.m_ury - env.m_lly) / resY);

  m_ui->m_dimLLineEdit->setText(QString::number(maxRows));
}

void te::mnt::ImageGenerationDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("TIFF (*.tif *.TIF);;"), 0);

  if (fileName.isEmpty())
    return;

  try
  {
    te::core::FileSystem::remove(fileName.toUtf8().data());
  }
  catch (const std::exception& e)
  {
    QMessageBox::information(this, tr("Image Generation"), e.what());
    return;
  }

  boost::filesystem::path outfile(fileName.toUtf8().data());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::mnt::ImageGenerationDialog::onHelpPushButtonClicked()
{
}

void te::mnt::ImageGenerationDialog::onOkPushButtonClicked()
{
  try
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (!m_inputLayer.get())
      throw te::common::Exception(TE_TR("Select a input layer."));

    te::map::DataSetLayer* indsLayer = dynamic_cast<te::map::DataSetLayer*>(m_inputLayer.get());
    if (!indsLayer)
      throw te::common::Exception(TE_TR("Can not execute this operation on this type of layer."));

    te::da::DataSourcePtr inDataSource = te::da::GetDataSource(indsLayer->getDataSourceId(), true);
    if (!inDataSource.get())
      throw te::common::Exception(TE_TR("The selected input data source can not be accessed."));

    std::map<std::string, std::string> outdsinfo;
    std::string inDsetName = indsLayer->getDataSetName();

    // Checking consistency of output paramenters
    if (m_ui->m_repositoryLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Select a repository for the resulting layer."));

    if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Define a name for the resulting layer."));

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toUtf8().data();
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toUtf8().data());

    if (te::core::FileSystem::exists(uri.string()))
      throw te::common::Exception(TE_TR("Output file already exists. Remove it or select a new name and try again."));

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();

    bool ok;
    if (m_ui->m_dummycheckBox->isChecked())
    {
      m_dummy = m_ui->m_dummylineEdit->text().toDouble(&ok);
      if (!ok)
        throw te::common::Exception(TE_TR("Define Dummy Value."));
    }

    double resxo = m_ui->m_resXLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define X resolution."));
    double resyo = m_ui->m_resYLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define Y resolution."));

    te::mnt::Shadow *somb = new te::mnt::Shadow();
    somb->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
    somb->setOutput(outdsinfo, "GDAL");
    somb->setSRID(m_outsrid);

    somb->setParams(m_ui->m_azimuthLineEdit->text().toDouble(), m_ui->m_elevationLineEdit->text().toDouble(), m_ui->m_reliefLineEdit->text().toDouble(),
      m_ui->m_vminRasterLineEdit->text().toDouble(), m_ui->m_vmaxRasterLineEdit->text().toDouble() - 1, m_ui->m_vminLineEdit->text().toDouble(), m_ui->m_vmaxLineEdit->text().toDouble() - 1,
      m_dummy, m_ui->m_dimCLineEdit->text().toUInt(), m_ui->m_dimLLineEdit->text().toUInt(), resxo, resyo);

    somb->run();

    delete somb;

    m_outputLayer = te::qt::widgets::createLayer("GDAL", outdsinfo);

    resetDraw();
  }
  catch (te::common::Exception& e)
  {
    QApplication::restoreOverrideCursor();
    QMessageBox::information(this, "Shadow Image Generation", e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  accept();
}

void te::mnt::ImageGenerationDialog::onCancelPushButtonClicked()
{
  resetDraw();
  reject();
}

void  te::mnt::ImageGenerationDialog::resetDraw()
{
  m_canvas->clear();
  m_mapDisplay->repaint();

  if (m_canvas)
  {
    delete m_canvas;
    m_canvas = 0;
  }
}

void te::mnt::ImageGenerationDialog::onSrsToolButtonClicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));

  if (srsDialog.exec() == QDialog::Rejected)
    return;

  int newSRID = srsDialog.getSelectedSRS().first;

  setSRID(newSRID);

}

void te::mnt::ImageGenerationDialog::setSRID(int newSRID)
{
  if (newSRID <= 0)
  {
    m_ui->m_resSRIDLabel->setText("No SRS defined");
  }
  else
  {
    std::string name = te::srs::SpatialReferenceSystemManager::getInstance().getName((unsigned int)newSRID);
    if (name.size())
      m_ui->m_resSRIDLabel->setText(name.c_str());
    else
      m_ui->m_resSRIDLabel->setText(QString("%1").arg(newSRID));
  }
  m_outsrid = newSRID;

}

void te::mnt::ImageGenerationDialog::getRaster()
{
  if (m_inputLayer)
  {
    std::unique_ptr<te::da::DataSet> ds = m_inputLayer->getData();

    if (ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
      m_previewRaster = ds->getRaster(rpos);
    }
  }
}

void te::mnt::ImageGenerationDialog::onPreviewPushButtonClicked()
{
  if (!m_previewRaster.get())
    getRaster();
  drawPreview(m_previewRaster);
}

void te::mnt::ImageGenerationDialog::drawPreview(std::unique_ptr<te::rst::Raster>& raster)
{
  if (!raster.get())
    return;

  if (raster->getNumberOfBands() > 1)
  {
    throw te::common::Exception(TE_TR("Layer isn't Regular Grid."));
  }

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(raster->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(raster->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(raster->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(raster->getNumberOfBands());

  bool ok;
  if (m_ui->m_dummycheckBox->isChecked())
  {
    m_dummy = m_ui->m_dummylineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define Dummy Value."));
  }

  double resxo = m_ui->m_resXLineEdit->text().toDouble(&ok);
  if (!ok)
    throw te::common::Exception(TE_TR("Define X resolution."));
  double resyo = m_ui->m_resYLineEdit->text().toDouble(&ok);
  if (!ok)
    throw te::common::Exception(TE_TR("Define Y resolution."));

  te::mnt::Shadow *somb = new te::mnt::Shadow();
  //somb->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
  somb->setOutput(rinfo, "MEM");
  somb->setSRID(m_outsrid);

  somb->setParams(m_ui->m_azimuthLineEdit->text().toDouble(), m_ui->m_elevationLineEdit->text().toDouble(), m_ui->m_reliefLineEdit->text().toDouble(),
    m_ui->m_vminRasterLineEdit->text().toDouble(), m_ui->m_vmaxRasterLineEdit->text().toDouble() - 1, m_ui->m_vminLineEdit->text().toDouble(), m_ui->m_vmaxLineEdit->text().toDouble() - 1,
    m_dummy, raster->getNumberOfColumns(), raster->getNumberOfRows(), resxo, resyo);

  te::rst::Raster *outrst = somb->GenerateImage(raster.get());
  
  const te::gm::Envelope& env = m_mapDisplay->getExtent();
  const te::gm::Envelope* envRst = &env;
  te::se::Style* style = te::se::CreateCoverageStyle(outrst->getNumberOfBands());
  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  assert(cs);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    // Draw raster
    te::map::DrawRaster(outrst, m_canvas, *envRst, m_mapDisplay->getSRID(), *envRst, m_outsrid, cs, m_mapDisplay->getScale());

    m_mapDisplay->repaint();
  }
  catch (te::common::Exception&)
  {

  }

  QApplication::restoreOverrideCursor();
  delete somb;
}

void te::mnt::ImageGenerationDialog::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
{
  m_mapDisplay = mapDisplay;

  const te::gm::Envelope& env = m_mapDisplay->getExtent();

  QPixmap* draft = m_mapDisplay->getDraftPixmap();
  draft->fill(Qt::transparent);

  if (m_canvas)
  {
    delete m_canvas;
    m_canvas = 0;
  }

  // Prepares the canvas
  m_canvas = new te::qt::widgets::Canvas(m_mapDisplay->width(), m_mapDisplay->height());
  m_canvas->setDevice(draft, false);
  m_canvas->setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  m_canvas->setPolygonContourWidth(2);
  m_canvas->setPolygonContourColor(te::color::RGBAColor(100, 177, 216, TE_OPAQUE));
  m_canvas->setPolygonFillColor(te::color::RGBAColor(100, 177, 216, 0));

  connect(m_mapDisplay, SIGNAL(extentChanged()), this, SLOT(onMapDisplayExtentChanged()));
}

void te::mnt::ImageGenerationDialog::onMapDisplayExtentChanged()
{
  if (m_previewRaster.get())
    drawPreview(m_previewRaster);
}

