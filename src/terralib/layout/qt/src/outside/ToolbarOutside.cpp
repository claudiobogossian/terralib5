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
  \file ToolbarOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarOutside.h"
#include "Context.h"
#include "AbstractScene.h"
#include "Scene.h"
#include "OutsideModelObservable.h"
#include "ItemObserver.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "../../../../geometry/Envelope.h"
#include "EnumMode.h"
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

te::layout::ToolbarOutside::ToolbarOutside( OutsideController* controller, Observable* o ) :
	QDockWidget("", 0, 0),
	OutsideObserver(controller, o)
{
	te::gm::Envelope box = m_model->getBox();	

	setVisible(false);
	setWindowTitle("Layout - Toolbar");
  setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
  setMinimumSize(200, 10);

  createToolbar();
  setWidget(m_toolbar);
}

te::layout::ToolbarOutside::~ToolbarOutside()
{

}

void te::layout::ToolbarOutside::updateObserver( ContextItem context )
{
	setVisible(context.getShow());
	if(context.getShow() == true)
		show();
	else
		hide();
}

void te::layout::ToolbarOutside::setPosition( const double& x, const double& y )
{
	move(x,y);
	refresh();
}

te::gm::Coord2D te::layout::ToolbarOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ToolbarOutside::createToolbar()
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
  
  createMapToolButton();

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

  QToolButton* btnLegend = new QToolButton;
  btnLegend->setText("Legend");
  btnLegend->setGeometry(0,0,10,10);
  btnLegend->setCheckable(true);
  btnLegend->setToolTip("Legend Object");
  connect(btnLegend, SIGNAL(toggled(bool)), this, SLOT(onClickLegendTool(bool)));
  btnLegend->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-default-legend"));  

  /*QToolButton* btnSalveProps = new QToolButton;
  btnSalveProps->setText("Save Props");
  btnSalveProps->setGeometry(0,0,10,10);
  btnSalveProps->setCheckable(true);
  connect(btnSalveProps, SIGNAL(toggled(bool)), this, SLOT(onClickSalvePropsTool(bool)));*/

  QToolButton *btnMapGrid = new QToolButton;
  btnMapGrid->setText("Map Grid");
  btnMapGrid->setGeometry(0,0,10,10);
  btnMapGrid->setCheckable(true);
  btnMapGrid->setToolTip("Map Grid Object");
  connect(btnMapGrid, SIGNAL(toggled(bool)), this, SLOT(onClickMapGridTool(bool)));
  btnMapGrid->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-map-grid")); 

  m_toolbar->addWidget(btnPan);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnZoomIn);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnZoomOut);
  m_toolbar->addSeparator();
  /*_toolbar->addWidget(cmbUnitsMetrics);
  _toolbar->addSeparator();*/
  m_toolbar->addWidget(btnMapGrid);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnRectangle);
  m_toolbar->addSeparator();
  m_toolbar->addWidget(btnLegend);
  m_toolbar->addSeparator();
  /*m_toolbar->addWidget(btnSalveProps);
  m_toolbar->addSeparator();*/
}

void te::layout::ToolbarOutside::createMapToolButton()
{
  QToolButton *btnMap = new QToolButton;
  btnMap->setText("Map");
  btnMap->setGeometry(0,0,10,10);
  btnMap->setCheckable(true);
  btnMap->setToolTip("Default Map Object");
  connect(btnMap, SIGNAL(toggled(bool)), this, SLOT(onClickMapTool(bool)));
  btnMap->setIcon(QIcon::fromTheme(LAYOUT_IMAGES_PNG"/layout-default-map"));

  /*QMenu *menu = new QMenu();
  QAction *testAction = new QAction("test menu item", this);
  menu->addAction(testAction);

  QToolButton* maptoolButton = new QToolButton();
  maptoolButton->setMenu(menu);
  maptoolButton->setPopupMode(QToolButton::InstantPopup);*/

  m_toolbar->addWidget(btnMap);
  m_toolbar->addSeparator();
}

void te::layout::ToolbarOutside::onClickPanTool(bool toggled)
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
     Context::getInstance()->setMode(TypePan);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickZoomInTool(bool toggled)
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeZoomIn);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickZoomOutTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeZoomIn);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onIndexChanged(int index)
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();
  Context::getInstance()->setMode(TypeUnitsMetricsChange);

  if(TPInch == index)
    Context::getInstance()->setUnitMetric(TPInch);
  if(TPMillimeter == index)
    Context::getInstance()->setUnitMetric(TPMillimeter);

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickMapTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeCreateMap);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickMapGridTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeCreateMapGrid);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickRectangleTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeCreateRectangle);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickSalvePropsTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeSavePropsJSON);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}

void te::layout::ToolbarOutside::onClickLegendTool( bool toggled )
{
  bool result = true;
  LayoutMode mode = Context::getInstance()->getMode();

  if(toggled == false)
  {
    Context::getInstance()->setMode(TypeNone);
  }
  else
  {
    Context::getInstance()->setMode(TypeCreateLegend);
  }

  if(mode == Context::getInstance()->getMode())
    result = false;

  emit changeContext(result);
}