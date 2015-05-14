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
  \file terralib/qt/widgets/help/HelpManager.h

  \brief A singleton that contains a pointer to a help manager implementation.
*/

#ifndef __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGER_H
#define __TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGER_H

//TerraLib
#include "../../../common/Singleton.h"
#include "../Config.h"

//Qt
#include <QtCore/QString>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
//Forward declaration
      class HelpManagerImpl;

      /*!
        \class HelpManager

        \brief Facade for a te::qt::widgets::HelpManagerImpl object.

        This class holds a pointer to a te::qt::widgets::HelpManagerImpl object. Before use it, the client MUST set the pointer to a valid HelpManagerImpl object.
        If it was not done, nothing will happen when calling methods neither exceptions will be raised.
      */
      class TEQTWIDGETSEXPORT HelpManager : public te::common::Singleton<HelpManager>
      {
        friend class te::common::Singleton<HelpManager>;

      public:
        
        /*!
          \brief Updates the HelpManagerImpl pointer. 

          \note This object does NOT take the ownership of the pointer. The caller must manage the pointers. This means that the previous pointer will NOT be deleted. This method, only reassigns the pointer.
          \note The client must ensure that the pointer of the HelpManagerImpl will be valid until the application has finished. If, for some reason, the client of this class needs to delete the pointer, it MUST 
                use this method to set NULL or 0 to the HelpManagerImpl to be used (a null pointer means no operation to be executed), otherwise it can take to an unhandled exception of invalid pointer.
        */
        void setImpl(HelpManagerImpl* impl);

        /*! \brief Returns the pointer to the HelpManagerImpl object being used. */
        HelpManagerImpl* getImpl() const;

        /** @name Facade methods
         *  Facade for the te::qt::widgets::HelpManagerImpl API.
         *
         *  \note See the te::qt::widgets::HelpManagerImpl API for detais on how to call methods with the right argument's syntax.
         */
        //@{

        /*!
          \brief Opens the help on the selected page.

          \param htmRef Reference to some page on the help collection.

          \param nSpace
        */
        void showHelp(const QString& htmRef, const QString& nSpace = "");

        /*!
          \brief Appends a project of help documentation to the existing collection.

          \param docRef Reference to the help project to be appended.
        */
        void appendDoc(const QString& docRef);

        //@}

      protected:

        /*! \brief Protected constructor. Can not be called by clients. */
        HelpManager();

        HelpManagerImpl* m_impl;    //!< Pointer to HelpManagerImpl to be used.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif //__TERRALIB_QT_WIDGETS_HELP_INTERNAL_HELPMANAGER_H

