/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/help/AssistantHelpManagerImpl.h

  \brief An implementation of HelpManager that uses QAssistant to show help files.
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_ASSISTANTHELPMANAGERIMPL_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_ASSISTANTHELPMANAGERIMPL_H

//TerraLib
#include "HelpManagerImpl.h"

// Boost
#include <boost/noncopyable.hpp>

//Qt
#include <QObject>
#include <QStringList>


//Forward declarations
class QProcess;

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        
        \class AssistantManagerImpl

        \brief A help manager that uses the QAssistant to manage help files.

      */
      class TEQTWIDGETSEXPORT AssistantHelpManagerImpl : public QObject, public HelpManagerImpl, public boost::noncopyable
      {
        Q_OBJECT 

        public:

          /*!
            \brief Constructor.

            \param parent Used by QObject constructor.
          */
          AssistantHelpManagerImpl(QObject* parent=0);

          /*! \brief Destructor. */
          ~AssistantHelpManagerImpl();

        public slots:

          void showHelp (const QString& htmRef);

          void appendDoc(const QString& docRef);

        protected:

          QProcess* m_proc;

          QStringList m_regDocs;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_ASSISTANTHELPMANAGERIMPL_H
