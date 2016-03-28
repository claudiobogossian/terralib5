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
  \file te/core/URI.h

  \brief A class for representing URIs (Uniform Resource Identifier).

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

// TerraLib
#include "URIParts.h"

// STL
#include <string>

// Boost
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>

namespace te
{
  namespace core
  {
    class URI
    {
    public:

      // typedefs
      typedef std::string string_type;
      //      typedef string_type::iterator iterator;
      typedef string_type::const_iterator const_iterator;
      //      typedef std::iterator_traits<iterator>::value_type value_type;
      //      typedef boost::iterator_range<const_iterator> const_range_type;


      /*!
       * \brief Default constructor.
      */
      URI();

      /*!
       * \brief The constructor that receives a URI, check the enconde, validate it and parses it.
       * \param uri An URI to be parsed.
       * \exception URIException when the given URI isn't valid.
       */
      URI(string_type uri);

      //        template <typename InputIter, class Alloc = std::allocator<value_type> >
      //        explicit URI(const InputIter &first, const InputIter &last, const Alloc &alloc = Alloc());
      //
      //        template <class Source, class Alloc = std::allocator<value_type>>
      //        explicit URI(const Source& source, const Alloc& alloc = Alloc());

      /*!
       * \brief Copy constructor.
      */
      URI(const URI& other);

      //      URI(URI&& other) noexcept;

      /*!
       * \brief Default destructor.
      */
      ~URI() = default;

      /*!
       * \brief Assingment operator.
      */
      URI& operator=(const URI& other);

      //      URI& operator=(URI&& other) noexcept;

      /*!
       * \brief Swap operation.
      */
      void swap(URI& other) noexcept;

      /*!
       * \brief Parse the URI stored in uri_ member.
       *
       *      It uses regex to validate and parse the given URI.
       *      After this, if the given URI is valid, the match_ member will have the references
       *  to all parts of the URI.
       *      The regex split the URI by using groups in regex, "(...)", so knowing the group number,
       *  you can require the corresponding match, from match_.
       *      It's important to verify the groups sequence after updating the search regex.
       *
       * \exception URIException when the given URI isn't valid.
       */
      void parse();

      //      void parseScheme(const_iterator& begin_it, const_iterator end_it);
      //      void parseHost(const_iterator& begin_it, const_iterator end_it);
      //      void parsePort(const_iterator& begin_it, const_iterator end_it);
      //      void parsePath(const_iterator& begin_it, const_iterator end_it);
      //      void parseQuery(const_iterator& begin_it, const_iterator end_it);
      //      void parseFragment(const_iterator& begin_it, const_iterator end_it);
      //      void parseUserInfo(const_iterator& begin_it, const_iterator end_it);

      // iterators
      //      const_iterator begin() const;

      //      const_iterator end() const;

      //      const_range_type scheme_range() const;

      //      const_range_type userInfo_range() const;

      //      const_range_type host_range() const;

      //      const_range_type port_range() const;

      //      const_range_type path_range() const;

      //      const_range_type query_range() const;

      //      const_range_type fragment_range() const;

      // accessors

      /*!
       * \brief Retrieving the full URI

       * \return Returns the complete URI.
       */
      string_type uri() const;

      /*!
       * \brief Retrieving the scheme

       * \return Returns the URI scheme.
       */
      string_type scheme() const;

      /*!
       * \brief Retrieving the user information

       * \return Returns the URI user information.
       */
      string_type user() const;

      /*!
       * \brief Retrieving the password information

       * \return Returns the URI password information.
       */
      string_type password() const;

      /*!
       * \brief Retrieving the host

       * \return Returns the URI host.
       */
      string_type host() const;

      /*!
       * \brief Retrieving the port

       * \return Returns the URI port.
       */
      string_type port() const;

      /*!
       * \brief Retrieving the path

       * \return Returns the URI path.
       */
      string_type path() const;

      /*!
       * \brief Retrieving the query

       * \return Returns the URI query.
       */
      string_type query() const;

      /*!
       * \brief Retrieving the fragment

       * \return Returns the URI fragment.
       */
      string_type fragment() const;

      /*!
       * \brief Return if the given URI is valid or not.

       * \return Returns true if the given URI is valid.
       */
      bool isValid() const;

      /*!
       * \brief Check if the uri_ contains any invalid character and parse it to his hexadecimal value
       */
      void encode();
      string_type hex_to_letter(int i);

    private:

      string_type uri_;
      boost::match_results< const_iterator > match_;
      bool isValid_;
      //      URIParts<const_iterator> uriParts_;
    };

    //    template <typename InputIter, class Alloc> inline
    //    URI::URI(const InputIter &first, const InputIter &last, const Alloc &alloc)
    //      : uri_(first, last), isValid_(false)
    //    {
    //      parse();
    //    }

    //    template <class Source, class Alloc> inline
    //    URI::URI(const Source& source, const Alloc& alloc)
    //
    //    {
    //    }

    //    inline URI::const_range_type URI::scheme_range() const { return uriParts_.scheme; }

    //    inline URI::const_range_type URI::userInfo_range() const
    //    {
    //      return uriParts_.hier_part.user_info ? uriParts_.hier_part.user_info.get()
    //                                           : const_range_type();
    //    }

    //    inline URI::const_range_type URI::host_range() const
    //    {
    //      return uriParts_.hier_part.host ? uriParts_.hier_part.host.get()
    //                                      : const_range_type();
    //    }

    //    inline URI::const_range_type URI::port_range() const
    //    {
    //      return uriParts_.hier_part.port ? uriParts_.hier_part.port.get()
    //                                      : const_range_type();
    //    }

    //    inline URI::const_range_type URI::path_range() const
    //    {
    //      return uriParts_.hier_part.path ? uriParts_.hier_part.path.get()
    //                                      : const_range_type();
    //    }

    //    inline URI::const_range_type URI::query_range() const
    //    {
    //      return uriParts_.query ? uriParts_.query.get() : const_range_type();
    //    }

    //    inline URI::const_range_type URI::fragment_range() const
    //    {
    //      return uriParts_.fragment ? uriParts_.fragment.get() : const_range_type();
    //    }

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_URI_URI_H__
