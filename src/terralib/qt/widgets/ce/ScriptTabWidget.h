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
  \file ScriptTabWidget.h

  \brief The main tab widget may be used to display several scripts.
*/

#ifndef __TERRALIB_CODE_EDITOR_CORE_INTERNAL_SCRIPTTABWIDGET_H
#define __TERRALIB_CODE_EDITOR_CORE_INTERNAL_SCRIPTTABWIDGET_H

// TerraLib Code Editor
#include "../Config.h"

// Qt
#include <QtGui/QTabWidget.h>

namespace tce
{
// Forwar declaration
  class Application;

  /*!
    \class ScriptTabWidget

    \brief The main tab widget may be used to display several scripts.
  */
  class TCECOREEXPORT ScriptTabWidget : public QTabWidget
  {
    Q_OBJECT

    public:      

      ScriptTabWidget(Application* parent);

      ~ScriptTabWidget();

      Application* getApplication() const;

    protected:

      void mousePressEvent(QMouseEvent* evt);

    private:

      Application* m_parent;
  };

} // end namespace tce

#endif  // __TERRALIB_CODE_EDITOR_CORE_INTERNAL_SCRIPTTABWIDGET_H

