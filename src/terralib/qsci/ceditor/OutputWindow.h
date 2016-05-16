/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qsci/ceditor/OutputWindow.h

  \brief A widget to output the script execution.
*/

#ifndef __TERRALIB_QSCI_CEDITOR_OUTPUTWINDOW_H__
#define __TERRALIB_QSCI_CEDITOR_OUTPUTWINDOW_H__

// TerraLib Code Editor
#include "../Config.h"

// Qt
#include <QDockWidget>

// Forward declaration
class QTextEdit;

namespace te
{
  namespace ce
  {
    /*!
      \class OutputWindow

      \brief A widget to output the script execution.
     */
    class TECEDITOREXPORT OutputWindow : public QDockWidget
    {
      Q_OBJECT

      public:

        OutputWindow(QWidget* parent = 0);

        ~OutputWindow();

        const QTextEdit* getText() const;

        QTextEdit* getText();

      private:

        QTextEdit* m_txt;

    };

  } // end namespace tce
}

#endif  // __TERRALIB_QSCI_CEDITOR_OUTPUTWINDOW_H__
