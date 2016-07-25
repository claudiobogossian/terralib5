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
\file terralib/qt/widgets/se/TextSymbolizerProperty.cpp

      \brief A widget used to define the text symbolizer
      properties, such as:
      - basic fill
      - font
      - halo
*/

// Terralib
#include "../../../se/Halo.h"
#include "../../../se/Fill.h"
#include "../../../se/Font.h"
#include "../../../se/LabelPlacement.h"
#include "../../../se/LinePlacement.h"
#include "../../../se/ParameterValue.h"
#include "../../../se/PointPlacement.h"
#include "../../../se/TextSymbolizer.h"
#include "HaloPropertyItem.h"
#include "LinePlacementPropertyItem.h"
#include "PointPlacementPropertyItem.h"
#include "TextPropertyItem.h"
#include "TextSymbolizerProperty.h"

// Qt
#include <QGridLayout>
#include <QToolBox>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>

te::qt::widgets::TextSymbolizerProperty::TextSymbolizerProperty(QWidget* parent) : m_setLocalSymbol(false), m_symb(new te::se::TextSymbolizer)
{
  QGridLayout* layout = new QGridLayout(this);
  this->setLayout(layout);

  QToolBox* tb = new QToolBox(this);
  layout->addWidget(tb);

  QtTreePropertyBrowser* basicPropBrowser = new QtTreePropertyBrowser(this);
  basicPropBrowser->setIndentation(10);
  basicPropBrowser->setPropertiesWithoutValueMarked(true);
  basicPropBrowser->setRootIsDecorated(false);
  basicPropBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  layout->addWidget(basicPropBrowser);

  m_text = new te::qt::widgets::TextPropertyItem(basicPropBrowser);
  
  m_pp = new te::qt::widgets::PointPlacementPropertyItem(basicPropBrowser);

  m_lp = new te::qt::widgets::LinePlacementPropertyItem(basicPropBrowser);

  m_halo = new te::qt::widgets::HaloPropertyItem(basicPropBrowser);

  tb->addItem(basicPropBrowser, "Text Symbology");

  connect(m_text, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
  connect(m_pp, SIGNAL(pointPlacementChanged()), this, SLOT(onPointPlacementChanged()));
  connect(m_lp, SIGNAL(linePlacementChanged()), this, SLOT(onLinePlacementChanged()));
  connect(m_halo, SIGNAL(haloChanged()), this, SLOT(onHaloChanged()));
}

te::qt::widgets::TextSymbolizerProperty::~TextSymbolizerProperty()
{
}

void te::qt::widgets::TextSymbolizerProperty::setSymbolizer(te::se::TextSymbolizer* symb)
{
  m_symb = symb;

  m_setLocalSymbol = true;

  m_text->setTextSymbolizer(m_symb);

  if (m_symb->getLabelPlacement() && m_symb->getLabelPlacement()->getPointPlacement())
    m_pp->setPointPlacement(m_symb->getLabelPlacement()->getPointPlacement());
  else
    m_pp->resetPointPlacement();

  if (m_symb->getLabelPlacement() && m_symb->getLabelPlacement()->getLinePlacement())
    m_lp->setLinePlacement(m_symb->getLabelPlacement()->getLinePlacement());
  else
    m_lp->resetLinePlacement();

  if (m_symb->getHalo())
    m_halo->setHalo(m_symb->getHalo());
  else
    m_halo->resetHalo();

  m_setLocalSymbol = false;
}

te::se::Symbolizer* te::qt::widgets::TextSymbolizerProperty::getSymbolizer() const
{
  return m_symb;
}

void te::qt::widgets::TextSymbolizerProperty::setLabels(std::vector<std::string> names)
{
  m_text->setLabels(names);
}

void te::qt::widgets::TextSymbolizerProperty::onTextChanged()
{
  te::se::TextSymbolizer* ts = m_text->getTextSymbolizer();

  m_symb->setLabel(ts->getLabel()->clone());
  m_symb->setFont(ts->getFont()->clone());
  m_symb->setFill(ts->getFill()->clone());

  delete ts;

  emit symbolizerChanged();
}

void te::qt::widgets::TextSymbolizerProperty::onPointPlacementChanged()
{
  te::se::LabelPlacement* lp = new te::se::LabelPlacement();

  lp->setPointPlacement(m_pp->getPointPlacement());

  if (m_symb->getLabelPlacement() && m_symb->getLabelPlacement()->getLinePlacement())
    lp->setLinePlacement(m_symb->getLabelPlacement()->getLinePlacement()->clone());

  m_symb->setLabelPlacement(lp);

  emit symbolizerChanged();
}

void te::qt::widgets::TextSymbolizerProperty::onLinePlacementChanged()
{
  te::se::LabelPlacement* lp = new te::se::LabelPlacement();

  lp->setLinePlacement(m_lp->getLinePlacement());

  if (m_symb->getLabelPlacement() && m_symb->getLabelPlacement()->getPointPlacement())
    lp->setPointPlacement(m_symb->getLabelPlacement()->getPointPlacement()->clone());

  m_symb->setLabelPlacement(lp);

  emit symbolizerChanged();
}

void te::qt::widgets::TextSymbolizerProperty::onHaloChanged()
{
  m_symb->setHalo(m_halo->getHalo());

  emit symbolizerChanged();
}
