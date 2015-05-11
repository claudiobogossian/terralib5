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
  \file JsContext.h

  \brief A class for keeping reference to a persistent context.
 */

#ifndef __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSCONTEXT_H
#define __TERRALIB_BINDING_V8_COMMON_INTERNAL_JVM_H

// TerraLib
#include "Config.h"

// Google V8
#include <v8.h>

namespace te
{
  namespace v8
  {
    namespace common
    {
      /*!
        \class JsContext

        \brief A class for keeping reference to a persistent context.

        As stated in the Google's V8 JavaScript Engine, a context is
        an executiion environment that allows separate, unrelated, JavaScript applications
        to run in a single instance of V8.

        In TerraLib, several plugins can use the same context.

        \sa JsContextManager
       */
      class TEV8COMMONEXPORT JsContext
      {
        public:

          /*! \brief Default constructor. */
          JsContext();

          /*! \brief Destructor. */
          ~JsContext();

          ::v8::Persistent<::v8::Context> getCtx() const
          {
            return m_ctx;
          }

        private:

          /*!
            \brief Copy constructor not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.
           */
          JsContext(const JsContext& rhs);

          /*!
            \brief Assignment operator not allowed.

            \param rhs The right-hand-side copy that would be used to copy from.

            \return A reference to this.
           */
          JsContext& operator=(const JsContext& rhs);

        private:

          ::v8::Persistent<::v8::Context> m_ctx ; //!< The context to run JavaScript code.
      };

    } // end namespace common
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSCONTEXT_H
