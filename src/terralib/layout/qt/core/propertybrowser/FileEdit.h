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

#ifndef __TERRALIB_LAYOUT_INTERNAL_FILE_EDIT_H 
#define __TERRALIB_LAYOUT_INTERNAL_FILE_EDIT_H

// Qt
#include <QWidget>
#include <QLineEdit>
#include <QString>

namespace te
{
  namespace layout
  {
    class FileEdit : public QWidget
    {
      Q_OBJECT

      public:

        FileEdit(QWidget *parent = 0);

        virtual ~FileEdit();

        void setFilePath(const QString &filePath);

        QString filePath() const;

        void setFilter(const QString &filter);

        QString filter();

      signals:

        void filePathChanged(const QString &filePath);

      protected:

        void focusInEvent(QFocusEvent *e);

        void focusOutEvent(QFocusEvent *e);

        void keyPressEvent(QKeyEvent *e);

        void keyReleaseEvent(QKeyEvent *e);

      private slots:

        void buttonClicked();

      private:
        
        QLineEdit* m_lineEdit;
        QString    m_filter;
    };

  }
}

#endif

