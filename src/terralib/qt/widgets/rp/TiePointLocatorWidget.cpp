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
  \file terralib/qt/widgets/rp/TiePointLocatorWidget.cpp

  \brief This file has the TiePointLocatorWidget class.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/GTFactory.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../geometry/Point.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../../srs/Converter.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "../../widgets/srs/SRSManagerDialog.h"
#include "../../widgets/Utils.h"
#include "RasterNavigatorWidget.h"
#include "TiePointLocatorWidget.h"
#include "ui_RasterNavigatorWidgetForm.h"
#include "ui_TiePointLocatorWidgetForm.h"

// Qt
#include <QGridLayout>
#include <QMessageBox>
#include <QPixmap>

// STL
#include <memory>

#define PATTERN_SIZE 18

/* TiePointData Class*/

te::qt::widgets::TiePointData::TiePointData():
  m_acqType(InvalidAcquisitionT),
  m_selected(false)
{

}

te::qt::widgets::TiePointData::TiePointData(const TiePointData& other)
{
  operator=( other );
}

te::qt::widgets::TiePointData::~TiePointData()
{

}

const te::qt::widgets::TiePointData& te::qt::widgets::TiePointData::operator=(const TiePointData& other)
{
  m_acqType = other.m_acqType;
  m_tiePoint = other.m_tiePoint;
  m_selected = other.m_selected;
  return other;
}

/* TiePointLocatorWidget Class*/

te::qt::widgets::TiePointLocatorWidget::TiePointLocatorWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::TiePointLocatorWidgetForm),
    m_tiePointHasFirstCoord(false),
    m_tiePointIdCounter(0)
{
  m_ui->setupUi(this);

  m_ui->m_x1LineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_y1LineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_x2LineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_y2LineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_resXLineEdit->setValidator(new QDoubleValidator(this));
  m_ui->m_resYLineEdit->setValidator(new QDoubleValidator(this));

  m_ui->m_selectAllToolButton->setIcon(QIcon::fromTheme("table-select"));
  m_ui->m_unselectAllToolButton->setIcon(QIcon::fromTheme("table-unselect"));
  m_ui->m_deleteSelectedToolButton->setIcon(QIcon::fromTheme("table-delete-select"));
  m_ui->m_autoAcquireTiePointsToolButton->setIcon(QIcon::fromTheme("wand"));
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_refreshToolButton->setIcon(QIcon::fromTheme("view-refresh"));
  m_ui->m_doneToolButton->setIcon(QIcon::fromTheme("check"));

  //connects
  connect(m_ui->m_autoAcquireTiePointsToolButton, SIGNAL(clicked()), this, SLOT(onAutoAcquireTiePointsToolButtonClicked()));
  connect(m_ui->m_selectAllToolButton, SIGNAL(clicked()), this, SLOT(onSelectAllToolButtonClicked()));
  connect(m_ui->m_unselectAllToolButton, SIGNAL(clicked()), this, SLOT(onUnselectAllToolButtonClicked()));
  connect(m_ui->m_deleteSelectedToolButton, SIGNAL(clicked()), this, SLOT(onDeleteSelectedToolButtonClicked()));
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_refreshToolButton, SIGNAL(clicked()), this, SLOT(onRefreshToolButtonClicked()));
  connect(m_ui->m_doneToolButton, SIGNAL(clicked()), this, SLOT(onDoneToolButtonClicked()));
  connect(m_ui->m_tiePointsTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onTiePointsTableWidgetItemSelectionChanged()));
  connect(m_ui->m_sridPushButton, SIGNAL(clicked()), this, SLOT(onSRIDPushButtonClicked()));


// connects
  connect(this, SIGNAL(tiePointsUpdated()), this, SLOT(onTiePointsUpdated()));

//define mark selected
  te::se::Stroke* strokeSel = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillSel = te::se::CreateFill("#0000FF", "1.0");
  m_markSelected = te::se::CreateMark("circle", strokeSel, fillSel);

  m_rgbaMarkSelected = te::map::MarkRendererManager::getInstance().render(m_markSelected, PATTERN_SIZE);

  QPixmap markSelPix = getPixmap(m_rgbaMarkSelected);
  setSelectedTiePointMarkLegend(markSelPix);

//define mark unselected
  te::se::Stroke* strokeUnsel = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillUnsel = te::se::CreateFill("#000000", "1.0");
  m_markUnselected = te::se::CreateMark("cross", strokeUnsel, fillUnsel);

  m_rgbaMarkUnselected = te::map::MarkRendererManager::getInstance().render(m_markUnselected, PATTERN_SIZE);

  QPixmap markPix = getPixmap(m_rgbaMarkUnselected);
  setTiePointMarkLegend(markPix);

//define mark reference
  te::se::Stroke* strokeRef = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillRef = te::se::CreateFill("#FF0000", "1.0");
  m_markRef = te::se::CreateMark("x", strokeRef, fillRef);

  m_rgbaMarkRef = te::map::MarkRendererManager::getInstance().render(m_markRef, PATTERN_SIZE);

  QPixmap markRefPix = getPixmap(m_rgbaMarkRef);
  setReferenceTiePointMarkLegend(markRefPix);

  startAdvancedOptions();

  startUpNavigators();
}

te::qt::widgets::TiePointLocatorWidget::~TiePointLocatorWidget()
{
  te::common::Free(m_rgbaMarkSelected, PATTERN_SIZE);
  delete m_markSelected;

  te::common::Free(m_rgbaMarkUnselected, PATTERN_SIZE);
  delete m_markUnselected;

  te::common::Free(m_rgbaMarkRef, PATTERN_SIZE);
  delete m_markRef;
}

