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
  \file AbstractSettingWidget.h

  \brief An abstraction for setting widgets.
*/

#ifndef __TERRALIB_QT_AF_ABSTRACTSETTINGWIDGET_H
#define __TERRALIB_QT_AF_ABSTRACTSETTINGWIDGET_H

// TerraLib
#include "../ApplicationController.h"

// Qt
#include <QWidget>
#include <QMessageBox>

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class AbstractSettingWidget

        \brief A frame for setting Table options.
      */
      class AbstractSettingWidget : public QWidget
      {
        Q_OBJECT
        
        public:

          /*!
            \brief
          */
          AbstractSettingWidget(QWidget* parent = 0);

          /*!
            \brief
          */
          virtual ~AbstractSettingWidget();
          
          /*!
            \brief
          */
          virtual void saveChanges() = 0;

          /*!
            \brief
          */
          virtual void resetState() = 0;

          /*!
            \brief
          */
          virtual void getHelpInformations(QString& ns, QString& helpFile) = 0;

          /*!
            \brief
          */
          QString getResumeText();

          /*!
            \brief
          */
          bool hasChanges();

          /*!
          \brief
          */
          virtual void setApplicationController(te::qt::af::ApplicationController* app);

        public slots:
          
          virtual void onApplyButtonClicked();

        signals:
        
          /*!
            \brief Signal must be emited when some settings has been changed. Use the argument to tell if the 
          */
          void updateApplyButtonState(const bool&);

        protected:

          virtual void hideEvent(QHideEvent * event);

          /*!
            \brief Indicates that there's unsaved information. Use this method after each change in informations of the widget.

            \param state If true indicates that there are unsave information.
          */
          virtual void changeApplyButtonState(const bool& state);

          bool m_hasChanged;  //!< Flag that defines if the widget has changes to apply.

          QString m_resumeText;

          te::qt::af::ApplicationController* m_app;
      };

      inline AbstractSettingWidget::AbstractSettingWidget(QWidget* parent):
      QWidget(parent),
      m_hasChanged(false),
      m_app(0)
      {
      }

      inline AbstractSettingWidget::~AbstractSettingWidget()
      {
      }

      inline QString AbstractSettingWidget::getResumeText()
      {
        return m_resumeText;
      }

      inline bool AbstractSettingWidget::hasChanges()
      {
        return m_hasChanged;
      }

      inline void AbstractSettingWidget::setApplicationController(te::qt::af::ApplicationController* app)
      {
        m_app = app;
      }

      inline void AbstractSettingWidget::onApplyButtonClicked()
      {
        saveChanges();

        changeApplyButtonState(false);
      }

      inline void AbstractSettingWidget::hideEvent(QHideEvent * event)
      {
        if(m_hasChanged)
        {
          QString msg = tr("There are unsaved changes. Do you want to save it?");

          if(QMessageBox::question(this, tr("Application settings customization"), msg, QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            saveChanges();
          else
            resetState();
        }

        m_hasChanged = false;

        QWidget::hideEvent(event);
      }

      inline void AbstractSettingWidget::changeApplyButtonState(const bool& state)
      {
        m_hasChanged = state;
        emit updateApplyButtonState(m_hasChanged);
      }
    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif // __TERRALIB_QT_AF_ABSTRACTSETTINGWIDGET_H

