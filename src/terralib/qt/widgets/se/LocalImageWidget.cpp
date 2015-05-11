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
  \file terralib/qt/widgets/se/LocalImageWidget.cpp

  \brief A widget used to build an external graphic element that references a local image. e.g. a SVG file, a PNG file, etc.
*/

// TerraLib
#include "../../../se/ExternalGraphic.h"
#include "../../../xlink/SimpleLink.h"
#include "LocalImageWidget.h"
#include "ui_LocalImageWidgetForm.h"

// Qt
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>

// STL
#include <cassert>

te::qt::widgets::LocalImageWidget::LocalImageWidget(QWidget* parent, Qt::WindowFlags f)
  : QWidget(parent, f),
    m_ui(new Ui::LocalImageWidgetForm),
    m_eg(new te::se::ExternalGraphic)
{
  m_ui->setupUi(this);

  // Bulding the filter string
  m_filter = tr("Images") + " ( ";
  QList<QByteArray> formats = QImageReader::supportedImageFormats();
  for(int i = 0; i < formats.size() - 1; ++i)
    m_filter += "*." + formats[i] + " ";
  m_filter += ")";

  // Signals & slots
  connect(m_ui->m_browsePushButton, SIGNAL(pressed()), this, SLOT(onBrowsePushButtonPressed()));
}

te::qt::widgets::LocalImageWidget::~LocalImageWidget()
{
  delete m_eg;
}

void te::qt::widgets::LocalImageWidget::setExternalGraphic(const te::se::ExternalGraphic* eg)
{
  assert(eg);

  delete m_eg;

  m_eg = eg->clone();

  updateUi();
}

te::se::ExternalGraphic* te::qt::widgets::LocalImageWidget::getExternalGraphic() const
{
  return m_eg->clone();
}

void te::qt::widgets::LocalImageWidget::updateUi()
{
  // Updating path
  const te::xl::SimpleLink* link = m_eg->getOnlineResource();
  assert(link);
  QString path = link->getHref().c_str();
  m_ui->m_pathLineEdit->setText(path);

  // Loading image...
  QImage img;
  if(!img.load(path))
  {
    QMessageBox::critical(this, tr("Error"), tr("The referenced image cannot be loaded."));
    return;
  }

  // Bulding image details...
  QString details = QString::number(img.width()) + " x " + QString::number(img.height()) + " pixels\n";
  details += tr("Format: ") + QImageReader::imageFormat(path).toUpper() + "\n";
  details += QString::number(img.depth()) + " " + tr("bits per pixel") + "\n";
  QString alpha;
  img.hasAlphaChannel() ? alpha = tr("Yes") : alpha = tr("No");
  details += tr("Alpha Channel: ") + alpha;
  m_ui->m_detailsTextEdit->setText(details);

  // Adjusting image preview...
  if(img.width() > m_ui->m_imageLabel->size().width())
    img = img.scaledToWidth(m_ui->m_imageLabel->size().width(), Qt::SmoothTransformation);
  m_ui->m_imageLabel->setPixmap(QPixmap::fromImage(img));
}

void te::qt::widgets::LocalImageWidget::onBrowsePushButtonPressed()
{
  QString path = QFileDialog::getOpenFileName(this, tr("Select an Image File"), "", m_filter);
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

  emit externalGraphicChanged(img.size());

  updateUi();
}
