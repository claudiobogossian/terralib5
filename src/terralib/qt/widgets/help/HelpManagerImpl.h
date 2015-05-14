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
  \file terralib/qt/widgets/help/HelpManagerImpl.h

  \brief An interface for a help manager implementation.
*/
#ifndef __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGERIMPL_H
#define __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGERIMPL_H

//TerraLib
#include "../Config.h"

//Qt
#include <QtCore/QString>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HelpManagerImpl

        \brief An interface for help managers implementations.
      */
      class TEQTWIDGETSEXPORT HelpManagerImpl
      {
        public:

          /*!
            \brief Opens the help on the selected page.

            \param htmRef Reference to some page on the help collection.

            \param nSpace Namespace to be used.

            \note There is a syntax for the argument. If it is not respected, we CAN NOT garantee the correct operation. 
                  The right syntax is as follows: "qthelp://<NAMESPACE defined in Qt help project/<virtual folder>/<fileName>",
                  i.e. qthelp://dpi.inpe.br.terraview.5.0/doc/associando_arquivos_sites.htm, "dpi.inpe.br.terraview.5.0" is the namespace defined in the Qt help project file, 
                  "doc" is the virtual folder, also defined in Qt project file, and "associando_arquivos_sites.htm" is file compiled within the project.
          */
          virtual void showHelp(const QString& htmRef, const QString& nSpace = "") = 0;


          /*!
            \brief
          */
//          virtual void showDocumentation (const QString& page, const QString& nSpace="") = 0;

          /*!
            \brief Appends a collection of help documentation to the existing collection.

            \param docRef Reference to the help collection to be appended.

            \note The docRef argument refers to an existing Qt Help project file. See Qt documentation for details on how to create these kind of files. 
          */
          virtual void appendDoc(const QString& docRef) = 0;
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGERIMPL_H
