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

  \author Gilberto Ribeiro de Queiroz
  \author Vinicius Campanha
 */

#ifndef __TERRALIB_CORE_URI_URI_H__
#define __TERRALIB_CORE_URI_URI_H__

/*
  The URI class is based on the specification from Glyn Matthews and Dean Michael Berris:
      Title: A URI Library for C++.
      Document Number: N3625.
      Date: 2013-04-30.
      Authors: Glyn Matthews and Dean Michael Berris
      Homepage: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3625.html.

  The code below is based on the implementation available at:
  https://github.com/cpp-netlib/cpp-netlib

  The original copyright notices is:
// Copyright 2009, 2010, 2011, 2012 Dean Michael Berris, Jeroen Habraken, Glyn
// Matthews.
// Distributed under the Boost Software License, Version 1.0.
 */

#include "../Config.h"

// STL
#include <string>

// Boost
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>

namespace te
{
  namespace core
  {
    /*!
      \class URI

      \brief A class for representing an Uniform Resource Identifier (URI).
     */
    class TEMODULEEXPORT URI
    {
      public:

// exported types
        typedef std::string string_type;
        typedef string_type::const_iterator const_iterator;

        /*! \brief Default constructor. */
        URI();

        /*!
          \brief A constructor from a string.

           This constructor check the URI enconding,
           then parse it and validate.

           \param uri A string with the URI to be parsed.

           \exception URIException when the given URI isn't valid.
         */
        explicit URI(const string_type& uri);

        /*! \brief Copy constructor. */
        URI(const URI& other);

        //URI(URI&& other) noexcept;

        /*! Default destructor. */
        ~URI() = default;

        /*! \brief Assingment operator. */
        URI& operator=(const URI& other);

        //URI& operator=(URI&& other) noexcept;

        /*!
          \brief Retrieving the full URI

          \return Returns the complete URI.
         */
        const string_type& uri() const;

        /*!
          \brief Retrieving the scheme

          \return Returns the URI scheme.
         */
        string_type scheme() const;

        /*!
          \brief Retrieving the user information

          \return Returns the URI user information.
         */
        string_type user() const;

        /*!
          \brief Retrieving the password information

          \return Returns the URI password information.
         */
        string_type password() const;

        /*!
          \brief Retrieving the host

          \return Returns the URI host.
         */
        string_type host() const;

        /*!
          \brief Retrieving the port

          \return Returns the URI port.
         */
        string_type port() const;

        /*!
          \brief Retrieving the path

          \return Returns the URI path.
         */
        string_type path() const;

        /*!
         \brief Retrieving the query

         \return Returns the URI query.
         */
        string_type query() const;

        /*!
          \brief Retrieving the fragment

          \return Returns the URI fragment.
         */
        string_type fragment() const;

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

        string_type hexToLetter(int i);

      private:

        string_type uri_;
        boost::match_results< const_iterator > match_;
        bool isValid_;
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_URI_URI_H__
