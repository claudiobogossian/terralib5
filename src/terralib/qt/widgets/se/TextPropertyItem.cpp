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
\file terralib/qt/widgets/se/TextPropertyItem.cpp

    \brief A widget used to define the text se object.
*/


#include "../../../common/StringUtils.h"
#include "../../../se/SvgParameter.h"
#include "../../../se/TextSymbolizer.h"
#include "../../../se/Utils.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicFillPropertyItem.h"
#include "TextPropertyItem.h"

// STL
#include <cassert>

// QtPropertyBrowser


te::qt::widgets::TextPropertyItem::TextPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_textSymbolizer(new te::se::TextSymbolizer)
{
  //build property browser basic fill
  QtProperty* textProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Text Properties"));

  //label
  QStringList emptyList;
  m_labelProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->addProperty(tr("Label"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_labelProperty, emptyList);
  textProperty->addSubProperty(m_labelProperty);

  //font
  m_fontProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->addProperty(tr("Font"));
  textProperty->addSubProperty(m_fontProperty);

  addProperty(textProperty, tr("Text Properties"), QColor(89, 230, 226));

  m_bf = new te::qt::widgets::BasicFillPropertyItem(m_propertyBrowser, QColor(89, 230, 226));

  connect(m_bf, SIGNAL(fillChanged()), SLOT(onFillChanged()));

  // Setups initial fill
  m_textSymbolizer->setFill(m_bf->getFill());

  m_update = true;
}

te::qt::widgets::TextPropertyItem::~TextPropertyItem()
{
  delete m_textSymbolizer;
}

void te::qt::widgets::TextPropertyItem::valueChanged(QtProperty* p, int value)
{
  if (!m_update)
    return;

  if (p == m_labelProperty)
  {
    m_update = false;

    m_textSymbolizer->setLabel(new te::se::ParameterValue(m_labels[value]));
    
    emit textChanged();
    m_update = true;
  }
}

void te::qt::widgets::TextPropertyItem::valueChanged(QtProperty* p, const QFont &value)
{
  if (!m_update)
    return;

  if (p == m_fontProperty)
  {
    m_update = false;
    
    QFont f = value;

    int fontStyle = f.style();
    int weightStyle = f.weight();

    // Updating text
    te::se::Font* font = new te::se::Font();
    font->setFamily(f.family().toStdString());
    font->setSize(te::common::Convert2String(f.pointSize()));
    font->setStyle((te::se::Font::FontStyleType)fontStyle);
    font->setWeight((te::se::Font::FontWeightType)weightStyle);

    m_textSymbolizer->setFont(font);

    emit textChanged();
    m_update = true;
  }
}

void te::qt::widgets::TextPropertyItem::onFillChanged()
{
  m_textSymbolizer->setFill(m_bf->getFill());

  emit textChanged();
}

void te::qt::widgets::TextPropertyItem::setTextSymbolizer(const te::se::TextSymbolizer* ts)
{
  assert(ts);

  // Verifying if this widget can deal with the given fill...
  delete m_textSymbolizer;

  m_textSymbolizer = dynamic_cast<te::se::TextSymbolizer*>(ts->clone());

  updateUi();
  m_update = true;
}

void te::qt::widgets::TextPropertyItem::setLabels(std::vector<std::string> labels)
{
  m_labels = labels;

  QStringList list;

  for (std::size_t i = 0; i < labels.size(); ++i)
  {
    list << labels[i].c_str();
  }

  m_update = false;
  te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setEnumNames(m_labelProperty, list);
  m_update = true;
}

te::se::TextSymbolizer* te::qt::widgets::TextPropertyItem::getTextSymbolizer() const
{
  return dynamic_cast<te::se::TextSymbolizer*>(m_textSymbolizer->clone());
}

void te::qt::widgets::TextPropertyItem::updateUi()
{
  m_update = true;

  //update font
  const te::se::Font* f = m_textSymbolizer->getFont();

  QFont font;
  font.setFamily(te::se::GetString(f->getFamily()).c_str());
  font.setPointSize(te::se::GetDouble(f->getSize()));
  font.setStyle((QFont::Style)te::se::GetInt(f->getStyle()));
  font.setWeight(te::se::GetInt(f->getWeight()));
  
  te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->setValue(m_fontProperty, font);

  //update label
  std::string label = te::se::GetString(m_textSymbolizer->getLabel());

  for (std::size_t t = 0; t < m_labels.size(); ++t)
  {
    if (m_labels[t] == label)
    {
      te::qt::widgets::AbstractPropertyManager::getInstance().m_enumManager->setValue(m_labelProperty, (int)t);
      break;
    }
  }

  //update fill
  if (m_textSymbolizer->getFill())
    m_bf->setFill(m_textSymbolizer->getFill());

  m_update = false;
}

