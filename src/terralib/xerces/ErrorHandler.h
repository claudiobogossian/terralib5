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
  \file ErrorHandler.h
   
  \brief This class implements Xerces interface for error handlers.
*/

#ifndef __TERRALIB_XERCES_INTERNAL_ERRORHANDLER_H
#define __TERRALIB_XERCES_INTERNAL_ERRORHANDLER_H

// TerraLib
#include "Config.h"

// STL
#include <string>
#include <vector>

// Xerces-C++
#include <xercesc/sax/ErrorHandler.hpp>

namespace te
{
  namespace xerces
  {
    /*!
      \class ErrorHandler

      \brief This class implements Xerces interface for error handlers.

      \sa Reader, ReaderHandler
    */
    class TEXERCESEXPORT ErrorHandler : public xercesc::ErrorHandler
    {
      public:

        /*! \brief Default constructor. */
        ErrorHandler() { }

        /*! \brief Destructor. */
        ~ErrorHandler() { }

        /*!
          \brief It receives from the parser a notification of a warning.

          \param exc The warning information.
        */
        void warning(const xercesc::SAXParseException& exc);

        /*!
          \brief It receives from the parser a notification of a recoverable error.

          \param exc The error information.
        */
        void error(const xercesc::SAXParseException& exc);

        /*!
          \brief It receives from the parser a notification of a non-recoverable error.

          \param exc The error information.
        */
        void fatalError(const xercesc::SAXParseException& exc);

        /*! brief It resets the error handler object on its reuse. */
        void resetErrors();

        /*!
          \brief It returns the number of reported errors.

          \return The number of reported errors.
        */
        const std::size_t getNumErros() const;

        /*!
          \brief It returns the n-th error message.

          \param i The error message position in the internal conteiner.

          \return The n-th error message.

          \note This method doesn't check the index range.
        */
        const std::string& getError(std::size_t i) const;

        /*! 
          \brief It returns a concatenation of all reported error messages.

          \return A concatenation of all reported error messages.
        */
        const std::string getErrors() const;

      protected:

        std::vector<std::string> m_errors;  //!< The error messages.
    };

  } // end namespace xerces
}   // end namespace te

#endif  // __TERRALIB_XERCES_INTERNAL_ERRORHANDLER_H