Ui::TiePointLocatorWidgetForm* te::qt::widgets::TiePointLocatorWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::TiePointLocatorWidget::getTiePoints( std::vector< te::gm::GTParameters::TiePoint >& tiePoints ) const
{
  tiePoints.clear();

  std::auto_ptr<te::da::DataSet> ds(m_refLayer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  te::qt::widgets::TiePointData::TPContainerT::const_iterator itB = m_tiePoints.begin();

  const te::qt::widgets::TiePointData::TPContainerT::const_iterator itE = m_tiePoints.end();

  tiePoints.reserve( m_tiePoints.size() );

  // create a SRS converter
  std::auto_ptr<te::srs::Converter> converter(new te::srs::Converter());
  converter->setSourceSRID(m_refLayer->getSRID());
  converter->setTargetSRID(m_ui->m_sridLineEdit->text().toInt());

  while( itB != itE )
  {
    te::gm::GTParameters::TiePoint tp;

    tp.first = itB->second.m_tiePoint.second;

    te::gm::Coord2D c = inputRst->getGrid()->gridToGeo(itB->second.m_tiePoint.first.x, itB->second.m_tiePoint.first.y);

    converter->convert(c.x, c.y, c.x, c.y);

    tp.second = c;

    tiePoints.push_back(tp);
    ++itB;
  }
}

void te::qt::widgets::TiePointLocatorWidget::getTiePointsIdxCoords( std::vector< te::gm::GTParameters::TiePoint >& tiePoints ) const
{
  tiePoints.clear();

  std::auto_ptr<te::da::DataSet> ds(m_refLayer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

  te::qt::widgets::TiePointData::TPContainerT::const_iterator itB = m_tiePoints.begin();

  const te::qt::widgets::TiePointData::TPContainerT::const_iterator itE = m_tiePoints.end();

  tiePoints.reserve( m_tiePoints.size() );

  while( itB != itE )
  {
    te::gm::GTParameters::TiePoint tp = itB->second.m_tiePoint;

    tiePoints.push_back(tp);

    ++itB;
  }
}

const te::qt::widgets::TiePointData::TPContainerT&  te::qt::widgets::TiePointLocatorWidget::getTiePointContainer()
{
  return m_tiePoints;
}

bool te::qt::widgets::TiePointLocatorWidget::getReferenceTiePointCoord(te::gm::Coord2D& coordRef)
{
  if(m_tiePointHasFirstCoord)
    coordRef = m_currentTiePoint.first;

  return m_tiePointHasFirstCoord;
}

void te::qt::widgets::TiePointLocatorWidget::setReferenceLayer(te::map::AbstractLayerPtr layer)
{
  m_refLayer = layer;

  m_refNavigator->set(layer);
  
  //list bands
  std::auto_ptr<te::da::DataSet> ds = m_refLayer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      m_ui->m_referenceBand1ComboBox->clear();

      for(unsigned band1Idx = 0; band1Idx < inputRst->getNumberOfBands(); ++band1Idx)
        m_ui->m_referenceBand1ComboBox->addItem(QString::number(band1Idx));

      
      QString strSRID;
      strSRID.setNum(m_refLayer->getSRID());
      m_ui->m_inputSRIDLineEdit->setText(strSRID);

      QString strResX;
      strResX.setNum(inputRst->getGrid()->getResolutionX());
      m_ui->m_inputResXLineEdit->setText(strResX);

      QString strResY;
      strResY.setNum(inputRst->getGrid()->getResolutionY());
      m_ui->m_inputResYLineEdit->setText(strResY);
    }
  }
}

void te::qt::widgets::TiePointLocatorWidget::setAdjustLayer(te::map::AbstractLayerPtr layer)
{
  m_adjLayer = layer;

  m_adjNavigator->set(layer);

  //list bands
  std::auto_ptr<te::da::DataSet> ds = m_adjLayer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      m_ui->m_referenceBand2ComboBox->clear();

      for(unsigned band2Idx = 0; band2Idx < inputRst->getNumberOfBands(); ++band2Idx)
        m_ui->m_referenceBand2ComboBox->addItem(QString::number(band2Idx));

      QString strSRID;
      strSRID.setNum(m_adjLayer->getSRID());
      m_ui->m_sridLineEdit->setText(strSRID);

      QString strResX;
      strResX.setNum(inputRst->getGrid()->getResolutionX());
      m_ui->m_resXLineEdit->setText(strResX);

      QString strResY;
      strResY.setNum(inputRst->getGrid()->getResolutionY());
      m_ui->m_resYLineEdit->setText(strResY);
    }
  }
}

void te::qt::widgets::TiePointLocatorWidget::getOutputSRID(int& srid)
{
  srid = m_ui->m_sridLineEdit->text().toInt();
}

void te::qt::widgets::TiePointLocatorWidget::getOutputResolution(double& resX, double& resY)
{
  resX = m_ui->m_resXLineEdit->text().toDouble();
  resY = m_ui->m_resYLineEdit->text().toDouble();
}

void te::qt::widgets::TiePointLocatorWidget::refCoordPicked(double x, double y)
{
  assert(m_refLayer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_refLayer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      m_currentTiePoint.first = inputRst->getGrid()->geoToGrid(x, y);

      m_tiePointHasFirstCoord = true;
    }
  }
}

