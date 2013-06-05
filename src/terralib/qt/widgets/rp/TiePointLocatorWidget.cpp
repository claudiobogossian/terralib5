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
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Coord2D.h"
#include "../../../geometry/GTFactory.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "TiePointLocatorWidget.h"
#include "ui_TiePointLocatorWidgetForm.h"

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

  m_ui->m_selectAllToolButton->setIcon(QIcon::fromTheme("table-select"));
  m_ui->m_unselectAllToolButton->setIcon(QIcon::fromTheme("table-unselect"));
  m_ui->m_deleteSelectedToolButton->setIcon(QIcon::fromTheme("table-delete-select"));
  m_ui->m_autoAcquireTiePointsToolButton->setIcon(QIcon::fromTheme("wand"));
  m_ui->m_addToolButton->setIcon(QIcon::fromTheme("list-add"));
  m_ui->m_refreshToolButton->setIcon(QIcon::fromTheme("view-refresh"));
  m_ui->m_showRefNavTtoolButton->setIcon(QIcon::fromTheme("view-map-display-ref"));
  m_ui->m_showAdjNavTtoolButton->setIcon(QIcon::fromTheme("view-map-display-adj"));

  //connects
  connect(m_ui->m_autoAcquireTiePointsToolButton, SIGNAL(clicked()), this, SLOT(onAutoAcquireTiePointsToolButtonClicked()));
  connect(m_ui->m_selectAllToolButton, SIGNAL(clicked()), this, SLOT(onSelectAllToolButtonClicked()));
  connect(m_ui->m_unselectAllToolButton, SIGNAL(clicked()), this, SLOT(onUnselectAllToolButtonClicked()));
  connect(m_ui->m_deleteSelectedToolButton, SIGNAL(clicked()), this, SLOT(onDeleteSelectedToolButtonClicked()));
  connect(m_ui->m_addToolButton, SIGNAL(clicked()), this, SLOT(onAddToolButtonClicked()));
  connect(m_ui->m_refreshToolButton, SIGNAL(clicked()), this, SLOT(onRefreshToolButtonClicked()));
  connect(m_ui->m_tiePointsTableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onTiePointsTableWidgetItemSelectionChanged()));

  startAdvancedOptions();
}

te::qt::widgets::TiePointLocatorWidget::~TiePointLocatorWidget()
{
}

Ui::TiePointLocatorWidgetForm* te::qt::widgets::TiePointLocatorWidget::getForm() const
{
  return m_ui.get();
}

void te::qt::widgets::TiePointLocatorWidget::getTiePoints( std::vector< te::gm::GTParameters::TiePoint >& tiePoints ) const
{
  tiePoints.clear();

  te::qt::widgets::TiePointData::TPContainerT::const_iterator itB = m_tiePoints.begin();

  const te::qt::widgets::TiePointData::TPContainerT::const_iterator itE = m_tiePoints.end();

  tiePoints.reserve( m_tiePoints.size() );

  while( itB != itE )
  {
    tiePoints.push_back( itB->second.m_tiePoint );
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
  
  //list bands
  te::da::DataSet* ds = m_refLayer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      m_ui->m_referenceBand1ComboBox->clear();

      for(unsigned band1Idx = 0; band1Idx < inputRst->getNumberOfBands(); ++band1Idx)
        m_ui->m_referenceBand1ComboBox->addItem(QString::number(band1Idx));
    }
  }

  delete ds;
}

void te::qt::widgets::TiePointLocatorWidget::setAdjustLayer(te::map::AbstractLayerPtr layer)
{
  m_adjLayer = layer;

  //list bands
  te::da::DataSet* ds = m_adjLayer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      m_ui->m_referenceBand2ComboBox->clear();

      for(unsigned band2Idx = 0; band2Idx < inputRst->getNumberOfBands(); ++band2Idx)
        m_ui->m_referenceBand2ComboBox->addItem(QString::number(band2Idx));
    }
  }

  delete ds;
}

void te::qt::widgets::TiePointLocatorWidget::refCoordPicked(double x, double y)
{
  assert(m_refLayer.get());

  //get input raster
  te::da::DataSet* ds = m_refLayer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
    {
      m_currentTiePoint.first = inputRst->getGrid()->geoToGrid(x, y);

      m_tiePointHasFirstCoord = true;
    }
  }

  delete ds;
}

void te::qt::widgets::TiePointLocatorWidget::adjCoordPicked(double x, double y)
{
  assert(m_adjLayer.get());

  //get input raster
  te::da::DataSet* ds = m_adjLayer->getData();

  if(ds)
  {
    std::size_t rpos = te::da::GetFirstPropertyPos(ds, te::dt::RASTER_TYPE);
    te::rst::Raster* inputRst = ds->getRaster(rpos);

    if(inputRst)
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

  delete ds;
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

void te::qt::widgets::TiePointLocatorWidget::onAutoAcquireTiePointsToolButtonClicked()
{

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

  m_ui->m_gaussianFilterIterationsLineEdit->setText(QString::number(m_inputParameters.m_moravecGaussianFilterIterations));

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

  m_inputParameters.m_moravecGaussianFilterIterations = m_ui->m_gaussianFilterIterationsLineEdit->text().toUInt();

  m_inputParameters.m_moravecMinAbsCorrelation = m_ui->m_minAbsCorrelationLineEdit->text().toDouble();

  m_inputParameters.m_moravecWindowWidth = m_ui->m_moravecWindowWidthLineEdit->text().toUInt();

  m_inputParameters.m_surfMaxNormEuclideanDist = m_ui->m_maxNormEuclideanDistLineEdit->text().toDouble();

  m_inputParameters.m_surfOctavesNumber = m_ui->m_octavesNumberLineEdit->text().toUInt();

  m_inputParameters.m_surfScalesNumber = m_ui->m_scalesNumberLineEdit->text().toUInt();
}
