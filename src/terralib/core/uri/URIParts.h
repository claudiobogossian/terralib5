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
  \file te/core/URIParts.h

  \brief A struct that represents the parts of an URI.

  \author Gilberto Ribeiro de Queiroz
  \author Vinicius Campanha
 */
 
/*
  This implementation is based on teh source code of cppnetlib.
 */
 
// Copyright 2009, 2010, 2011, 2012 Dean Michael Berris, Jeroen Habraken, Glyn
// Matthews.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __TERRALIB_CORE_URI_URIPARTS_H__
#define __TERRALIB_CORE_URI_URIPARTS_H__

// STL
#include <iterator>

// Boost
#include <boost/optional.hpp>
#include <boost/range/iterator_range.hpp>

namespace te
{
  namespace core
  {

    template <class FwdIter>
    struct HierarchicalPart
    {
      boost::optional<boost::iterator_range<FwdIter> > user_info;
      boost::optional<boost::iterator_range<FwdIter> > host;
      boost::optional<boost::iterator_range<FwdIter> > port;
      boost::optional<boost::iterator_range<FwdIter> > path;

      FwdIter begin() const { return std::begin(user_info); }

      FwdIter end() const { return std::end(path); }

      void update()
      {
        if (!user_info)
        {
          if (host)
          {
            user_info = boost::iterator_range<FwdIter>(std::begin(host.get()),
                                                       std::begin(host.get()));
          }
          else if (path)
          {
            user_info = boost::iterator_range<FwdIter>(std::begin(path.get()),
                                                       std::begin(path.get()));
          }
        }

        if (!host)
        {
          host = boost::iterator_range<FwdIter>(std::begin(path.get()),
                                                std::begin(path.get()));
        }

        if (!port)
        {
          port = boost::iterator_range<FwdIter>(std::end(host.get()),
                                                std::end(host.get()));
        }

        if (!path)
        {
          path = boost::iterator_range<FwdIter>(std::end(port.get()),
                                                std::end(port.get()));
        }
      }
  
    };

    template <class FwdIter>
    struct URIParts
    {
      boost::iterator_range<FwdIter> scheme;
      HierarchicalPart<FwdIter> hier_part;
      boost::optional<boost::iterator_range<FwdIter> > query;
      boost::optional<boost::iterator_range<FwdIter> > fragment;

      FwdIter begin() const { return std::begin(scheme); }

      FwdIter end() const { return std::end(fragment); }

      void update()
      {
        hier_part.update();

        if (!query)
        {
          query = boost::iterator_range<FwdIter>(std::end(hier_part.path.get()),
                                                 std::end(hier_part.path.get()));
        }

        if (!fragment)
        {
          fragment = boost::iterator_range<FwdIter>(std::end(query.get()),
                                                    std::end(query.get()));
        }
      }
    };


  }  // end namespace core
}    // end namespace te

#endif // __TERRALIB_CORE_URI_URIPARTS_H__
