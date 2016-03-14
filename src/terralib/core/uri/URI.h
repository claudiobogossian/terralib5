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

namespace te
{
  namespace core
  {

    enum class URIError
    {
      invalid_syntax,
      invalid_uri,
      invalid_scheme,
      invalid_user_info,
      invalid_host,
      invalid_port,
      invalid_path,
      invalid_query,
      invalid_fragment,
    };

    enum class URIComparisonLevel
    {
      string_comparison,
      case_normalization,
      percent_encoding_normalization,
      path_segment_normalization,
      scheme_based_normalization,
      protocol_based_normalization,
    };

    class URI
    {
      public:

// typedefs
        typedef std::string string_type;
        typedef string_type::iterator iterator;
        typedef string_type::const_iterator const_iterator;
        typedef std::iterator_traits<iterator>::value_type value_type;
        typedef boost::iterator_range<const_iterator> const_range_type;

// constructors and destructor
        URI();

        template <typename InputIter, class Alloc = std::allocator<value_type> >
        URI(const InputIter &first, const InputIter &last, const Alloc &alloc = Alloc());

        template <class Source, class Alloc = std::allocator<value_type>>
        explicit URI(const Source& source, const Alloc& alloc = Alloc());

        URI(const URI& other);

        URI(URI&& other) noexcept;

        ~URI();

// assignment
        URI& operator=(const URI& other);

        URI& operator=(URI&& other) noexcept;

// swap
        void swap(URI& other) noexcept;

// iterators
        const_iterator begin() const;

        const_iterator end() const;

        const_range_type scheme_range() const { return uriParts_.scheme; }

        const_range_type user_info_range() const
        {
          return uriParts_.hier_part.user_info ? uriParts_.hier_part.user_info.get()
                                               : const_range_type();
        }

       const_range_type host_range() const
       {
         return uriParts_.hier_part.host ? uriParts_.hier_part.host.get()
                                         : const_range_type();
       }

       const_range_type port_range() const
       {
         return uriParts_.hier_part.port ? uriParts_.hier_part.port.get()
                                         : const_range_type();
       }

       const_range_type path_range() const
       {
         return uriParts_.hier_part.path ? uriParts_.hier_part.path.get()
                                         : const_range_type();
       }

       const_range_type query_range() const
       {
         return uriParts_.query ? uriParts_.query.get() : const_range_type();
       }

       const_range_type fragment_range() const
       {
         return uriParts_.fragment ? uriParts_.fragment.get() : const_range_type();
       }

// accessors

        string_type scheme() const;
        string_type user_info() const;
        string_type host() const;
        string_type port() const;
        string_type path() const;
        string_type authority() const;
        string_type query() const;
        string_type fragment() const;
        
      private:

        string_type uri_;
        URIParts<const_iterator> uriParts_;
        bool isValid_;
    };

  }  // end namespace core
}    // end namespace te

#endif  // __TERRALIB_CORE_URI_URI_H__
