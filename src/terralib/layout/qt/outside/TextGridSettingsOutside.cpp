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
  \file TextGridSettingsOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridSettingsOutside.h"
#include "ui_TextGridSettings.h"
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/pattern/mvc/OutsideController.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../outside/TextGridSettingsController.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/TextGridSettingsConfigProperties.h"

// STL
#include <string>
#include <sstream> 

// Qt
#include <QCheckBox>
#include <QColorDialog>

te::layout::TextGridSettingsOutside::TextGridSettingsOutside( OutsideController* controller, Observable* o ) :
  QDialog(0),
  OutsideObserver(controller, o),
  m_ui(new Ui::TextGridSettings)
{
  m_ui->setupUi(this);

  m_propertiesNames = new TextGridSettingsConfigProperties;
  
  init();
}

te::layout::TextGridSettingsOutside::~TextGridSettingsOutside()
{
  if(m_propertiesNames)
  {
    delete m_propertiesNames;
    m_propertiesNames = 0;
  }
}

void te::layout::TextGridSettingsOutside::init()
{
  m_ui->lneEdtBorderWidth->setValidator(new  QDoubleValidator(this));
  m_ui->lneEdtColumnsNumber->setValidator(new  QDoubleValidator(this));

  m_ui->lneEdtColumnsWidth->setValidator(new  QDoubleValidator(this));
  m_ui->lneEdtRowsNumber->setValidator(new  QDoubleValidator(this));

  m_ui->lneEdtTablePadding->setValidator(new  QDoubleValidator(this));
  m_ui->lneEdtTableSpacing->setValidator(new  QDoubleValidator(this));

  m_ui->lneEdtTableWidth->setValidator(new  QDoubleValidator(this));
  
  m_ui->frmBorderColor->setAutoFillBackground(true);
  m_ui->frmBorderColor->installEventFilter(this);

  m_ui->frmHeaderHorizontalColor->setAutoFillBackground(true);
  m_ui->frmHeaderHorizontalColor->installEventFilter(this);

  m_ui->frmHeaderVerticalColor->setAutoFillBackground(true);
  m_ui->frmHeaderVerticalColor->installEventFilter(this);

  m_ui->frmRowsEven->setAutoFillBackground(true);  
  m_ui->frmRowsEven->installEventFilter(this);

  m_ui->frmRowsOdd->setAutoFillBackground(true); 
  m_ui->frmRowsOdd->installEventFilter(this);

  m_ui->frmTableColor->setAutoFillBackground(true); 
  m_ui->frmTableColor->installEventFilter(this);
}

void te::layout::TextGridSettingsOutside::updateObserver( ContextItem context )
{
  setVisible(context.isShow());
  if(context.isShow() == true)
    show();
  else
    hide();
}

void te::layout::TextGridSettingsOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::TextGridSettingsOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::TextGridSettingsOutside::load()
{
  initCombo(m_ui->cmbColumnsWidthConstraints, m_propertiesNames->getColumnWidthConstraints());

  initCombo(m_ui->cmbTableWidthConstraints, m_propertiesNames->getWidthConstraints());

  initCombo(m_ui->cmbHeaderAlign, m_propertiesNames->getHeaderAlignment());

  initCombo(m_ui->cmbBorderStyle, m_propertiesNames->getBorderStyle());

  initCombo(m_ui->cmbCellAlign, m_propertiesNames->getCellAlignment());

  initDouble(m_ui->lneEdtBorderWidth, m_propertiesNames->getBorderWidth());

  initInt(m_ui->lneEdtColumnsNumber, m_propertiesNames->getColumnNumber());

  initDouble(m_ui->lneEdtColumnsWidth, m_propertiesNames->getColumnWidth());

  initInt(m_ui->lneEdtRowsNumber, m_propertiesNames->getRowNumber());

  initDouble(m_ui->lneEdtTablePadding, m_propertiesNames->getPadding());

  initDouble(m_ui->lneEdtTableSpacing, m_propertiesNames->getSpacing());

  initDouble(m_ui->lneEdtTableWidth, m_propertiesNames->getWidth());

  initColor(m_ui->frmBorderColor, m_propertiesNames->getBorderGridColor());

  initColor(m_ui->frmHeaderHorizontalColor, m_propertiesNames->getHeaderHorizontalColor());

  initColor(m_ui->frmHeaderVerticalColor, m_propertiesNames->getHeaderVerticalColor());

  initColor(m_ui->frmRowsEven, m_propertiesNames->getEvenRow());

  initColor(m_ui->frmRowsOdd, m_propertiesNames->getOddRow());

  initColor(m_ui->frmTableColor, m_propertiesNames->getTableColor());
}

te::color::RGBAColor te::layout::TextGridSettingsOutside::configColor( QWidget* widget )
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

void te::layout::TextGridSettingsOutside::blockComponents()
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

void te::layout::TextGridSettingsOutside::unblockComponents()
{
  QObjectList list = children();

  QList<QObject*>::Iterator it;
  for(it = list.begin() ; it != list.end() ; ++it)
  {
    QObject* obj = (*it);
    QWidget* w = dynamic_cast<QWidget*>(obj);
    if(w)
    {
      if(!w->isVisible())
      {
        w->setEnabled(true);
      }
    }
  }
}

void te::layout::TextGridSettingsOutside::on_pbApply_clicked()
{
  emit updateProperty(); 

  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    controller->clearUpdate();
  }
}

void te::layout::TextGridSettingsOutside::on_pbCancel_clicked()
{
  reject();
}

