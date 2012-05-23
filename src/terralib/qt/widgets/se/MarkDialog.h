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
  \file terralib/qt/widgets/se/MarkDialog.h

  \brief A dialog used to build a mark element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
namespace Ui { class MarkDialogForm; }

namespace te
{
// Forward declarations
  namespace se
  {
    class Mark;
  }

  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class MarkWidget;

      /*!
        \class MarkDialog

        \brief A dialog used to build a mark element.
      */
      class TEQTWIDGETSEXPORT MarkDialog : public QDialog
      {
        Q_OBJECT

        public:

          MarkDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~MarkDialog();

        public:

          static te::se::Mark* getMark(const te::se::Mark* initialMark, QWidget* parent = 0, const QString& title = "");

          te::se::Mark* getMark() const;

        private:

          std::auto_ptr<Ui::MarkDialogForm> m_ui;

          te::qt::widgets::MarkWidget* m_markWidget;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKDIALOG_H
