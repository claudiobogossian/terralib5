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
  \file terralib/dataaccess/datasource/ScopedTransaction.cpp

  \brief An utility class to coordinate transactions.
*/

// TerraLib
#include "DataSourceTransactor.h"
#include "ScopedTransaction.h"

te::da::ScopedTransaction::ScopedTransaction(te::da::DataSourceTransactor& transactor)
  : m_t(transactor),
    m_rollback(true)
{
  if(m_t.isInTransaction())
    m_rollback = false;

  m_t.begin();
}

te::da::ScopedTransaction::~ScopedTransaction()
{
  if(m_rollback)
    m_t.rollBack();
}

void te::da::ScopedTransaction::commit()
{
  if(!m_rollback)
    return;

  m_t.commit();

  m_rollback = false;
}