void te::layout::TextGridSettingsOutside::on_cmbColumnsWidthConstraints_currentIndexChanged( const QString & text )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    controller->addUpdateProperty(m_propertiesNames->getColumnWidthConstraints(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_cmbTableWidthConstraints_currentIndexChanged( const QString & text )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    controller->addUpdateProperty(m_propertiesNames->getWidthConstraints(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_cmbHeaderAlign_currentIndexChanged( const QString & text )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    controller->addUpdateProperty(m_propertiesNames->getHeaderAlignment(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_cmbBorderStyle_currentIndexChanged( const QString & text )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    controller->addUpdateProperty(m_propertiesNames->getBorderStyle(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_cmbCellAlign_currentIndexChanged( const QString & text )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(text.toStdString(), dataType->getDataTypeString());
    controller->addUpdateProperty(m_propertiesNames->getCellAlignment(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtBorderWidth_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtBorderWidth->text().toDouble(), dataType->getDataTypeDouble());
    controller->addUpdateProperty(m_propertiesNames->getBorderWidth(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtColumnsNumber_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtColumnsNumber->text().toInt(), dataType->getDataTypeInt());
    controller->addUpdateProperty(m_propertiesNames->getColumnNumber(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtColumnsWidth_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtColumnsWidth->text().toDouble(), dataType->getDataTypeDouble());
    controller->addUpdateProperty(m_propertiesNames->getColumnWidth(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtRowsNumber_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtRowsNumber->text().toInt(), dataType->getDataTypeInt());
    controller->addUpdateProperty(m_propertiesNames->getRowNumber(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtTablePadding_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtTablePadding->text().toDouble(), dataType->getDataTypeDouble());
    controller->addUpdateProperty(m_propertiesNames->getPadding(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtTableSpacing_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtTableSpacing->text().toDouble(), dataType->getDataTypeDouble());
    controller->addUpdateProperty(m_propertiesNames->getSpacing(), variant);
  }
}

void te::layout::TextGridSettingsOutside::on_lneEdtTableWidth_editingFinished()
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(controller)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Variant variant;
    variant.setValue(m_ui->lneEdtTableWidth->text().toDouble(), dataType->getDataTypeDouble());
    controller->addUpdateProperty(m_propertiesNames->getWidth(), variant);
  }
}

void te::layout::TextGridSettingsOutside::initString( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
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

void te::layout::TextGridSettingsOutside::initInt( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(!controller)
    return;

  std::ostringstream convert;
  Property prop = controller->getProperty(nameComponent);
  convert << prop.getValue().toInt();

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::TextGridSettingsOutside::initDouble( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(!controller)
    return;

  std::ostringstream convert;
  convert.precision(15);
  Property prop = controller->getProperty(nameComponent);
  double number = prop.getValue().toDouble();
  convert << number;

  QLineEdit* edit = dynamic_cast<QLineEdit*>(widget);
  if(edit)
  {
    edit->setText(convert.str().c_str());
  }
}

void te::layout::TextGridSettingsOutside::initBool( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QCheckBox* chk = dynamic_cast<QCheckBox*>(widget);

  if(chk)
  {
    chk->setChecked(prop.getValue().toBool());
  }
}

void te::layout::TextGridSettingsOutside::initColor( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
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

void te::layout::TextGridSettingsOutside::initCombo( QWidget* widget, std::string nameComponent )
{
  TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
  if(!controller)
    return;

  Property prop = controller->getProperty(nameComponent);

  QComboBox* combo = dynamic_cast<QComboBox*>(widget);

  if(!combo)
    return;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int index = -1;
  QVariant variant;

  if(prop.getType() == dataType->getDataTypeBool())
  {
    variant.setValue(prop.getValue().toBool());
  }
  else if(prop.getType() == dataType->getDataTypeDouble())
  {
    variant.setValue(prop.getValue().toDouble());
  }
  else if(prop.getType() == dataType->getDataTypeInt())
  {
    variant.setValue(prop.getValue().toInt());
  }
  else if(prop.getType() == dataType->getDataTypeString())
  {
    variant.setValue(QString(prop.getValue().toString().c_str()));
  }

  index = combo->findData(variant);
  if ( index != -1 ) 
  { 
    combo->setCurrentIndex(index);
  }
}

bool te::layout::TextGridSettingsOutside::eventFilter( QObject * watched, QEvent * event )
{
  if(event->type() != QEvent::MouseButtonPress)
    return QWidget::eventFilter(watched, event);

  QWidget* wgt = 0;
  std::string name;

  if(watched == m_ui->frmBorderColor)
  {
    wgt = m_ui->frmBorderColor;
    name = m_propertiesNames->getBorderGridColor();
  }
  else if(watched == m_ui->frmHeaderHorizontalColor)
  {
    wgt = m_ui->frmHeaderHorizontalColor;
    name = m_propertiesNames->getHeaderHorizontalColor();
  }
  else if(watched == m_ui->frmHeaderVerticalColor)
  {
    wgt = m_ui->frmHeaderVerticalColor;
    name = m_propertiesNames->getHeaderVerticalColor();
  }
  else if(watched == m_ui->frmRowsEven)
  {
    wgt = m_ui->frmRowsEven;
    name = m_propertiesNames->getEvenRow();
  }
  else if(watched == m_ui->frmRowsOdd)
  {
    wgt = m_ui->frmRowsOdd;
    name = m_propertiesNames->getOddRow();
  }
  else if(watched == m_ui->frmTableColor)
  {
    wgt = m_ui->frmTableColor;
    name = m_propertiesNames->getTableColor();
  }

  if(wgt)
  {
    te::color::RGBAColor color = configColor(wgt);
    
    TextGridSettingsController* controller = dynamic_cast<TextGridSettingsController*>(m_controller);
    if(controller)
    {
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      Variant variant;
      variant.setValue(color, dataType->getDataTypeColor());
      controller->addUpdateProperty(name, variant);
    }
  }

  return QWidget::eventFilter(watched, event);
}
