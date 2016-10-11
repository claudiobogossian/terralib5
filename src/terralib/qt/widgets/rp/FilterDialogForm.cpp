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
  \file terralib/qt/widgets/rp/FilterDialogForm.h

  \brief This file has the FilterDialogForm class.
*/

// TerraLib
#include "../../../common/progress/ProgressManager.h"
#include "../../../common/StringUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/dataset/DataSetType.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Utils.h"
#include "../../../maptools/Utils.h"
#include "../../../raster/RasterProperty.h"
#include "../../../rp/Filter.h"
#include "../../../rp/Module.h"
#include "../../../se/CoverageStyle.h"
#include "../../../se/Utils.h"
#include "../../widgets/tools/ExtentAcquire.h"
#include "../canvas/Canvas.h"
#include "../canvas/MapDisplay.h"
#include "../progress/ProgressViewerDialog.h"
#include "FilterDialogForm.h"
#include "MaskDialog.h"
#include "Utils.h"
#include "ui_FilterDialogForm.h"

//QT
#include <QApplication>
#include <QGridLayout>
#include <QMessageBox>
#include <QCheckBox>

//Boost
#include <boost/lexical_cast.hpp>

te::qt::widgets::FilterDialogForm::FilterDialogForm(QWidget* parent)
  : QDialog(parent),
    m_ui(new Ui::FilterDialogForm)
{
//build form
  m_ui->setupUi(this);

  m_geom = 0;
  m_mapDisplay = 0;
  m_canvas = 0;

  m_ui->m_maskToolButton->setIcon(QIcon::fromTheme("mask"));
  m_ui->m_loadMaskToolButton->setIcon(QIcon::fromTheme("mask-fill"));
  m_ui->m_maskDefaultValueLineEdit->setValidator(new QDoubleValidator(this));

  //build input layer
  QGridLayout* inputLayout = new QGridLayout(m_ui->m_inputWidget);
  inputLayout->setContentsMargins(0, 0, 0, 0);
  m_inputWidget = new te::qt::widgets::InputLayerWidget(m_ui->m_inputWidget);
  inputLayout->addWidget(m_inputWidget);

  //build output parameters
  QGridLayout* outputLayout = new QGridLayout(m_ui->m_outputWidget);
  outputLayout->setContentsMargins(0, 0, 0, 0);
  m_outputWidget = new te::qt::widgets::OutputRasterWidget(m_ui->m_outputWidget);
  outputLayout->addWidget(m_outputWidget);

  m_ui->m_helpPushButton->setNameSpace("dpi.inpe.br.plugins");
  m_ui->m_helpPushButton->setPageReference("plugins/rp/rp_filter.html");

  //connects
  connect(m_ui->m_listWidget, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
  connect(m_ui->m_typeComboBox, SIGNAL(activated(int)), SLOT(onFilterTypeComboBoxActivated(int)));
  connect(m_ui->m_maskToolButton, SIGNAL(clicked()), SLOT(onMaskToolButtonClicked()));
  connect(m_ui->m_loadMaskToolButton, SIGNAL(clicked()), SLOT(onLoadMaskToolButtonClicked()));
  connect(m_ui->m_previewCheckBox, SIGNAL(toggled(bool)), SLOT(onPreviewCheckBoxToggled(bool)));
  connect(m_ui->m_newROIPushButton, SIGNAL(clicked()), this, SLOT(onNewROIPushButtonClicked()));
  connect(m_inputWidget, SIGNAL(layerChanged()), this, SLOT(layerChanged()));
  connect(m_ui->m_okPushButton, SIGNAL(clicked()), SLOT(onOkPushButtonClicked()));

  listFilterTypes();
}

te::qt::widgets::FilterDialogForm::~FilterDialogForm()
{
}

bool te::qt::widgets::FilterDialogForm::isComplete() const
{
  if(m_ui->m_listWidget->selectedItems().isEmpty())
    return false;

  return true;
}

void te::qt::widgets::FilterDialogForm::setParams()
{
  m_ui->m_listWidget->clear();

  std::auto_ptr<te::da::DataSetType> dsType = m_layer->getSchema();
  te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
  std::auto_ptr<te::da::DataSet> dsRaster = m_layer->getData();
  std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());

  for (int b = 0; b < raster->getNumberOfBands(); b++)
  {
    m_ui->m_listWidget->addItem(te::common::Convert2String(b).c_str());
  }
}

