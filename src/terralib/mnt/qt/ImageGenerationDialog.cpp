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
#include "../../common/Exception.h"
#include "../../common/progress/ProgressManager.h"
#include "../../core/logger/Logger.h"
#include "../../core/translator/Translator.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../maptools/Utils.h"
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


void getRasterElementLine(te::rst::Raster* inputRaster, int line, std::vector< std::complex<double> > &val)
{
  int ncols = inputRaster->getNumberOfColumns();
  std::vector< std::complex<double> > aux;
  for (int col = 0; col < ncols; col++)
  {
    inputRaster->getValues(col, line, aux);
    val[col] = aux[0];
  }
}


te::mnt::ImageGenerationDialog::ImageGenerationDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::ImageGenerationDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>()),
  m_inputLayer(0),
  m_mapDisplay(0),
  m_canvas(0),
  m_raster(0),
  m_geom(0),
  m_previewRaster(0), 
  m_outputLayer(0),
  m_inDataSource(0)
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));
  connect(m_ui->m_layerSearchToolButton, SIGNAL(clicked()), this, SLOT(onlayerSearchToolButtonClicked()));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_previewCheckBox, SIGNAL(toggled(bool)), this, SLOT(onPreviewCheckBoxToggled(bool)));
  connect(m_navigator.get(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onPreviewChanged()));
  connect(m_navigator.get(), SIGNAL(previewClicked()), this, SLOT(apply()));
  connect(m_ui->m_allImageRadioButton, SIGNAL(toggled(bool)), this, SLOT(onAllImageRadioButtonToggled(bool)));
  connect(m_ui->m_visibleAreaRadioButton, SIGNAL(toggled(bool)), this, SLOT(onVisibleAreaRadioButtonToggled(bool)));
  connect(m_ui->m_roiRadioButton, SIGNAL(toggled(bool)), this, SLOT(onRoiRadioButtonToggled(bool)));
  connect(m_ui->m_newROIPushButton, SIGNAL(clicked()), this, SLOT(onNewROIPushButtonClicked()));

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

  //teste
  m_ui->m_previewCheckBox->setVisible(false);
  m_ui->m_allImageRadioButton->setVisible(false);
  m_ui->m_visibleAreaRadioButton->setVisible(false);
  m_ui->m_roiRadioButton->setVisible(false);
  m_ui->m_newROIPushButton->setVisible(false);

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
    boost::filesystem::remove(fileName.toUtf8().data());
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
  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

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

    if (boost::filesystem::exists(uri))
      throw te::common::Exception(TE_TR("Output file already exists. Remove it or select a new name and try again."));

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();

    bool ok;
    double resxo = m_ui->m_resXLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define X resolution."));
    double resyo = m_ui->m_resYLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define Y resolution."));

    //get input raster
    std::auto_ptr<te::da::DataSet> inds(m_inputLayer->getData());
    std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());
    if (inputRst->getNumberOfBands() > 1)
    {
      throw te::common::Exception(TE_TR("Layer isn't Regular Grid."));
    }

    std::vector< std::complex<double> > dummy;
    if (m_ui->m_dummycheckBox->isChecked())
    {
      bool ok;
      dummy.push_back(m_ui->m_dummylineEdit->text().toDouble(&ok));
      if (!ok)
        throw te::common::Exception(TE_TR("Define Dummy Value."));
    }
    else
      dummy.push_back(inputRst.get()->getBand(0)->getProperty()->m_noDataValue);

    double resxi = inputRst.get()->getResolutionX();
    double resyi = inputRst.get()->getResolutionY();
    unsigned int outputWidth = m_ui->m_dimCLineEdit->text().toUInt();
    unsigned int outputHeight = m_ui->m_dimLLineEdit->text().toUInt();
    int X1 = inputRst.get()->getExtent()->getLowerLeftX();
    int Y2 = inputRst.get()->getExtent()->getUpperRightY();
    te::gm::Coord2D ulc(X1, Y2);
    te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, resxo, resyo, &ulc, m_outsrid);
    std::vector<te::rst::BandProperty*> bands;
    bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "Shadow Image"));
    bands[0]->m_nblocksx = 1;
    bands[0]->m_nblocksy = (int)outputHeight;
    bands[0]->m_blkw = (int)outputWidth;
    bands[0]->m_blkh = 1;
    bands[0]->m_colorInterp = te::rst::GrayIdxCInt;

    double minval = m_ui->m_vminLineEdit->text().toDouble();
    double maxval = m_ui->m_vmaxLineEdit->text().toDouble() - 1;

    double vmin = m_ui->m_vminRasterLineEdit->text().toDouble();
    double vmax = m_ui->m_vmaxRasterLineEdit->text().toDouble() - 1;

    double pi = 3.1415927;
    double teta = (90. - m_ui->m_azimuthLineEdit->text().toDouble()) * (pi / 180.);
    double phi = (m_ui->m_elevationLineEdit->text().toDouble() * pi) / (double)180;
    double exRelief = m_ui->m_reliefLineEdit->text().toDouble();

    //Set coefficients of the ilumination
    double cx = cos(teta) * cos(phi);
    double cy = sin(teta) * cos(phi);
    double cz = sin(phi);

    double ambi = minval + ((maxval - minval) * 0.2);
    double difu = maxval - ((maxval - minval) * 0.2);

    int nlines = inputRst.get()->getNumberOfRows();
    int ncols = inputRst.get()->getNumberOfColumns();

    te::common::TaskProgress task("Generating shadow image...", te::common::TaskProgress::UNDEFINED, (int)(outputHeight*outputWidth));

    // create raster
    std::auto_ptr<te::rst::Raster> outRst(te::rst::RasterFactory::make("GDAL", grid, bands, outdsinfo));
    te::rst::Raster* out = outRst.get();

    double minimum = 0;
    double maximum = 0;

    std::vector < std::complex<double> > val1(ncols);
    std::vector < std::complex<double> > val2(ncols);
    std::vector < std::complex<double> > val3(ncols);

    for (int line = 0; line < nlines-2; line++)
    {
      if (!task.isActive())
        throw te::common::Exception(TE_TR("Canceled by user"));
      task.pulse();

      if (line == 0)
      {
        getRasterElementLine(inputRst.get(), line, val1);
        getRasterElementLine(inputRst.get(), line + 1, val2);
        getRasterElementLine(inputRst.get(), line + 2, val3);
      }
      else
      {
        val1.swap(val2);
        val2.swap(val3);
        getRasterElementLine(inputRst.get(), line + 2, val3);
      }

      for (int col = 1; col < ncols - 1; col++)
      {
        double val[8];
        val[0] = val1[col - 1].real();
        val[1] = val1[col].real();
        val[2] = val1[col + 1].real();
        val[3] = val2[col - 1].real();
        val[4] = val2[col + 1].real();
        val[5] = val3[col - 1].real();
        val[6] = val3[col].real();
        val[7] = val3[col + 1].real();
        double val0 = val2[col].real();

        if ((val0 >= vmin) && (val0 <= vmax))
        {
          double dzdx, dzdy;
          if (calcLocalGradient(inputRst.get(), line, col, vmin, vmax, dzdx, dzdy, val))
          {
            dzdx = (dzdx * exRelief);
            dzdy = (dzdy * exRelief);

            double d = sqrt((dzdx * dzdx) + (dzdy * dzdy) + 1);
            double costeta = (-(dzdx * cx) - (dzdy * cy) + cz) / d;

            if (costeta < 0)
              costeta = 0;

            double value = ambi + difu * costeta;
            out->setValue(col, line, value);

            if (value > maximum)
              maximum = value;
            else if (value < minimum)
              minimum = value;
          }
          else
            out->setValue(col, line, 0);
        }
        else
          out->setValue(col, line, m_dummy);
      } //for (int col = 1; col < ncols - 1; col++)
    } //for (int line = 0; line < nlines; line++)

    val1.clear();
    val2.clear();
    val3.clear();

    outRst.release();
    delete out;
    inputRst.release();
    inds.release();

    m_outputLayer = te::qt::widgets::createLayer("GDAL", outdsinfo);
  }
  catch (te::common::Exception& e)
  {
    QApplication::restoreOverrideCursor();
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::information(this, "Shadow Image Generation", e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  te::common::ProgressManager::getInstance().removeViewer(id);
  accept();

}

void te::mnt::ImageGenerationDialog::onCancelPushButtonClicked()
{
  reject();
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

bool te::mnt::ImageGenerationDialog::calcLocalGradient(te::rst::Raster* inputRaster, int line, int col, double vmin, double vmax,
  double& dx, double& dy, double *val)
{
  dx = 0;
  dy = 0;

  double factor = 1;

  double deltaX = inputRaster->getResolutionX() * factor;
  double deltaY = inputRaster->getResolutionY() * factor;

  if (((val[1] <= vmax) && (val[1] >= vmin)) && ((val[3] <= vmax) && (val[3] >= vmin)) &&
    ((val[6] <= vmax) && (val[6] >= vmin)) && ((val[4] <= vmax) && (val[4] >= vmin)))
  {
    if (((val[0] <= vmax) && (val[0] >= vmin)) && ((val[2] <= vmax) && (val[2] >= vmin)) &&
      ((val[5] <= vmax) && (val[5] >= vmin)) && ((val[7] <= vmax) && (val[7] >= vmin)))
    {//Calculate dzdx and dzdy derivative values with neighbourhood 8
      dx = ((val[7] + (2 * val[4]) + val[2]) - (val[5] + (2 * val[3]) + val[0])) / (8 * deltaX);
      dy = ((val[7] + (2 * val[6]) + val[5]) - (val[2] + (2 * val[1]) + val[0])) / (8 * deltaY);
    }
    else
    {//Calculate dzdx and dzdy derivative values with the neighbourhood 4
      dx = (val[4] - val[3]) / (2 * deltaX);
      dy = (val[6] - val[1]) / (2 * deltaY);
    }
  }
  else
  {//neighborhood 4 using extreme neighbors
    if (((val[0] <= vmax) && (val[0] >= vmin)) && ((val[2] <= vmax) && (val[2] >= vmin)) &&
      ((val[5] <= vmax) && (val[5] >= vmin)) && ((val[7] <= vmax) && (val[7] >= vmin)))
    {
      dx = ((val[7] + val[2]) - (val[5] + val[0])) / (4 * deltaX);
      dy = ((val[7] + val[5]) - (val[2] + val[0])) / (4 * deltaY);
    }
    else
      return false;
  }

  return true;
}

void te::mnt::ImageGenerationDialog::onAllImageRadioButtonToggled(bool isChecked)
{
  if (isChecked)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    std::auto_ptr<te::da::DataSet> ds = m_inputLayer->getData();

    if (ds.get())
    {
      std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

      m_raster = ds->getRaster(rpos).release();

      //m_histogramWidget->setInputRaster(m_raster);

      //drawHistogram();

      if (m_ui->m_previewCheckBox->isChecked())
      {
        applyPreview();
      }
    }

    QApplication::restoreOverrideCursor();
  }
}

void te::mnt::ImageGenerationDialog::onNewROIPushButtonClicked()
{
  if (m_ui->m_newROIPushButton->isChecked())
  {
    te::qt::widgets::ExtentAcquire* ea = new te::qt::widgets::ExtentAcquire(m_mapDisplay, Qt::ArrowCursor);
    m_mapDisplay->setCurrentTool(ea);

    connect(ea, SIGNAL(extentAcquired(te::gm::Envelope)), this, SLOT(onEnvelopeAcquired(te::gm::Envelope)));
  }
  else
  {
    m_mapDisplay->setCurrentTool(NULL);
  }
}

void te::mnt::ImageGenerationDialog::onPreviewCheckBoxToggled(bool isChecked)
{
  if (isChecked)
  {
    applyPreview();
  }
  else
  {
    if (m_raster)
      drawPreview(m_raster);
  }
}

void te::mnt::ImageGenerationDialog::onRoiRadioButtonToggled(bool isChecked)
{
  if (isChecked)
  {
    m_ui->m_newROIPushButton->setEnabled(true);
  }
  else
  {
    m_ui->m_newROIPushButton->setEnabled(false);
  }
}

void te::mnt::ImageGenerationDialog::onVisibleAreaRadioButtonToggled(bool isChecked)
{
  if (isChecked)
  {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    te::gm::Envelope reprojectedBBOX(m_mapDisplay->getExtent());
    reprojectedBBOX.transform(m_mapDisplay->getSRID(), m_inputLayer->getSRID());

    if (!reprojectedBBOX.intersects(m_inputLayer->getExtent()))
    {
      QMessageBox::warning(this, tr("Warning"), tr("Visible area doesn't intersect with the layer."));
      return;
    }

    te::gm::Envelope ibbox = reprojectedBBOX.intersection(m_inputLayer->getExtent());

    m_geom = 0;

    if (ibbox.isValid())
      m_geom = te::gm::GetGeomFromEnvelope(&ibbox, m_inputLayer->getSRID());

    m_canvas->draw(m_geom);

    m_mapDisplay->repaint();

    getRasterFromROI();

    QApplication::restoreOverrideCursor();
  }
}

void te::mnt::ImageGenerationDialog::getRasterFromROI()
{
  switch (m_geom->getGeomTypeId())
  {
    case te::gm::PolygonType:
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
      std::auto_ptr<te::da::DataSet> dsRaster = m_inputLayer->getData();
      std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());

      std::map<std::string, std::string> info;
      info["FORCE_MEM_DRIVER"] = "TRUE";
      //execute clipping
      m_raster = raster->trim(m_geom->getMBR(), info);

      //m_histogramWidget->setInputRaster(m_raster);

      //drawHistogram();

      if (m_ui->m_previewCheckBox->isChecked())
      {
        applyPreview();
      }

      QApplication::restoreOverrideCursor();

      break;
    }
  }
}

