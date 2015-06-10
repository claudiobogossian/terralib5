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
  \file QtUtils.cpp
   
  \brief This file contains utility functions for dealing with Qt controls.
 */

// TerraLib
#include "../../common/STLUtils.h"
#include "../../maptools/MarkRendererManager.h"
#include "../../maptools/Canvas.h"
#include "../../se/Fill.h"
#include "../../se/Mark.h"
#include "../../se/Stroke.h"
#include "../../se/Utils.h"
#include "../../dataaccess/datasource/DataSourceCapabilities.h"
#include "../../dataaccess/datasource/DataSourceFactory.h"
#include "../../dataaccess/datasource/DataSource.h"
#include "../../common/StringUtils.h"
#include "Utils.h"

// Qt
#include <QApplication>
#include <QActionGroup>
#include <QImage>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QString>

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

QAction* te::qt::widgets::FindAction(const QString& actText, QMenu* mnu)
{
  return mnu->findChild<QAction*>(actText);
}

QAction* te::qt::widgets::FindAction(const QString& actText, QMenuBar* mnuBar)
{
  return mnuBar->findChild<QAction*>(actText);
}

QActionGroup* te::qt::widgets::FindActionGroup(const QString& actGroupText, QMenu* mnu)
{
  return mnu->findChild<QActionGroup*>(actGroupText);
}

QActionGroup* te::qt::widgets::FindActionGroup(const QString& actGroupText, QMenuBar* mnuBar)
{
  return mnuBar->findChild<QActionGroup*>(actGroupText);
}

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
  QMenu* mnu = te::qt::widgets::FindMenu(mnuName, p);

  if(mnu == 0)
  {
    QStringList ls = mnuName.split("."); 
    mnu = new QMenu(ls[ls.size()-1], p);
    mnu->setObjectName(mnuName);

    QMenu* helpMnu = te::qt::widgets::FindMenu("Help", p);

    QAction* act = (helpMnu == 0) ? 0 : helpMnu->menuAction();
     
    p->insertMenu(act, mnu);
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

te::color::RGBAColor te::qt::widgets::Convert2TerraLib(const QColor& color)
{
  return te::color::RGBAColor(color.red(), color.green(), color.blue(), color.alpha());
}

QColor te::qt::widgets::Convert2Qt(const te::color::RGBAColor& color)
{
  QColor qcolor(color.getRgba());
  qcolor.setAlpha(qAlpha(color.getRgba()));
  return qcolor;
}

void te::qt::widgets::Config2DrawPolygons(te::map::Canvas* canvas, const QColor& fillColor, const QColor& contourColor, const std::size_t& contourWidth)
{
  canvas->setPolygonContourWidth(contourWidth);
  canvas->setPolygonContourColor(Convert2TerraLib(contourColor));
  canvas->setPolygonFillColor(Convert2TerraLib(fillColor));
}

void te::qt::widgets::Config2DrawLines(te::map::Canvas* canvas, const QColor& color, const std::size_t& width)
{
  canvas->setLineWidth(width);
  canvas->setLineColor(Convert2TerraLib(color));
}

void te::qt::widgets::Config2DrawPoints(te::map::Canvas* canvas, const QColor& color, const std::size_t& width)
{
  canvas->setPointWidth(width);
  canvas->setPointColor(Convert2TerraLib(color));
}

void te::qt::widgets::Config2DrawPoints(te::map::Canvas* canvas, const QString& markName, const std::size_t& size,
                                        const QColor& fillColor, const QColor& contourColor, const std::size_t& contourWidth)
{
  te::se::Stroke* stroke = te::se::CreateStroke(contourColor.name().toStdString(),
                                                QString::number(contourWidth).toStdString(),
                                                QString::number(contourColor.alphaF()).toStdString());

  te::se::Fill* fill = te::se::CreateFill(fillColor.name().toStdString(),
                                          QString::number(fillColor.alphaF()).toStdString());

  te::se::Mark* mark = te::se::CreateMark(markName.toStdString(), stroke, fill);

  te::color::RGBAColor** rgba = te::map::MarkRendererManager::getInstance().render(mark, size);

  canvas->setPointColor(te::color::RGBAColor(0, 0, 0, TE_TRANSPARENT));
  canvas->setPointPattern(rgba, size, size);

  te::common::Free(rgba, size);

  delete mark;
}

void te::qt::widgets::Config2DrawLayerSelection(te::map::Canvas* canvas, const QColor& selectionColor, const te::gm::GeomType& type)
{
  switch(type)
  {
    case te::gm::PolygonType:
    case te::gm::PolygonZType:
    case te::gm::PolygonMType:
    case te::gm::PolygonZMType:
    case te::gm::MultiPolygonType:
    case te::gm::MultiPolygonZType:
    case te::gm::MultiPolygonMType:
    case te::gm::MultiPolygonZMType:
    {
      QColor fillColor = selectionColor;
      fillColor.setAlpha(128);

      Config2DrawPolygons(canvas, fillColor, Qt::black, 2);
    }
    break;

    case te::gm::LineStringType:
    case te::gm::LineStringZType:
    case te::gm::LineStringMType:
    case te::gm::LineStringZMType:
    case te::gm::MultiLineStringType:
    case te::gm::MultiLineStringZType:
    case te::gm::MultiLineStringMType:
    case te::gm::MultiLineStringZMType:
    {
      QColor fillColor = selectionColor;
      fillColor.setAlpha(128);

      Config2DrawLines(canvas, fillColor, 6);
    }
    break;

    case te::gm::PointType:
    case te::gm::PointZType:
    case te::gm::PointMType:
    case te::gm::PointZMType:
    case te::gm::MultiPointType:
    case te::gm::MultiPointZType:
    case te::gm::MultiPointMType:
    case te::gm::MultiPointZMType:
    {
      QColor fillColor = selectionColor;
      fillColor.setAlpha(70);

      QColor contourColor = selectionColor;
      contourColor.setAlpha(150);

      Config2DrawPoints(canvas, "square", 24, fillColor, contourColor,2);
    }
    break;

    default:
      return;
  }
}

QPixmap te::qt::widgets::CreatePixmapIcon(const int& size, const QColor& penColor, const QColor& brushColor, const int& contourSize)
{
  QPixmap pix(size, size);
  pix.fill(Qt::transparent);

  int offset = 2;

  QPainterPath path;
  path.addRect(offset, offset, pix.width() -  2 * offset, pix.height() - 2 * offset);

  QPen pen;
  pen.setColor(penColor);
  pen.setWidth(contourSize);

  QBrush brush;
  brush.setColor(brushColor);

  QPainter p(&pix);
  p.setPen(pen);
  p.setBrush(brushColor);

  p.fillPath(path, brush);
  p.drawPath(path);
    
  return pix;
}

void te::qt::widgets::AddFilePathToSettings(const QString& path, const QString& typeFile)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QString key = "Last used file path/" + typeFile;

  sett.setValue(key, path);
}

