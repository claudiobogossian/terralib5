/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/fe/AbstractOp.cpp
   
  \brief Auxiliary classes and functions to serialize filter operations from a XML document.
*/

// TerraLib
#include "../../common/Translator.h"
#include "../../fe/BBOXOp.h"
#include "../../fe/BinaryComparisonOp.h"
#include "../../fe/BinaryLogicOp.h"
#include "../../fe/BinarySpatialOp.h"
#include "../../fe/DistanceBuffer.h"
#include "../../fe/Globals.h"
#include "../../fe/Literal.h"
#include "../../fe/PropertyIsBetween.h"
#include "../../fe/PropertyIsLike.h"
#include "../../fe/PropertyIsNull.h"
#include "../../fe/PropertyName.h"
#include "../../fe/UnaryLogicOp.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../Exception.h"
#include "AbstractOp.h"
#include "Expression.h"

// STL
#include <cassert>

// Boost
#include <boost/format.hpp>


/* @name AbstractOp Reader Methods */
//@{

te::fe::AbstractOp* BinaryComparsionOpReader(te::xml::Reader& reader);

te::fe::AbstractOp* BetweenReader(te::xml::Reader& reader);
te::fe::AbstractOp* LikeReader(te::xml::Reader& reader);
te::fe::AbstractOp* NullReader(te::xml::Reader& reader);

te::fe::AbstractOp* BinaryLogicOpReader(te::xml::Reader& reader);
te::fe::AbstractOp* UnaryLogicOpReader(te::xml::Reader& reader);

te::fe::AbstractOp* BinarySpatialOpReader(te::xml::Reader& reader);
te::fe::AbstractOp* DistanceBufferReader(te::xml::Reader& reader);
te::fe::AbstractOp* BBOXReader(te::xml::Reader& reader);

//@}

/* @name AbstractOp Writer Methods */
//@{

void BinaryComparsionOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);

void BetweenWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);
void LikeWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);
void NullWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);

void BinaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);
void UnaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);

void BinarySpatialOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);
void DistanceBufferOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);
void BBOXWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer);

//@}

void te::serialize::AbstractOp::reg(const std::string& opName, const AbstractOpFnctSerializeType& fncts)
{
  m_fncts[opName] = fncts;
}

te::fe::AbstractOp* te::serialize::AbstractOp::read(te::xml::Reader& reader) const
{
  std::string opName = reader.getElementLocalName();

  AbstractOpFnctIdxType::const_iterator it = m_fncts.find(opName);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a reader for the following operator type: %1%.")) % opName).str());

  assert(it->second.second);

  return it->second.first(reader);
}

void te::serialize::AbstractOp::write(const te::fe::AbstractOp* op, te::xml::Writer& writer) const
{
  assert(op);

  AbstractOpFnctIdxType::const_iterator it = m_fncts.find(op->getName());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_SERIALIZATION("Could not find a writer for the following operator type: %1%.")) % op->getName()).str());

  assert(it->second.second);

  return it->second.second(op, writer);
}

te::serialize::AbstractOp::~AbstractOp()
{
}

te::serialize::AbstractOp::AbstractOp()
{
  // BinaryComparisonOp
  m_fncts[te::fe::Globals::sm_propertyIsEqualTo] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));
  m_fncts[te::fe::Globals::sm_propertyIsGreaterThan] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));
  m_fncts[te::fe::Globals::sm_propertyIsGreaterThanOrEqualTo] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));
  m_fncts[te::fe::Globals::sm_propertyIsLessThan] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));
  m_fncts[te::fe::Globals::sm_propertyIsLessThanOrEqualTo] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));
  m_fncts[te::fe::Globals::sm_propertyIsNotEqualTo] = std::make_pair(AbstractOpReadFnctType(&BinaryComparsionOpReader), AbstractOpWriteFnctType(&BinaryComparsionOpWriter));

  // IsBetween
  m_fncts[te::fe::Globals::sm_propertyIsBetween] = std::make_pair(AbstractOpReadFnctType(&BetweenReader), AbstractOpWriteFnctType(&BetweenWriter));

  // IsLike
  m_fncts[te::fe::Globals::sm_propertyIsLike] = std::make_pair(AbstractOpReadFnctType(&LikeReader), AbstractOpWriteFnctType(&LikeWriter));

  // Null
  m_fncts[te::fe::Globals::sm_propertyIsNull] = std::make_pair(AbstractOpReadFnctType(&NullReader), AbstractOpWriteFnctType(&NullWriter));

  // BinaryLogicOp
  m_fncts[te::fe::Globals::sm_and] = std::make_pair(AbstractOpReadFnctType(&BinaryLogicOpReader), AbstractOpWriteFnctType(&BinaryLogicOpWriter));
  m_fncts[te::fe::Globals::sm_or] = std::make_pair(AbstractOpReadFnctType(&BinaryLogicOpReader), AbstractOpWriteFnctType(&BinaryLogicOpWriter));

  // UnaryLogicOp
  m_fncts[te::fe::Globals::sm_not] = std::make_pair(AbstractOpReadFnctType(&UnaryLogicOpReader), AbstractOpWriteFnctType(&UnaryLogicOpWriter));

  // BBOX
  m_fncts[te::fe::Globals::sm_bbox] = std::make_pair(AbstractOpReadFnctType(&BBOXReader), AbstractOpWriteFnctType(&BBOXWriter));

  // Binary SpatialOp
  m_fncts[te::fe::Globals::sm_contains] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_crosses] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_disjoint] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_equals] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_intersects] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_overlaps] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_touches] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));
  m_fncts[te::fe::Globals::sm_within] = std::make_pair(AbstractOpReadFnctType(&BinarySpatialOpReader), AbstractOpWriteFnctType(&BinarySpatialOpWriter));

  // DistanceBuffer
  m_fncts[te::fe::Globals::sm_beyond] = std::make_pair(AbstractOpReadFnctType(&DistanceBufferReader), AbstractOpWriteFnctType(&DistanceBufferOpWriter));
  m_fncts[te::fe::Globals::sm_dWithin] = std::make_pair(AbstractOpReadFnctType(&DistanceBufferReader), AbstractOpWriteFnctType(&DistanceBufferOpWriter));
}