void te::mnt::ImageGenerationDialog::applyPreview()
{
  if (!m_raster)
    return;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(m_raster->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(m_raster->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(m_raster->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(m_raster->getNumberOfBands());

  //algoOutputParams.m_createdOutRasterDSType = "MEM";
  //algoOutputParams.m_createdOutRasterInfo = rinfo;

  ////run contrast
  //te::rp::Contrast algorithmInstance;

  try
  {
 /*   if (algorithmInstance.initialize(algoInputParams))
    {
      if (algorithmInstance.execute(algoOutputParams))
      {
        te::dt::AbstractData* abs = algoOutputParams.m_outRasterPtr->clone();

        te::rst::Raster* rst = static_cast<te::rst::Raster*>(abs);

        m_previewRaster = rst;

        double min = m_previewRaster->getBand(0)->getMinValue(true).real();
        double max = m_previewRaster->getBand(0)->getMaxValue(true).real();

        m_histogramWidget->setOutputRaster(algoOutputParams.m_createdOutRasterPtr.release());*/

        if (m_ui->m_previewCheckBox->isChecked())
          drawPreview(m_previewRaster);
      //}
  //}
  }
  catch (...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Constrast error."));
    te::common::ProgressManager::getInstance().removeViewer(id);
    QApplication::restoreOverrideCursor();
  }

  te::common::ProgressManager::getInstance().removeViewer(id);

  QApplication::restoreOverrideCursor();
}


void te::mnt::ImageGenerationDialog::drawPreview(te::rst::Raster* raster)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  const te::gm::Envelope& envRaster = *raster->getExtent();
  const te::gm::Envelope& env = m_mapDisplay->getExtent();
  const te::gm::Envelope* envRst = &env;

  // Draw raster
  //te::map::DrawRaster(raster, m_canvas, *envRst, m_mapDisplay->getSRID(), *envRst, raster->getSRID(), cs, m_mapDisplay->getScale());

  if (m_ui->m_roiRadioButton->isChecked())
  {
    if (!m_geom->isValid())
      return;

    m_canvas->draw(m_geom);
  }

  m_mapDisplay->repaint();


  QApplication::restoreOverrideCursor();
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
