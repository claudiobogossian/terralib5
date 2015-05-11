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
  \file ToolbarsWidget.h

  \brief A frame for setting tool bars options.
*/

#ifndef __TERRALIB_QT_AF_TOOLBARSWIDGET_H
#define __TERRALIB_QT_AF_TOOLBARSWIDGET_H

// TerraLib
#include "../Config.h"
#include "AbstractSettingWidget.h"

// STL
#include <set>

// Forward declaration
namespace Ui 
{ 
  class ToolbarsWidgetForm; 
}
class QToolBar;

namespace te
{
  namespace qt
  {
    namespace af
    {
      class TEQTAFEXPORT ToolbarsWidget : public AbstractSettingWidget
      {
        Q_OBJECT
        
        public:

          ToolbarsWidget(QWidget* parent=0);

          ~ToolbarsWidget();

          virtual void saveChanges();

          virtual void resetState();

          virtual void getHelpInformations(QString& ns, QString& helpFile);

        protected slots:

          void currentToolbarChanged(int idx);

          void onAddToolbarButtonClicked();

          void onRemoveToolbarButtonClicked();

          void onApplyButtonClicked();

          void updateActions(QAction* act, const bool& toAdd);

        protected:

          std::set<QToolBar*> m_createdBars;

          std::set<QToolBar*> m_removedToolBars;    //!< Indexes of the removed bars.

          std::vector< QList<QAction*> > m_actions;

        private:

          Ui::ToolbarsWidgetForm* m_ui;
      };
    }
  }
}

#endif //__TERRALIB_QT_AF_TOOLBARSWIDGET_H
