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
  \file terralib/qt/widgets/rp/TiePointLocatorWizardPage.cpp

  \brief This file defines a class for a tie point locator Wizard page.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../dataaccess/dataset/DataSet.h"
#include "../../../dataaccess/utils/Utils.h"
#include "../../../geometry/Point.h"
#include "../../../maptools/MarkRendererManager.h"
#include "../../../raster/Grid.h"
#include "../../../raster/Raster.h"
#include "../../../se/Fill.h"
#include "../../../se/Mark.h"
#include "../../../se/Stroke.h"
#include "../../../se/Utils.h"
#include "../../widgets/canvas/Canvas.h"
#include "../../widgets/canvas/MapDisplay.h"
#include "RasterNavigatorDialog.h"
#include "RasterNavigatorWidget.h"
#include "TiePointLocatorWidget.h"
#include "TiePointLocatorWizardPage.h"
#include "ui_RasterNavigatorWidgetForm.h"
#include "ui_TiePointLocatorWidgetForm.h"

// Qt
#include <QGridLayout>

#define PATTERN_SIZE 18

te::qt::widgets::TiePointLocatorWizardPage::TiePointLocatorWizardPage(QWidget* parent)
  : QWizardPage(parent)
{
//build form
  QGridLayout* layout = new QGridLayout(this);
  m_tiePointWidget.reset( new te::qt::widgets::TiePointLocatorWidget(this));
  layout->addWidget(m_tiePointWidget.get(), 0, 0);

// connects
  connect(m_tiePointWidget->getForm()->m_showRefNavTtoolButton, SIGNAL(toggled(bool)), this, SLOT(showReferenceNavigator(bool)));
  connect(m_tiePointWidget->getForm()->m_showAdjNavTtoolButton, SIGNAL(toggled(bool)), this, SLOT(showAdjustNavigator(bool)));

//define mark selected
  te::se::Stroke* strokeSel = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillSel = te::se::CreateFill("#0000FF", "1.0");
  m_markSelected = te::se::CreateMark("circle", strokeSel, fillSel);

  m_rgbaMarkSelected = te::map::MarkRendererManager::getInstance().render(m_markSelected, PATTERN_SIZE);

//define mark unselected
  te::se::Stroke* strokeUnsel = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillUnsel = te::se::CreateFill("#000000", "1.0");
  m_markUnselected = te::se::CreateMark("cross", strokeUnsel, fillUnsel);

  m_rgbaMarkUnselected = te::map::MarkRendererManager::getInstance().render(m_markUnselected, PATTERN_SIZE);

//define mark reference
  te::se::Stroke* strokeRef = te::se::CreateStroke("#000000", "2");
  te::se::Fill* fillRef = te::se::CreateFill("#FF0000", "1.0");
  m_markRef = te::se::CreateMark("x", strokeRef, fillRef);

  m_rgbaMarkRef = te::map::MarkRendererManager::getInstance().render(m_markRef, PATTERN_SIZE);

//configure page
  this->setTitle(tr("Tie Point Locator"));
  this->setSubTitle(tr("Used to acquire a set of tie points."));

  startUpNavigators();
}

te::qt::widgets::TiePointLocatorWizardPage::~TiePointLocatorWizardPage()
{
  te::common::Free(m_rgbaMarkSelected, PATTERN_SIZE);
  delete m_markSelected;

  te::common::Free(m_rgbaMarkUnselected, PATTERN_SIZE);
  delete m_markUnselected;

  te::common::Free(m_rgbaMarkRef, PATTERN_SIZE);
  delete m_markRef;
}

bool te::qt::widgets::TiePointLocatorWizardPage::isComplete() const
{
  return true;
}

te::qt::widgets::TiePointLocatorWidget* te::qt::widgets::TiePointLocatorWizardPage::getWidget()
{
  return m_tiePointWidget.get();
}

void te::qt::widgets::TiePointLocatorWizardPage::setReferenceLayer(te::map::AbstractLayerPtr layer)
{
  m_refLayer = layer;

  m_tiePointWidget->setReferenceLayer(layer);

  m_refNavigator->set(layer);
}

void te::qt::widgets::TiePointLocatorWizardPage::setAdjustLayer(te::map::AbstractLayerPtr layer)
{
  m_adjLayer = layer;

  m_tiePointWidget->setAdjustLayer(layer);

  m_adjNavigator->set(layer);
}

void te::qt::widgets::TiePointLocatorWizardPage::showReferenceNavigator(bool show)
{
  assert(m_refNavigator);

  if(show)
    m_refNavigator->show();
  else
    m_refNavigator->hide();
}

void te::qt::widgets::TiePointLocatorWizardPage::showAdjustNavigator(bool show)
{
  assert(m_adjNavigator);

  if(show)
    m_adjNavigator->show();
  else
    m_adjNavigator->hide();
}

void te::qt::widgets::TiePointLocatorWizardPage::onReferenceNavigatorClosed()
{
  m_tiePointWidget->getForm()->m_showRefNavTtoolButton->setChecked(false);
}

void te::qt::widgets::TiePointLocatorWizardPage::onAdjustNavigatorClosed()
{
  m_tiePointWidget->getForm()->m_showAdjNavTtoolButton->setChecked(false);
}

