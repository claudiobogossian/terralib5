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
  \file terralib/qt/widgets/se/Symbol.cpp

  \brief This class represents a symbol.
*/

// TerraLib
#include "../../../common/STLUtils.h"
#include "../../../se/Symbolizer.h"
#include "Symbol.h"

// STL
#include <cassert>

te::qt::widgets::Symbol::Symbol()
{
}

te::qt::widgets::Symbol::Symbol(const te::qt::widgets::Symbol& rhs)
{
  m_info = rhs.getInfo();
  for(std::size_t i = 0; i < rhs.m_symbs.size(); ++i)
    addSymbolizer(rhs.m_symbs[i]->clone());
}

te::qt::widgets::Symbol::~Symbol()
{
  te::common::FreeContents(m_symbs);
}

const te::qt::widgets::SymbolInfo& te::qt::widgets::Symbol::getInfo() const
{
  return m_info;
}

void te::qt::widgets::Symbol::setInfo(const SymbolInfo& info)
{
  m_info = info;
}

std::size_t te::qt::widgets::Symbol::getSymbolizersCount() const
{
  return m_symbs.size();
}

te::se::Symbolizer* te::qt::widgets::Symbol::getSymbolizer(const std::size_t& i) const
{
  assert(i < m_symbs.size());

  return m_symbs[i];
}

te::se::Symbolizer* te::qt::widgets::Symbol::operator[](const std::size_t& i) const
{
  return getSymbolizer(i);
}

const std::vector<te::se::Symbolizer*>& te::qt::widgets::Symbol::getSymbolizers() const
{
  return m_symbs;
}

void te::qt::widgets::Symbol::addSymbolizer(te::se::Symbolizer* symb)
{
  assert(symb);

  m_symbs.push_back(symb);
}

void te::qt::widgets::Symbol::setSymbolizer(const std::size_t& i, te::se::Symbolizer* symb)
{
  assert(symb);
  assert(i < m_symbs.size());

  delete m_symbs[i];
  m_symbs[i] = symb;
}

void te::qt::widgets::Symbol::removeSymbolizer(const std::size_t& i)
{
  assert(i < m_symbs.size());

  delete m_symbs[i];
  m_symbs.erase(m_symbs.begin() + i);
}

void te::qt::widgets::Symbol::swapSymbolizers(const std::size_t& first, const std::size_t& second)
{
   assert(first < m_symbs.size());
   assert(second < m_symbs.size());

   if(first == second)
     return;

  // Swapping...
  te::se::Symbolizer* symb = m_symbs[first];
  m_symbs[first] = m_symbs[second];
  m_symbs[second] = symb;
}

te::qt::widgets::Symbol* te::qt::widgets::Symbol::clone() const
{
  return new Symbol(*this);
}
