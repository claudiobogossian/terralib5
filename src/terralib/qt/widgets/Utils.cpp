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
  \file QtUtils.cpp
   
  \brief This file contains utility functions for dealing with Qt controls.
 */

// TerraLib
#include "Utils.h"

// Qt
#include <QApplication>
#include <QtGui/QImage>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QTreeWidgetItemIterator>

void te::qt::widgets::SetChildrenCheckState(QTreeWidgetItem* item, int column, Qt::CheckState state)
{
  if(item && item->child(0))
  {
    QTreeWidgetItemIterator it(item->child(0));
  
    while(*it)
    {
      (*it)->setCheckState(column, state);

      ++it;
    }
  }
}

QImage* te::qt::widgets::GetImage(te::color::RGBAColor** img, int width, int height)
{
  QImage* qimg = new QImage(width, height, QImage::Format_ARGB32);

  for(int i = 0; i < height; ++i)
  {
    unsigned char* u = qimg->scanLine(i);

    for(int j = 0; j < width; ++j)
    {
      te::color::RGBAColor c = img[i][j];
      QRgb val = qRgba(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha());
      QRgb* v = (QRgb*)(u+j*4);
      *v = val;
    }
  }

  return qimg;
}

te::color::RGBAColor** te::qt::widgets::GetImage(QImage* img)
{
  te::color::RGBAColor** rgba = new te::color::RGBAColor*[img->height()];
  for(int i = 0; i < img->height(); ++i)
  {
    te::color::RGBAColor* s = new te::color::RGBAColor[img->width()];

    for(int j = 0; j < img->width(); ++j)
      s[j] = img->pixel(j, i);

    rgba[i] = s;
  }

  return rgba;
}

QStyle::StandardPixmap toQStyle(const QMessageBox::Icon& icon)
{
  switch(icon)
  {
    case QMessageBox::Question:
      return QStyle::SP_MessageBoxQuestion;
    break;

    case QMessageBox::Warning:
      return QStyle::SP_MessageBoxWarning;
    break;

    case QMessageBox::Critical:
      return QStyle::SP_MessageBoxCritical;    
    break;

    default:
      return QStyle::SP_MediaVolumeMuted;
    break;
  }
}

QMenu* te::qt::widgets::FindMenu(const QString& mnuText, QMenu* mnu)
{
  return mnu->findChild<QMenu*>(mnuText);
}

QMenu* te::qt::widgets::FindMenu(const QString& mnuText, QMenuBar* bar)
{
  return bar->findChild<QMenu*>(mnuText);
}

//QMenu* GetMenu(const QString& fullName, QMenu* mnu)
//{
//  QStringList ls = fullName.split('.');
//
//  while (!ls.empty())
//  {
//    QMenu* menu = te::qt::widgets::FindMenu(ls[0], mnu);
//
//    if(menu == 0)
//    {
//      menu = new QMenu(mnu);
//      mnu->addMenu(menu);
//
//
//    }
//  }
//}

QMenu* CreateMenu(const QString& mnuName, QMenu* p)
{
  QMenu* mnu = p->findChild<QMenu*>(mnuName);

  if(mnu == 0)
  {
    QStringList ls = mnuName.split("."); 
    mnu = p->addMenu(ls[ls.size()-1]);

    mnu->setObjectName(mnuName);
  }

  return mnu;
}

QMenu* CreateMenu(const QString& mnuName, QMenuBar* p)
{
  QMenu* mnu = p->findChild<QMenu*>(mnuName);

  if(mnu == 0)
  {
    QStringList ls = mnuName.split("."); 
    mnu = p->addMenu(ls[ls.size()-1]);

    mnu->setObjectName(mnuName);
  }

  return mnu;
}

QMenu* te::qt::widgets::GetMenu(const QString& mnuText, QMenu* mnu)
{
  QMenu* res = FindMenu(mnuText, mnu);

  QStringList mnus = mnuText.split('.');

  for(int i=1; i<mnus.size(); i++)
    mnus[i] = mnus[i-1] + "." + mnus[i];

  for(int i=0; i<mnus.size(); i++)
    res = (i == 0) ? 
      CreateMenu(mnus[i], mnu) : 
      CreateMenu(mnus[i], res);

  return res;
}

QMenu* te::qt::widgets::GetMenu(const QString& mnuText, QMenuBar* bar)
{
  QMenu* res = FindMenu(mnuText, bar);

  QStringList mnus = mnuText.split('.');

  for(int i=1; i<mnus.size(); i++)
    mnus[i] = mnus[i-1] + "." + mnus[i];

  for(int i=0; i<mnus.size(); i++)
  {
    res = (i == 0) ?
      CreateMenu(mnus[i], bar) :
      CreateMenu(mnus[i], res);
  }
  return res;
}

QAction* te::qt::widgets::FindAction(const QString& actText, QMenu* mnu)
{
  return mnu->findChild<QAction*>(actText);
}

QAction* te::qt::widgets::FindAction(const QString& actText, QMenuBar* mnuBar)
{
  return mnuBar->findChild<QAction*>(actText);
}
