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
  \file terralib/qt/af/SplashScreenManager.cpp

  \brief A singleton for holding he application splash screen.
*/

// TerraLib
#include "SplashScreenManager.h"

// Qt
#include <QtCore/QCoreApplication>

void te::qt::af::SplashScreenManager::showMessage(const QString& message)
{
  if(m_sc == 0)
    return;

  m_sc->showMessage(message, m_msgAlignment, m_msgColor);

  QCoreApplication::processEvents(); 
}

void te::qt::af::SplashScreenManager::set(QSplashScreen* impl, int alignment, const QColor& color)
{
  m_sc = impl;
  m_msgAlignment = alignment;
  m_msgColor = color;
}

void te::qt::af::SplashScreenManager::setLogo(const QPixmap& pix)
{
  if(m_sc != 0)
    m_sc->setPixmap(pix);
}

void te::qt::af::SplashScreenManager::close()
{
  if(m_sc != 0 && m_sc->isVisible())
    m_sc->hide();
}

te::qt::af::SplashScreenManager::SplashScreenManager()
  : m_sc(0),
    m_msgAlignment(Qt::AlignLeft),
    m_msgColor(Qt::black)
{
}

te::qt::af::SplashScreenManager::~SplashScreenManager()
{
}

