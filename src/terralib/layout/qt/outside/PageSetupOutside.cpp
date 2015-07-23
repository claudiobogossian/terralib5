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
  \file PageSetupOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PageSetupOutside.h"
#include "ui_PageSetup.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../core/pattern/singleton/Context.h"

// STL
#include <string>
#include <sstream> 

// Qt
#include <QColorDialog> 
#include <QFrame>
#include <QLineEdit>
#include <QValidator>
#include <QComboBox>
#include <QCheckBox> 
#include <QMessageBox>
#include <QObjectList>

te::layout::PageSetupOutside::PageSetupOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_orientation(te::layout::Portrait),
  m_paperType(te::layout::A4),
  m_ui(new Ui::PageSetup)
{
  m_ui->setupUi(this);

  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint 
  | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

  init();
}

te::layout::PageSetupOutside::~PageSetupOutside()
{

}

void te::layout::PageSetupOutside::init()
{
  m_ui->lneLeft->setValidator(new  QDoubleValidator(this));
  m_ui->lneTop->setValidator(new  QDoubleValidator(this));
  m_ui->lneRight->setValidator(new  QDoubleValidator(this));
  m_ui->lneBottom->setValidator(new  QDoubleValidator(this));

  m_ui->lneCustomWidth->setValidator(new  QDoubleValidator(this));
  m_ui->lneCustomHeight->setValidator(new  QDoubleValidator(this));
}

void te::layout::PageSetupOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::PageSetupOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::PageSetupOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::PageSetupOutside::load()
{
  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();

  double w = 0;
  double h = 0;
  pConfig->getPaperSize(w, h);

  configureOrientationPage();
  configurePageSize();
}

void te::layout::PageSetupOutside::configureOrientationPage()
{
  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();
  m_orientation = pConfig->getPaperOrientantion();

  if (m_orientation == te::layout::Landscape)
  {
    m_ui->rdbLandscape->setChecked(true);
  }
  else
  {
    m_ui->rdbPortrait->setChecked(true);
  }
}

void te::layout::PageSetupOutside::configurePageSize()
{
  m_ui->cmbPageSize->clear();
  int index = 0;
  index = m_ui->cmbPageSize->findData("ISO A0 - 841 x 1189 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A0 - 841 x 1189 mm");

  index = m_ui->cmbPageSize->findData("ISO A1 - 594 x 841 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A1 - 594 x 841 mm");

  index = m_ui->cmbPageSize->findData("ISO A2 - 420 x 594 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A2 - 420 x 594 mm");

  index = m_ui->cmbPageSize->findData("ISO A3 - 297 x 420 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A3 - 297 x 420 mm");

  index = m_ui->cmbPageSize->findData("ISO A4 - 210 x 297 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A4 - 210 x 297 mm");

  index = m_ui->cmbPageSize->findData("ISO A5 - 148 x 210 mm");
  m_ui->cmbPageSize->insertItem(index,"ISO A5 - 148 x 210 mm");

  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();

  QString curItem;
  if (pConfig->getPaperType() == te::layout::A0)
    curItem = "ISO A0 - 841 x 1189 mm";
  else if (pConfig->getPaperType() == te::layout::A1)
    curItem = "ISO A1 - 594 x 841 mm";
  else if (pConfig->getPaperType() == te::layout::A2)
    curItem = "ISO A2 - 420 x 594 mm";
  else if (pConfig->getPaperType() == te::layout::A3)
    curItem = "ISO A3 - 297 x 420 mm";
  else if (pConfig->getPaperType() == te::layout::A4)
    curItem = "ISO A4 - 210 x 297 mm";
  else if (pConfig->getPaperType() == te::layout::A5)
    curItem = "ISO A5 - 148 x 210 mm";
  else
    curItem = "Custom"; 

  index = m_ui->cmbPageSize->findText(curItem);
  m_ui->cmbPageSize->setCurrentIndex(index);

  switchSize();

  if(pConfig->getPaperType() == te::layout::Custom)
  {
    m_ui->gbCustom->setEnabled(true);
  }
}

void te::layout::PageSetupOutside::switchSize()
{
  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();

  double w = 0;
  double h = 0;
  pConfig->getPaperSize(w, h);

  QVariant wv(w);
  m_ui->lneCustomWidth->setText(wv.toString());
  QVariant hv(w);
  m_ui->lneCustomHeight->setText(hv.toString());
}

void te::layout::PageSetupOutside::on_cmbPageSize_currentIndexChanged( const QString & text )
{
  std::string curItem;
  if (text.toStdString().compare("ISO A0 - 841 x 1189 mm") == 0)
    m_paperType = te::layout::A0;
  else if (text.toStdString().compare("ISO A1 - 594 x 841 mm") == 0)
    m_paperType = te::layout::A1;
  else if (text.toStdString().compare("ISO A2 - 420 x 594 mm") == 0)
    m_paperType = te::layout::A2;
  else if (text.toStdString().compare("ISO A3 - 297 x 420 mm") == 0)
    m_paperType = te::layout::A3;
  else if (text.toStdString().compare("ISO A4 - 210 x 297 mm") == 0)
    m_paperType = te::layout::A4;
  else if (text.toStdString().compare("ISO A5 - 148 x 210 mm") == 0)
    m_paperType = te::layout::A5;
  else
    m_paperType = te::layout::Custom; 
}

void te::layout::PageSetupOutside::on_pbApply_clicked()
{
  PaperConfig* pConfig =  Context::getInstance().getPaperConfig();
  
  if(m_orientation != pConfig->getPaperOrientantion() 
    || m_paperType != pConfig->getPaperType())
  {
    pConfig->setPaperType(m_paperType);
    pConfig->setPaperOrientation(m_orientation);
    emit changeConfig();
  }
}

void te::layout::PageSetupOutside::on_rdbLandscape_clicked()
{
  if(m_ui->rdbLandscape->isChecked())
  {
    m_orientation = te::layout::Landscape;
  }
}

void te::layout::PageSetupOutside::on_rdbPortrait_clicked()
{
  if(m_ui->rdbPortrait->isChecked())
  {
    m_orientation = te::layout::Portrait;
  }
}
