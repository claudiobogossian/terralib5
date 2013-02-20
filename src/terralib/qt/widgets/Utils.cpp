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

  for(int i = 0; i < width; ++i)
  {
    unsigned char* u = qimg->scanLine(i);

    for(int j = 0; j < height; ++j)
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

QMenu* SearchMenu(const QString& mnuText, QMenu* mnu)
{
  if(mnu->objectName() == mnuText)
    return mnu;

  QList<QMenu*> objs = mnu->findChildren<QMenu*>();
  
  if(!objs.isEmpty())
  {
    QList<QMenu*>::iterator it;
    
    for(it = objs.begin(); it != objs.end(); ++it)
    {
      QMenu* res = SearchMenu(mnuText, *it);

      if(res != 0)
        return res;
    }
  }

  return 0;
}

QMenu* te::qt::widgets::FindMenu(const QString& mnuText, QMenu* mnu)
{
  if(mnu->objectName() == mnuText)
    return mnu;

  if(mnuText.isEmpty())
    return 0;

  QStringList m_txts = mnuText.split(".");
  QString mnuName = m_txts.at(0);

  QMenu* sub_m = SearchMenu(mnuName, mnu);

  if(m_txts.size() > 1)
    return FindMenu(mnuText.mid(mnuName.size()+1), sub_m);

  return sub_m;
}

QMenu* te::qt::widgets::FindMenu(const QString& mnuText, QMenuBar* bar)
{
  QList<QMenu*> mnus = bar->findChildren<QMenu*>();
  QList<QMenu*>::iterator it;
  QMenu* sub = 0;

  for(it = mnus.begin(); it != mnus.end(); ++it)
  {
    sub = FindMenu(mnuText, *it);

    if(sub != 0)
      break;
  }

  return sub;
}

QMenu* te::qt::widgets::GetMenu(const QString& mnuText, QMenu* mnu)
{
  QStringList m_txts = mnuText.split(".");
  QMenu* sub;

  QString mnuName = m_txts.at(0);
  sub = SearchMenu(mnuName, mnu);

  if(sub == 0)
    sub = mnu->addMenu(mnuName);

  return ((m_txts.size() > 1) ? GetMenu(mnuText.mid(mnuName.size()+1), sub) : sub);
}

QMenu* te::qt::widgets::GetMenu(const QString& mnuText, QMenuBar* bar)
{
  QStringList m_txts = mnuText.split(".");
  QList<QMenu*> mnus = bar->findChildren<QMenu*>();
  QList<QMenu*>::iterator it;
  QMenu* sub = 0;
  QString mnuName = m_txts.at(0);

  for(it = mnus.begin(); it != mnus.end(); ++it)
    if((*it)->objectName() == mnuName)
    {
      sub = *it;
      break;
    }

  if(sub == 0)
    sub = bar->addMenu(mnuName);

  return ((m_txts.size() > 1) ? GetMenu(mnuText.mid(mnuName.size()+1), sub) : sub);
}

QAction* SearchAction(QMenu* mnu, const QString& actName)
{
  QList<QAction*> acts = mnu->actions();

  for(int i=0; i<acts.size(); i++)
  {
    QAction* act = acts.at(i);

    if(act->objectName() == actName)
      return act;
  }

  return 0;
}

QAction* te::qt::widgets::FindAction(const QString& actText, QMenu* mnu)
{
  int pos = actText.lastIndexOf(".");
  QString mnuName = actText.left(pos);
  QString actionName = actText.mid(pos+1);

  QMenu* sub_mnu = ((pos > 0) ? FindMenu(mnuName, mnu) : mnu);

  return SearchAction(sub_mnu, actionName);
}

QAction* te::qt::widgets::FindAction(const QString& actText, QMenuBar* mnuBar)
{
  QList<QMenu*> mnus = mnuBar->findChildren<QMenu*>();

  for(int i=0; i<mnus.size(); i++)
  {
    QAction* act = FindAction(actText, mnus.at(i));

    if(act != 0)
      return act;
  }

  return 0;
}
