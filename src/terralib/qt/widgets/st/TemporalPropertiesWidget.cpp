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
  \file terralib/qt/widgets/st/TimePropertiesWidget.cpp

  \brief A widget used to adjust a temporal layer's properties
*/

//Terralib
#include "../../../dataaccess.h"
#include "../../../datatype/Enums.h"
#include "../../../datatype/Property.h"
#include "TemporalPropertiesWidget.h"
#include "ui_TemporalPropertiesWidgetForm.h"

//QT
#include <QWidget>

te::qt::widgets::TemporalPropertiesWidget::TemporalPropertiesWidget(QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f),
  m_ui(new Ui::TemporalPropertiesWidgetForm)
{
  m_ui->setupUi(this);

  m_ui->m_typeComboBox->addItem(tr("Date Time"), QVariant(te::dt::DATETIME_TYPE));
  m_ui->m_typeComboBox->addItem(tr("Ordinal"),  QVariant(te::dt::ORDINAL_TIME_INSTANT));
  m_ui->m_typeComboBox->addItem(tr("String"),  QVariant(te::dt::STRING_TYPE));

  m_ui->m_ordTypeComboBox->addItem(tr("Ordinal Instant"), QVariant(te::dt::ORDINAL_TIME_INSTANT));
  m_ui->m_ordTypeComboBox->addItem(tr("Ordinal Period"), QVariant(te::dt::ORDINAL_TIME_PERIOD));
  m_ui->m_ordTypeComboBox->addItem(tr("User defined"), QVariant(-1));

  m_ui->m_tempUnitComboBox->addItem(tr("Year"), QVariant(te::dt::YEAR));
  m_ui->m_tempUnitComboBox->addItem(tr("Month"), QVariant(te::dt::MONTH));
  m_ui->m_tempUnitComboBox->addItem(tr("Day"), QVariant(te::dt::DAY));
  m_ui->m_tempUnitComboBox->addItem(tr("Hour"), QVariant(te::dt::HOUR));
  m_ui->m_tempUnitComboBox->addItem(tr("Minute"), QVariant(te::dt::MINUTE));
  m_ui->m_tempUnitComboBox->addItem(tr("Second"), QVariant(te::dt::SECOND));
  m_ui->m_tempUnitComboBox->addItem(tr("Week"), QVariant(te::dt::WEEK));
  m_ui->m_tempUnitComboBox->addItem(tr("Day of the Week"), QVariant(te::dt::DAY_OF_WEEK));
  m_ui->m_tempUnitComboBox->addItem(tr("Day of the Year"), QVariant(te::dt::DAY_OF_YEAR));
  m_ui->m_tempUnitComboBox->addItem(tr("Unknown")), QVariant(te::dt::UNKNOWN);

  m_ui->m_formatComboBox->addItem(QString::fromStdString("YYYY"), QVariant(te::dt::YYYY));
  m_ui->m_formatComboBox->addItem(QString::fromStdString("YY"), QVariant(te::dt::YY));
  m_ui->m_formatComboBox->addItem(QString::fromStdString("YYYYMM"), QVariant(te::dt::YYYYMM));
  m_ui->m_formatComboBox->addItem(QString::fromStdString("YYYYDDD"), QVariant(te::dt::YYYYDDD));

  // connect signal and slots
  connect(m_ui->m_typeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeCBCurrentIndexChanged(int)));
  connect(m_ui->m_ordTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onOrdTypeCBCurrentIndexChanged(int)));
  connect(m_ui->m_tempUnitComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onUnitTypeCBCurrentIndexChanged(int)));
}

te::qt::widgets::TemporalPropertiesWidget::~TemporalPropertiesWidget()
{
}

Ui::TemporalPropertiesWidgetForm* te::qt::widgets::TemporalPropertiesWidget::getForm()
{
  return m_ui.get();
}

std::string te::qt::widgets::TemporalPropertiesWidget::getPhenomenonTime()
{
  return m_ui->m_phenomenomComboBox->currentText().toStdString();
}

std::string te::qt::widgets::TemporalPropertiesWidget::getResultTime()
{
  return m_ui->m_resultComboBox->currentText().toStdString();
}

std::string te::qt::widgets::TemporalPropertiesWidget::getValidTime()
{
  return m_ui->m_validComboBox->currentText().toStdString();
}

int te::qt::widgets::TemporalPropertiesWidget::getDateType()
{
  return m_ui->m_typeComboBox->currentIndex();
}

int te::qt::widgets::TemporalPropertiesWidget::getOrdinalType()
{
  return m_ui->m_ordTypeComboBox->currentIndex();
}

int te::qt::widgets::TemporalPropertiesWidget::getTemporalUnit()
{
  return m_ui->m_tempUnitComboBox->currentIndex();
}

std::string te::qt::widgets::TemporalPropertiesWidget::getTemporalValue()
{
  return m_ui->m_tempValueLineEdit->text().toStdString();
}

std::string te::qt::widgets::TemporalPropertiesWidget::getUserOrdinalType()
{
  return m_ui->m_ordinalLineEdit->text().toStdString();
}

int te::qt::widgets::TemporalPropertiesWidget::getDateFormat()
{
	return m_ui->m_formatComboBox->itemData(m_ui->m_formatComboBox->currentIndex()).toInt();
}

void te::qt::widgets::TemporalPropertiesWidget::onTypeCBCurrentIndexChanged(int index)
{
  if(m_ui->m_typeComboBox->itemData(index).toInt() == te::dt::STRING_TYPE)
  {
    m_ui->m_formatComboBox->setEnabled(true);
    m_ui->m_ordTypeComboBox->setEnabled(false);
  }
  else if(m_ui->m_typeComboBox->itemData(index).toInt() == te::dt::ORDINAL_TIME_INSTANT)
  {
    m_ui->m_ordTypeComboBox->setEnabled(true);
    m_ui->m_formatComboBox->setEnabled(false);
  }
  else
  {
    m_ui->m_formatComboBox->setEnabled(false);
    m_ui->m_ordTypeComboBox->setEnabled(false);
  }
}

void te::qt::widgets::TemporalPropertiesWidget::onOrdTypeCBCurrentIndexChanged(int index)
{
  if(m_ui->m_ordTypeComboBox->itemData(index).toInt() == -1)
    m_ui->m_ordinalLineEdit->setEnabled(true);
  else
    m_ui->m_ordinalLineEdit->setEnabled(false);
}

void te::qt::widgets::TemporalPropertiesWidget::onUnitTypeCBCurrentIndexChanged(int index)
{
  if(m_ui->m_tempUnitComboBox->itemData(index).toInt() == te::dt::UNKNOWN)
    m_ui->m_tempValueLineEdit->setEnabled(true);
  else
    m_ui->m_tempValueLineEdit->setEnabled(false);
}

void te::qt::widgets::TemporalPropertiesWidget::setUp(const te::da::DataSetTypePtr dataType)
{
  QString item;
  m_dataType = dataType;

  const std::vector<te::dt::Property*>& properties = dataType.get()->getProperties();

  for (std::size_t i = 0; i < properties.size(); i++)
  {
    if(properties.at(i)->getType() == te::dt::DATETIME_TYPE)
    {
      item = QString::fromStdString(properties.at(i)->getName());
      m_ui->m_phenomenomComboBox->addItem(item);
      m_ui->m_resultComboBox->addItem(item);
      m_ui->m_validComboBox->addItem(item);
    }
  }
}
