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
  \file terralib/qt/widgets/se/StyleExplorer.cpp

  \brief A widget used to explore a style.
*/

// TerraLib
#include "../../../se/Style.h"
#include "../../../se/Symbolizer.h"
#include "../../../se/Rule.h"
#include "StyleExplorer.h"
#include "SymbologyPreview.h"

// STL
#include <cassert>
#include <vector>

te::qt::widgets::StyleExplorer::StyleExplorer(QWidget* parent)
  : QTreeWidget(parent)
{
  // Setup
  setAlternatingRowColors(true);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setHeaderLabel(tr("Style Explorer"));
  setIconSize(QSize(32, 16));

  // Signals & slots
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(onItemClicked(QTreeWidgetItem*, int)));
}

te::qt::widgets::StyleExplorer::~StyleExplorer()
{
}

void te::qt::widgets::StyleExplorer::setStyle(te::se::Style* style)
{
  assert(style);

  m_style = style;

  initialize();
}

te::se::Rule* te::qt::widgets::StyleExplorer::getCurrentRule() 
{
  QTreeWidgetItem* selectedItem = getSelectedItem();
  if(selectedItem == 0)
    return 0;

  if(selectedItem->type() == RULE)
    return getRule(selectedItem);

  if(selectedItem->type() == SYMBOLIZER)
    return getRule(selectedItem->parent());

  return 0;
}

te::se::Symbolizer* te::qt::widgets::StyleExplorer::getCurrentSymbolizer()
{
  QTreeWidgetItem* selectedItem = getSelectedItem();
  if(selectedItem != 0 && selectedItem->type() == SYMBOLIZER)
    return getSymbolizer(selectedItem);

  return 0;
}

void te::qt::widgets::StyleExplorer::updateStyleTree()
{
  initialize();
}

void te::qt::widgets::StyleExplorer::goUpSymbolizer()
{
  te::se::Rule* r = getCurrentRule();
  te::se::Symbolizer* s = getCurrentSymbolizer();

  //check if exist current objects
  if(r && s)
  {
    //check if exist more than one symbolizer in current rule
    if(r->getSymbolizers().size() > 1)
    {
      //check if current symbolizer is not the first symbolizer
      int index = getSymbolizerIndex(r, s);

      if(index > 0)
        swapSymbolizers(r, index, index - 1);

      initialize();
    }
  }
}

void te::qt::widgets::StyleExplorer::goDownSymbolizer()
{
  te::se::Rule* r = getCurrentRule();
  te::se::Symbolizer* s = getCurrentSymbolizer();

  //check if exist current objects
  if(r && s)
  {
    //check if exist more than one symbolizer in current rule
    if(r->getSymbolizers().size() > 1)
    {
      //check if current symbolizer is not the first symbolizer
      int index = getSymbolizerIndex(r, s);

      if(index >= 0 && index < r->getSymbolizers().size() - 1)
        swapSymbolizers(r, index, index + 1);

      initialize();
    }
  }
}

void te::qt::widgets::StyleExplorer::initialize()
{
  clear();

  QTreeWidgetItem* root = new QTreeWidgetItem(this, STYLE);
  root->setText(0, m_style->getType().c_str());

  std::size_t nRules = m_style->getNRules();
  for(std::size_t i = 0; i < nRules; ++i) // for each rule
  {
    const te::se::Rule* rule = m_style->getRule(i);
    std::vector<te::se::Symbolizer*> symbs = rule->getSymbolizers();

    QTreeWidgetItem* ruleItem = new QTreeWidgetItem(root, RULE);
    ruleItem->setText(0, tr("Rule"));
    ruleItem->setData(0, Qt::UserRole, (int)i);
    ruleItem->setIcon(0, QIcon(SymbologyPreview::build(symbs, iconSize())));

    for(std::size_t j = 0; j < symbs.size(); ++j) // for each symbolizer
    {
      QTreeWidgetItem* symbItem = new QTreeWidgetItem(ruleItem, SYMBOLIZER);
      symbItem->setText(0, tr(symbs[j]->getType().c_str()));
      symbItem->setData(0, Qt::UserRole, (int)j);
      symbItem->setIcon(0, QIcon(SymbologyPreview::build(symbs[j], iconSize())));
    }
  }

  expandAll();
}

te::se::Rule* te::qt::widgets::StyleExplorer::getRule(QTreeWidgetItem* item) const
{
  assert(item && item->type() == RULE);
  
  // Gets the Rule index
  std::size_t index = item->data(0, Qt::UserRole).toUInt();
  assert(index >= 0 && index < m_style->getNRules());
  
  return m_style->getRule(index);
}

te::se::Symbolizer* te::qt::widgets::StyleExplorer::getSymbolizer(QTreeWidgetItem* item) const
{
  assert(item && item->type() == SYMBOLIZER);

  // Gets the rule associated with the symbolizer
  QTreeWidgetItem* parent = item->parent();
  assert(parent);
  const te::se::Rule* rule = getRule(parent);
  assert(rule);

  // Gets the rule symbolizers
  std::vector<te::se::Symbolizer*> symbs = rule->getSymbolizers();

  // Gets the Symbolizer index
  std::size_t index = item->data(0, Qt::UserRole).toUInt();
  assert(index >= 0 && index < symbs.size());

  return symbs[index];
}

QTreeWidgetItem* te::qt::widgets::StyleExplorer::getSelectedItem() const
{
  QList<QTreeWidgetItem*> selected = selectedItems();
  if(selected.empty())
    return 0;

  return selected.at(0);
}

int te::qt::widgets::StyleExplorer::getSymbolizerIndex(te::se::Rule* r, te::se::Symbolizer* s) const
{
  assert(r);
  assert(s);

  for(size_t t = 0; t < r->getSymbolizers().size(); ++t)
  {
    if(s == r->getSymbolizers()[t])
      return t;
  }

  return -1;
}

void te::qt::widgets::StyleExplorer::swapSymbolizers(te::se::Rule* r, int indexFirst, int indexSecond)
{
  assert(r);

  // Swap symbolizers
  te::se::Symbolizer* first = r->getSymbolizer(indexFirst)->clone();
  te::se::Symbolizer* second = r->getSymbolizer(indexSecond)->clone();

  r->setSymbolizer(indexFirst,second);
  r->setSymbolizer(indexSecond, first);
}

void te::qt::widgets::StyleExplorer::onItemClicked(QTreeWidgetItem* item, int /*column*/)
{
  switch(item->type())
  {
    case STYLE:
    break;

    case RULE:
      emit ruleClicked(getRule(item));
    break;

    case SYMBOLIZER:
      emit symbolizerClicked(getSymbolizer(item));
    break;
  }
}

void te::qt::widgets::StyleExplorer::onSymbolizerChanged(te::se::Symbolizer* symb)
{
  QTreeWidgetItem* symbolizerItem = getSelectedItem();
  std::size_t index = symbolizerItem->data(0, Qt::UserRole).toUInt();

  const te::se::Rule* rule = getCurrentRule();
  assert(rule);

  // rule->setSymbolizer(index, symb);

  // Updating item
  symbolizerItem->setText(0, tr(symb->getType().c_str()));
  symbolizerItem->setIcon(0, QIcon(SymbologyPreview::build(symb, iconSize())));
}
