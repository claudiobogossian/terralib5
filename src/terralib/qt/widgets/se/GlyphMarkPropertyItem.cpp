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
  \file terralib/qt/widgets/se/GlyphMarkPropertyItem.cpp

  \brief A widget used to define a glyph object.
*/

// TerraLib
#include "../../../se/Config.h"
#include "../../../se/Mark.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "BasicFillPropertyItem.h"
#include "CharMapWidget.h"
#include "GlyphMarkPropertyItem.h"
#include "GlyphMarkRenderer.h"

// Qt
#include <QDialog>
#include <QGridLayout>
#include <QScrollArea>
#include <QPushButton>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>

// STL
#include <cassert>


te::qt::widgets::GlyphMarkPropertyItem::GlyphMarkPropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c) ,
  m_mark(new te::se::Mark), m_update(false)
{
  //build property browser basic fill
  QtProperty* glyphProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Glyph Mark"));

  //font
  m_fontProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->addProperty(tr("Font"));
  glyphProperty->addSubProperty(m_fontProperty);

  //char
  m_charProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->addProperty(tr("Char"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->setValue(m_charProperty, QString("0"));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_dlgFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(setDlg(QWidget *, QtProperty *)));
  glyphProperty->addSubProperty(m_charProperty);

  addProperty(glyphProperty, tr("Glyph Mark"), QColor(175, 175, 255));

  m_basicFill = new te::qt::widgets::BasicFillPropertyItem(pb, QColor(175, 175, 255));

  connect(m_basicFill, SIGNAL(fillChanged()), SLOT(onFillChanged()));

  updateMarkName();
  m_mark->setFill(m_basicFill->getFill());
}

te::qt::widgets::GlyphMarkPropertyItem::~GlyphMarkPropertyItem()
{
}

void te::qt::widgets::GlyphMarkPropertyItem::setMark(const te::se::Mark* mark)
{
  assert(mark);

  delete m_mark;

  m_mark = mark->clone();

  updateUi();
}

te::se::Mark* te::qt::widgets::GlyphMarkPropertyItem::getMark() const
{
  return m_mark->clone();
}

void te::qt::widgets::GlyphMarkPropertyItem::updateUi()
{
  const std::string* name = m_mark->getWellKnownName();
  assert(name); // TODO: Exception?

  QString qName(name->c_str());
  QString fontName;
  QChar charCode;

  try
  {
     te::qt::widgets::GlyphMarkRenderer::decode(qName, fontName, charCode);
  }
  catch(...)
  {
  }

  QFont font(fontName);

  int intCharCode = charCode.unicode();
  QString strCharCode;
  strCharCode.setNum(intCharCode);

  m_update = true;

  te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->setValue(m_charProperty, strCharCode);
  te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->setValue(m_fontProperty, font);
  
  m_update = false;

  const te::se::Fill* fill = m_mark->getFill();
  if(fill)
    m_basicFill->setFill(fill);
}

void te::qt::widgets::GlyphMarkPropertyItem::updateMarkName()
{
  QFont f = te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->value(m_fontProperty);

  QString valStr = te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->value(m_charProperty);

  QString name = te::qt::widgets::GlyphMarkRenderer::encode(f.family(), valStr.toInt());

  m_mark->setWellKnownName(new std::string(name.toStdString()));

  if(!m_update)
    emit markChanged();
}

void te::qt::widgets::GlyphMarkPropertyItem::valueChanged(QtProperty* p, const QFont &value)
{
  if(m_fontProperty == p )
    updateMarkName();
}

void te::qt::widgets::GlyphMarkPropertyItem::valueChanged(QtProperty* p, const QString &value)
{
  if(m_charProperty == p)
    updateMarkName();
}

void te::qt::widgets::GlyphMarkPropertyItem::setDlg(QWidget *parent, QtProperty * prop)
{
  if(prop == m_charProperty)
  {
    m_charDlg = parent;

    connect(m_charDlg, SIGNAL(showDlg()), this, SLOT(showCharMapDlg()));
  }
}

void te::qt::widgets::GlyphMarkPropertyItem::showCharMapDlg()
{
  QFont f = te::qt::widgets::AbstractPropertyManager::getInstance().m_fontManager->value(m_fontProperty);

  //build form
  QDialog d;

  QGridLayout* l = new QGridLayout(&d);
  l->setObjectName("mainLayout");
  QScrollArea* sa = new QScrollArea(&d);
  sa->setFixedSize(300, 200);
  QGridLayout* lsa = new QGridLayout(&d);
  lsa->setObjectName("scrollAreaLayout");
  sa->setLayout(lsa);

  te::qt::widgets::CharMapWidget* cmw = new te::qt::widgets::CharMapWidget(sa);
  lsa->addWidget(cmw);
  l->addWidget(sa,0, 0, 1, 2);
  connect(cmw, SIGNAL(charSelected(const unsigned int&)), this, SLOT(charMapSelected(const unsigned int&)));
  cmw->setFont(f);
  sa->setWidget(cmw);
  
  QSpacerItem* spacer = new QSpacerItem(16, 16, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
  l->addItem(spacer, 1, 0);
  QPushButton* pb = new QPushButton(tr("Close"), &d);
  connect(pb, SIGNAL(clicked()), &d, SLOT(close()));
  l->addWidget(pb, 1, 1);
  l->setSizeConstraint(QLayout::SetFixedSize);

  d.setLayout(l);
  d.setWindowTitle(tr("Char Map"));
  d.setWindowFlags(Qt::Tool);
  d.exec();


  //if(m_charMapWidget->isVisible())
  //{
  //  m_charMapWidget->close();
  //}

  //

  //m_charMapWidget->setFont(f);

  //m_charMapWidget->show();
}

void te::qt::widgets::GlyphMarkPropertyItem::charMapSelected(const unsigned int& charCode)
{
  QString str;
  str.setNum(charCode);

  te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->setValue(m_charProperty, str);

  updateMarkName();
}

void te::qt::widgets::GlyphMarkPropertyItem::onFillChanged()
{
  m_mark->setFill(m_basicFill->getFill());
  emit markChanged();
}
