/*  Copyright (C) 2008-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/fe/serialization/AbstractOp.cpp
   
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

te::fe::AbstractOp* BinaryComparsionOpReader(const char* opName, te::xml::Reader& reader);

te::fe::AbstractOp* BetweenReader(const char* opName, te::xml::Reader& reader);
te::fe::AbstractOp* LikeReader(const char* opName, te::xml::Reader& reader);
te::fe::AbstractOp* NullReader(const char* opName, te::xml::Reader& reader);

te::fe::AbstractOp* BinaryLogicOpReader(const char* opName, te::xml::Reader& reader);
te::fe::AbstractOp* UnaryLogicOpReader(const char* opName, te::xml::Reader& reader);

te::fe::AbstractOp* BinarySpatialOpReader(const char* opName, te::xml::Reader& reader);
te::fe::AbstractOp* DistanceBufferReader(const char* opName, te::xml::Reader& reader);
te::fe::AbstractOp* BBOXReader(const char* opName, te::xml::Reader& reader);

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

std::string GetQualifiedName(const te::fe::AbstractOp* op)
{
  std::string name = "ogc:";
  name += op->getName();
  return name;
}

void te::fe::serialize::AbstractOp::reg(const std::string& opName, const AbstractOpFnctSerializeType& fncts)
{
  m_fncts[opName] = fncts;
}

te::fe::AbstractOp* te::fe::serialize::AbstractOp::read(te::xml::Reader& reader) const
{
  std::string opName = reader.getElementLocalName();

  AbstractOpFnctIdxType::const_iterator it = m_fncts.find(opName);

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_FE("Could not find a reader for the following operator type: %1%.")) % opName).str());

  assert(it->second.second);

  return it->second.first(m_names.find(opName)->second, reader);
}

void te::fe::serialize::AbstractOp::write(const te::fe::AbstractOp* op, te::xml::Writer& writer) const
{
  assert(op);

  AbstractOpFnctIdxType::const_iterator it = m_fncts.find(op->getName());

  if(it == m_fncts.end())
    throw Exception((boost::format(TR_FE("Could not find a writer for the following operator type: %1%.")) % op->getName()).str());

  assert(it->second.second);

  return it->second.second(op, writer);
}

te::fe::serialize::AbstractOp::~AbstractOp()
{
}

te::fe::serialize::AbstractOp::AbstractOp()
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

  // string to char*
  m_names[te::fe::Globals::sm_propertyIsEqualTo] = te::fe::Globals::sm_propertyIsEqualTo;
  m_names[te::fe::Globals::sm_propertyIsGreaterThan] = te::fe::Globals::sm_propertyIsGreaterThan;
  m_names[te::fe::Globals::sm_propertyIsGreaterThanOrEqualTo] = te::fe::Globals::sm_propertyIsGreaterThanOrEqualTo;
  m_names[te::fe::Globals::sm_propertyIsLessThan] = te::fe::Globals::sm_propertyIsLessThan;
  m_names[te::fe::Globals::sm_propertyIsLessThanOrEqualTo] = te::fe::Globals::sm_propertyIsLessThanOrEqualTo;
  m_names[te::fe::Globals::sm_propertyIsNotEqualTo] = te::fe::Globals::sm_propertyIsNotEqualTo;
  m_names[te::fe::Globals::sm_propertyIsBetween] = te::fe::Globals::sm_propertyIsBetween;
  m_names[te::fe::Globals::sm_propertyIsLike] = te::fe::Globals::sm_propertyIsLike;
  m_names[te::fe::Globals::sm_propertyIsNull] = te::fe::Globals::sm_propertyIsNull;
  m_names[te::fe::Globals::sm_and] = te::fe::Globals::sm_and;
  m_names[te::fe::Globals::sm_or] = te::fe::Globals::sm_or;
  m_names[te::fe::Globals::sm_not] = te::fe::Globals::sm_not;
  m_names[te::fe::Globals::sm_bbox] = te::fe::Globals::sm_bbox;
  m_names[te::fe::Globals::sm_contains] = te::fe::Globals::sm_contains;
  m_names[te::fe::Globals::sm_crosses] = te::fe::Globals::sm_crosses;
  m_names[te::fe::Globals::sm_disjoint] = te::fe::Globals::sm_disjoint;
  m_names[te::fe::Globals::sm_equals] = te::fe::Globals::sm_equals;
  m_names[te::fe::Globals::sm_intersects] = te::fe::Globals::sm_intersects;
  m_names[te::fe::Globals::sm_overlaps] = te::fe::Globals::sm_overlaps;
  m_names[te::fe::Globals::sm_touches] = te::fe::Globals::sm_touches;
  m_names[te::fe::Globals::sm_within] = te::fe::Globals::sm_within;
  m_names[te::fe::Globals::sm_beyond] = te::fe::Globals::sm_beyond;
  m_names[te::fe::Globals::sm_dWithin] = te::fe::Globals::sm_dWithin;
}

te::fe::AbstractOp* BinaryComparsionOpReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == opName);

  std::auto_ptr<te::fe::BinaryComparisonOp> op(new te::fe::BinaryComparisonOp(opName));

  reader.next();

  op->setFirst( te::fe::serialize::Expression::getInstance().read(reader));

  op->setSecond( te::fe::serialize::Expression::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* BetweenReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_propertyIsBetween);

  std::auto_ptr<te::fe::PropertyIsBetween> op(new te::fe::PropertyIsBetween);

  reader.next();

  op->setExpression(te::fe::serialize::Expression::getInstance().read(reader));

  op->setLowerBoundary(te::fe::serialize::Expression::getInstance().read(reader));

  op->setUpperBoundary(te::fe::serialize::Expression::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* LikeReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_propertyIsLike);
  assert(reader.hasAttrs());

  std::auto_ptr<te::fe::PropertyIsLike> op(new te::fe::PropertyIsLike);
  
  std::string wildCard = reader.getAttr("wildCard");
  assert(!wildCard.empty());
  op->setWildCard(wildCard);

  std::string singleChar = reader.getAttr("singleChar");
  assert(!singleChar.empty());
  op->setSingleChar(singleChar);

  std::string escapeChar = reader.getAttr("escapeChar");
  assert(!escapeChar.empty());
  op->setEscapeChar(escapeChar);

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  op->setPropertyName(static_cast<te::fe::PropertyName*>(te::fe::serialize::Expression::getInstance().read(reader)));

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "Literal");

  op->setLiteral(static_cast<te::fe::Literal*>(te::fe::serialize::Expression::getInstance().read(reader)));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* NullReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_propertyIsNull);

  std::auto_ptr<te::fe::PropertyIsNull> op(new te::fe::PropertyIsNull);

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  op->setPropertyName(static_cast<te::fe::PropertyName*>(te::fe::serialize::Expression::getInstance().read(reader)));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* BinaryLogicOpReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == opName);

  std::auto_ptr<te::fe::BinaryLogicOp> op(new te::fe::BinaryLogicOp(opName));

  reader.next();

  // TODO: The BinaryLogicOp has a set of AbstractOp's. I don't know how to do this. The Reader never tells about END_ELEMENT's?
  /*while(reader.getNodeType() != te::xml::END_ELEMENT)
    op->add(te::serialize::AbstractOp::getInstance().read(reader));*/

  // So, for while, read only two AbstractOp's...
  op->add(te::fe::serialize::AbstractOp::getInstance().read(reader));
  op->add(te::fe::serialize::AbstractOp::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* UnaryLogicOpReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_not);

  std::auto_ptr<te::fe::UnaryLogicOp> op(new te::fe::UnaryLogicOp(opName));

  reader.next();

  op->setOp(te::fe::serialize::AbstractOp::getInstance().read(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* BinarySpatialOpReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == opName);

  std::auto_ptr<te::fe::BinarySpatialOp> op(new te::fe::BinarySpatialOp(opName));

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  op->setProperty(static_cast<te::fe::PropertyName*>(te::fe::serialize::Expression::getInstance().read(reader)));

  // TODO: read Envelope or Geometry!
  // op->setGeometry(...);
  // op->setEnvelope(...);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* DistanceBufferReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == opName);

  std::auto_ptr<te::fe::DistanceBuffer> op(new te::fe::DistanceBuffer(opName));

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  op->setProperty(static_cast<te::fe::PropertyName*>(te::fe::serialize::Expression::getInstance().read(reader)));

  // TODO: read geom and distance!
  // op->setGeometry(...);
  // op->setDistance(...);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

te::fe::AbstractOp* BBOXReader(const char* opName, te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == te::fe::Globals::sm_bbox);

  std::auto_ptr<te::fe::BBOXOp> op(new te::fe::BBOXOp);

  reader.next();

  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "PropertyName");

  op->setProperty(static_cast<te::fe::PropertyName*>(te::fe::serialize::Expression::getInstance().read(reader)));

  // TODO: read envelope !
  // op->setEnvelope(...);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return op.release();
}

void BinaryComparsionOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinaryComparisonOp* binaryOp = dynamic_cast<const te::fe::BinaryComparisonOp*>(op);

  if(binaryOp == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(binaryOp->getFirst(), writer);

  te::fe::serialize::Expression::getInstance().write(binaryOp->getSecond(), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void BetweenWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsBetween* between = dynamic_cast<const te::fe::PropertyIsBetween*>(op);

  if(between == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(between->getExpression(), writer);

  te::fe::serialize::Expression::getInstance().write(between->getLowerBoundary(), writer);

  te::fe::serialize::Expression::getInstance().write(between->getUpperBoundary(), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void LikeWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsLike* like = dynamic_cast<const te::fe::PropertyIsLike*>(op);

  if(like == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

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

  te::fe::serialize::Expression::getInstance().write(like->getPropertyName(), writer);

  te::fe::serialize::Expression::getInstance().write(like->getLiteral(), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void NullWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::PropertyIsNull* isNull = dynamic_cast<const te::fe::PropertyIsNull*>(op);

  if(isNull == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(isNull->getPropertyName(), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void BinaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinaryLogicOp* binaryLogicOp = dynamic_cast<const te::fe::BinaryLogicOp*>(op);

  if(binaryLogicOp == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  assert(binaryLogicOp->size() >= 2);

  for(std::size_t i = 0; i < binaryLogicOp->size(); ++i)
    te::fe::serialize::AbstractOp::getInstance().write(binaryLogicOp->getOp(i), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void UnaryLogicOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::UnaryLogicOp* unaryLogicOp = dynamic_cast<const te::fe::UnaryLogicOp*>(op);

  if(unaryLogicOp == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::AbstractOp::getInstance().write(unaryLogicOp->getOp(), writer);

  writer.writeEndElement(GetQualifiedName(op));
}

void BinarySpatialOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BinarySpatialOp* binarySpatialOp = dynamic_cast<const te::fe::BinarySpatialOp*>(op);

  if(binarySpatialOp == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(binarySpatialOp->getProperty(), writer);

  // Envelope || Geometry
  //te::gml::Envelope* env = binarySpatialOp->getEnvelope();
  //te::gm::Geometry* geom = binarySpatialOp->getGeometry();

  // TODO: write Envelope or Geometry!

  writer.writeEndElement(GetQualifiedName(op));
}

void DistanceBufferOpWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::DistanceBuffer* db = dynamic_cast<const te::fe::DistanceBuffer*>(op);

  if(db == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(db->getProperty(), writer);

  te::gm::Geometry* geom = db->getGeometry();
  assert(geom);

  te::common::Distance* d = db->getDistance();
  assert(d);

  // TODO: write geom and distance!

  writer.writeEndElement(GetQualifiedName(op));
}

void BBOXWriter(const te::fe::AbstractOp* op, te::xml::Writer& writer)
{
  const te::fe::BBOXOp* bbox = dynamic_cast<const te::fe::BBOXOp*>(op);

  if(bbox == 0)
    return;

  writer.writeStartElement(GetQualifiedName(op));

  te::fe::serialize::Expression::getInstance().write(bbox->getProperty(), writer);

  te::gml::Envelope* env = bbox->getEnvelope();
  assert(env);

  // TODO: write envelope !

  writer.writeEndElement(GetQualifiedName(op));
}
