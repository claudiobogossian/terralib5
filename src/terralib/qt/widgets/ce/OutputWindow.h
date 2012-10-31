/*  Copyright (C) 2011-20011 National Institute For Space Research (INPE) - Brazil.

    This file is part of TerraLib Code Editor - A widget component for scripting with TerraLib Platform.

    TerraLib Code Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib Code Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TerraLib Code Editor. See COPYING. If not, write to
    Gilberto Ribeiro de Queiroz at <gribeiro@dpi.inpe.br>.
 */

/*!
  \file OutputWindow.h

  \brief A widget to output the script execution.
*/

#ifndef __TERRALIB_CODE_EDITOR_CORE_INTERNAL_OUTPUTWINDOW_H
#define __TERRALIB_CODE_EDITOR_CORE_INTERNAL_OUTPUTWINDOW_H

// TerraLib Code Editor
#include "Config.h"

// Qt
#include <QtGui/QDockWidget>

// Forward declaration
class QTextEdit;

namespace tce
{
  /*!
    \class OutputWindow

    \brief A widget to output the script execution.
  */
  class TCECOREEXPORT OutputWindow : public QDockWidget
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

#endif  // __TERRALIB_CODE_EDITOR_CORE_INTERNAL_OUTPUTWINDOW_H