QString te::qt::widgets::GetFilePathFromSettings(const QString& typeFile)
{
  QSettings sett(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

  QString key = "Last used file path/" + typeFile;

  return sett.value(key).toString();
}

QString te::qt::widgets::Convert2Qt(const std::string& text, const te::common::CharEncoding& encoding)
{
  try
  {
    switch(encoding)
    {
      case te::common::UNKNOWN_CHAR_ENCODING:
        return text.c_str();

      case te::common::UTF8:
        return QString::fromUtf8(text.c_str());

      case te::common::LATIN1:
        return QString::fromLatin1(text.c_str());

      // continues...

      default:
      {
#ifdef TERRALIB_CHARENCODING_ENABLED
        std::string latin1 = te::common::CharEncodingConv::convert(text, encoding, te::common::LATIN1);
        return QString::fromLatin1(latin1.c_str());
#else
        return text.c_str();
#endif
      }
    }
  }
  catch(...)
  {
    return text.c_str();
  }
}

QString te::qt::widgets::GetDiskRasterFileSelFilter()
{
  QString filter;
  
  if( te::da::DataSourceFactory::find( "GDAL" ) )
  {
    std::auto_ptr< te::da::DataSource > dsPtr = te::da::DataSourceFactory::make( "GDAL" );
    
    if( dsPtr.get() )
    {
      std::map< std::string, std::string > specCap = dsPtr->getCapabilities().getSpecificCapabilities();
      
      if( specCap.find( "SUPPORTED_EXTENSIONS" ) != specCap.end() )
      {
        std::string fileExtensions = specCap[ "SUPPORTED_EXTENSIONS" ];
        std::string extFilter;
        std::string uCaseToken;
        std::string lCaseToken;
        std::vector< std::string > tokens;
        te::common::Tokenize( fileExtensions, tokens, ";" );
        
        for( unsigned int tokensIdx = 0 ; tokensIdx < tokens.size() ; ++tokensIdx )
        {
          uCaseToken = te::common::Convert2UCase( tokens[ tokensIdx ] );
          lCaseToken = te::common::Convert2LCase( tokens[ tokensIdx ] );
          extFilter = uCaseToken + " Raster File (" + "*." + lCaseToken + " *." +
            uCaseToken + ")";
          
          if( uCaseToken == "TIF" )
          {
            filter = QString( extFilter.c_str() ) + ( filter.isEmpty() ? "" : ";;" ) + filter;
          }
          else
          {
            filter += ( filter.isEmpty() ? "" : ";;" ) + QString( extFilter.c_str() );
          }
        }
      }
    }
  }
  
  filter += QString( ( filter.isEmpty() ? "" : ";;" ) ) + 
    "Web Map Service - WMS (*.xml *.wms);;Web Coverage Service - WCS (*.xml *.wcs);;All Files (*.*)";
  
  return filter;
}

