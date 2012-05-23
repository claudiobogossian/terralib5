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
  \file terralib/qt/widgets/se/MarkWidget.h

  \brief A widget used to build a mark element.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKWIDGET_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QWidget>

// STL
#include <memory>
#include <string>
#include <vector>

// Forward declaraion
class QListWidget;
namespace Ui { class MarkWidgetForm; }

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
      class BasicFillWidget;
      class BasicStrokeWidget;

      /*!
        \class MarkWidget

        \brief A widget used to build a mark element.
      */
      class TEQTWIDGETSEXPORT MarkWidget : public QWidget
      {
        Q_OBJECT

        public:

          MarkWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~MarkWidget();

        public:

          void setMark(const te::se::Mark* mark);

          te::se::Mark* getMark() const;

        protected:

          void updateUi();

          void updateMarkIcons();

        protected slots:

          void onMarkTypeChanged(const QString& currentText);

          void onStrokeChanged();

          void onStrokeGroupBoxToggled(bool on);

          void onFillChanged();

          void onFillGroupBoxToggled(bool on);

        private:

          std::auto_ptr<Ui::MarkWidgetForm> m_ui;

          te::qt::widgets::BasicFillWidget* m_fillWidget;

          te::qt::widgets::BasicStrokeWidget* m_strokeWidget;

          QListWidget* m_contentsMarkWidget;

          te::se::Mark* m_mark;

          std::vector<std::string> m_supportedMarks;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_MARKWIDGET_H
