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
  \file terralib/qt/widgets/se/LocalImagePropertyItem.h

  \brief A widget used to define a local image graphic for a se symbolizer.
*/

// TerraLib
#include "../../../se/Config.h"
#include "../../../se/ExternalGraphic.h"
#include "../../../xlink/SimpleLink.h"
#include "../propertybrowser/AbstractPropertyManager.h"
#include "LocalImagePropertyItem.h"
#include "ExternalGraphicRenderer.h"

// STL
#include <cassert>

// Qt
#include <QDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QImageReader>
#include <QMessageBox>
#include <QScrollArea>
#include <QPushButton>

// QtPropertyBrowser
#include <QtPropertyBrowser/QtTreePropertyBrowser>
#include <QtPropertyBrowser/QtVariantPropertyManager>


te::qt::widgets::LocalImagePropertyItem::LocalImagePropertyItem(QtTreePropertyBrowser* pb, QColor c) : te::qt::widgets::AbstractPropertyItem(pb, c) ,
  m_eg(new te::se::ExternalGraphic), m_update(false)
{
  //build property browser basic fill
  QtProperty* localImgProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_groupManager->addProperty(tr("Local Image"));

  //dlg
  m_imgProperty = te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->addProperty(tr("Image"));
  te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->setValue(m_imgProperty, QString("0"));
  connect(te::qt::widgets::AbstractPropertyManager::getInstance().m_dlgFactory, SIGNAL(internalDlg(QWidget *, QtProperty *)), this, SLOT(setDlg(QWidget *, QtProperty *)));
  localImgProperty->addSubProperty(m_imgProperty);

  addProperty(localImgProperty, tr("Local Image"), QColor(255, 190, 160));
}

te::qt::widgets::LocalImagePropertyItem::~LocalImagePropertyItem()
{
}

void te::qt::widgets::LocalImagePropertyItem::setExternalGraphic(const te::se::ExternalGraphic* eg)
{
  assert(eg);

  delete m_eg;

  m_eg = eg->clone();

  updateUi();
}

te::se::ExternalGraphic* te::qt::widgets::LocalImagePropertyItem::getExternalGraphic() const
{
  return m_eg->clone();
}

void te::qt::widgets::LocalImagePropertyItem::updateUi()
{
  // Updating path
  const te::xl::SimpleLink* link = m_eg->getOnlineResource();
  assert(link);
  QString path = link->getHref().c_str();

  te::qt::widgets::AbstractPropertyManager::getInstance().m_strDlgManager->setValue(m_imgProperty, path);
}

void te::qt::widgets::LocalImagePropertyItem::valueChanged(QtProperty* p, const QString &value)
{

}

void te::qt::widgets::LocalImagePropertyItem::setDlg(QWidget *parent, QtProperty * prop)
{
  if(prop == m_imgProperty)
  {
    m_dlg = parent;

    connect(m_dlg, SIGNAL(showDlg()), this, SLOT(showImgDlg()));
  }
}

void te::qt::widgets::LocalImagePropertyItem::showImgDlg()
{
  // Bulding the filter string
  QString filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    filter += "*." + formats[i] + " ";
  filter += ")";

  QString path = QFileDialog::getOpenFileName(this, tr("Select an Image File"), "", filter);
  if(path.isNull())
    return;

  // Try load image
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(this, tr("Error"), tr("The selected image cannot be loaded."));
    return;
  }

  // Updating external graphic path
  te::xl::SimpleLink* link = new te::xl::SimpleLink;
  link->setHref(path.toStdString());
  m_eg->setOnlineResource(link);

  // Updating external graphic format
  QString f(QImageReader::imageFormat(path));
  m_eg->setFormat("image/" + f.toStdString()); // It's correct?!

  emit externalGraphicChanged();

  updateUi();
}
