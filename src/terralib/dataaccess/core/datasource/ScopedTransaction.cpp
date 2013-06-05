/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/core/datasource/ScopedTransaction.cpp

  \brief An utility class to coordinate transactions.
*/

// TerraLib
#include "DataSource.h"
#include "ScopedTransaction.h"

te::da::core::ScopedTransaction::ScopedTransaction(DataSource& ds)
  : m_ds(ds),
    m_rollback(true)
{
  if(m_ds.isInTransaction())
    m_rollback = false;

  m_ds.begin();
}

te::da::core::ScopedTransaction::~ScopedTransaction()
{
  if(m_rollback)
    m_ds.rollBack();
}

void te::da::core::ScopedTransaction::commit()
{
  if(!m_rollback)
    return;

  m_ds.commit();

  m_rollback = false;
}
