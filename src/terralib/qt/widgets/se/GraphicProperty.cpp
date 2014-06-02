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
  \file terralib/qt/widgets/se/GraphicPropertyItem.cpp

  \brief A widget used to define the general properties of a se object.
*/

// Terralib
#include "../../../maptools/Utils.h"
#include "../../../se.h"
#include "../../../xlink/SimpleLink.h"
#include "GraphicProperty.h"
#include "GraphicPropertyItem.h"
#include "GlyphMarkPropertyItem.h"
#include "LocalImagePropertyItem.h"
#include "WellKnownMarkPropertyItem.h"

// Qt
#include <QGridLayout>
#include <QToolBox>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>


te::qt::widgets::GraphicProperty::GraphicProperty(QWidget* widget) :
  m_setLocalGraphic(false), m_graphic(new te::se::Graphic)
{
  QGridLayout* layout = new QGridLayout(this);
  this->setLayout(layout);

  QToolBox* tb = new QToolBox(this);
  layout->addWidget(tb);

  QtTreePropertyBrowser* graphBrowser = new QtTreePropertyBrowser(this);
  graphBrowser->setIndentation(10);
  graphBrowser->setPropertiesWithoutValueMarked(true);
  graphBrowser->setRootIsDecorated(false);
  graphBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  m_graphp = new te::qt::widgets::GraphicPropertyItem(graphBrowser);
  tb->addItem(graphBrowser, "Graphic Properties");

  QtTreePropertyBrowser* wkBrowser = new QtTreePropertyBrowser(this);
  wkBrowser->setIndentation(10);
  wkBrowser->setPropertiesWithoutValueMarked(true);
  wkBrowser->setRootIsDecorated(false);
  wkBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);
  
  m_mp = new te::qt::widgets::WellKnownMarkPropertyItem(wkBrowser);
  tb->addItem(wkBrowser, "Well Known Mark");

  QtTreePropertyBrowser* gpBrowser = new QtTreePropertyBrowser(this);
  gpBrowser->setIndentation(10);
  gpBrowser->setPropertiesWithoutValueMarked(true);
  gpBrowser->setRootIsDecorated(false);
  gpBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  m_gp = new te::qt::widgets::GlyphMarkPropertyItem(gpBrowser);
  tb->addItem(gpBrowser, "Glyph Mark");

  QtTreePropertyBrowser* liBrowser = new QtTreePropertyBrowser(this);
  liBrowser->setIndentation(10);
  liBrowser->setPropertiesWithoutValueMarked(true);
  liBrowser->setRootIsDecorated(false);
  liBrowser->setResizeMode(QtTreePropertyBrowser::ResizeToContents);

  m_li = new te::qt::widgets::LocalImagePropertyItem(liBrowser);
  tb->addItem(liBrowser, "Local Image");

  connect(m_graphp, SIGNAL(graphicChanged()), this, SLOT(onGraphicPropertyChanged()));
  connect(m_mp, SIGNAL(markChanged()), this, SLOT(onWellKnownMarkChanged()));
  connect(m_gp, SIGNAL(markChanged()), this, SLOT(onGlyphMarkChanged()));
  connect(m_li, SIGNAL(externalGraphicChanged()), this, SLOT(onLocalImageChanged()));

  // Setups initial graphic
  m_graphic->add(m_mp->getMark());
}

te::qt::widgets::GraphicProperty::~GraphicProperty()
{
}

bool te::qt::widgets::GraphicProperty::setGraphic(const te::se::Graphic* graphic)
{
  assert(graphic);

  delete m_graphic;

  m_setLocalGraphic = true;

  m_graphic = graphic->clone();

  m_graphp->setGraphic(m_graphic);

  // Verifying if this widget can deal with the given graphic...
  const std::vector<te::se::Mark*> marks = m_graphic->getMarks();
  if(marks.empty() == false)
  {
    te::se::Mark* mark = marks[0];
    const std::string* name = mark->getWellKnownName();
    if(name != 0)
    {
      std::size_t found = name->find("ttf://");
      if(found != std::string::npos)
        m_gp->setMark(marks[0]);

      found = name->find("://");
      if(found == std::string::npos)
        m_mp->setMark(marks[0]);
    }
  }

  const std::vector<te::se::ExternalGraphic*> extGraphics = m_graphic->getExternalGraphics();
  if(extGraphics.empty() == false)
  {
    te::se::ExternalGraphic* g = extGraphics[0];
    const te::xl::SimpleLink* link = g->getOnlineResource();
    if(link == 0)
      return false;
  
    const std::string href = link->getHref();
    if(href.empty())
      return false;

    QImage img;
    if(!img.load(href.c_str()))
      return false;

    // I know it!
    m_li->setExternalGraphic(g);
  }

  m_setLocalGraphic = false;

  return true;
}

te::se::Graphic* te::qt::widgets::GraphicProperty::getGraphic() const
{
  return m_graphic->clone();
}

void te::qt::widgets::GraphicProperty::onGraphicPropertyChanged()
{
  te::se::Graphic* g = m_graphp->getGraphic();
  
  if(g->getSize())
    m_graphic->setSize(g->getSize()->clone());

  if(g->getRotation())
    m_graphic->setRotation(g->getRotation()->clone());

  if(g->getOpacity())
    m_graphic->setOpacity(g->getOpacity()->clone());

  if(!m_setLocalGraphic)
    emit graphicChanged();
}

void te::qt::widgets::GraphicProperty::onWellKnownMarkChanged()
{
  te::se::Mark* mark = m_mp->getMark();
  if(!mark)
    return;

  m_graphic->clear();
  m_graphic->add(mark);


  if(!m_setLocalGraphic)
    emit graphicChanged();
}

void te::qt::widgets::GraphicProperty::onGlyphMarkChanged()
{
  te::se::Mark* mark = m_gp->getMark();
  if(!mark)
    return;

  m_graphic->clear();
  m_graphic->add(mark);
 

  if(!m_setLocalGraphic)
    emit graphicChanged();
}

void te::qt::widgets::GraphicProperty::onLocalImageChanged()
{
  te::se::ExternalGraphic* eg = m_li->getExternalGraphic();
  if(!eg)
    return;

  m_graphic->clear();
  m_graphic->add(eg);

  if(!m_setLocalGraphic)
    emit graphicChanged();
}
