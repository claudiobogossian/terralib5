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
  \file GridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GridSettingsOutside.h"
#include "ui_GridSettings.h"
#include "OutsideObserver.h"
#include "OutsideController.h"
#include "GridSettingsController.h"
#include "Variant.h"
#include "PlanarGridSettingsConfigProperties.h"
#include "GeodesicGridSettingsConfigProperties.h"

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

te::layout::GridSettingsOutside::GridSettingsOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::GridSettings)
{
  m_planarGridSettings = new PlanarGridSettingsConfigProperties;
  m_geodesicGridSettings = new GeodesicGridSettingsConfigProperties;

  m_ui->setupUi(this);

  setWindowFlags( Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint 
    | Qt::WindowTitleHint | Qt::WindowStaysOnTopHint );

  init();
}

te::layout::GridSettingsOutside::~GridSettingsOutside()
{
  if(m_planarGridSettings)
  {
    delete m_planarGridSettings;
    m_planarGridSettings = 0;
  }

  if(m_geodesicGridSettings)
  {
    delete m_geodesicGridSettings;
    m_geodesicGridSettings = 0;
  }
}

void te::layout::GridSettingsOutside::init()
{
  m_ui->lneVrtPlanarDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzPlanarDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneCornerHrzGeoDisplacement->setValidator(new  QDoubleValidator(this));
  m_ui->lneCornerVrtGeoDisplacement->setValidator(new  QDoubleValidator(this));

  m_ui->lneHrzPlanarGap->setValidator(new  QDoubleValidator(this));
  m_ui->lneVrtPlanarGap->setValidator(new  QDoubleValidator(this));

  m_ui->lneX1->setValidator(new  QDoubleValidator(this));
  m_ui->lneY1->setValidator(new  QDoubleValidator(this));
  m_ui->lneX2->setValidator(new  QDoubleValidator(this));
  m_ui->lneY2->setValidator(new  QDoubleValidator(this));
  m_ui->lneX3->setValidator(new  QDoubleValidator(this));
  m_ui->lneY3->setValidator(new  QDoubleValidator(this));
  m_ui->lneX4->setValidator(new  QDoubleValidator(this));
  m_ui->lneY4->setValidator(new  QDoubleValidator(this));
  m_ui->cmbScale->hide();
  m_ui->ckDefineScale->hide();

  m_ui->fraCornerTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextGeoColor->setAutoFillBackground(true);
  m_ui->fraGridTextPlanarColor->setAutoFillBackground(true);
  m_ui->fraPlanarLineColor->setAutoFillBackground(true);  
}

void te::layout::GridSettingsOutside::updateObserver( ContextItem context )
{
  setVisible(context.getShow());
  if(context.getShow() == true)
    show();
  else
    hide();

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;
}

void te::layout::GridSettingsOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::GridSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::GridSettingsOutside::blockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(false);
      }
    }
  }
}

void te::layout::GridSettingsOutside::unblockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(w->isVisible())
      {
        w->setEnabled(true);
      }
    }
  }
}

bool te::layout::GridSettingsOutside::checkValidDegreeValue(const QString &value)
{
  int									degree = 0, minute = 0;
  float								second = 0;
  int									status = 0;
  std::basic_string <char>::size_type		index;
  std::string							strDegree = "";
  double								ret = 0;

  strDegree=value.toLatin1();
  if((index=strDegree.find("º")) !=std::string::npos)	
  {
    strDegree.replace(index,1,"");
  }
  if((index=strDegree.find("°")) !=std::string::npos)
  {
    strDegree.replace(index,1,"");
  }

  if((index=strDegree.find("'")) !=std::string::npos)	
  {
    strDegree.replace(index,1,"");
  }
  if((index=strDegree.find("'")) !=std::string::npos)	
  {
    strDegree.replace(index,1,"");
  }

  status=sscanf(strDegree.c_str(),"%d %d %f",&degree,&minute,&second);
  if(status!=3)	return false;

  return true;

}

te::color::RGBAColor te::layout::GridSettingsOutside::configColor( QWidget* widget )
{
  te::color::RGBAColor rgbaColor;

  QPalette ptt(widget->palette());
  QBrush brush = ptt.brush(widget->backgroundRole());

  QColor bColor = brush.color();
  rgbaColor.setColor(bColor.red(), bColor.green(), bColor.blue());

  QColor color = QColorDialog::getColor(brush.color(),this, "Color" );

  if(!color.isValid())	
    return rgbaColor;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), color);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);

  rgbaColor.setColor(color.red(), color.green(), color.blue());

  return rgbaColor;
}