void te::qt::widgets::FilterDialogForm::setList(std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::list<te::map::AbstractLayerPtr> layersRasterList;

  std::list<te::map::AbstractLayerPtr>::iterator it = layerList.begin();
  //Filter only raster layers
  while (it != layerList.end())
  {
    std::auto_ptr<te::da::DataSetType> dsType = it->get()->getSchema();
    if (dsType->hasRaster())
      layersRasterList.push_back(it->get());
    ++it;
  }

  m_inputWidget->setLayerList(layersRasterList);

  m_layer = layersRasterList.begin()->get();

  setParams();
}

void te::qt::widgets::FilterDialogForm::setMapDisplay(te::qt::widgets::MapDisplay* mapDisplay)
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
  m_canvas = new Canvas(m_mapDisplay->width(), m_mapDisplay->height());
  m_canvas->setDevice(draft, false);
  m_canvas->setWindow(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  m_canvas->setPolygonContourWidth(2);
  m_canvas->setPolygonContourColor(te::color::RGBAColor(100, 177, 216, TE_OPAQUE));
  m_canvas->setPolygonFillColor(te::color::RGBAColor(100, 177, 216, 0));
}

te::rp::Filter::InputParameters  te::qt::widgets::FilterDialogForm::getInputParams()
{
  int idx = m_ui->m_typeComboBox->currentIndex();

  te::rp::Filter::InputParameters algoInputParams;
  algoInputParams.m_iterationsNumber = m_ui->m_iterationsSpinBox->value();
  algoInputParams.m_enableProgress = true;
  algoInputParams.m_filterType = (te::rp::Filter::InputParameters::FilterType)m_ui->m_typeComboBox->itemData(idx).toInt();

  if(algoInputParams.m_filterType == te::rp::Filter::InputParameters::UserDefinedWindowT)
  {
    algoInputParams.m_windowH = m_window.size1();
    algoInputParams.m_windowW = m_window.size2();
    algoInputParams.m_window = m_window;
  }

  int nBands = m_ui->m_listWidget->count();
  for(int i = 0; i < nBands; ++i)
  {
    if(m_ui->m_listWidget->item(i)->isSelected())
      algoInputParams.m_inRasterBands.push_back(i);
  }

  return algoInputParams;
}

void te::qt::widgets::FilterDialogForm::onFilterTypeComboBoxActivated(int index)
{
  int filterType = m_ui->m_typeComboBox->itemData(index).toInt();

  bool flag = filterType == te::rp::Filter::InputParameters::UserDefinedWindowT;

  m_ui->m_maskSizeSpinBox->setEnabled(flag);
  m_ui->m_maskToolButton->setEnabled(flag);
  m_ui->m_maskDefaultValueLineEdit->setEnabled(flag);
}

void te::qt::widgets::FilterDialogForm::onMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_ui->m_maskSizeSpinBox->value(), m_ui->m_maskSizeSpinBox->value(),
    m_ui->m_maskDefaultValueLineEdit->text().isEmpty() ? 0 : m_ui->m_maskDefaultValueLineEdit->text().toDouble());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();

    m_ui->m_loadMaskToolButton->setEnabled(true);
  }
}

void te::qt::widgets::FilterDialogForm::onLoadMaskToolButtonClicked()
{
  te::qt::widgets::MaskDialog dlg(this);

  dlg.setMaskSize(m_window);

  m_ui->m_maskSizeSpinBox->setValue(m_window.size1());

  if(dlg.exec() == QDialog::Accepted)
  {
    m_window = dlg.getMatrix();
  }
}

void te::qt::widgets::FilterDialogForm::onNewROIPushButtonClicked()
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

