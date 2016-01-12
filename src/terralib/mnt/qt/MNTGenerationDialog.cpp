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
\file terralib/mnt/qt/MNTGenerationDialog.cpp

\brief A dialog for Retangular Grid generation
*/

//terralib
#include "../../common/Exception.h"
#include "../../common/progress/ProgressManager.h"
#include "../../common/Translator.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSourceInfoManager.h"
#include "../../dataaccess/datasource/DataSourceManager.h"
#include "../../dataaccess/utils/Utils.h"
#include "../../geometry/GeometryProperty.h"
#include "../../maptools/DataSetLayer.h"
#include "../../mnt/core/CalculateGrid.h"
#include "../../mnt/core/TINCalculateGrid.h"
#include "../../qt/widgets/datasource/selector/DataSourceSelectorDialog.h"
#include "../../qt/widgets/progress/ProgressViewerDialog.h"
#include "../../qt/widgets/rp/Utils.h"
#include "../../raster.h"
#include "../../raster/Interpolator.h"
#include "../../raster/RasterFactory.h"

#include "MNTGenerationDialog.h"
#include "ui_MNTGenerationDialogForm.h"

// Qt
#include <QFileDialog>
#include <QMessageBox>

// BOOST
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

te::mnt::MNTGenerationDialog::MNTGenerationDialog(QWidget* parent , Qt::WindowFlags f)
  : QDialog(parent, f),
  m_ui(new Ui::MNTGenerationDialogForm),
  m_layers(std::list<te::map::AbstractLayerPtr>())
{
  // add controls
  m_ui->setupUi(this);

  //signals
  connect(m_ui->m_layersComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onInputComboBoxChanged(int)));
  
  connect(m_ui->m_dummycheckBox, SIGNAL(toggled(bool)), m_ui->m_dummylineEdit, SLOT(setEnabled(bool)));

  connect(m_ui->m_interpolatorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(oninterpolatorComboBoxChanged(int)));

  m_ui->m_dimLLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_dimCLineEdit->setValidator(new QIntValidator(this));
  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(0, 100, 4, this));

  connect(m_ui->m_resXLineEdit, SIGNAL(editingFinished()), this, SLOT(onResXLineEditEditingFinished()));
  connect(m_ui->m_resYLineEdit, SIGNAL(editingFinished()), this, SLOT(onResYLineEditEditingFinished()));
  connect(m_ui->m_dimCLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimCLineEditEditingFinished()));
  connect(m_ui->m_dimLLineEdit, SIGNAL(editingFinished()), this, SLOT(onDimLLineEditEditingFinished()));

  connect(m_ui->m_targetDatasourceToolButton, SIGNAL(pressed()), this, SLOT(onTargetDatasourceToolButtonPressed()));
  connect(m_ui->m_targetFileToolButton, SIGNAL(pressed()), this, SLOT(onTargetFileToolButtonPressed()));

  connect(m_ui->m_helpPushButton, SIGNAL(clicked()), this, SLOT(onHelpPushButtonClicked()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));

  for (int i = 2; i < 10; i++)
    m_ui->m_powerComboBox->addItem(QString::number(i));

  //Default Values
  m_ui->m_tensionLineEdit->setText("40");
  m_ui->m_smothLineEdit->setText("0.1");
  m_ui->m_minPtsMitLineEdit->setText("50");

}

te::mnt::MNTGenerationDialog::~MNTGenerationDialog()
{
}

void te::mnt::MNTGenerationDialog::setLayers(std::list<te::map::AbstractLayerPtr> layers)
{
  m_layers = layers;

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();

  while (it != m_layers.end())
  {
    if (it->get())
    {
      if (it->get()->isValid())
        m_ui->m_layersComboBox->addItem(QString(it->get()->getTitle().c_str()), QVariant(it->get()->getId().c_str()));
    }
    ++it;
  }
}

te::map::AbstractLayerPtr te::mnt::MNTGenerationDialog::getLayer()
{
  return m_outputLayer;
}

