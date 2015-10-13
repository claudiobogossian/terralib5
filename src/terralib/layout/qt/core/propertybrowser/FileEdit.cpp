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
    \file FileEdit.cpp

    \brief

    \ingroup layout
*/

// TerraLib
#include "FileEdit.h"

// Qt
#include <QHBoxLayout>
#include <QToolButton>
#include <QFileDialog>
#include <QFocusEvent>

te::layout::FileEdit::FileEdit(QWidget *parent)
    : QWidget(parent)
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setMargin(0);
  layout->setSpacing(0);

  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));

  QToolButton *button = new QToolButton(this);
  button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
  button->setText(QLatin1String("..."));

  layout->addWidget(m_lineEdit);
  layout->addWidget(button);

  setFocusProxy(m_lineEdit);
  setFocusPolicy(Qt::StrongFocus);
  setAttribute(Qt::WA_InputMethodEnabled);

  connect(m_lineEdit, SIGNAL(textEdited(const QString &)),
    this, SIGNAL(filePathChanged(const QString &)));
  connect(button, SIGNAL(clicked()),
    this, SLOT(buttonClicked()));
}

te::layout::FileEdit::~FileEdit()
{

}

void te::layout::FileEdit::buttonClicked()
{
  QString filePath = QFileDialog::getOpenFileName(this, tr("Choose a file"), m_lineEdit->text(), m_filter);
  if (filePath.isNull())
    return;
  m_lineEdit->setText(filePath);
  emit filePathChanged(filePath);
}

void te::layout::FileEdit::setFilePath(const QString &filePath)
{
  if (m_lineEdit->text() != filePath)
    m_lineEdit->setText(filePath);
}

QString te::layout::FileEdit::filePath() const
{
  return m_lineEdit->text();
}

void te::layout::FileEdit::setFilter(const QString &filter)
{
  m_filter = filter;
}

QString te::layout::FileEdit::filter()
{
  return m_filter;
}

void te::layout::FileEdit::focusInEvent(QFocusEvent *e)
{
  m_lineEdit->event(e);
  if (e->reason() == Qt::TabFocusReason || e->reason() == Qt::BacktabFocusReason) {
    m_lineEdit->selectAll();
  }
  QWidget::focusInEvent(e);
}

void te::layout::FileEdit::focusOutEvent(QFocusEvent *e)
{
  m_lineEdit->event(e);
  QWidget::focusOutEvent(e);
}

void te::layout::FileEdit::keyPressEvent(QKeyEvent *e)
{
  m_lineEdit->event(e);
}

void te::layout::FileEdit::keyReleaseEvent(QKeyEvent *e)
{
  m_lineEdit->event(e);
}

