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
  \file terralib/dataaccess/query/TemplateEncoder.cpp

  \brief A query encoder for binary operator expressions.
*/

// TerraLib
#include "Function.h"
#include "SQLVisitor.h"
#include "TemplateEncoder.h"

// STL
#include <cassert>
#include <cstdlib>

te::da::TemplateEncoder::TemplateEncoder(const std::string& name,
                                         const std::string& t)
  : SQLFunctionEncoder(name)
{
  std::size_t len = t.length();
  std::string chunk;
  std::string idx;

  for(std::size_t i = 0; i < len; ++i)
  {
    char c = t[i];
    
    if(c == '$')
    {
      if(chunk.empty() == false)
      {
        m_chuncks.push_back(chunk);
        chunk = "";
      }

      while(++i < len)
      {
        c = t[i];

        if(isdigit(c))
        {
          idx += c;          
        }
        else
        {
          chunk += c;
          break;
        }
      }

      m_argIdx.push_back(atoi(idx.c_str()));
      idx = "";
    }
    else
    {
      chunk += c;
    }
  }

  if(chunk.empty() == false)
    m_chuncks.push_back(chunk);
}

void te::da::TemplateEncoder::toSQL(const Function& f,
                                    std::string& buff,
                                    SQLVisitor& v) const
{
  assert(f.getNumArgs() == m_argIdx.size());
  std::size_t nchunks = m_chuncks.size();
  std::size_t nargs = m_argIdx.size();

  buff += m_name;

  for(size_t i = 0; i < nchunks; ++i)
  {
    if(i < nargs)
    {
      buff += m_chuncks[i];
      assert(f[i]);
      f[i]->accept(v);
    }
    else
    {
      buff += m_chuncks[i];
    }
  }
}

