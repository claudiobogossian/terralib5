/****************************************************************************
**
** Copyright (C) 2006 Trolltech ASA. All rights reserved.
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** This file may be used under the terms of the GNU General Public License
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
#include "FileEditFactory.h"
#include "FileEdit.h"

te::layout::FileEditFactory::FileEditFactory(QObject *parent /*= 0*/)
  : QtAbstractEditorFactory<FilePathManager>(parent)
{

}

te::layout::FileEditFactory::~FileEditFactory()
{
  QList<FileEdit *> editors = m_editorToProperty.keys();
  QListIterator<FileEdit *> it(editors);
  while (it.hasNext())
    delete it.next();
}

void te::layout::FileEditFactory::connectPropertyManager(FilePathManager *manager)
{
  connect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
    this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
  connect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
    this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

QWidget *te::layout::FileEditFactory::createEditor(FilePathManager *manager,
        QtProperty *property, QWidget *parent)
{
  FileEdit *editor = new FileEdit(parent);
  editor->setFilePath(manager->value(property));
  editor->setFilter(manager->filter(property));
  m_createdEditors[property].append(editor);
  m_editorToProperty[editor] = property;

  connect(editor, SIGNAL(filePathChanged(const QString &)),
    this, SLOT(slotSetValue(const QString &)));
  connect(editor, SIGNAL(destroyed(QObject *)),
    this, SLOT(slotEditorDestroyed(QObject *)));
  return editor;
}

void te::layout::FileEditFactory::disconnectPropertyManager(FilePathManager *manager)
{
  disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QString &)),
    this, SLOT(slotPropertyChanged(QtProperty *, const QString &)));
  disconnect(manager, SIGNAL(filterChanged(QtProperty *, const QString &)),
    this, SLOT(slotFilterChanged(QtProperty *, const QString &)));
}

void te::layout::FileEditFactory::slotPropertyChanged(QtProperty *property,
                const QString &value)
{
  if (!m_createdEditors.contains(property))
    return;

  QList<FileEdit *> editors = m_createdEditors[property];
  QListIterator<FileEdit *> itEditor(editors);
  while (itEditor.hasNext())
    itEditor.next()->setFilePath(value);
}

void te::layout::FileEditFactory::slotFilterChanged(QtProperty *property,
            const QString &filter)
{
  if (!m_createdEditors.contains(property))
    return;

  QList<FileEdit *> editors = m_createdEditors[property];
  QListIterator<FileEdit *> itEditor(editors);
  while (itEditor.hasNext())
    itEditor.next()->setFilter(filter);
}

void te::layout::FileEditFactory::slotSetValue(const QString &value)
{
  QObject *object = sender();
  QMap<FileEdit *, QtProperty *>::ConstIterator itEditor = m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd())
  {
    if (itEditor.key() == object) 
    {
      QtProperty *property = itEditor.value();
      FilePathManager *manager = propertyManager(property);
      if (!manager)
        return;
      manager->setValue(property, value);
      return;
    }
    itEditor++;
  }
}

void te::layout::FileEditFactory::slotEditorDestroyed(QObject *object)
{
  QMap<FileEdit *, QtProperty *>::ConstIterator itEditor =
    m_editorToProperty.constBegin();

  while (itEditor != m_editorToProperty.constEnd()) 
  {
    if (itEditor.key() == object) 
    {
      FileEdit *editor = itEditor.key();
      QtProperty *property = itEditor.value();
      m_editorToProperty.remove(editor);
      m_createdEditors[property].removeAll(editor);
      if (m_createdEditors[property].isEmpty())
        m_createdEditors.remove(property);
      return;
    }
    itEditor++;
  }
}