void te::qt::widgets::TiePointLocatorWizardPage::onRefMapDisplayExtentChanged()
{
  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWizardPage::onAdjMapDisplayExtentChanged()
{
  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWizardPage::onRefPointPicked(double x, double y, te::qt::widgets::MapDisplay* map)
{
  m_tiePointWidget->refCoordPicked(x, y);

  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWizardPage::onAdjPointPicked(double x, double y, te::qt::widgets::MapDisplay* map)
{
  m_tiePointWidget->adjCoordPicked(x, y);

  drawTiePoints();
}

void te::qt::widgets::TiePointLocatorWizardPage::startUpNavigators()
{
  //reference
  m_refNavigator = new te::qt::widgets::RasterNavigatorDialog(this, Qt::Tool);
  m_refNavigator->setWindowTitle(tr("Referece"));
  m_refNavigator->setMinimumSize(400, 400);
  m_refNavigator->getWidget()->hideGeomTool(true);
  m_refNavigator->getWidget()->hideInfoTool(true);

  connect(m_refNavigator, SIGNAL(navigatorClosed()), this, SLOT(onReferenceNavigatorClosed()));
  connect(m_refNavigator->getWidget(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onRefMapDisplayExtentChanged()));
  connect(m_refNavigator->getWidget(), SIGNAL(pointPicked(double, double, te::qt::widgets::MapDisplay*)), 
    this, SLOT(onRefPointPicked(double, double, te::qt::widgets::MapDisplay*)));

  //adjust
  m_adjNavigator = new te::qt::widgets::RasterNavigatorDialog(this, Qt::Tool);
  m_adjNavigator->setWindowTitle(tr("Adjust"));
  m_adjNavigator->setMinimumSize(400, 400);
  m_adjNavigator->getWidget()->hideGeomTool(true);
  m_adjNavigator->getWidget()->hideInfoTool(true);

  connect(m_adjNavigator, SIGNAL(navigatorClosed()), this, SLOT(onAdjustNavigatorClosed()));
  connect(m_adjNavigator->getWidget(), SIGNAL(mapDisplayExtentChanged()), this, SLOT(onAdjMapDisplayExtentChanged()));
  connect(m_adjNavigator->getWidget(), SIGNAL(pointPicked(double, double, te::qt::widgets::MapDisplay*)), 
    this, SLOT(onAdjPointPicked(double, double, te::qt::widgets::MapDisplay*)));
}

void te::qt::widgets::TiePointLocatorWizardPage::drawTiePoints()
{
  //get displays
  te::qt::widgets::MapDisplay* refDisplay = m_refNavigator->getWidget()->getDisplay();
  refDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
  const te::gm::Envelope& refMapExt = refDisplay->getExtent();
  te::qt::widgets::Canvas refCanvasInstance(refDisplay->getDraftPixmap());
  refCanvasInstance.setWindow(refMapExt.m_llx, refMapExt.m_lly, refMapExt.m_urx, refMapExt.m_ury);

  te::qt::widgets::MapDisplay* adjDisplay = m_adjNavigator->getWidget()->getDisplay();
  adjDisplay->getDraftPixmap()->fill(QColor(0, 0, 0, 0));
  const te::gm::Envelope& adjMapExt = adjDisplay->getExtent();
  te::qt::widgets::Canvas adjCanvasInstance(adjDisplay->getDraftPixmap());
  adjCanvasInstance.setWindow(adjMapExt.m_llx, adjMapExt.m_lly, adjMapExt.m_urx, adjMapExt.m_ury);

  //get rasters
  if(!m_refLayer.get())
    return;
  te::da::DataSet* dsRef = m_refLayer->getData();
  if(!dsRef)
    return;
  std::size_t rpos = te::da::GetFirstPropertyPos(dsRef, te::dt::RASTER_TYPE);
  te::rst::Raster* rstRef = dsRef->getRaster(rpos);
  if(!rstRef)
    return;

  if(!m_adjLayer.get())
    return;
  te::da::DataSet* dsAdj = m_adjLayer->getData();
  if(!dsAdj)
    return;
  rpos = te::da::GetFirstPropertyPos(dsAdj, te::dt::RASTER_TYPE);
  te::rst::Raster* rstAdj = dsAdj->getRaster(rpos);
  if(!rstAdj)
    return;

  //get tie points
  te::qt::widgets::TiePointData::TPContainerT tpc = m_tiePointWidget->getTiePointContainer();

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
    refCoord.x = refCoord.x + 6;
    rstRef->getGrid()->gridToGeo(refCoord.x, refCoord.y, refGeoCoord.x, refGeoCoord.y );
    refPoint.setX(refGeoCoord.x);
    refPoint.setY(refGeoCoord.y);
    refCanvasInstance.drawText(&refPoint, QString::number(id).toStdString());

    //adj coord
    te::gm::Coord2D adjCoord = it->second.m_tiePoint.second;
    te::gm::Coord2D adjGeoCoord;
    rstAdj->getGrid()->gridToGeo(adjCoord.x, adjCoord.y, adjGeoCoord.x, adjGeoCoord.y );

    te::gm::Point adjPoint(adjGeoCoord.x, adjGeoCoord.y);
    adjCanvasInstance.draw(&adjPoint);

    //adj text
    adjCoord.x = adjCoord.x + 6;
    rstAdj->getGrid()->gridToGeo(adjCoord.x, adjCoord.y, adjGeoCoord.x, adjGeoCoord.y );
    adjPoint.setX(adjGeoCoord.x);
    adjPoint.setY(adjGeoCoord.y);
    adjCanvasInstance.drawText(&adjPoint, QString::number(id).toStdString());

    ++it;
  }

  //draw ref coord if exist
  te::gm::Coord2D refCoord;
  if(m_tiePointWidget->getReferenceTiePointCoord(refCoord))
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

  delete dsRef;
  delete dsAdj;
}