void te::layout::GridSettingsOutside::load()
{
  /* Grid */
  
  initCombo(m_ui->cmbUnit, m_planarGridSettings->getUnit());
  
  initBool(m_ui->chkShowPlanar, m_planarGridSettings->getVisible());

  initCombo(m_ui->cmbPlanarStyle, m_planarGridSettings->getStyle());

  initCombo(m_ui->cmbGridStyle, m_geodesicGridSettings->getStyle());
  
  initBool(m_ui->chkShowGeodesic, m_geodesicGridSettings->getVisible());

  ///* Line */
  
  initDouble(m_ui->lneHrzPlanarGap, m_planarGridSettings->getLneHrzGap());
  
  initDouble(m_ui->lneVrtPlanarGap, m_planarGridSettings->getLneVrtGap());
  
  initDouble(m_ui->lneHorizontalGap, m_geodesicGridSettings->getLneHrzGap());
  
  initDouble(m_ui->lneVerticalGap, m_geodesicGridSettings->getLneVrtGap());

  initColor(m_ui->fraPlanarLineColor, m_planarGridSettings->getLineColor());

  initCombo(m_ui->cmbPlanarLineType, m_planarGridSettings->getLineStyle());

  initCombo(m_ui->cmbPlanarLineWidth, m_planarGridSettings->getLineWidth());

  initCombo(m_ui->cmbLineType, m_geodesicGridSettings->getStyle());

  initCombo(m_ui->cmbLineWidth, m_geodesicGridSettings->getLineWidth());

  initColor(m_ui->fraLineColor, m_geodesicGridSettings->getLineColor());

  ///*Text: Basic Configuration*/

  initCombo(m_ui->cmbPlanarTextSize, m_planarGridSettings->getPointTextSize());

  initCombo(m_ui->cmbPlanarFont, m_planarGridSettings->getFontText());

  initColor(m_ui->fraGridTextPlanarColor, m_planarGridSettings->getTextColor());

  initBool(m_ui->chkSuperscriptPlanarText, m_planarGridSettings->getSuperscriptText());

  initCombo(m_ui->cmbGeoFont, m_geodesicGridSettings->getFontText());

  initCombo(m_ui->cmbGeoTextSize, m_geodesicGridSettings->getPointTextSize());

  initColor(m_ui->fraGridTextGeoColor, m_geodesicGridSettings->getTextColor());

  initBool(m_ui->chkSuperscriptGeoText, m_geodesicGridSettings->getSuperscriptText());

  ///*Text: Advanced configuration*/

  initDouble(m_ui->xGridInitialPoint_planar_textField, m_planarGridSettings->getInitialGridPointX());

  initDouble(m_ui->yGridInitialPoint_planar_textField, m_planarGridSettings->getInitialGridPointY());

  initDouble(m_ui->xGridInitialPoint_geo_textField, m_geodesicGridSettings->getInitialGridPointX());

  initDouble(m_ui->yGridInitialPoint_geo_textField, m_geodesicGridSettings->getInitialGridPointY());

  initBool(m_ui->chkBottomPlanarText, m_planarGridSettings->getBottomText());

  initBool(m_ui->chkLeftPlanarText, m_planarGridSettings->getLeftText());

  initBool(m_ui->chkRightPlanarText, m_planarGridSettings->getRightText());

  initBool(m_ui->chkTopPlanarText, m_planarGridSettings->getTopText());

  initBool(m_ui->chkBottomGeoText, m_geodesicGridSettings->getBottomText());

  initBool(m_ui->chkLeftGeoText, m_geodesicGridSettings->getLeftText());

  initBool(m_ui->chkRightGeoText, m_geodesicGridSettings->getRightText());

  initBool(m_ui->chkTopGeoText, m_geodesicGridSettings->getTopText());

  initBool(m_ui->chkBottomRotatePlanarText, m_planarGridSettings->getBottomRotateText());

  initBool(m_ui->chkLeftRotatePlanarText, m_planarGridSettings->getLeftRotateText());

  initBool(m_ui->chkRightRotatePlanarText, m_planarGridSettings->getRightRotateText());

  initBool(m_ui->chkTopRotatePlanarText, m_planarGridSettings->getTopRotateText());

  initBool(m_ui->chkBottomRotateGeoText, m_geodesicGridSettings->getBottomRotateText());

  initBool(m_ui->chkLeftRotateGeoText, m_geodesicGridSettings->getLeftRotateText());

  initBool(m_ui->chkRightRotateGeoText, m_geodesicGridSettings->getRightRotateText());

  initBool(m_ui->chkTopRotateGeoText, m_geodesicGridSettings->getTopRotateText());

  initDouble(m_ui->lneVrtPlanarDisplacement, m_planarGridSettings->getLneVrtDisplacement());

  initBool(m_ui->chkVisibleTextsPlanarText, m_planarGridSettings->getVisibleAllTexts());

  initDouble(m_ui->lneHrzPlanarDisplacement, m_planarGridSettings->getLneHrzDisplacement());

  initDouble(m_ui->lneVrtGeoDisplacement, m_geodesicGridSettings->getLneVrtDisplacement());

  initDouble(m_ui->lneHrzGeoDisplacement, m_geodesicGridSettings->getLneHrzDisplacement());

  initBool(m_ui->chkDegreesGeoText, m_geodesicGridSettings->getDegreesText());

  initBool(m_ui->chkMinutesGeoText, m_geodesicGridSettings->getMinutesText());

  initBool(m_ui->chkSecondsGeoText, m_geodesicGridSettings->getSecondsText());

  initBool(m_ui->chkVisibleTextsGeoText, m_geodesicGridSettings->getVisibleAllTexts());

  initCombo(m_ui->cmbCornerGeoFont, m_geodesicGridSettings->getFontTextCorner());

  initCombo(m_ui->cmbCornerGeoTextSize, m_geodesicGridSettings->getPointTextSize());
  
  initColor(m_ui->fraCornerTextGeoColor, m_geodesicGridSettings->getTextColorCorner());
  
  initDouble(m_ui->lneCornerHrzGeoDisplacement, m_geodesicGridSettings->getLneCornerHrzDisplacement());

  initDouble(m_ui->lneCornerVrtGeoDisplacement, m_geodesicGridSettings->getLneCornerVrtDisplacement());
  
  initBool(m_ui->chkLowerRightCornerGeoText, m_geodesicGridSettings->getLowerRightCornerText());

  initBool(m_ui->chkUpperRightCornerGeoText, m_geodesicGridSettings->getUpperRightCornerText());

  initBool(m_ui->chkLowerLeftCornerGeoText, m_geodesicGridSettings->getLowerLeftCornerText());

  initBool(m_ui->chkUpperLeftCornerGeoText, m_geodesicGridSettings->getUpperLeftCornerText());
  
  /*Geodesic: Topographic Map*/
  
  initBool(m_ui->ckDefineScale, m_geodesicGridSettings->getDefineScale());

  initCombo(m_ui->cmbScale, m_geodesicGridSettings->getScale());

  initBool(m_ui->ckbClip, m_geodesicGridSettings->getClip());

  initDouble(m_ui->lneX1, m_geodesicGridSettings->getLneX1());

  initDouble(m_ui->lneX2, m_geodesicGridSettings->getLneX2());

  initDouble(m_ui->lneY1, m_geodesicGridSettings->getLneY1());

  initDouble(m_ui->lneY2, m_geodesicGridSettings->getLneY2());

  initDouble(m_ui->lneX3, m_geodesicGridSettings->getLneX3());

  initDouble(m_ui->lneX4, m_geodesicGridSettings->getLneX4());

  initDouble(m_ui->lneY3, m_geodesicGridSettings->getLneY3());

  initDouble(m_ui->lneY4, m_geodesicGridSettings->getLneY4());
}

