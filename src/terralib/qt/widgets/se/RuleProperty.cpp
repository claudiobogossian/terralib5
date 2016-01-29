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
\file terralib/qt/widgets/se/RuleProperty.cpp

\brief A widget used to define the rule properties, such as:
- scale
- description
*/

// Terralib
#include "../../../se/Rule.h"
#include "RuleProperty.h"
#include "ScalePropertyItem.h"

// Qt
#include <QGridLayout>
#include <QToolBox>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>

te::qt::widgets::RuleProperty::RuleProperty(QWidget* parent) 
  : m_rule(0)
{
  QGridLayout* layout = new QGridLayout(this);
  this->setLayout(layout);

  QToolBox* tb = new QToolBox(this);
  layout->addWidget(tb);

  QtTreePropertyBrowser* propBrowser = new QtTreePropertyBrowser(this);
  propBrowser->setIndentation(10);
  propBrowser->setPropertiesWithoutValueMarked(true);
  propBrowser->setRootIsDecorated(false);
  propBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  layout->addWidget(propBrowser);

  m_sp = new te::qt::widgets::ScalePropertyItem(propBrowser);

  tb->addItem(propBrowser, "Rule Properties");

  connect(m_sp, SIGNAL(scaleMinChanged()), this, SLOT(onScaleMinChanged()));
  connect(m_sp, SIGNAL(scaleMaxChanged()), this, SLOT(onScaleMaxChanged()));
}

te::qt::widgets::RuleProperty::~RuleProperty()
{
}

void te::qt::widgets::RuleProperty::setRule(te::se::Rule* rule)
{
  m_rule = rule;

  m_sp->setMinScaleDenominator(m_rule->getMinScaleDenominator());

  m_sp->setMaxScaleDenominator(m_rule->getMaxScaleDenominator());
}

te::se::Rule* te::qt::widgets::RuleProperty::getRule() const
{
  return m_rule;
}

void te::qt::widgets::RuleProperty::onScaleMinChanged()
{
  if (m_rule)
  {
    m_rule->setMinScaleDenominator(m_sp->getMinScaleDenominator());
  }
}

void te::qt::widgets::RuleProperty::onScaleMaxChanged()
{
  if (m_rule)
  {
    m_rule->setMaxScaleDenominator(m_sp->getMaxScaleDenominator());
  }
}