void te::mnt::MNTGenerationDialog::onInputComboBoxChanged(int index)
{
  m_ui->m_interpolatorComboBox->clear();
  m_inputLayer = 0;

  m_ui->m_ZcomboBox->clear();
  m_ui->m_ZcomboBox->hide();
  m_ui->m_Zlabel->hide();

  std::list<te::map::AbstractLayerPtr>::iterator it = m_layers.begin();
  std::string layerID = m_ui->m_layersComboBox->itemData(index, Qt::UserRole).toString().toStdString();
  while (it != m_layers.end())
  {
    if (layerID == it->get()->getId().c_str())
    {
      m_inputLayer = it->get();
      std::auto_ptr<te::da::DataSetType> dsType = m_inputLayer->getSchema();
      if (dsType->hasGeom())
      {
        std::auto_ptr<te::gm::GeometryProperty>geomProp(te::da::GetFirstGeomProperty(dsType.get()));

        m_ui->m_inputstackedWidget->setCurrentIndex(1);

        te::gm::GeomType gmType = geomProp->getGeometryType();
        if (gmType == te::gm::PointType || gmType == te::gm::MultiPointType ||
          gmType == te::gm::PointZType || gmType == te::gm::MultiPointZType ||
          gmType == te::gm::PointMType || gmType == te::gm::MultiPointMType ||
          gmType == te::gm::PointZMType || gmType == te::gm::MultiPointZMType ||
          gmType == te::gm::LineStringType || gmType == te::gm::MultiLineStringType ||
          gmType == te::gm::LineStringZType || gmType == te::gm::MultiLineStringZType ||
          gmType == te::gm::LineStringMType || gmType == te::gm::MultiLineStringMType ||
          gmType == te::gm::LineStringZMType || gmType == te::gm::MultiLineStringZMType) //samples
        {
          m_inputType = SAMPLE;
          m_ui->m_interpolatorComboBox->addItem("Weighted Avg./Z Value/Quadrant");
          m_ui->m_interpolatorComboBox->addItem("Weighted Average/Quadrant");
          m_ui->m_interpolatorComboBox->addItem("Weighted Average");
          m_ui->m_interpolatorComboBox->addItem("Simple Average");
          m_ui->m_interpolatorComboBox->addItem("Nearest Neighbor");
          m_ui->m_interpolatorComboBox->addItem("Bilinear Spline");
          m_ui->m_interpolatorComboBox->addItem("Bicubic Spline");
         // m_ui->m_interpolatorComboBox->addItem("Mitasova Spline");

          m_ui->m_ZcomboBox->show();
          m_ui->m_Zlabel->show();

          std::vector<te::dt::Property*> props = dsType->getProperties();
          for (std::size_t i = 0; i < props.size(); ++i)
          {
            switch (props[i]->getType())
            {
            case te::dt::FLOAT_TYPE:
            case te::dt::DOUBLE_TYPE:
            case te::dt::INT16_TYPE:
            case te::dt::INT32_TYPE:
            case te::dt::INT64_TYPE:
            case te::dt::UINT16_TYPE:
            case te::dt::UINT32_TYPE:
            case te::dt::UINT64_TYPE:
            case te::dt::NUMERIC_TYPE:
              m_ui->m_ZcomboBox->addItem(QString(props[i]->getName().c_str()), QVariant(props[i]->getName().c_str()));
              break;
            }
          }

        }
        if (gmType == te::gm::TINType || gmType == te::gm::MultiPolygonType || gmType == te::gm::PolyhedralSurfaceType ||
          gmType == te::gm::TINZType || gmType == te::gm::MultiPolygonZType || gmType == te::gm::PolyhedralSurfaceZType)//TIN
        {
          m_inputType = TIN;
          std::auto_ptr<te::da::DataSet> dataquery;
          te::da::DataSourcePtr ds = te::da::GetDataSource(m_inputLayer->getDataSourceId());
          std::string qry("Select type1, type2, type3 from ");
          qry += m_inputLayer->getTitle();
          qry += " where (type1 > 3 and type1 < 7) or (type2 > 3 and type2 < 7) or (type3 > 3 and type3 < 7)";
          dataquery = ds->query(qry);
          m_ui->m_interpolatorComboBox->addItem("Linear");
          m_ui->m_interpolatorComboBox->addItem("Quintic without breaklines");
          if (!dataquery->isEmpty())
            m_ui->m_interpolatorComboBox->addItem("Quintic with breaklines");
        }
      }
      if (dsType->hasRaster()) //GRID
      {
        m_ui->m_inputstackedWidget->setCurrentIndex(0);
        m_inputType = GRID;
        m_ui->m_interpolatorComboBox->addItem("Bilinear");
        m_ui->m_interpolatorComboBox->addItem("Bicubic");

      }
      dsType.release();
      break;
    }
    it++;
  }
}


