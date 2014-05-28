/*  Copyright (C) 2011-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file DisplayWidget.h

  \brief A frame for setting display options.
*/

#ifndef __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H
#define __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H

// TerraLib
#include "../Config.h"

// Qt
#include "AbstractSettingWidget.h"

// STL
#include <memory>

// Forward declaration
namespace Ui 
{ 
  class GeneralConfigWidgetForm; 
}

class QPixmap;

namespace te
{
  namespace qt
  {
    namespace af
      {
      /*!
        \class DisplayWidget

        \brief A frame for setting display options.
      */
      class TEQTAFEXPORT GeneralConfigWidget : public AbstractSettingWidget
      {
        Q_OBJECT

        public:

          GeneralConfigWidget(QWidget* parent = 0);

          ~GeneralConfigWidget();

          void saveChanges();

          void resetState();
          
          void getHelpInformations(QString& ns, QString& helpFile);

          bool eventFilter(QObject* watched, QEvent* e);

        protected slots:

          void valueChanged(QString s);

          void iconSizeChanged(int size);

          void addPluginCategory();

          void removePluginCategory();

          void textChanged(const QString&);

          void colorChanged(const QColor& c);

        protected:

          bool m_needRestart;

          std::auto_ptr<QPixmap> m_sysIcon;
          std::auto_ptr<QPixmap> m_sizeIcon;
          std::auto_ptr<QPixmap> m_aboutIcon;
          std::auto_ptr<QPixmap> m_terraIcon;
          std::auto_ptr<QPixmap> m_toolBarSizeIcon;

        private:

          std::auto_ptr<Ui::GeneralConfigWidgetForm> m_ui;
      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_GENERALCONFIGWIDGET_H

