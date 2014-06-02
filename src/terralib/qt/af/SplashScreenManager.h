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
  \file terralib/qt/af/SplashScreenManager.h

  \brief A singleton for holding he application splash screen.
*/

#ifndef __TERRALIB_QT_AF_INTERNAL_SPLASHSCREENMANAGER_H
#define __TERRALIB_QT_AF_INTERNAL_SPLASHSCREENMANAGER_H

// TerraLib
#include "../../common/Singleton.h"
#include "Config.h"

// Qt
#include <QSplashScreen>

namespace te
{
  namespace qt
  {
    namespace af
    {
      /*!
        \class SplashScreenManager

        \brief A singleton for holding he application splash screen.
      */
      class TEQTAFEXPORT SplashScreenManager : public te::common::Singleton<SplashScreenManager>
      {
        friend class te::common::Singleton<SplashScreenManager>;

        public:

          /*!
            \brief This will cause the text to be drawn on the splash screen and a call to Application::processEvents() will be made.

            \param message The message to be drawn.
          */
          void showMessage(const QString& message);

          /*!
            \brief It sets the splash screen to be used during the application startup.

            \param impl       Any splash screen widget.
            \param alignment  The alignment of the text.
            \param color      The text color.

            \note The caller is responsible for keeping the splash screen alive until the application is fully initialized.

            \note The application will use the splash screen implementation only during the application startup.

            \note The manager will not take the ownership of the splash screen.
          */
          void set(QSplashScreen* impl, int alignment = Qt::AlignLeft, const QColor& color = Qt::black);

          /*!
            \brief Updates the image to be presented on splash screen.

            \param pix New image to be presented.
          */
          void setLogo(const QPixmap& pix);

          /*!
            \brief Closes the splash screen.

            This function can be used when something fails and the splash screen is visible.
          */
          void close();

        protected:

          /*! \brief It initializes the singleton. */
          SplashScreenManager();

          /*! \brief Destructor. */
          ~SplashScreenManager();

        private:

          QSplashScreen* m_sc;  //!< A reference to the splash screen.
          int m_msgAlignment;   //!< The alignment used to draw the message text.
          QColor m_msgColor;    //!< The color used to draw the message text.
      };

    }   // end namespace af
  }     // end namespace qt
}       // end namespace te

#endif  // __TERRALIB_QT_AF_INTERNAL_SPLASHSCREENMANAGER_H