void te::qt::widgets::TiePointLocatorWidget::adjCoordPicked(double x, double y)
{
  assert(m_adjLayer.get());

  //get input raster
  std::auto_ptr<te::da::DataSet> ds = m_adjLayer->getData();

  if(ds.get())
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds.get(), te::dt::RASTER_TYPE);
    std::auto_ptr<te::rst::Raster> inputRst = ds->getRaster(rpos);

    if(inputRst.get())
    {
      if(m_tiePointHasFirstCoord)
      {
        m_currentTiePoint.second = inputRst->getGrid()->geoToGrid(x, y);

        m_tiePointHasFirstCoord = false;

        //add tie point
        TiePointData tpD;
        tpD.m_acqType = TiePointData::ManualAcquisitionT;
        tpD.m_tiePoint = m_currentTiePoint;

        ++m_tiePointIdCounter;
        m_tiePoints[ m_tiePointIdCounter ] = tpD;

        tiePointsTableUpdate();
      }
    }
  }
}

void te::qt::widgets::TiePointLocatorWidget::setTiePointMarkLegend(QPixmap p)
{
  m_ui->m_tiePointLabel->setPixmap(p);
}

void te::qt::widgets::TiePointLocatorWidget::setSelectedTiePointMarkLegend(QPixmap p)
{
  m_ui->m_selectedTiePointLabel->setPixmap(p);
}

void te::qt::widgets::TiePointLocatorWidget::setReferenceTiePointMarkLegend(QPixmap p)
{
  m_ui->m_refTiePointLabel->setPixmap(p);
}

void te::qt::widgets::TiePointLocatorWidget::createSelection(int initialId)
{
  m_ui->m_tiePointsTableWidget->clearSelection();

  for(int i = 0; i < m_ui->m_tiePointsTableWidget->rowCount(); ++i)
  {
    QTableWidgetItem* item = m_ui->m_tiePointsTableWidget->item(i, 0);

    int curId = item->text().toInt();

    
    if(curId >= initialId)
      m_ui->m_tiePointsTableWidget->selectRow(i);
  }
}

