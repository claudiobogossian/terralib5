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
  \file MergeMemory.h

  \brief Merge Vector Processing functions.
*/

//Terralib
#include "../dataaccess/utils/Utils.h"
#include "../geometry/GeometryProperty.h"
#include "../memory/DataSet.h"
#include "../memory/DataSetItem.h"
#include "MergeMemory.h"
#include "Utils.h"

const int BLOCKSIZE = 10000;

te::vp::MergeMemory::MergeMemory()
{}

te::vp::MergeMemory::~MergeMemory()
{}

bool te::vp::MergeMemory::run() throw( te::common::Exception )
{
  std::auto_ptr<te::da::DataSetType> dst(getOutputDst());
  te::mem::DataSet* ds = new te::mem::DataSet(dst.get());

  te::gm::GeometryProperty* fgGrop = te::da::GetFirstGeomProperty(m_firstDst);
  te::gm::GeometryProperty* sgGrop = te::da::GetFirstGeomProperty(m_secondDst);
  te::gm::GeometryProperty* outgGrop = te::da::GetFirstGeomProperty(dst.get());
  
  std::size_t fgPos = m_firstDst->getPropertyPosition(fgGrop);
  std::size_t sgPos = m_secondDst->getPropertyPosition(sgGrop);
  std::size_t outgPos = dst->getPropertyPosition(outgGrop);
  
  int pkCount = 0;
  int count = 0;

  m_firstDs->moveBeforeFirst();
  m_secondDs->moveBeforeFirst();

  std::map<std::string, std::string> op;

  while (m_firstDs->moveNext())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);
    item->setInt32(0, pkCount);
    ++pkCount;

    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
      if (!m_properties[i].first.empty())
      {
        item->setValue(m_properties[i].first, m_firstDs->getValue(m_properties[i].first).release());
      }
    }

    item->setGeometry(outgPos, m_firstDs->getGeometry(fgPos).release());

    ds->add(item);

    if (count == 0)
    {
      te::vp::Save(m_outDsrc.get(), ds, dst.get());

      delete ds;

      ds = new te::mem::DataSet(dst.get());
    }

    if ((count / BLOCKSIZE) >= 1)
    {
      m_outDsrc->add(dst->getName(), ds, op);

      delete ds;

      ds = new te::mem::DataSet(dst.get());
    }

    ++count;
  }

  while (m_secondDs->moveNext())
  {
    te::mem::DataSetItem* item = new te::mem::DataSetItem(ds);
    item->setInt32(0, pkCount);
    ++pkCount;

    for (std::size_t i = 0; i < m_properties.size(); ++i)
    {
      if (!m_properties[i].second.empty())
      {
        if (!m_properties[i].first.empty())
        {
          item->setValue(m_properties[i].first, m_secondDs->getValue(m_properties[i].second).release());
        }
        else
        {
          item->setValue(m_properties[i].second, m_secondDs->getValue(m_properties[i].second).release());
        }
      }
    }

    std::auto_ptr<te::gm::Geometry> geom = m_secondDs->getGeometry(sgPos);

    if (fgGrop->getSRID() != sgGrop->getSRID())
    {
      geom->transform(fgGrop->getSRID());
    }

    item->setGeometry(outgPos, geom.release());

    ds->add(item);

    if ((count / BLOCKSIZE) >= 1)
    {
      m_outDsrc->add(dst->getName(), ds, op);

      delete ds;

      ds = new te::mem::DataSet(dst.get());
    }

    ++count;
  }

  if (ds)
  {
    m_outDsrc->add(dst->getName(), ds, op);
    delete ds;
  }

  return true;
}
