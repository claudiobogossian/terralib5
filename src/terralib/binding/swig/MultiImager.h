/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file MultiImager.h

  \brief 
*/

#include <terralib/qt/widgets/canvas/MultiThreadMapDisplay.h>

// Qt 
#include <QtCore/QEventLoop>
#include <QtCore/QObject>
//#include <QtCore/QThread>
#include <QtCore/QTimer>

// STL 
#include <memory>
#include <iostream>

class MultiImager : public QObject
{
  Q_OBJECT

  public:

    MultiImager(QObject* parent=0);

    ~MultiImager();

    void setMapDisplay(te::qt::widgets::MultiThreadMapDisplay* display);

    void refresh();

  protected:

    std::auto_ptr<te::qt::widgets::MultiThreadMapDisplay> m_display;
};

inline MultiImager::MultiImager(QObject* parent) :
QObject(parent)
{
}

inline MultiImager::~MultiImager()
{
}

inline void MultiImager::setMapDisplay(te::qt::widgets::MultiThreadMapDisplay* display)
{
  m_display.reset(display);
}

inline void MultiImager::refresh() 
{
  m_display->refresh();

  QEventLoop q;
  QTimer tT;

  tT.setSingleShot(true);

  connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
  connect(m_display.get(), SIGNAL(drawLayersFinished(const QMap<QString, QString>&)), &q, SLOT(quit()));

  tT.start(300000);
  q.exec();

  while (true)
    if(tT.isActive())
    {
      tT.stop();
      return;
    }
 }
