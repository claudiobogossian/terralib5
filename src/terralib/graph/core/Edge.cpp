#include "Edge.h"

// Terralib Includes
#include "../datatype/AbstractData.h"
#include "../common/STLUtils.h"

te::graph::Edge::Edge(int id, int vFrom, int vTo, bool isNew) : 
  m_edgeId(id), 
  m_vertexIdFrom(vFrom),
  m_vertexIdTo(vTo), 
  m_dirty(false),
  m_new(isNew)
{
}

te::graph::Edge::Edge(te::graph::Edge* rhs)
{
  m_edgeId = rhs->getId();
  m_vertexIdFrom = rhs->getIdFrom();
  m_vertexIdTo = rhs->getIdTo();
  m_dirty = false;
  m_new = true;

  this->setAttributeVecSize(rhs->getAttributes().size());

  for(size_t t = 0; t < rhs->getAttributes().size(); ++ t)
  {
    te::dt::AbstractData* ad = rhs->getAttributes()[t]->clone();
    
    this->addAttribute(t, ad);
  }
}

te::graph::Edge::~Edge()
{
  te::common::FreeContents(m_attrs);
}

int te::graph::Edge::getId()
{
  return m_edgeId;
}

int te::graph::Edge::getIdFrom()
{
  return m_vertexIdFrom;
}

int te::graph::Edge::getIdTo()
{
  return m_vertexIdTo;
}

std::vector<te::dt::AbstractData*>& te::graph::Edge::getAttributes()
{
  return m_attrs;
}

void te::graph::Edge::setAttributeVecSize(int size)
{
  m_attrs.resize(size);
}

void te::graph::Edge::addAttribute(int idx, te::dt::AbstractData* ad)
{
  if(m_attrs[idx])
    delete m_attrs[idx];
    
   m_attrs[idx] = ad;

   return;
}

void te::graph::Edge::removeAttribute(int idx)
{
  delete m_attrs[idx];

  return;
}

void te::graph::Edge::setDirty(bool flag)
{
  m_dirty = flag;
}

bool te::graph::Edge::isDirty()
{
  return m_dirty;
}

bool te::graph::Edge::isNew()
{
  return m_new;
}