void te::layout::GridSettingsOutside::on_pbApply_clicked()
{
  emit updateProperty(); 

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    controller->clearUpdate();
  }
}

void te::layout::GridSettingsOutside::on_pbClose_clicked()
{
  accept();
}

void te::layout::GridSettingsOutside::on_helpPushButton_clicked()
{

}

void te::layout::GridSettingsOutside::on_pbCancel_clicked()
{
  reject();
}

void te::layout::GridSettingsOutside::on_cmbUnit_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_planarGridSettings->getUnit(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkShowPlanar_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkShowPlanar->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getVisible(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_planarGridSettings->getStyle(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbGridStyle_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getStyle(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkShowGeodesic_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkShowGeodesic->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getVisible(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarGap_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneHrzPlanarGap->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getLneHrzGap(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarGap_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneVrtPlanarGap->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getLneVrtGap(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneHorizontalGap_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->lneHorizontalGap->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneHorizontalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneHorizontalGap->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneHrzGap(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneVerticalGap_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->lneVerticalGap->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneVerticalGap->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneVrtGap(), variant);
  }
}

void te::layout::GridSettingsOutside::on_pbPlanarLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraPlanarLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(color, DataTypeColor);
    controller->addUpdateProperty(m_planarGridSettings->getLineColor(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_planarGridSettings->getLineStyle(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarLineWidth_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toInt(), DataTypeInt);
    controller->addUpdateProperty(m_planarGridSettings->getLineWidth(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbLineType_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getLineStyle(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbLineWidth_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLineWidth(), variant);
  }
}

void te::layout::GridSettingsOutside::on_pbLineColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraLineColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(color, DataTypeColor);
    controller->addUpdateProperty(m_geodesicGridSettings->getLineColor(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toInt(), DataTypeInt);
    controller->addUpdateProperty(m_planarGridSettings->getPointTextSize(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbPlanarFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_planarGridSettings->getFontText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextPlanarColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextPlanarColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(color, DataTypeColor);
    controller->addUpdateProperty(m_planarGridSettings->getTextColor(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkSuperscriptPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getSuperscriptText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getFontText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toInt(), DataTypeInt);
    controller->addUpdateProperty(m_geodesicGridSettings->getPointTextSize(), variant);
  }
}

void te::layout::GridSettingsOutside::on_pbGridTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraGridTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(color, DataTypeColor);
    controller->addUpdateProperty(m_geodesicGridSettings->getTextColor(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkSuperscriptGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkSuperscriptGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getSuperscriptText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_planar_textField_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->xGridInitialPoint_planar_textField->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getInitialGridPointX(), variant);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_planar_textField_editingFinished()
{
  /*  
  if(checkValidDegreeValue(m_ui->yGridInitialPoint_geo_textField->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->yGridInitialPoint_planar_textField->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getInitialGridPointY(), variant);
  }
}

void te::layout::GridSettingsOutside::on_xGridInitialPoint_geo_textField_editingFinished()
{
  /*if(checkValidDegreeValue(m_ui->xGridInitialPoint_geo_textField->text()) == false)
  {
    QMessageBox::information(this, tr("Information"), tr("Invalid Geodesic value! Try for example 0° 1' 0''"));	
    m_ui->lneVerticalGap->setFocus();
    return;
  }*/

  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->xGridInitialPoint_geo_textField->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getInitialGridPointX(), variant);
  }
}

void te::layout::GridSettingsOutside::on_yGridInitialPoint_geo_textField_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->yGridInitialPoint_geo_textField->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getInitialGridPointY(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkBottomPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getBottomText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLeftPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getLeftText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkRightPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkRightPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getRightText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkTopPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkTopPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getTopText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkBottomGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getBottomText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLeftGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getLeftText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkRightGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkRightGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getRightText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkTopGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkTopGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getTopText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkBottomRotatePlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getBottomRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLeftRotatePlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getLeftRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkRightRotatePlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getRightRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotatePlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkTopRotatePlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getTopRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkBottomRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkBottomRotateGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getBottomRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLeftRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLeftRotateGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getLeftRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkRightRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkRightRotateGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getRightRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkTopRotateGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkTopRotateGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getTopRotateText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtPlanarDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneVrtPlanarDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getLneVrtDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsPlanarText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkVisibleTextsPlanarText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_planarGridSettings->getVisibleAllTexts(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzPlanarDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneHrzPlanarDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_planarGridSettings->getLneHrzDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneVrtGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneVrtGeoDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneHrzDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneHrzGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneHrzGeoDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneHrzDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkDegreesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkDegreesGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getDegreesText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkMinutesGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkMinutesGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getMinutesText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkSecondsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkSecondsGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getSecondsText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkVisibleTextsGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getVisibleAllTexts(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoFont_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toStdString(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getFontTextCorner(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbCornerGeoTextSize_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toInt(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getPointTextSizeCorner(), variant);
  }
}

void te::layout::GridSettingsOutside::on_pbCornerTextGeoColor_clicked()
{
  te::color::RGBAColor color = configColor(m_ui->fraCornerTextGeoColor);
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(color, DataTypeColor);
    controller->addUpdateProperty(m_geodesicGridSettings->getTextColorCorner(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerHrzGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneCornerHrzGeoDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneCornerHrzDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneCornerVrtGeoDisplacement_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneCornerVrtGeoDisplacement->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneCornerVrtDisplacement(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLowerRightCornerGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getLowerRightCornerText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperRightCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkUpperRightCornerGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getUpperRightCornerText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkLowerLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkLowerLeftCornerGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getLowerLeftCornerText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkUpperLeftCornerGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkUpperLeftCornerGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getUpperLeftCornerText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_chkVisibleCornerTextsGeoText_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->chkVisibleCornerTextsGeoText->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getVisibleCornerTextsText(), variant);
  }
}

void te::layout::GridSettingsOutside::on_ckDefineScale_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->ckDefineScale->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getDefineScale(), variant);
  }
}

void te::layout::GridSettingsOutside::on_cmbScale_currentIndexChanged( const QString & text )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(text.toInt(), DataTypeString);
    controller->addUpdateProperty(m_geodesicGridSettings->getScale(), variant);
  }
}

void te::layout::GridSettingsOutside::on_ckbClip_clicked()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->ckbClip->isChecked(), DataTypeBool);
    controller->addUpdateProperty(m_geodesicGridSettings->getClip(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneX1_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneX1->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneX1(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneX2_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneX2->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneX2(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneY1_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneY1->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneY1(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneY2_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneY2->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneY2(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneX3_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneX3->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneX3(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneX4_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneX4->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneX4(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneY3_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneY3->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneY3(), variant);
  }
}

void te::layout::GridSettingsOutside::on_lneY4_editingFinished()
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(controller)
  {
    Variant variant;
    variant.setValue(m_ui->lneY4->text().toDouble(), DataTypeDouble);
    controller->addUpdateProperty(m_geodesicGridSettings->getLneY4(), variant);
  }
}

void te::layout::GridSettingsOutside::initString( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(prop.getValue().toString().c_str());
    return;
  }
}

void te::layout::GridSettingsOutside::initInt( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  ostringstream convert;
  Property prop = controller->getProperty(nameComponent);
  convert << prop.getValue().toInt();

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::GridSettingsOutside::initDouble( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  ostringstream convert;
  Property prop = controller->getProperty(nameComponent);
  convert << prop.getValue().toDouble();
  
  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::GridSettingsOutside::initBool( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);
  
  if(chk)
  {
    chk->setChecked(prop.getValue().toBool());
  }
}

void te::layout::GridSettingsOutside::initColor( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);
    
  te::color::RGBAColor color = prop.getValue().toColor();
  QColor qcolor(color.getRed(), color.getGreen(), color.getBlue());

  if(!qcolor.isValid())	
    return;

  if(!widget)
    return;

  QPalette paltt(widget->palette());
  paltt.setColor(widget->backgroundRole(), qcolor);
  widget->setPalette(paltt);
  widget->setAutoFillBackground(true);
}

void te::layout::GridSettingsOutside::initCombo( QWidget* widget, std::string nameComponent )
{
  GridSettingsController* controller = dynamic_cast<GridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if(!combo)
    return;

  int index = -1;
  QVariant variant;

  switch(prop.getType())
  {
  case DataTypeBool:
    variant.setValue(prop.getValue().toBool());
    break;
  case DataTypeDouble:
    variant.setValue(prop.getValue().toDouble());
    break;
  case DataTypeInt:
    variant.setValue(prop.getValue().toInt());
    break;
  case DataTypeString:
    variant.setValue(QString(prop.getValue().toString().c_str()));
    break;
  default:
    index = -1;
  }
  
  index = combo->findData(variant);
  if ( index != -1 ) 
  { 
    combo->setCurrentIndex(index);
  }
}
