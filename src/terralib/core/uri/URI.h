/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/URI.h

  \brief A class for representing an Uniform Resource Identifier (URI).

  \author Vinicius Campanha
  \author Gilberto Ribeiro de Queiroz
 */

#ifndef __TERRALIB_CORE_URI_URI_H__
#define __TERRALIB_CORE_URI_URI_H__

// TerraLib
#include "Config.h"

// STL
#include <string>

// Boost
#include <boost/regex.hpp>

namespace te
{
  namespace core
  {
    /*!
      \class URI

      \brief A class for representing an Uniform Resource Identifier (URI).
     */
    class TECOREEXPORT URI
    {
      public:

        /*! \brief Default constructor. */
        URI();

        /*!
          \brief A constructor from a string.

           This constructor check the URI enconding,
           then parse it and validate.

           \param uri A string with the URI to be parsed.

           \exception URIException when the given URI isn't valid.
         */
        explicit URI(const std::string& uri);

        /*! \brief Copy constructor. */
        URI(const URI& other);

        /*! Default destructor. */
        ~URI() = default;

        /*! \brief Assingment operator. */
        URI& operator=(const URI& other);

        /*!
          \brief Retrieving the full URI

          \return Returns the complete URI.
         */
        const std::string& uri() const;

        /*!
          \brief Retrieving the scheme

          \return Returns the URI scheme.
         */
        std::string scheme() const;

        /*!
          \brief Retrieving the user information

          \return Returns the URI user information.
         */
        std::string user() const;

        /*!
          \brief Retrieving the password information

          \return Returns the URI password information.
         */
        std::string password() const;

        /*!
          \brief Retrieving the host

          \return Returns the URI host.
         */
        std::string host() const;

        /*!
          \brief Retrieving the port

          \return Returns the URI port.
         */
        std::string port() const;

        /*!
          \brief Retrieving the path

          \return Returns the URI path.
         */
        std::string path() const;

        /*!
         \brief Retrieving the query

         \return Returns the URI query.
         */
        std::string query() const;

        /*!
          \brief Retrieving the fragment

          \return Returns the URI fragment.
         */
        std::string fragment() const;

        /*!
          \brief Return if the given URI is valid or not.

         \return Returns true if the given URI is valid.
         */
        bool isValid() const;

      private:

        /*! \brief Swap operation. */
        void swap(URI& other);

        /*!
          \brief Parse the URI stored in uri_ member.

          It uses regex to validate and parse the given URI.

          After this, if the given URI is valid, the match_ member
          will have the references to all parts of the URI.

          The regex split the URI by using named groups in
          regex, "(?<name>...)", so knowing the group name,
          you can require the corresponding group, from match_.

          \exception URIException when the given URI isn't valid.
         */
        void parse();

        /*!
          \brief Check if the uri_ contains any invalid character and parse
                 it to his hexadecimal value
         */
        void encode();

        std::string hexToLetter(int i);

      private:

        std::string uri_;
        boost::match_results< std::string::const_iterator > match_;
        bool isValid_;
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_URI_URI_H__