void te::mnt::MNTGenerationDialog::oninterpolatorComboBoxChanged(int index)
{
  m_ui->m_interparamStackedWidget->hide();

  std::string inter = m_ui->m_interpolatorComboBox->itemData(index, Qt::UserRole).toString().toStdString();

  switch (m_inputType)
  {
  case SAMPLE:
  {
    m_ui->m_interparamStackedWidget->show();
    double raio = std::sqrt(m_inputLayer->getExtent().getWidth() * m_inputLayer->getExtent().getWidth() +
      m_inputLayer->getExtent().getHeight() * m_inputLayer->getExtent().getHeight()) / 5.;
    m_ui->m_radiusLineEdit->setText(QString::number(raio, 'f', 4));

    switch (index)
    {
    case 0: //Weighted Average/Z Value/Quadrant
    case 1: //Weighted Average/Quadrant
    case 2: //Weighted Average
      m_ui->m_interparamStackedWidget->setCurrentIndex(0);
      m_ui->m_powerLabel->show();
      m_ui->m_powerComboBox->show();
      break;
    case 3: //Simple Average
    case 4: //Nearest Neighbor
      m_ui->m_interparamStackedWidget->setCurrentIndex(0);
      m_ui->m_powerLabel->hide();
      m_ui->m_powerComboBox->hide();
      break;
    case 5: //Bilinear Spline
    case 6: //Bicubic Spline
      m_ui->m_interparamStackedWidget->setCurrentIndex(1);
      break;
    case 7: //Mitasova Spline
      m_ui->m_interparamStackedWidget->setCurrentIndex(2);
      break;
    }
    break;
  }
    case TIN:
      switch (index)
      {
      case 0:
      case 1:
      case 2:
        break;
      }
      break;
    case GRID:
      switch (index)
      {
      case 0:
      case 1:
        break;
      }
  }
}

void te::mnt::MNTGenerationDialog::onResXLineEditEditingFinished()
{
  double resX = m_ui->m_resXLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxCols = (int)ceil((env.m_urx - env.m_llx) / resX);

  m_ui->m_dimCLineEdit->setText(QString::number(maxCols));
}

void te::mnt::MNTGenerationDialog::onResYLineEditEditingFinished()
{
  double resY = m_ui->m_resYLineEdit->text().toDouble();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  int maxRows = (int)ceil((env.m_ury - env.m_lly) / resY);

  m_ui->m_dimLLineEdit->setText(QString::number(maxRows));
}

void te::mnt::MNTGenerationDialog::onDimLLineEditEditingFinished()
{
  int cols = m_ui->m_dimCLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  double resX = (env.m_urx - env.m_llx) / cols;

  m_ui->m_resXLineEdit->setText(QString::number(resX));
}

void te::mnt::MNTGenerationDialog::onDimCLineEditEditingFinished()
{
  int rows = m_ui->m_dimLLineEdit->text().toInt();

  te::gm::Envelope env = m_inputLayer->getExtent();

  if (!env.isValid())
  {
    QMessageBox::warning(this, tr("DTM Generation"), tr("Invalid envelope!"));
    return;
  }

  double resY = (env.m_ury - env.m_lly) / rows;

  m_ui->m_resYLineEdit->setText(QString::number(resY));
}

void te::mnt::MNTGenerationDialog::onTargetDatasourceToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_newLayerNameLineEdit->setEnabled(true);
  te::qt::widgets::DataSourceSelectorDialog dlg(this);
  dlg.exec();

  std::list<te::da::DataSourceInfoPtr> dsPtrList = dlg.getSelecteds();

  if (dsPtrList.empty())
    return;

  std::list<te::da::DataSourceInfoPtr>::iterator it = dsPtrList.begin();

  m_ui->m_repositoryLineEdit->setText(QString(it->get()->getTitle().c_str()));

  m_outputDatasource = *it;

  m_toFile = false;
}

