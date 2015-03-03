/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file PrintScene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "PrintScene.h"
#include "../../core/pattern/mvc/ItemObserver.h"
#include "../../core/PaperConfig.h"
#include "../../core/pattern/singleton/Context.h"
#include "Scene.h"

// STL
#include <sstream>

// Qt
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>

te::layout::PrintScene::PrintScene( QGraphicsScene* scene, PaperConfig* config ):
  m_scene(scene),
  m_config(config),
  m_printState(te::layout::NoPrinter)
{
 
}

te::layout::PrintScene::~PrintScene()
{

}

void te::layout::PrintScene::printPreview()
{
  if(!m_scene)
    return;

  QPrinter* printer = createPrinter();

  QPrintPreviewDialog *preview = new QPrintPreviewDialog(printer);
  connect(preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPaper(QPrinter*)));

  if(preview->exec() == QDialog::Rejected || m_printState == te::layout::PrintingScene)
  {
    if(printer)
    {
      delete printer;
      printer = 0;
    }
  }

  if(preview)
  {
    delete preview;
    preview = 0;

    Scene* sc = dynamic_cast<Scene*>(m_scene);
    if(sc)
    {
      sc->redrawItems();
    }
  }
}

void te::layout::PrintScene::printPaper( QPrinter* printer )
{
  if(!printer)
    return;

  //Impressão de parte da Cena
  //Não é necessário mudar a escala do View

  QPainter newPainter(printer);
  newPainter.setRenderHint(QPainter::Antialiasing);

  double dpiX = Context::getInstance().getDpiX();
  double dpiY = Context::getInstance().getDpiY();

  Context::getInstance().setDpiX(printer->logicalDpiX());
  Context::getInstance().setDpiY(printer->logicalDpiY());

  double zoomFactor = Context::getInstance().getZoomFactor();
  Context::getInstance().setZoomFactor(1.);

  renderScene(&newPainter, printer);

  Context::getInstance().setZoomFactor(zoomFactor);
  Context::getInstance().setDpiX(dpiX);
  Context::getInstance().setDpiY(dpiY);
}

QPrinter* te::layout::PrintScene::createPrinter()
{
  if(!m_scene)
    return 0;

  PaperConfig* conf = Context::getInstance().getPaperConfig();

  if(!conf)
    return 0;

  double w = 0;
  double h = 0;
  conf->getPaperSize(w, h);

  QPrinter* printer = new QPrinter(QPrinter::HighResolution);
  QSizeF sf(w, h);
  printer->setPaperSize(sf, QPrinter::Millimeter);

  if(conf->getPaperOrientantion() == Portrait)
  {
    printer->setOrientation( QPrinter::Portrait );
  }
  else
  {
    printer->setOrientation( QPrinter::Landscape );
  }
  printer->pageRect(QPrinter::Millimeter);

  return printer;
}

void te::layout::PrintScene::renderScene( QPainter* newPainter, QPrinter* printer )
{
  if(!m_scene)
    return;

  if(!newPainter)
    return;

  if(!printer)
    return;

  if(m_printState == NoPrinter)
  {
    m_printState = PreviewScene;
  }
  else if(m_printState == PreviewScene)
  {
    m_printState = PrintingScene;
  }
  
  double w = 0;
  double h = 0;

  PaperConfig* conf = Context::getInstance().getPaperConfig();
  conf->getPaperSize(w, h);
        
  //Box Paper in the Scene (Source)
  QRectF mmSourceRect(0, 0, w, h);
  
  //Paper size using the printer dpi (Target)
  QRect pxTargetRect(0, 0, newPainter->device()->width(), newPainter->device()->height());

  /* Print Paper (Scene to Printer)
  draw items with printer painter */
  
  //Convert Paper Size world to screen coordinate. Uses dpi printer.
  QSizeF paperPixelBox = printer->paperSize(QPrinter::DevicePixel);

  //Mirroring Y-Axis
  newPainter->translate( paperPixelBox.width() / 2, paperPixelBox.height() / 2 );
  newPainter->scale( 1, -1 );
  newPainter->translate( -(paperPixelBox.width() / 2), -(paperPixelBox.height() / 2) );

  deselectAllItems();

  m_scene->render(newPainter, pxTargetRect, mmSourceRect); 
}

bool te::layout::PrintScene::exportToPDF()
{ 
  QWidget* wg = (QWidget*)QApplication::desktop();
  QString fileName = QFileDialog::getSaveFileName(wg, tr("Save Image File"), QDir::currentPath(), tr("PDF Files (*.pdf)"));

  if(fileName.isEmpty())
  {
    return false;
  }

  QPrinter* printer= createPrinter();

  printer->setOutputFormat(QPrinter::PdfFormat);
  printer->setOutputFileName(fileName); 
  printPaper(printer);

  if(printer)
  {
    delete printer;
    printer = 0;
  } 

  QMessageBox msgBox;
  msgBox.setIcon(QMessageBox::Information);
  msgBox.setText("PDF exported successfully!");    
  msgBox.exec();

  return true;
}

void te::layout::PrintScene::deselectAllItems()
{
  if(!m_scene)
    return;

  QList<QGraphicsItem*> graphicsItems = m_scene->items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      item->setSelected(false);
    }
  }
}
