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
  \file terralib/maptools/FolderLayer.cpp

  \brief A layer that can be used as a container for other kind of layers.
*/

// TerraLib
#include "FolderLayer.h"

const std::string te::map::FolderLayer::sm_type("FOLDERLAYER");

te::map::FolderLayer::FolderLayer(AbstractLayer* parent)
  : AbstractLayer(parent)
{
}

te::map::FolderLayer::FolderLayer(const std::string& id, AbstractLayer* parent)
  : AbstractLayer(id, parent)
{
}

te::map::FolderLayer::FolderLayer(const std::string& id,
                                  const std::string& title,
                                  AbstractLayer* parent)
  : AbstractLayer(id, title, parent)
{
}

te::map::FolderLayer::~FolderLayer()
{
}

void te::map::FolderLayer::setVisibility(Visibility v)
{
  if(hasChildren())
  {
    int numChildren = getChildrenCount();
    std::vector<AbstractLayer*> childrenVec(numChildren);

    for(int i = 0; i < numChildren; ++i)
    {
      childrenVec[i] = static_cast<AbstractLayer*>(getChild(i).get());
      childrenVec[i]->setVisibility(v);
    }
  }

  // Set the status of the flag that indicates if the layer visibility has changed
  m_visibilityChanged = false;
  Visibility prevVisibility = m_visibility;
  m_visibility = v;
  
  if(m_visibility != prevVisibility)
    m_visibilityChanged = true;
}

void te::map::FolderLayer::updateVisibility()
{
  Visibility prevVisibility = m_visibility;
  m_visibilityChanged = false;

  if(hasChildren() == false)
  {
    m_visibility = te::map::NOT_VISIBLE;

    if(m_visibility != prevVisibility)
      m_visibilityChanged = true;

    return;
  }

  bool allVisible = true;
  bool allNotVisible = true;

  int numChildren = getChildrenCount();
  std::vector<AbstractLayer*> childrenVec(numChildren);

  for(int i = 0; i < numChildren; ++i)
  {
    childrenVec[i] = static_cast<AbstractLayer*>(getChild(i).get());

    if(childrenVec[i]->getVisibility() == te::map::NOT_VISIBLE)
      allVisible = false;
    else if(childrenVec[i]->getVisibility() == te::map::VISIBLE)
      allNotVisible = false;
    else if(childrenVec[i]->getVisibility() == te::map::PARTIALLY_VISIBLE)
    {
      allVisible = false;
      allNotVisible = false;
    }
  }

  m_visibility = te::map::PARTIALLY_VISIBLE;

  if(allVisible)
    m_visibility = te::map::VISIBLE;
  else if(allNotVisible)
    m_visibility = te::map::NOT_VISIBLE;

  if(m_visibility != prevVisibility)
    m_visibilityChanged = true;

  updateVisibilityOfAncestors();
}

std::auto_ptr<te::map::LayerSchema> te::map::FolderLayer::getSchema() const
{
  return std::auto_ptr<LayerSchema>(0);
}

std::auto_ptr<te::da::DataSet> te::map::FolderLayer::getData(te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::map::FolderLayer::getData(const std::string& /*propertyName*/,
                                                             const te::gm::Envelope* /*e*/,
                                                             te::gm::SpatialRelation /*r*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::map::FolderLayer::getData(const std::string& /*propertyName*/,
                                                             const te::gm::Geometry* /*g*/,
                                                             te::gm::SpatialRelation /*r*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::map::FolderLayer::getData(te::da::Expression* /*restriction*/,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy accessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>(0);
}

std::auto_ptr<te::da::DataSet> te::map::FolderLayer::getData(const te::da::ObjectIdSet* oids,
                                                             te::common::TraverseType /*travType*/,
                                                             const te::common::AccessPolicy) const
{
  return std::auto_ptr<te::da::DataSet>(0);
}

bool te::map::FolderLayer::isValid() const
{
  return true;
}

void te::map::FolderLayer::draw(Canvas* /*canvas*/, const te::gm::Envelope& /*bbox*/, int /*srid*/)
{
}

const std::string& te::map::FolderLayer::getType() const
{
  return sm_type;
}