void te::qt::widgets::FilterDialogForm::onPreviewCheckBoxToggled(bool isChecked)
{
  if (isChecked)
  {
    m_ui->m_newROIPushButton->setEnabled(true);
    m_ui->m_newROIPushButton->setChecked(true);
  }
  else
  {
    m_ui->m_newROIPushButton->setEnabled(false);
    m_ui->m_newROIPushButton->setChecked(false);
  }

  onNewROIPushButtonClicked();
}

void te::qt::widgets::FilterDialogForm::onOkPushButtonClicked()
{
  //check output parameters
  if (m_outputWidget->getOutputRaster().empty())
  {
    QMessageBox::information(this, "Filter", "Output image is not defined.");
    return;
  }

  if (!execute())
  {
    QMessageBox::information(this, "Filter", "Error to execute the filter.");
    return;
  }

  accept();
}

void te::qt::widgets::FilterDialogForm::apply()
{
  if(m_ui->m_listWidget->selectedItems().isEmpty())
  {
    QMessageBox::warning(this, tr("Warning"), tr("No band was selected."));
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);

  //get preview raster
  te::rst::Raster* inputRst;

  //set segmenters parameters
  te::rp::Filter::InputParameters algoInputParams = getInputParams();

  algoInputParams.m_inRasterPtr = inputRst;

  te::rp::Filter::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(inputRst->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(inputRst->getNumberOfBands());

  algoOutputParams.m_rType = "MEM";
  algoOutputParams.m_rInfo = rinfo;

  //run contrast
  te::rp::Filter algorithmInstance;

  try
  {
    if(algorithmInstance.initialize(algoInputParams))
    {
      if(algorithmInstance.execute(algoOutputParams))
      {
        std::auto_ptr<te::rst::Raster> rst = algoOutputParams.m_outputRasterPtr;
      }
    }
  }
  catch(...)
  {
    QMessageBox::warning(this, tr("Warning"), tr("Filter error."));
  }

  QApplication::restoreOverrideCursor();

  //delete input raster dataset
  delete inputRst;
}

void te::qt::widgets::FilterDialogForm::layerChanged()
{
  m_layer = m_inputWidget->getInputLayer();

  setParams();
}

void te::qt::widgets::FilterDialogForm::onEnvelopeAcquired(te::gm::Envelope env)
{
  if (!env.isValid())
    return;

  if (env.getLowerLeftX() == 0 && env.getLowerLeftY() == 0 && env.getUpperRightX() == 0 && env.getUpperRightY() == 0)
    return;

  // verify if is necessary convert the raster to the given srid
  bool needRemap = false;
  if ((m_mapDisplay->getSRID() != TE_UNKNOWN_SRS) && (m_layer->getSRID() != TE_UNKNOWN_SRS) && (m_mapDisplay->getSRID() != m_layer->getSRID()))
    needRemap = true;

  te::gm::Envelope envOut = env;

  // create a SRS converter
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

  if (needRemap)
  {
    converter->setSourceSRID(m_mapDisplay->getSRID());
    converter->setTargetSRID(m_layer->getSRID());

    converter->convert(envOut.getLowerLeftX(), envOut.getLowerLeftY(), envOut.m_llx, envOut.m_lly);
    converter->convert(envOut.getUpperRightX(), envOut.getUpperRightY(), envOut.m_urx, envOut.m_ury);
  }

  if (!envOut.intersects(m_layer->getExtent()))
  {
    QMessageBox::warning(this, tr("Warning"), tr("ROI is invalid."));
    return;
  }

  m_geom = 0;

  if (env.isValid())
    m_geom = te::gm::GetGeomFromEnvelope(&env, m_mapDisplay->getSRID());

  m_canvas->draw(m_geom);

  m_mapDisplay->repaint();

  getRasterFromROI();
}

bool te::qt::widgets::FilterDialogForm::execute()
{
  //get layer
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  //run filter
  te::rp::Filter algorithmInstance;

  te::rp::Filter::InputParameters algoInputParams = getInputParams();
  algoInputParams.m_inRasterPtr = inputRst.get();

  te::rp::Filter::OutputParameters algoOutputParams;
  algoOutputParams.m_rInfo = m_outputWidget->getInfo();
  algoOutputParams.m_rType = m_outputWidget->getType();

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if (algorithmInstance.initialize(algoInputParams))
  {
    if (algorithmInstance.execute(algoOutputParams))
    {
      algoOutputParams.reset();

      //set output layer
      m_outputLayer = te::qt::widgets::createLayer(m_outputWidget->getType(),
        m_outputWidget->getInfo());

      QApplication::restoreOverrideCursor();

      te::common::ProgressManager::getInstance().removeViewer(id);

      QMessageBox::information(this, tr("Filter"), tr("Filter ended sucessfully"));
    }
    else
    {
      QApplication::restoreOverrideCursor();

      QMessageBox::critical(this, tr("Filter"), tr("Filter execution error.") +
        (" " + te::rp::Module::getLastLogStr()).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      return false;
    }
  }
  else
  {
    QApplication::restoreOverrideCursor();

    QMessageBox::critical(this, tr("Filter"), tr("Filter initialization error.") +
      (" " + te::rp::Module::getLastLogStr()).c_str());

    te::common::ProgressManager::getInstance().removeViewer(id);

    return false;
  }

  emit addLayer(m_outputLayer);

  return true;
}

void te::qt::widgets::FilterDialogForm::listBands()
{
  m_ui->m_listWidget->clear();

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_layer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);

    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      for(unsigned int i = 0; i < inputRst->getNumberOfBands(); ++i)
      {
        m_ui->m_listWidget->addItem(QString::number(i));
      }
    }
  }
}

