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

#include "GraphicPropertyItem.h"
#include "AbstractPropertyManager.h"

#include "GlyphMarkPropertyItem.h"
#include "LocalImagePropertyItem.h"
#include "WellKnownMarkPropertyItem.h"

#include "../../../maptools/Utils.h"
#include "../../../se.h"
#include "../../../xlink/SimpleLink.h"


// Qt
#include "../../../../../third-party/qt/propertybrowser/qtpropertybrowser.h"

// STL
#include <cassert>


te::qt::widgets::GraphicPropertyItem::GraphicPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c),
  m_graphic(new te::se::Graphic), m_setLocalGraphic(false)
{
  //build property browser basic stroke
  QtProperty* generalProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Graphic"));

  //size
  m_sizeProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Size"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_sizeProperty, 16);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMinimum(m_sizeProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMaximum(m_sizeProperty, 1024);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setSingleStep(m_sizeProperty, 1);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setDecimals(m_sizeProperty, 2);
  generalProperty->addSubProperty(m_sizeProperty);

  //angle
  m_angleProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->addProperty(tr("Angle"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_angleProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMinimum(m_angleProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setMaximum(m_angleProperty, 360);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setSingleStep(m_angleProperty, 10);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setDecimals(m_angleProperty, 2);
  generalProperty->addSubProperty(m_angleProperty);

  //opacity
  m_opacityProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->addProperty(tr("Opacity"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMinimum(m_opacityProperty, 0);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setMaximum(m_opacityProperty, 100);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setSingleStep(m_opacityProperty, 10);
  generalProperty->addSubProperty(m_opacityProperty);

  //displacement
  m_displacementProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->addProperty(tr("Displacement"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setValue(m_displacementProperty, QPointF(0., 0.));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setDecimals(m_displacementProperty, 2);
  generalProperty->addSubProperty(m_displacementProperty);

  //anchor
  m_anchorProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->addProperty(tr("Anchor"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setValue(m_anchorProperty, QPointF(0.5, 0.5));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setDecimals(m_anchorProperty, 2);
  generalProperty->addSubProperty(m_anchorProperty);

  addProperty(generalProperty, tr("Graphic"), QColor(210, 210, 210));

  m_mp = new te::qt::widgets::WellKnownMarkPropertyItem(pb);
  m_gp = new te::qt::widgets::GlyphMarkPropertyItem(pb);
  m_li = new te::qt::widgets::LocalImagePropertyItem(pb);

  connect(m_mp, SIGNAL(markChanged()), this, SLOT(onWellKnownMarkChanged()));
  connect(m_gp, SIGNAL(markChanged()), this, SLOT(onGlyphMarkChanged()));
  connect(m_li, SIGNAL(externalGraphicChanged()), this, SLOT(onLocalImageChanged()));

  // Setups initial graphic
  m_graphic->add(m_mp->getMark());
}

te::qt::widgets::GraphicPropertyItem::~GraphicPropertyItem()
{
}

bool te::qt::widgets::GraphicPropertyItem::setGraphic(const te::se::Graphic* graphic)
{
  assert(graphic);

  delete m_graphic;

  m_setLocalGraphic = true;

  m_graphic = graphic->clone();

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

  updateUi();

  m_setLocalGraphic = false;

  return true;
}

te::se::Graphic* te::qt::widgets::GraphicPropertyItem::getGraphic() const
{
  return m_graphic->clone();
}

void te::qt::widgets::GraphicPropertyItem::valueChanged(QtProperty *p, int value)
{
  if(m_setLocalGraphic)
    return;

  if(p == m_opacityProperty)
  {
    double opacity = value / 100.0;
    m_graphic->setOpacity(new te::se::ParameterValue(QString::number(opacity, 'g', 2).toStdString()));
    emit graphicChanged();
  }
}

void te::qt::widgets::GraphicPropertyItem::valueChanged(QtProperty *p, double value)
{
  if(m_setLocalGraphic)
    return;

  QString valueStr;
  valueStr.setNum(value);

  if(p == m_sizeProperty)
  {
    m_graphic->setSize(new te::se::ParameterValue(valueStr.toStdString()));
    emit graphicChanged();
  }
  else if(p == m_angleProperty)
  {
    m_graphic->setRotation(new te::se::ParameterValue(valueStr.toStdString()));
    emit graphicChanged();
  }
}

void te::qt::widgets::GraphicPropertyItem::valueChanged(QtProperty* p, const QPointF &value)
{
  if(m_setLocalGraphic)
    return;

  QString xStr, yStr;
  xStr.setNum(value.x());
  yStr.setNum(value.y());

  if(p == m_displacementProperty)
  {
    te::se::Displacement* disp = new te::se::Displacement;
    disp->setDisplacementX(new te::se::ParameterValue(xStr.toStdString()));
    disp->setDisplacementY(new te::se::ParameterValue(yStr.toStdString()));
    m_graphic->setDisplacement(disp);

    emit graphicChanged();
  }
  else if(p == m_anchorProperty)
  {
    te::se::AnchorPoint* ac = new te::se::AnchorPoint;
    ac->setAnchorPointX(new te::se::ParameterValue(xStr.toStdString()));
    ac->setAnchorPointY(new te::se::ParameterValue(yStr.toStdString()));
    m_graphic->setAnchorPoint(ac);

    emit graphicChanged();
  }
}

void te::qt::widgets::GraphicPropertyItem::updateUi()
{
  // Size
  const te::se::ParameterValue* size = m_graphic->getSize();
  if(size)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_sizeProperty, te::map::GetDouble(size));

  // Rotation
  const te::se::ParameterValue* rotation = m_graphic->getRotation();
  if(rotation)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_doubleManager->setValue(m_angleProperty, te::map::GetDouble(rotation));

  // Opacity
  const te::se::ParameterValue* opacity = m_graphic->getOpacity();
  if(opacity)
    te::qt::widgets::AbstractPropertyManager::getInstance().m_intSliderManager->setValue(m_opacityProperty, te::map::GetDouble(opacity) * 100);

  // Displacement
  const te::se::Displacement* disp = m_graphic->getDisplacement();
  if(disp)
  {
    const te::se::ParameterValue* dispx = disp->getDisplacementX();
    const te::se::ParameterValue* dispy = disp->getDisplacementY();
    if(dispx && dispy)
      te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setValue(m_displacementProperty, 
        QPointF(te::map::GetDouble(dispx), te::map::GetDouble(dispy)));
  }

  // Anchor Point
  const te::se::AnchorPoint* ac = m_graphic->getAnchorPoint();
  if(ac)
  {
    const te::se::ParameterValue* acx = ac->getAnchorPointX();
    const te::se::ParameterValue* acy = ac->getAnchorPointY();
    if(acx && acy)
      te::qt::widgets::AbstractPropertyManager::getInstance().m_pointFManager->setValue(m_anchorProperty, 
        QPointF(te::map::GetDouble(acx), te::map::GetDouble(acy)));
  }
}

void te::qt::widgets::GraphicPropertyItem::onWellKnownMarkChanged()
{
  te::se::Mark* mark = m_mp->getMark();
  if(!mark)
    return;

  m_graphic->clear();
  m_graphic->add(mark);


  if(!m_setLocalGraphic)
    emit graphicChanged();
}

void te::qt::widgets::GraphicPropertyItem::onGlyphMarkChanged()
{
  te::se::Mark* mark = m_gp->getMark();
  if(!mark)
    return;

  m_graphic->clear();
  m_graphic->add(mark);
 

  if(!m_setLocalGraphic)
    emit graphicChanged();
}

void te::qt::widgets::GraphicPropertyItem::onLocalImageChanged()
{
  te::se::ExternalGraphic* eg = m_li->getExternalGraphic();
  if(!eg)
    return;

  m_graphic->clear();
  m_graphic->add(eg);

  if(!m_setLocalGraphic)
    emit graphicChanged();
}