void te::qt::widgets::TiePointLocatorWidget::onAutoAcquireTiePointsToolButtonClicked()
{
  // creating the algorithm parameters
  std::auto_ptr<te::da::DataSet> dsRef(m_refLayer->getData());
  std::size_t rpos = te::da::GetFirstPropertyPos(dsRef.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRstRef = dsRef->getRaster(rpos);

  std::auto_ptr<te::da::DataSet> dsAdj(m_adjLayer->getData());
  rpos = te::da::GetFirstPropertyPos(dsAdj.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> inputRstAdj = dsAdj->getRaster(rpos);

  te::rp::TiePointsLocator::InputParameters inputParams = m_inputParameters;
  inputParams.m_enableProgress = true;

  inputParams.m_inRaster1Ptr = inputRstRef.get();
  inputParams.m_inRaster2Ptr = inputRstAdj.get();

  te::gm::Envelope auxEnvelope1(m_refNavigator->getCurrentExtent());
  double r1LLX = 0;
  double r1LLY = 0;
  double r1URX = 0;
  double r1URY = 0;
  inputParams.m_inRaster1Ptr->getGrid()->geoToGrid(auxEnvelope1.m_llx, auxEnvelope1.m_lly, r1LLX, r1LLY);
  inputParams.m_inRaster1Ptr->getGrid()->geoToGrid(auxEnvelope1.m_urx, auxEnvelope1.m_ury, r1URX, r1URY);
  inputParams.m_raster1TargetAreaColStart = (unsigned int)std::max( 0.0, r1LLX);
  inputParams.m_raster1TargetAreaLineStart = (unsigned int)std::max( 0.0, r1URY);
  inputParams.m_raster1TargetAreaWidth = ((unsigned int)std::min((double)inputParams.m_inRaster1Ptr->getNumberOfColumns(), r1URX)) - inputParams.m_raster1TargetAreaColStart + 1;
  inputParams.m_raster1TargetAreaHeight = ((unsigned int)std::min((double)inputParams.m_inRaster1Ptr->getNumberOfRows(), r1LLY)) - inputParams.m_raster1TargetAreaColStart + 1;

  te::gm::Envelope auxEnvelope2(m_adjNavigator->getCurrentExtent());
  double r2LLX = 0;
  double r2LLY = 0;
  double r2URX = 0;
  double r2URY = 0;
  inputParams.m_inRaster2Ptr->getGrid()->geoToGrid(auxEnvelope2.m_llx, auxEnvelope2.m_lly, r2LLX, r2LLY);
  inputParams.m_inRaster2Ptr->getGrid()->geoToGrid(auxEnvelope2.m_urx, auxEnvelope2.m_ury, r2URX, r2URY);
  inputParams.m_raster2TargetAreaColStart = (unsigned int)std::max( 0.0, r2LLX);
  inputParams.m_raster2TargetAreaLineStart = (unsigned int)std::max( 0.0, r2URY);
  inputParams.m_raster2TargetAreaWidth = ((unsigned int)std::min((double)inputParams.m_inRaster2Ptr->getNumberOfColumns(), r2URX)) - inputParams.m_raster2TargetAreaColStart + 1;
  inputParams.m_raster2TargetAreaHeight = ((unsigned int)std::min((double)inputParams.m_inRaster2Ptr->getNumberOfRows(), r2LLY)) - inputParams.m_raster2TargetAreaColStart + 1;

  inputParams.m_inRaster1Bands.push_back(m_ui->m_referenceBand1ComboBox->currentText().toUInt());
  inputParams.m_inRaster2Bands.push_back(m_ui->m_referenceBand2ComboBox->currentText().toUInt());

  inputParams.m_pixelSizeXRelation = inputRstRef->getGrid()->getResolutionX() / m_ui->m_resXLineEdit->text().toDouble();
  inputParams.m_pixelSizeYRelation = inputRstRef->getGrid()->getResolutionY() / m_ui->m_resYLineEdit->text().toDouble();

  te::rp::TiePointsLocator::OutputParameters outputParams;

  // Looking for manual inserted tie-points for an initial estimate
  unsigned int manualTPNumber = 0;
  te::qt::widgets::TiePointData::TPContainerT::const_iterator itB = m_tiePoints.begin();
  const  te::qt::widgets::TiePointData::TPContainerT::const_iterator itE = m_tiePoints.end();
  double coordDiffX = 0;
  double coordDiffY = 0;

  while(itB != itE)
  {
    if(itB->second.m_acqType == TiePointData::ManualAcquisitionT)
    {
      coordDiffX = itB->second.m_tiePoint.first.x - itB->second.m_tiePoint.second.x;
      coordDiffY = itB->second.m_tiePoint.first.y - itB->second.m_tiePoint.second.y;

      inputParams.m_maxR1ToR2Offset += std::max(inputParams.m_maxR1ToR2Offset, (unsigned int)std::ceil(std::sqrt((coordDiffX * coordDiffX) +  (coordDiffY * coordDiffY))));
      ++manualTPNumber;
    }

    ++itB;
  }

  if(inputParams.m_maxR1ToR2Offset > 0)
  {
    inputParams.m_maxR1ToR2Offset /= manualTPNumber;
    inputParams.m_maxR1ToR2Offset += ((inputParams.m_maxR1ToR2Offset * 10) / 100);
  }

  // Executing the algorithm

  QApplication::setOverrideCursor(Qt::WaitCursor);

  try
  {
    te::rp::TiePointsLocator algorithmInstance;
        
    if(algorithmInstance.initialize(inputParams))
    {
      if(algorithmInstance.execute(outputParams))
      {
        const unsigned int tpsNmb = (unsigned int)outputParams.m_tiePoints.size();

        if(tpsNmb)
        {
          TiePointData auxTpData;
          auxTpData.m_acqType = TiePointData::AutomaticAcquisitionT;

          int initialId = m_tiePointIdCounter + 1;

          for(unsigned int tpIdx = 0; tpIdx < tpsNmb; ++tpIdx)
          {
            ++m_tiePointIdCounter;
            auxTpData.m_tiePoint = outputParams.m_tiePoints[ tpIdx ];
            m_tiePoints[ m_tiePointIdCounter ] = auxTpData;
          }

          tiePointsTableUpdate();

          createSelection(initialId);
        }
        else
        {
          QMessageBox::warning(this, tr("Warning"), tr("None tie points was located."));
        }
      }
    }
  }
  catch(...)
  {
    QApplication::restoreOverrideCursor();

    QMessageBox::warning(this, tr("Warning"), tr("Error locating tie points."));
  }

  QApplication::restoreOverrideCursor();
}

void te::qt::widgets::TiePointLocatorWidget::onSelectAllToolButtonClicked()
{
  m_ui->m_tiePointsTableWidget->selectAll();
}

void te::qt::widgets::TiePointLocatorWidget::onUnselectAllToolButtonClicked()
{
  m_ui->m_tiePointsTableWidget->clearSelection();
}

void te::qt::widgets::TiePointLocatorWidget::onDeleteSelectedToolButtonClicked()
{
  const int rowCount = m_ui->m_tiePointsTableWidget->rowCount();

  for( int row = 0 ; row < rowCount ; ++row )
  {
    QTableWidgetItem* itemPtr = m_ui->m_tiePointsTableWidget->item(row, 0);

    if(itemPtr->isSelected())
    {
      unsigned int tpID = itemPtr->text().toUInt();

      te::qt::widgets::TiePointData::TPContainerT::iterator deletionIt = m_tiePoints.find(tpID);

      assert(deletionIt != m_tiePoints.end());

      m_tiePoints.erase(deletionIt);
    }
  }

  if(m_tiePoints.empty())
  {
    m_tiePointIdCounter = 0;
  }

  m_tiePointsSelected.clear();

  tiePointsTableUpdate();
}

void te::qt::widgets::TiePointLocatorWidget::onAddToolButtonClicked()
{
  if((!m_ui->m_x1LineEdit->text().isEmpty()) && (!m_ui->m_y1LineEdit->text().isEmpty()) &&
     (!m_ui->m_x2LineEdit->text().isEmpty()) && (!m_ui->m_y2LineEdit->text().isEmpty()))
  {
    TiePointData tpD;
    tpD.m_acqType = TiePointData::ManualAcquisitionT;
    tpD.m_tiePoint.first.x = m_ui->m_x1LineEdit->text().toDouble();
    tpD.m_tiePoint.first.y = m_ui->m_y1LineEdit->text().toDouble();
    tpD.m_tiePoint.second.x = m_ui->m_x2LineEdit->text().toDouble();
    tpD.m_tiePoint.second.y = m_ui->m_y2LineEdit->text().toDouble();

    ++m_tiePointIdCounter;
    m_tiePoints[m_tiePointIdCounter] = tpD;

    tiePointsTableUpdate();
  }
}

void te::qt::widgets::TiePointLocatorWidget::onTiePointsTableWidgetItemSelectionChanged()
{
  transformationInfoUpdate();
}

void te::qt::widgets::TiePointLocatorWidget::onRefreshToolButtonClicked()
{
  updateAdvancedOptions();

  tiePointsTableUpdate();
}

void te::qt::widgets::TiePointLocatorWidget::onDoneToolButtonClicked()
{
  emit doneAcquiredTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onRefMapDisplayExtentChanged()
{
  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onAdjMapDisplayExtentChanged()
{
  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onRefPointPicked(double x, double y)
{
  refCoordPicked(x, y);

  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onAdjPointPicked(double x, double y)
{
  adjCoordPicked(x, y);

  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onTiePointsUpdated()
{
  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWidget::onSRIDPushButtonClicked()
{
  te::qt::widgets::SRSManagerDialog srsDialog(this);
  srsDialog.setWindowTitle(tr("Choose the SRS"));
  
  if(srsDialog.exec() == QDialog::Accepted)
  {
    std::pair<int, std::string> srid = srsDialog.getSelectedSRS();

    QString strSRID;
    strSRID.setNum(srid.first);
    m_ui->m_sridLineEdit->setText(strSRID);
  }
}

void te::qt::widgets::TiePointLocatorWidget::tiePointsTableUpdate()
{
  // building the geometric transformation
  te::gm::GTParameters transParams;

  te::qt::widgets::TiePointData::TPContainerT::const_iterator tPIt = m_tiePoints.begin();
  const te::qt::widgets::TiePointData::TPContainerT::const_iterator tPItEnd = m_tiePoints.end();

  while( tPIt != tPItEnd )
  {
    transParams.m_tiePoints.push_back(tPIt->second.m_tiePoint);
    ++tPIt;
  }

  std::string geoTransfName = m_ui->m_geomTransfNameComboBox->currentText().toStdString();

  std::auto_ptr<te::gm::GeometricTransformation> transfPtr(te::gm::GTFactory::make(geoTransfName));

  if(transfPtr.get())
  {
    if(!transfPtr->initialize(transParams))
      transfPtr.reset();
  }

  // updating the tie points table
  m_ui->m_tiePointsTableWidget->blockSignals( true );
  m_ui->m_tiePointsTableWidget->setSortingEnabled( false );

  m_ui->m_tiePointsTableWidget->setRowCount(0);

  double currTPError = 0;

  tPIt = m_tiePoints.begin();

  while( tPIt != tPItEnd )
  {
    int newrow = m_ui->m_tiePointsTableWidget->rowCount();
    m_ui->m_tiePointsTableWidget->insertRow(newrow);

    const te::gm::GTParameters::TiePoint& currTP = tPIt->second.m_tiePoint;
    currTPError = transfPtr.get() ? transfPtr->getDirectMappingError(currTP) : 0.0;

    //tie point id
    QTableWidgetItem* itemId = new QTableWidgetItem(QString::number(tPIt->first));
    itemId->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 0, itemId);
    
    //tie point current tie point error
    QTableWidgetItem* itemError = new QTableWidgetItem(QString::number(currTPError));
    itemError->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 1, itemError);

    //acquisition type
    QString type;
    if(tPIt->second.m_acqType == TiePointData::ManualAcquisitionT)
    {
      type = tr("Manual");
    }
    else
    {
      type = tr("Automatic");
    }

    QTableWidgetItem* itemType = new QTableWidgetItem(type);
    itemType->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 2, itemType);

    //ref x coord
    QTableWidgetItem* itemRefX = new QTableWidgetItem(QString::number(currTP.first.x));
    itemRefX->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 3, itemRefX);

    //ref y coord
    QTableWidgetItem* itemRefY = new QTableWidgetItem(QString::number(currTP.first.y));
    itemRefY->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 4, itemRefY);

    //adj x coord
    QTableWidgetItem* itemAdjX = new QTableWidgetItem(QString::number(currTP.second.x));
    itemAdjX->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 5, itemAdjX);

    //adj y coord
    QTableWidgetItem* itemAdjY = new QTableWidgetItem(QString::number(currTP.second.y));
    itemAdjY->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    m_ui->m_tiePointsTableWidget->setItem(newrow, 6, itemAdjY);

    //check if this item was selected
    std::set<int>::iterator it = m_tiePointsSelected.find(tPIt->first);

    if(it != m_tiePointsSelected.end())
    {
      m_ui->m_tiePointsTableWidget->selectRow(newrow);
    }

    ++tPIt;
  }

  m_ui->m_tiePointsTableWidget->setSortingEnabled(true);
  m_ui->m_tiePointsTableWidget->blockSignals( false );
  m_ui->m_tiePointsTableWidget->resizeColumnsToContents();

  transformationInfoUpdate();
}

void te::qt::widgets::TiePointLocatorWidget::transformationInfoUpdate()
{
  // creating the transformations parameters
  te::gm::GTParameters transParamsAllTP;
  te::gm::GTParameters transParamsSelectedTP;
  te::gm::GTParameters transParamsUnselectedTP;

  m_tiePointsSelected.clear();

  const int rowCount = m_ui->m_tiePointsTableWidget->rowCount();

  for(int row = 0; row < rowCount; ++row)
  {
     QTableWidgetItem* itemPtr = m_ui->m_tiePointsTableWidget->item(row, 0);

     int id = itemPtr->text().toUInt();

     te::qt::widgets::TiePointData::TPContainerT::iterator it = m_tiePoints.find(id);

     assert(it != m_tiePoints.end());

    const te::gm::GTParameters::TiePoint& tiePoint = it->second.m_tiePoint;

    if( itemPtr->isSelected() )
    {
      it->second.m_selected = true;

      transParamsSelectedTP.m_tiePoints.push_back(tiePoint);

      m_tiePointsSelected.insert(id);
    }
    else
    {
      it->second.m_selected = false;

      transParamsUnselectedTP.m_tiePoints.push_back(tiePoint);
    }

    transParamsAllTP.m_tiePoints.push_back(tiePoint);
  }

  // instantiating the transformations
  std::string geoTransfName = m_ui->m_geomTransfNameComboBox->currentText().toStdString();

  std::auto_ptr<te::gm::GeometricTransformation> transfAllTPPtr(te::gm::GTFactory::make(geoTransfName));
  if(transfAllTPPtr.get())
    if(!transfAllTPPtr->initialize(transParamsAllTP))
      transfAllTPPtr.reset();

  std::auto_ptr<te::gm::GeometricTransformation> transfSelectedTPPtr(te::gm::GTFactory::make(geoTransfName));
  if(transfSelectedTPPtr.get())
    if(!transfSelectedTPPtr->initialize(transParamsSelectedTP))
      transfSelectedTPPtr.reset();

  std::auto_ptr<te::gm::GeometricTransformation> transfUnselectedTPPtr(te::gm::GTFactory::make(geoTransfName));
  if(transfUnselectedTPPtr.get())
    if(!transfUnselectedTPPtr->initialize(transParamsUnselectedTP))
      transfUnselectedTPPtr.reset();

  // updating widgets
  m_ui->m_tiePointsNumberLineEdit->setText(QString::number(m_tiePoints.size()));

  if(transfAllTPPtr.get())
    m_ui->m_transformationRMSEAllLineEdit->setText(QString::number(transfAllTPPtr->getDirectMapRMSE()));
  else
    m_ui->m_transformationRMSEAllLineEdit->setText("N/A");

  if(transfSelectedTPPtr.get())
    m_ui->m_transformationRMSESelectedLineEdit->setText(QString::number(transfSelectedTPPtr->getDirectMapRMSE()));
  else
    m_ui->m_transformationRMSESelectedLineEdit->setText("N/A");
        
  if(transfUnselectedTPPtr.get())
    m_ui->m_transformationRMSEunselectedLineEdit->setText(QString::number(transfUnselectedTPPtr->getDirectMapRMSE()));
  else
    m_ui->m_transformationRMSEunselectedLineEdit->setText("N/A");

  //emit signal
  emit tiePointsUpdated();
}

void te::qt::widgets::TiePointLocatorWidget::startAdvancedOptions()
{
  m_ui->m_enableGeometryFilterCheckBox->setChecked(m_inputParameters.m_enableGeometryFilter);
  m_ui->m_enableMultiThreadCheckBox->setChecked(m_inputParameters.m_enableMultiThread);

  switch( m_inputParameters.m_interesPointsLocationStrategy )
  {
    case te::rp::TiePointsLocator::InputParameters::SurfStrategyT :
    {
      int idx = m_ui->m_interesPointsLocationStrategyComboBox->findText("Surf");
      m_ui->m_interesPointsLocationStrategyComboBox->setCurrentIndex(idx);
      break;
    }
    default:
    {
      int idx = m_ui->m_interesPointsLocationStrategyComboBox->findText("Moravec");
      m_ui->m_interesPointsLocationStrategyComboBox->setCurrentIndex(idx);
    }
  }

  te::gm::GTFactory::dictionary_type::const_iterator gtItB = te::gm::GTFactory::getDictionary().begin();
  const te::gm::GTFactory::dictionary_type::const_iterator gtItE = te::gm::GTFactory::getDictionary().end();

  while( gtItB != gtItE )
  {
    m_ui->m_geomTransfNameComboBox->addItem(QString(gtItB->first.c_str()));
    ++gtItB;
  }

  int idx = m_ui->m_geomTransfNameComboBox->findText(m_inputParameters.m_geomTransfName.c_str());
  m_ui->m_geomTransfNameComboBox->setCurrentIndex(idx);

  m_ui->m_geometryFilterAssuranceLineEdit->setText(QString::number(m_inputParameters.m_geometryFilterAssurance));

  m_ui->m_geomTransfMaxErrorLineEdit->setText(QString::number(m_inputParameters.m_geomTransfMaxError));

  switch(m_inputParameters.m_interpMethod)
  {
    case te::rst::Interpolator::Bilinear :
    {
      int idx = m_ui->m_interpMethodComboBox->findText("Bilinear");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
      break;
    }
    case te::rst::Interpolator::Bicubic :
    {
      int idx = m_ui->m_interpMethodComboBox->findText("Bicubic");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
      break;
    }
    default:
    {
      int idx = m_ui->m_interpMethodComboBox->findText("NearestNeighbor");
      m_ui->m_interpMethodComboBox->setCurrentIndex(idx);
    }
  }

  m_ui->m_maxTiePointsLineEdit->setText(QString::number(m_inputParameters.m_maxTiePoints));

  m_ui->m_correlationWindowWidthLineEdit->setText(QString::number(m_inputParameters.m_moravecCorrelationWindowWidth));

  m_ui->m_gaussianFilterIterationsLineEdit->setText(QString::number(m_inputParameters.m_moravecNoiseFilterIterations));

  m_ui->m_minAbsCorrelationLineEdit->setText(QString::number(m_inputParameters.m_moravecMinAbsCorrelation));

  m_ui->m_moravecWindowWidthLineEdit->setText(QString::number(m_inputParameters.m_moravecWindowWidth));

  m_ui->m_maxNormEuclideanDistLineEdit->setText(QString::number(m_inputParameters.m_surfMaxNormEuclideanDist));

  m_ui->m_octavesNumberLineEdit->setText(QString::number(m_inputParameters.m_surfOctavesNumber));

  m_ui->m_scalesNumberLineEdit->setText(QString::number(m_inputParameters.m_surfScalesNumber));
}

void te::qt::widgets::TiePointLocatorWidget::updateAdvancedOptions()
{
  m_inputParameters.m_enableGeometryFilter = m_ui->m_enableGeometryFilterCheckBox->isChecked();

  m_inputParameters.m_enableMultiThread = m_ui->m_enableMultiThreadCheckBox->isChecked();

  if(m_ui->m_interesPointsLocationStrategyComboBox->currentText() == "Surf")
    m_inputParameters.m_interesPointsLocationStrategy = te::rp::TiePointsLocator::InputParameters::SurfStrategyT;
  else
    m_inputParameters.m_interesPointsLocationStrategy = te::rp::TiePointsLocator::InputParameters::MoravecStrategyT;

  m_inputParameters.m_geomTransfName = m_ui->m_geomTransfNameComboBox->currentText().toStdString();

  m_inputParameters.m_geometryFilterAssurance = m_ui->m_geometryFilterAssuranceLineEdit->text().toDouble();

  m_inputParameters.m_geomTransfMaxError = m_ui->m_geomTransfMaxErrorLineEdit->text().toDouble();

  if(m_ui->m_interpMethodComboBox->currentText() == "Bilinear")
    m_inputParameters.m_interpMethod = te::rst::Interpolator::Bilinear;
  else if(m_ui->m_interpMethodComboBox->currentText() == "Bicubic")
    m_inputParameters.m_interpMethod = te::rst::Interpolator::Bicubic;
  else
    m_inputParameters.m_interpMethod = te::rst::Interpolator::NearestNeighbor;

  m_inputParameters.m_maxTiePoints =  m_ui->m_maxTiePointsLineEdit->text().toUInt();

  m_inputParameters.m_moravecCorrelationWindowWidth = m_ui->m_correlationWindowWidthLineEdit->text().toUInt();

  m_inputParameters.m_moravecNoiseFilterIterations = m_ui->m_gaussianFilterIterationsLineEdit->text().toUInt();

  m_inputParameters.m_moravecMinAbsCorrelation = m_ui->m_minAbsCorrelationLineEdit->text().toDouble();

  m_inputParameters.m_moravecWindowWidth = m_ui->m_moravecWindowWidthLineEdit->text().toUInt();

  m_inputParameters.m_surfMaxNormEuclideanDist = m_ui->m_maxNormEuclideanDistLineEdit->text().toDouble();

  m_inputParameters.m_surfOctavesNumber = m_ui->m_octavesNumberLineEdit->text().toUInt();

  m_inputParameters.m_surfScalesNumber = m_ui->m_scalesNumberLineEdit->text().toUInt();
}

void te::qt::widgets::TiePointLocatorWidget::startUpNavigators()
{
  //reference
  QGridLayout* layoutRef = new QGridLayout(m_ui->m_refWidget);

  m_refNavigator = new te::qt::widgets::RasterNavigatorWidget(m_ui->m_refWidget);
  m_refNavigator->setWindowTitle(tr("Reference"));
  m_refNavigator->setMinimumSize(550, 400);
  m_refNavigator->hideGeomTool(true);
  m_refNavigator->hideInfoTool(true);
  m_refNavigator->hideBoxTool(true);

  layoutRef->addWidget(m_refNavigator);
  layoutRef->setContentsMargins(0,0,0,0);

  connect(m_refNavigator, SIGNAL(mapDisplayExtentChanged()), this, SLOT(onRefMapDisplayExtentChanged()));
  connect(m_refNavigator, SIGNAL(pointPicked(double, double)), this, SLOT(onRefPointPicked(double, double)));

  //adjust
  QGridLayout* layoutAdj = new QGridLayout(m_ui->m_adjWidget);

  m_adjNavigator = new te::qt::widgets::RasterNavigatorWidget(m_ui->m_adjWidget);
  m_adjNavigator->setWindowTitle(tr("Adjust"));
  m_adjNavigator->setMinimumSize(550, 400);
  m_adjNavigator->hideGeomTool(true);
  m_adjNavigator->hideInfoTool(true);
  m_adjNavigator->hideBoxTool(true);

  layoutAdj->addWidget(m_adjNavigator);
  layoutAdj->setContentsMargins(0,0,0,0);

  connect(m_adjNavigator, SIGNAL(mapDisplayExtentChanged()), this, SLOT(onAdjMapDisplayExtentChanged()));
  connect(m_adjNavigator, SIGNAL(pointPicked(double, double)), this, SLOT(onAdjPointPicked(double, double)));
}

void te::qt::widgets::TiePointLocatorWidget::drawTiePoints()
{
  //get displays
  te::qt::widgets::MapDisplay* refDisplay = m_refNavigator->getDisplay();
  refDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
  const te::gm::Envelope& refMapExt = refDisplay->getExtent();
  te::qt::widgets::Canvas refCanvasInstance(refDisplay->getDraftPixmap());
  refCanvasInstance.setWindow(refMapExt.m_llx, refMapExt.m_lly, refMapExt.m_urx, refMapExt.m_ury);

  te::qt::widgets::MapDisplay* adjDisplay = m_adjNavigator->getDisplay();
  adjDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
  const te::gm::Envelope& adjMapExt = adjDisplay->getExtent();
  te::qt::widgets::Canvas adjCanvasInstance(adjDisplay->getDraftPixmap());
  adjCanvasInstance.setWindow(adjMapExt.m_llx, adjMapExt.m_lly, adjMapExt.m_urx, adjMapExt.m_ury);

  //get rasters
  if(!m_refLayer.get())
    return;
  std::auto_ptr<te::da::DataSet> dsRef = m_refLayer->getData();
  if(!dsRef.get())
    return;
  std::size_t rpos = te::da::GetFirstPropertyPos(dsRef.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstRef = dsRef->getRaster(rpos);
  if(!rstRef.get())
    return;

  if(!m_adjLayer.get())
    return;
  std::auto_ptr<te::da::DataSet> dsAdj = m_adjLayer->getData();
  if(!dsAdj.get())
    return;
  rpos = te::da::GetFirstPropertyPos(dsAdj.get(), te::dt::RASTER_TYPE);
  std::auto_ptr<te::rst::Raster> rstAdj = dsAdj->getRaster(rpos);
  if(!rstAdj.get())
    return;

  //get tie points
  te::qt::widgets::TiePointData::TPContainerT tpc = getTiePointContainer();

  te::qt::widgets::TiePointData::TPContainerT::const_iterator it = tpc.begin();

  while(it != tpc.end())
  {
    int id = it->first;

    te::qt::widgets::TiePointData tpd = it->second;

    refCanvasInstance.setPointColor(te::color::RGBAColor(0,0,0, TE_TRANSPARENT)); //GAMBI
    adjCanvasInstance.setPointColor(te::color::RGBAColor(0,0,0, TE_TRANSPARENT)); //GAMBI

    //configure mark
    if(tpd.m_selected)
    {
      refCanvasInstance.setPointPattern(m_rgbaMarkSelected, PATTERN_SIZE, PATTERN_SIZE);
      adjCanvasInstance.setPointPattern(m_rgbaMarkSelected, PATTERN_SIZE, PATTERN_SIZE);
    }
    else
    {
      refCanvasInstance.setPointPattern(m_rgbaMarkUnselected, PATTERN_SIZE, PATTERN_SIZE);
      adjCanvasInstance.setPointPattern(m_rgbaMarkUnselected, PATTERN_SIZE, PATTERN_SIZE);
    }

    //ref coord
    te::gm::Coord2D refCoord = it->second.m_tiePoint.first;
    te::gm::Coord2D refGeoCoord;
    rstRef->getGrid()->gridToGeo(refCoord.x, refCoord.y, refGeoCoord.x, refGeoCoord.y );

    te::gm::Point refPoint(refGeoCoord.x, refGeoCoord.y);
    refCanvasInstance.draw(&refPoint);

    //ref text
    QMatrix matrix = refCanvasInstance.getMatrix();
    QPointF pointCanvas = matrix.map(QPointF(refGeoCoord.x, refGeoCoord.y));
    pointCanvas.setX(pointCanvas.x() + 8);
    QPointF pointGeo = matrix.inverted().map(pointCanvas);
    refPoint.setX(pointGeo.x());
    refPoint.setY(pointGeo.y());
    refCanvasInstance.drawText(&refPoint, QString::number(id).toStdString());

    //adj coord
    te::gm::Coord2D adjCoord = it->second.m_tiePoint.second;
    te::gm::Coord2D adjGeoCoord;
    rstAdj->getGrid()->gridToGeo(adjCoord.x, adjCoord.y, adjGeoCoord.x, adjGeoCoord.y );

    te::gm::Point adjPoint(adjGeoCoord.x, adjGeoCoord.y);
    adjCanvasInstance.draw(&adjPoint);

    //adj text
    matrix = adjCanvasInstance.getMatrix();
    pointCanvas = matrix.map(QPointF(adjGeoCoord.x, adjGeoCoord.y));
    pointCanvas.setX(pointCanvas.x() + 8);
    pointGeo = matrix.inverted().map(pointCanvas);
    adjPoint.setX(pointGeo.x());
    adjPoint.setY(pointGeo.y());
    adjCanvasInstance.drawText(&adjPoint, QString::number(id).toStdString());

    ++it;
  }

  //draw ref coord if exist
  te::gm::Coord2D refCoord;
  if(getReferenceTiePointCoord(refCoord))
  {
    refCanvasInstance.setPointColor(te::color::RGBAColor(0,0,0, TE_TRANSPARENT)); //GAMBI
    refCanvasInstance.setPointPattern(m_rgbaMarkRef, PATTERN_SIZE, PATTERN_SIZE);

    te::gm::Coord2D refGeoCoord;
    rstRef->getGrid()->gridToGeo(refCoord.x, refCoord.y, refGeoCoord.x, refGeoCoord.y );

    te::gm::Point refPoint(refGeoCoord.x, refGeoCoord.y);
    refCanvasInstance.draw(&refPoint);
  }

  refDisplay->repaint();
  adjDisplay->repaint();
}

QPixmap te::qt::widgets::TiePointLocatorWidget::getPixmap(te::color::RGBAColor** rgba)
{
  QImage* img = te::qt::widgets::GetImage(rgba, PATTERN_SIZE, PATTERN_SIZE);

  QPixmap p = QPixmap::fromImage(*img);

  delete img;

  return p;
}