void te::qt::widgets::FilterDialogForm::listFilterTypes()
{
  m_ui->m_typeComboBox->clear();

  m_ui->m_typeComboBox->addItem(tr("Sobel"), te::rp::Filter::InputParameters::SobelFilterT);
  m_ui->m_typeComboBox->addItem(tr("Roberts"), te::rp::Filter::InputParameters::RobertsFilterT);
  m_ui->m_typeComboBox->addItem(tr("Mean"), te::rp::Filter::InputParameters::MeanFilterT);
  m_ui->m_typeComboBox->addItem(tr("Mode"), te::rp::Filter::InputParameters::ModeFilterT);
  m_ui->m_typeComboBox->addItem(tr("Median"), te::rp::Filter::InputParameters::MedianFilterT);
  m_ui->m_typeComboBox->addItem(tr("Dilation"), te::rp::Filter::InputParameters::DilationFilterT);
  m_ui->m_typeComboBox->addItem(tr("Erosion"), te::rp::Filter::InputParameters::ErosionFilterT);
  m_ui->m_typeComboBox->addItem(tr("User Defined"), te::rp::Filter::InputParameters::UserDefinedWindowT);

  onFilterTypeComboBoxActivated(0);
}

void te::qt::widgets::FilterDialogForm::applyPreview()
{
  if (!m_rasterPreview)
    return;

  //run filter
  te::rp::Filter algorithmInstance;

  te::rp::Filter::InputParameters algoInputParams = getInputParams();
  algoInputParams.m_inRasterPtr = m_rasterPreview;

  te::rp::Filter::OutputParameters algoOutputParams;

  std::map<std::string, std::string> rinfo;
  rinfo["MEM_RASTER_NROWS"] = boost::lexical_cast<std::string>(m_rasterPreview->getNumberOfRows());
  rinfo["MEM_RASTER_NCOLS"] = boost::lexical_cast<std::string>(m_rasterPreview->getNumberOfColumns());
  rinfo["MEM_RASTER_DATATYPE"] = boost::lexical_cast<std::string>(m_rasterPreview->getBandDataType(0));
  rinfo["MEM_RASTER_NBANDS"] = boost::lexical_cast<std::string>(m_rasterPreview->getNumberOfBands());

  algoOutputParams.m_rType = "MEM";
  algoOutputParams.m_rInfo = rinfo;

  //progress
  te::qt::widgets::ProgressViewerDialog v(this);
  int id = te::common::ProgressManager::getInstance().addViewer(&v);

  QApplication::setOverrideCursor(Qt::WaitCursor);

  if (algorithmInstance.initialize(algoInputParams))
  {
    if (algorithmInstance.execute(algoOutputParams))
    {
      //set output layer
      te::dt::AbstractData* abs = algoOutputParams.m_outputRasterPtr->clone();

      te::rst::Raster* rst = static_cast<te::rst::Raster*>(abs);

      if (m_ui->m_previewCheckBox->isChecked())
        drawPreview(rst);

      QApplication::restoreOverrideCursor();

      te::common::ProgressManager::getInstance().removeViewer(id);
    }
    else
    {
      QApplication::restoreOverrideCursor();

      QMessageBox::critical(this, tr("Filter"), tr("Filter execution error.") +
        (" " + te::rp::Module::getLastLogStr()).c_str());

      te::common::ProgressManager::getInstance().removeViewer(id);

      return;
    }
  }
  else
  {
    QApplication::restoreOverrideCursor();

    QMessageBox::critical(this, tr("Filter"), tr("Filter initialization error.") +
      (" " + te::rp::Module::getLastLogStr()).c_str());

    te::common::ProgressManager::getInstance().removeViewer(id);

    return;
  }
}

