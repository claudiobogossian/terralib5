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
  \file JsContextManager.h
  
  \brief A singleton for managing JavaScript contexts.
 */

#ifndef __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSCONTEXTMANAGER_H
#define __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSCONTEXTMANAGER_H

// TerraLib
#include "../../../common/Singleton.h"
#include "Config.h"

// STL
#include <map>

namespace te
{
  namespace v8
  {
    namespace common
    {
// Forward declarations
      class JsContext;

      /*!
        \class JsContextManager
      
        \brief A singleton for managing JavaScript contexts.

        \sa JVM
       */
      class TEV8COMMONEXPORT JsContextManager : public te::common::Singleton<JsContextManager>
      {
        friend class te::common::Singleton<JsContextManager>;

        public: 

          /*!
            \brief It returns the context identified by id.

            \param id The context identifier.

            \return A pointer to a context managed by this singleton or NULL if none is found.
           */
          JsContext* getCtx(const std::string& id) const;

          /*!
            \brief It adds a new context to be managed.

            \param id  The context id.
            \param ctx The context to be managed. The singleton will take the JVM ownership.

            \exception Exception It throws an exception if a context with the same ID already exists.
           */
          void add(const std::string& id, JsContext* ctx);

          /*! \brief It releases all contexts. */
          void clear();

        private:

          /*! \brief Singleton constructor. */
          JsContextManager();        

          /*! \brief Singleton destructor. */
          ~JsContextManager();

        private:
      
          std::map<std::string, JsContext*> m_ctxMap;  //!< A map from (JsContext id) to (JsContext instance).
      };

    } // end namespace common
  }   // end namespace v8
}     // end namespace te

#endif  // __TERRALIB_BINDING_V8_COMMON_INTERNAL_JSCONTEXTMANAGER_H