void te::mnt::MNTGenerationDialog::onTargetFileToolButtonPressed()
{
  m_ui->m_newLayerNameLineEdit->clear();
  m_ui->m_repositoryLineEdit->clear();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
    QString(), tr("TIFF (*.tif *.TIF);;"), 0, QFileDialog::DontConfirmOverwrite);

  if (fileName.isEmpty())
    return;

  boost::filesystem::path outfile(fileName.toStdString());
  std::string aux = outfile.leaf().string();
  m_ui->m_newLayerNameLineEdit->setText(aux.c_str());
  aux = outfile.string();
  m_ui->m_repositoryLineEdit->setText(aux.c_str());

  m_toFile = true;
  m_ui->m_newLayerNameLineEdit->setEnabled(false);
}

void te::mnt::MNTGenerationDialog::onHelpPushButtonClicked()
{
}

void te::mnt::MNTGenerationDialog::onOkPushButtonClicked()
{
  int srid;

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
    srid = indsLayer->getSRID();

    // Checking consistency of output paramenters
    if (m_ui->m_repositoryLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Select a repository for the resulting layer."));

    if (m_ui->m_newLayerNameLineEdit->text().isEmpty())
      throw te::common::Exception(TE_TR("Define a name for the resulting layer."));

    std::string outputdataset = m_ui->m_newLayerNameLineEdit->text().toStdString();
    boost::filesystem::path uri(m_ui->m_repositoryLineEdit->text().toStdString());

    if (boost::filesystem::exists(uri))
      throw te::common::Exception(TE_TR("Output file already exists. Remove it or select a new name and try again."));

    std::size_t idx = outputdataset.find(".");
    if (idx != std::string::npos)
      outputdataset = outputdataset.substr(0, idx);

    outdsinfo["URI"] = uri.string();

    double radius = m_ui->m_radiusLineEdit->text().toDouble();
    int pow = m_ui->m_powerComboBox->currentText().toInt();

    bool ok;
    double resxo = m_ui->m_resXLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define X resolution."));
    double resyo = m_ui->m_resYLineEdit->text().toDouble(&ok);
    if (!ok)
      throw te::common::Exception(TE_TR("Define Y resolution."));

    switch (m_inputType)
    {
      case SAMPLE:
      {
        int inter_i = m_ui->m_interpolatorComboBox->currentIndex();
        bool spline = false;
        switch (inter_i)
        {
        case 0://Weighted Avg./Z Value/Quadrant
          m_inter = MediaCotaQuad;
          break;
        case 1://Weighted Average/Quadrant
          m_inter = MediaQuad;
          break;
        case 2://Weighted Average;
          m_inter = MediaPonderada;
          break;
        case 3://Simple Average;
          m_inter = Media;
          break;
        case 4://Nearest Neighbor
          m_inter = Vizinho;
          break;
        case 5://Bilinear Spline
          m_inter = SplineBilinear;
          spline = true;
          break;
        case 6://Bicubic Spline
          m_inter = SplineBicubico;
          spline = true;
          break;
        case 7://Mitasova Spline
          m_inter = Mitasova;
          spline = true;
          break;
        }

        if (!spline)
        {
          te::mnt::CalculateGrid *grid = new te::mnt::CalculateGrid();

          grid->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
          grid->setOutput(outdsinfo);
          grid->setParams(m_ui->m_ZcomboBox->currentText().toStdString(), resxo, resyo, m_inter, radius, pow);
          grid->setSRID(srid);

          grid->run();

          delete grid;
        }
        else
        {
          unsigned int px = m_ui->m_sepXSpinBox->text().toUInt();
          unsigned int py = m_ui->m_sepYSpinBox->text().toUInt();
          unsigned int mp = m_ui->m_minPtsSpinBox->text().toUInt();
          double ov = m_ui->m_overDoubleSpinBox->text().toDouble();

          te::mnt::SplineInterpolationGrass *grid = new te::mnt::SplineInterpolationGrass(px, py, mp, ov);

          grid->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
          grid->setOutput(outdsinfo);
          grid->setParams(m_ui->m_ZcomboBox->currentText().toStdString(), resxo, resyo, m_inter, radius, pow);
          grid->setSRID(srid);

          grid->generateGrid();

          delete grid;

        }
        break;
      }
      case TIN:
      {
        int inter_i = m_ui->m_interpolatorComboBox->currentIndex();
        switch (inter_i)
        {
        case 0: //Linear
          m_inter = Linear;
          break;
        case 1: //Quintico
          m_inter = Quintico;
            break;
        case 2: //Quintico breakline
          m_inter = QuinticoBrkLine;
        }

        te::mnt::TINCalculateGrid *grid = new te::mnt::TINCalculateGrid();

        grid->setInput(inDataSource, inDsetName, inDataSource->getDataSetType(inDsetName));
        grid->setOutput(outdsinfo);
        grid->setParams(resxo, resyo, m_inter);
        grid->setSRID(srid);

        grid->run();

        delete grid;
        break;
      }
      case GRID:
      {
        //get input raster
        std::auto_ptr<te::da::DataSet> inds = m_inputLayer->getData();
        std::size_t rpos = te::da::GetFirstPropertyPos(inds.get(), te::dt::RASTER_TYPE);
        std::auto_ptr<te::rst::Raster> inputRst(inds->getRaster(rpos).release());

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

        int inter_i = m_ui->m_interpolatorComboBox->currentIndex();
        int inter = 0;
        switch (inter_i)
        {
        case 0: //Bilinear
          inter = te::rst::Bilinear;
          break;
        case 1: //Bicubico
          inter = te::rst::Bicubic;
          break;
        }

        te::rst::Interpolator interp(inputRst.get(), inter, dummy);
        double resxi = inputRst.get()->getResolutionX();
        double resyi = inputRst.get()->getResolutionY();
        unsigned int outputWidth = m_ui->m_dimCLineEdit->text().toUInt();
        unsigned int outputHeight = m_ui->m_dimLLineEdit->text().toUInt();
        int X1 = inputRst.get()->getExtent()->getLowerLeftX();
        int Y2 = inputRst.get()->getExtent()->getUpperRightY();
        te::gm::Coord2D ulc(X1, Y2);
        te::rst::Grid* grid = new te::rst::Grid(outputWidth, outputHeight, resxo, resyo, &ulc, inputRst.get()->getSRID());

        std::vector<te::rst::BandProperty*> bands;
        bands.push_back(new te::rst::BandProperty(0, te::dt::DOUBLE_TYPE, "DTM GRID"));
        bands[0]->m_nblocksx = 1;
        bands[0]->m_nblocksy = (int)outputHeight;
        bands[0]->m_blkw = (int)outputWidth;
        bands[0]->m_blkh = 1;
        bands[0]->m_colorInterp = te::rst::GrayIdxCInt;
       // bands[0]->m_noDataValue = dummy;

        te::common::TaskProgress task("Calculating DTM...", te::common::TaskProgress::UNDEFINED, outputHeight*outputWidth);

        // create raster
        std::auto_ptr<te::rst::Raster> outRst(te::rst::RasterFactory::make("GDAL", grid, bands, outdsinfo));
        te::rst::Raster* out = outRst.get();

        std::vector<std::complex<double> > value;
        double xi, yi, xo, yo;

        for (unsigned int l = 0; l < outputHeight; l++)
        {
          for (unsigned int c = 0; c < outputWidth; c++)
          {
            task.pulse();
            // Calculate the x and y coordinates of (l,c) corner of the output grid
            xo = (X1 + c * resxo + resxo / 2.);
            yo = (Y2 - l * resyo - resyo / 2.);

            // Calculate position of point (xs,ys) in the input grid
            xi = ((xo - (X1 + resxi / 2.)) / resxi);
            yi = (((Y2 - resyi / 2.) - yo) / resyi);
        
            interp.getValues(xi, yi, value);
            out->setValues(c, l, value);
          }
        }

        outRst.release();
        delete out;
        inputRst.release();
        inds.release();

        break;
      }
      default:
        break;
    }

    m_outputLayer = te::qt::widgets::createLayer("GDAL", outdsinfo);
  }
  catch (const std::exception& e)
  {
    QApplication::restoreOverrideCursor();
    te::common::ProgressManager::getInstance().removeViewer(id);
    QMessageBox::information(this, "DTM Generation", e.what());
    return;
  }

  QApplication::restoreOverrideCursor();
  te::common::ProgressManager::getInstance().removeViewer(id);
  accept();

}

void te::mnt::MNTGenerationDialog::onCancelPushButtonClicked()
{
  reject();
}

