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

#ifndef __TERRALIB_LAYOUT_INTERNAL_FILE_PATH_MANAGER_H 
#define __TERRALIB_LAYOUT_INTERNAL_FILE_PATH_MANAGER_H

#include <QtPropertyBrowser/qtpropertybrowser.h>
#include <QMap>
#include <QString>

namespace te
{
  namespace layout
  {
    class FilePathManager : public QtAbstractPropertyManager
    {
      Q_OBJECT

      public:
        FilePathManager(QObject *parent = 0);

        virtual ~FilePathManager();

        QString value(const QtProperty *property) const;

        QString filter(const QtProperty *property) const;

      public slots:

        void setValue(QtProperty *property, const QString &val);

        void setFilter(QtProperty *property, const QString &fil);

      signals:

        void valueChanged(QtProperty *property, const QString &val);

        void filterChanged(QtProperty *property, const QString &fil);

      protected:

        virtual QString valueText(const QtProperty *property) const;

        virtual void initializeProperty(QtProperty *property);

        virtual void uninitializeProperty(QtProperty *property);


      private:

        struct Data
        {
          QString value;
          QString filter;
        };

        QMap<const QtProperty *, Data> m_values;
    };

  }
}

#endif