te::fe::AbstractOp* BinaryComparsionOpReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* BetweenReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* LikeReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* NullReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* BinaryLogicOpReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* UnaryLogicOpReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* BinarySpatialOpReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* DistanceBufferReader(te::xml::Reader& reader)
{
  return 0;
}

te::fe::AbstractOp* BBOXReader(te::xml::Reader& reader)
{
  return 0;
}

void BinaryComparsionOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinaryComparisonOp* binaryOp = dynamic_cast<const te::fe::BinaryComparisonOp*>(op);

  if(binaryOp == 0)
    return;

  writer.writeStartElement(binaryOp->getName());

  te::serialize::Expression::getInstance().write(binaryOp->getFirst(), writer);

  te::serialize::Expression::getInstance().write(binaryOp->getSecond(), writer);

  writer.writeEndElement(binaryOp->getName());
}

void BetweenWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsBetween* between = dynamic_cast<const te::fe::PropertyIsBetween*>(op);

  if(between == 0)
    return;

  writer.writeStartElement(between->getName());

  te::serialize::Expression::getInstance().write(between->getExpression(), writer);

  te::serialize::Expression::getInstance().write(between->getLowerBoundary(), writer);

  te::serialize::Expression::getInstance().write(between->getUpperBoundary(), writer);

  writer.writeEndElement(between->getName());
}

void LikeWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsLike* like = dynamic_cast<const te::fe::PropertyIsLike*>(op);

  if(like == 0)
    return;

  writer.writeStartElement(like->getName());

  // Attributes
  std::string wildCard = like->getWildCard();
  assert(!wildCard.empty());

  std::string singleChar = like->getSingleChar();
  assert(!singleChar.empty());

  std::string escapeChar = like->getEscapeChar();
  assert(!escapeChar.empty());

  writer.writeAttribute("wildCard", wildCard);
  writer.writeAttribute("singleChar", singleChar);
  writer.writeAttribute("escapeChar", escapeChar);

  te::serialize::Expression::getInstance().write(like->getPropertyName(), writer);

  te::serialize::Expression::getInstance().write(like->getLiteral(), writer);

  writer.writeEndElement(like->getName());
}

void NullWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsNull* isNull = dynamic_cast<const te::fe::PropertyIsNull*>(op);

  if(isNull == 0)
    return;

  writer.writeStartElement(isNull->getName());

  te::serialize::Expression::getInstance().write(isNull->getPropertyName(), writer);

  writer.writeEndElement(isNull->getName());
}

void BinaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinaryLogicOp* binaryLogicOp = dynamic_cast<const te::fe::BinaryLogicOp*>(op);

  if(binaryLogicOp == 0)
    return;

  writer.writeStartElement(binaryLogicOp->getName());

  assert(binaryLogicOp->size() >= 2);

  for(std::size_t i = 0; i < binaryLogicOp->size(); ++i)
    te::serialize::AbstractOp::getInstance().write(binaryLogicOp->getOp(i), writer);

  writer.writeEndElement(binaryLogicOp->getName());
}

void UnaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::UnaryLogicOp* unaryLogicOp = dynamic_cast<const te::fe::UnaryLogicOp*>(op);

  if(unaryLogicOp == 0)
    return;

  writer.writeStartElement(unaryLogicOp->getName());

  te::serialize::AbstractOp::getInstance().write(unaryLogicOp->getOp(), writer);

  writer.writeEndElement(unaryLogicOp->getName());
}

void BinarySpatialOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinarySpatialOp* binarySpatialOp = dynamic_cast<const te::fe::BinarySpatialOp*>(op);

  if(binarySpatialOp == 0)
    return;

  writer.writeStartElement(binarySpatialOp->getName());

  te::serialize::Expression::getInstance().write(binarySpatialOp->getProperty(), writer);

  // Envelope || Geometry
  te::gml::Envelope* env = binarySpatialOp->getEnvelope();
  te::gm::Geometry* geom = binarySpatialOp->getGeometry();

  // TODO: write Envelope or Geometry!

  writer.writeEndElement(binarySpatialOp->getName());
}

void DistanceBufferOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::DistanceBuffer* db = dynamic_cast<const te::fe::DistanceBuffer*>(op);

  if(db == 0)
    return;

  writer.writeStartElement(db->getName());

  te::serialize::Expression::getInstance().write(db->getProperty(), writer);

  te::gm::Geometry* geom = db->getGeometry();
  assert(geom);

  te::common::Distance* d = db->getDistance();
  assert(d);

  // TODO: write geom and distance!

  writer.writeEndElement(db->getName());
}

void BBOXWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BBOXOp* bbox = dynamic_cast<const te::fe::BBOXOp*>(op);

  if(bbox == 0)
    return;

  writer.writeStartElement(bbox->getName());

  te::serialize::Expression::getInstance().write(bbox->getProperty(), writer);

  te::gml::Envelope* env = bbox->getEnvelope();
  assert(env);

  // TODO: write envelope !

  writer.writeEndElement(bbox->getName());
}