void te::qt::widgets::FilterDialogForm::drawPreview(te::rst::Raster* raster)
{
  QApplication::setOverrideCursor(Qt::WaitCursor);

  const te::gm::Envelope& envRaster = *raster->getExtent();
  const te::gm::Envelope& env = m_mapDisplay->getExtent();

  te::se::Style* style = te::se::CreateCoverageStyle(raster->getNumberOfBands());

  te::se::CoverageStyle* cs = dynamic_cast<te::se::CoverageStyle*>(style);
  assert(cs);

  // Draw raster
  te::map::DrawRaster(raster, m_canvas, envRaster, raster->getSRID(), env, m_mapDisplay->getSRID(), cs, m_mapDisplay->getScale());
 
  if (!m_geom->isValid())
    return;

   m_canvas->draw(m_geom);

  m_mapDisplay->repaint();

  QApplication::restoreOverrideCursor();
}

void te::qt::widgets::FilterDialogForm::getRasterFromROI()
{
  switch (m_geom->getGeomTypeId())
  {
    case te::gm::PolygonType:
    {
      QApplication::setOverrideCursor(Qt::WaitCursor);

      te::gm::Envelope* env = new te::gm::Envelope(*m_geom->getMBR());

      // verify if is necessary convert the raster to the given srid
      bool needRemap = false;
      if ((m_mapDisplay->getSRID() != TE_UNKNOWN_SRS) && (m_layer->getSRID() != TE_UNKNOWN_SRS) && (m_mapDisplay->getSRID() != m_layer->getSRID()))
        needRemap = true;

      // create a SRS converter
      std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());

      if (needRemap)
      {
        converter->setSourceSRID(m_mapDisplay->getSRID());
        converter->setTargetSRID(m_layer->getSRID());

        converter->convert(env->getLowerLeftX(), env->getLowerLeftY(), env->m_llx, env->m_lly);
        converter->convert(env->getUpperRightX(), env->getUpperRightY(), env->m_urx, env->m_ury);
      }

      std::auto_ptr<te::da::DataSetType> dsType = m_layer->getSchema();
      te::rst::RasterProperty* rasterProp = te::da::GetFirstRasterProperty(dsType.get());
      std::auto_ptr<te::da::DataSet> dsRaster = m_layer->getData();
      std::auto_ptr<te::rst::Raster> raster = dsRaster->getRaster(rasterProp->getName());

      std::map<std::string, std::string> info;
      info["FORCE_MEM_DRIVER"] = "TRUE";
      //execute clipping
      m_rasterPreview = raster->trim(env, info);

      if (m_ui->m_previewCheckBox->isChecked())
      {
        applyPreview();
      }

      QApplication::restoreOverrideCursor();

      break;
    }
  }
}

void te::qt::widgets::FilterDialogForm::closeEvent(QCloseEvent* e)
{
  if (m_canvas)
    m_canvas->clear();

  if (m_mapDisplay)
    m_mapDisplay->repaint();

  if (m_canvas)
  {
    delete m_canvas;
    m_canvas = 0;
  }

  emit closeTool();
}
