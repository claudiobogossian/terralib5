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
  \file terralib/qt/widgets/help/HelpManager.h

  \brief A singleton that contains a pointer to a help manager implementation.
*/

// TerraLib
#include "HelpManager.h"
#include "HelpManagerImpl.h"

void te::qt::widgets::HelpManager::setImpl(HelpManagerImpl* impl)
{
  m_impl = impl;
}

te::qt::widgets::HelpManagerImpl* te::qt::widgets::HelpManager::getImpl() const
{
  return m_impl;
}

void te::qt::widgets::HelpManager::showHelp(const QString& htmRef, const QString& nSpace)
{
  if(m_impl != 0)
    m_impl->showHelp(htmRef, nSpace);
}

void te::qt::widgets::HelpManager::appendDoc(const QString& docRef)
{
  if(m_impl != 0)
    m_impl->appendDoc(docRef);
}

te::qt::widgets::HelpManager::HelpManager()
  : te::common::Singleton<HelpManager>(),
    m_impl(0)
{
}

