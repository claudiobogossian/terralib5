/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file QToolbarWindowOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "QToolbarWindowOutside.h"
#include "LayoutContext.h"
#include "LayoutScene.h"
#include "QLayoutScene.h"
#include "LayoutOutsideModelObservable.h"
#include "LayoutItemObserver.h"
#include "LayoutOutsideObserver.h"
#include "LayoutOutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "LayoutMode.h"
#include "LayoutConfig.h" /*Layout Images*/

// Qt
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>

te::layout::QToolbarWindowOutside::QToolbarWindowOutside( LayoutOutsideController* controller, LayoutObservable* o ) :
	QDockWidget("", 0, 0),
	LayoutOutsideObserver(controller, o)
{
	te::gm::Envelope box = m_model->getBox();	

	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  setMinimumSize(200, 10);

  createToolbar();
  setWidget(m_toolbar);
}

te::layout::QToolbarWindowOutside::~QToolbarWindowOutside()
{

}

void te::layout::QToolbarWindowOutside::updateObserver( ContextLayoutItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::QToolbarWindowOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::QToolbarWindowOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::QToolbarWindowOutside::createToolbar()
{
  m_toolbar = new QToolBar;

  QToolButton *btnPan = new QToolButton;
  btnPan->setText("Pan");
  btnPan->setGeometry(0,0,10,10);
  btnPan->setCheckable(true);
  btnPan->setToolTip("Visualization Area: Pan");
  connect(btnPan, SIGNAL(toggled(bool)), this, SLOT(onClickPanTool(bool)));
  btnPan->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-paper-pan"));

  QToolButton *btnZoomIn = new QToolButton;
  btnZoomIn->setText("Zoom In");
  btnZoomIn->setGeometry(0,0,10,10);
  btnZoomIn->setCheckable(true);
  btnZoomIn->setToolTip("Visualization Area: Zoom In");
  connect(btnZoomIn, SIGNAL(toggled(bool)), this, SLOT(onClickZoomInTool(bool)));
  btnZoomIn->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-paper-zoom-in"));

  QToolButton *btnZoomOut = new QToolButton;
  btnZoomOut->setText("Zoom Out");
  btnZoomOut->setGeometry(0,0,10,10);
  btnZoomOut->setCheckable(true);
  btnZoomOut->setToolTip("Visualization Area: Zoom Out");
  connect(btnZoomOut, SIGNAL(toggled(bool)), this, SLOT(onClickZoomOutTool(bool)));
  btnZoomOut->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-paper-zoom-out"));

  QToolButton *btnMap = new QToolButton;
  btnMap->setText("Map");
  btnMap->setGeometry(0,0,10,10);
  btnMap->setCheckable(true);
  btnMap->setToolTip("Default Map Object");
  connect(btnMap, SIGNAL(toggled(bool)), this, SLOT(onClickMapTool(bool)));
  btnMap->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-default-map"));

  /*QComboBox *cmbUnitsMetrics = new QComboBox;
  cmbUnitsMetrics->insertItem(TPMillimeter, QIcon(), QString("Millimeter"));
  cmbUnitsMetrics->insertItem(TPInch, QIcon(), QString("Inch"));
  connect(cmbUnitsMetrics, SIGNAL(currentIndexChanged(int)), this, SLOT(onIndexChanged(int)));*/

  QToolButton* btnRectangle = new QToolButton;
  btnRectangle->setText("Rectangle");
  btnRectangle->setGeometry(0,0,10,10);
  btnRectangle->setCheckable(true);
  btnRectangle->setToolTip("Rectangle Object");
  connect(btnRectangle, SIGNAL(toggled(bool)), this, SLOT(onClickRectangleTool(bool)));
  btnRectangle->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-square"));  

  QToolButton* btnSalveProps = new QToolButton;
  btnSalveProps->setText("Save Props");
  btnSalveProps->setGeometry(0,0,10,10);
  btnSalveProps->setCheckable(true);
  connect(btnSalveProps, SIGNAL(toggled(bool)), this, SLOT(onClickSalvePropsTool(bool)));

  m_toolbar->addWidget(btnPan);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnZoomIn);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnZoomOut);
  m_toolbar->addSeparator();
  /*_toolbar->addWidget(cmbUnitsMetrics);
  _toolbar->addSeparator();*/
  m_toolbar->addWidget(btnMap);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnRectangle);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnSalveProps);
  m_toolbar->addSeparator();
}

void te::layout::QToolbarWindowOutside::onClickPanTool(bool toggled)
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
     LayoutContext::getInstance()->setMode(TypePan);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onClickZoomInTool(bool toggled)
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeZoomIn);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onClickZoomOutTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeZoomIn);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onIndexChanged(int index)
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();
  LayoutContext::getInstance()->setMode(TypeUnitsMetricsChange);

  if(TPInch == index)
    LayoutContext::getInstance()->setUnitMetric(TPInch);
  if(TPMillimeter == index)
    LayoutContext::getInstance()->setUnitMetric(TPMillimeter);

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onClickMapTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeCreateMap);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onClickRectangleTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeCreateRectangle);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::QToolbarWindowOutside::onClickSalvePropsTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = LayoutContext::getInstance()->getMode();

  if(toggled == false)
  {
    LayoutContext::getInstance()->setMode(TypeNone);
  }
  else
  {
    LayoutContext::getInstance()->setMode(TypeSavePropsJSON);
  }

  if(mode == LayoutContext::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}


