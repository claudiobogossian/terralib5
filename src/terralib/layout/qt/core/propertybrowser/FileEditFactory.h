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
    \file FileEdit.h

    \brief

    \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_FILE_EDIT_FACTORY_H 
#define __TERRALIB_LAYOUT_INTERNAL_FILE_EDIT_FACTORY_H

#include <QtPropertyBrowser/qtpropertybrowser.h>
#include "FilePathManager.h"

namespace te
{
  namespace layout
  {
    class FileEdit;

    class FileEditFactory : public QtAbstractEditorFactory<FilePathManager>
    {
      Q_OBJECT

      public:

        FileEditFactory(QObject *parent = 0);

        virtual ~FileEditFactory();

      protected:

        virtual void connectPropertyManager(FilePathManager *manager);

        virtual QWidget *createEditor(FilePathManager *manager, QtProperty *property,
          QWidget *parent);

        virtual void disconnectPropertyManager(FilePathManager *manager);

      private slots:

        void slotPropertyChanged(QtProperty *property, const QString &value);

        void slotFilterChanged(QtProperty *property, const QString &filter);

        void slotSetValue(const QString &value);

        void slotEditorDestroyed(QObject *object);

      private:

        QMap<QtProperty *, QList<FileEdit *> >  m_createdEditors;
        QMap<FileEdit *, QtProperty *>          m_editorToProperty;
    };
  }
}

#endif
