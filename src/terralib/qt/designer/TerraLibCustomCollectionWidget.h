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
  \file terralib/qt/designer/TerraLibCustomCollectionWidget.h

  \brief The main entry point of TerraLib components for Qt Designer.
*/

#ifndef __TERRALIB_QT_DESIGNER_INTERNAL_TERRALIBCUSTOMCOLLECTIONWIDGET_H
#define __TERRALIB_QT_DESIGNER_INTERNAL_TERRALIBCUSTOMCOLLECTIONWIDGET_H

// Qt
#include <QtCore/QObject>
#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>

namespace te
{
  namespace qt
  {
    namespace designer
    {
      /*!
        \class TerraLibCustomCollectionWidget

        \brief This class includes all component classes into a single library.

        \warning Remember to copy the auxiliary DLLs to the folder where QtDesigner is locate,
                 otherwise, you will have troubles!
      */
      class TerraLibCustomCollectionWidget : public QObject,
                                             public QDesignerCustomWidgetCollectionInterface
      {
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

        public:

          TerraLibCustomCollectionWidget(QObject* parent = 0);

          QList<QDesignerCustomWidgetInterface*> customWidgets() const;

        private:

          QList<QDesignerCustomWidgetInterface*> m_widgets;
      };

    } // end namespace designer
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_DESIGNER_INTERNAL_TERRALIBCUSTOMCOLLECTIONWIDGET_H


