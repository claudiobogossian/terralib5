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
  \file terralib/common/Exception.h

  \brief This class is designed to declare objects to be thrown as exceptions by TerraLib.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_EXCEPTION_H
#define __TERRALIB_COMMON_INTERNAL_EXCEPTION_H

// TerraLib
#include "Config.h"
#include "Enums.h"

// STL
#include <exception>
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class Exception

      \brief This class is designed to declare objects to be thrown as exceptions by TerraLib.

      What is an Exception?
      <ul>
      <li>It can be an error during a processing function: like out-of-memory, wrong path, wrong calculus;</li>
      <li>It can be triggered by a missed value or pointer;</li>
      <li>It can be triggered by a parameter out of range.</li>
      </ul>
      In other words, it can be any exception thrown by TerraLib when something goes wrong.

      \note TerraLib Exception class is a subclass of std::exception.

      \note In future we must revisit this class and remove the embbed std::string.
    */
    class TECOMMONEXPORT Exception : public virtual std::exception
    {
      public:

        /*! \brief Default constructor. */
        Exception() throw();

        /*!
          \brief It initializes a new Exception.

          \param what A brief description of what has raised the exception.
        */
        explicit Exception(const std::string& what, int code = UNKNOWN_EXCEPTION) throw();

        /*!
          \brief It initializes a new Exception.

          \param what A brief description of what has raised the exception.
        */
        explicit Exception(const char* const what, int code = UNKNOWN_EXCEPTION) throw();

        /*! \brief Destructor. */
        virtual ~Exception() throw();

        /*!
          \brief It gets the exception code.

          \return The exception code.
        */
        virtual int code() const throw();

        /*!
          \brief It outputs the exception message.

          \return The exception message.
        */
        virtual const char* what() const throw();

        /*!
          \brief It return the exception class name.

          \return the exception class name.
        */
        virtual const char* getClassName() const throw();

      protected:

        int m_code;        //!< The internal exception code.
        std::string m_msg; //!< The internal exception message.
    };

    /*!
      \brief It serializes the exception and sends it to the output stream.

      \param e An exception object.
      \param o An output stream.

      \return The output stream.
    */
    std::ostream& operator<<(const Exception& e, std::ostream& o);

  } // end namespace common
}   // end namespace te

/*!
  \brief This define can be used to add new exception classes to the system.
*/
#define TE_DECLARE_EXCEPTION_CLASS(API_DECL, ClassName, BaseExceptionClass)                           \
  class API_DECL ClassName : public virtual BaseExceptionClass                                        \
  {                                                                                                   \
    public:                                                                                           \
                                                                                                      \
      ClassName() throw() { }                                                                         \
                                                                                                      \
      explicit ClassName(const std::string& what, int code = te::common::UNKNOWN_EXCEPTION) throw();  \
                                                                                                      \
      explicit ClassName(const char* const what, int code = te::common::UNKNOWN_EXCEPTION) throw();   \
                                                                                                      \
      virtual ~ClassName() throw();                                                                   \
                                                                                                      \
      virtual const char* getClassName() const throw();                                               \
  };

/*!
  \brief This define can be used to add new exception classes to the system.
*/
#define TE_DEFINE_EXCEPTION_CLASS(ClassName, BaseExceptionClass, ClassNameAsLiteral) \
    ClassName::ClassName(const std::string& what, int code) throw()                  \
      : BaseExceptionClass(what, code)                                               \
    {                                                                                \
      m_code = code;                                                                 \
      m_msg = what;                                                                  \
    }                                                                                \
                                                                                     \
    ClassName::ClassName(const char* const what, int code) throw()                   \
      : BaseExceptionClass(what, code)                                               \
    {                                                                                \
      m_code = code;                                                                 \
      m_msg = what;                                                                  \
    }                                                                                \
                                                                                     \
    ClassName::~ClassName() throw()                                                  \
    {                                                                                \
    }                                                                                \
                                                                                     \
    const char* ClassName::getClassName() const throw()                              \
    {                                                                                \
      return ClassNameAsLiteral;                                                     \
    }

#endif  // __TERRALIB_COMMON_INTERNAL_EXCEPTION_H

