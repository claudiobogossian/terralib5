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
  \file terralib/xsd/XSDSerializer.cpp

  \brief Data serialization for the XSD module.
*/

// TerraLib
#include "../../../common/StringUtils.h"
#include "../../../xml/AbstractWriter.h"
#include "../../../xml/Reader.h"
#include "../../../xml/ReaderFactory.h"
#include "../../All.h"
#include "../../Annotated.h"
#include "../../Annotation.h"
#include "../../Any.h"
#include "../../AnyAttribute.h"
#include "../../AppInfo.h"
#include "../../Attribute.h"
#include "../../AttributeGroup.h"
#include "../../Choice.h"
#include "../../ComplexContent.h"
#include "../../ComplexType.h"
#include "../../Documentation.h"
#include "../../Element.h"
#include "../../Exception.h"
#include "../../Extension.h"
#include "../../Field.h"
#include "../../Group.h"
#include "../../Import.h"
#include "../../Include.h"
#include "../../Key.h"
#include "../../KeyRef.h"
#include "../../List.h"
#include "../../QName.h"
#include "../../Redefine.h"
#include "../../Restriction4ComplexContent.h"
#include "../../Restriction4SimpleContent.h"
#include "../../Restriction4SimpleType.h"
#include "../../Selector.h"
#include "../../SimpleContent.h"
#include "../../SimpleType.h"
#include "../../Schema.h"
#include "../../Sequence.h"
#include "../../Union.h"
#include "../../Unique.h"
#include "XSDSerializer.h"

// STL
#include <cassert>
#include <memory>
#include <set>

// Boost
#include <boost/format.hpp>

//#ifdef TE_ENABLED_XML

te::xsd::All* te::xsd::serialize::ReadAll(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "all");

  std::auto_ptr<All> all(new All);

// Id
  ReadIdentifiable(all.get(), reader);

// MinOccurs and MaxOccurs
  ReadOccurs(all.get(), reader);

  reader.next();

// Grammar: (annotation?,element*)

// Annotation
  ReadAnnotated(all.get(), reader);

// Elements
  while(reader.getNodeType() == te::xml::START_ELEMENT && (reader.getElementLocalName() == "element"))
    all->addElement(ReadElement(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return all.release();
}

void te::xsd::serialize::Save(All* all, te::xml::AbstractWriter& writer)
{
}

te::xsd::Annotation* te::xsd::serialize::ReadAnnotation(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "annotation");

  std::auto_ptr<Annotation> annotation(new Annotation());

  // Id
  ReadIdentifiable(annotation.get(), reader);

  reader.next();

  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       ((reader.getElementLocalName() == "appinfo") ||
        (reader.getElementLocalName() == "documentation")))
  {
    std::string tag = reader.getElementLocalName();
    if(tag == "appinfo")
    {
// Reads the AppInfo
      AppInfo* appinfo = new AppInfo(0, 0);

// Source
      std::size_t pos = reader.getAttrPosition("version");
      if(pos != std::string::npos)
        appinfo->setSource(new std::string(reader.getAttr(pos)));

// Value
      reader.next();
      assert(reader.getNodeType() == te::xml::VALUE);
      appinfo->setValue(new std::string(reader.getElementValue()));

// Adds the appinfo on annotation
      annotation->add(appinfo);

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of AppInfo element

      reader.next();

      continue;
    }

    if(tag == "documentation")
    {
// Reads the Documentation
      te::xsd::Documentation* doc = new te::xsd::Documentation(0);

// Source
      std::size_t pos = reader.getAttrPosition("version");
      if(pos != std::string::npos)
        doc->setSource(new std::string(reader.getAttr(pos)));

// xml:lang
      pos = reader.getAttrPosition("xml:lang");
      if(pos != std::string::npos)
        doc->setLang(new std::string(reader.getAttr(pos)));

// Value
      reader.next();
      assert(reader.getNodeType() == te::xml::VALUE);
      doc->setValue(new std::string(reader.getElementValue()));

// Adds the appinfo on annotation
      annotation->add(doc);

      reader.next();
      assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Documentation element

      reader.next();
    }
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return annotation.release();
}

void te::xsd::serialize::Save(Annotation* annotation, te::xml::AbstractWriter& writer)
{
}

te::xsd::Any* te::xsd::serialize::ReadAny(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "any");

  std::auto_ptr<Any> any(new Any);

// Id
  ReadIdentifiable(any.get(), reader);

// MinOccurs and MaxOccurs
  ReadOccurs(any.get(), reader);

// Namespace
  std::size_t pos = reader.getAttrPosition("namespace");

  if(pos != std::string::npos)
    any->setNamespace(new std::string(reader.getAttr(pos)));

// ProcessContents
  pos = reader.getAttrPosition("processContents");

  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    ProcessContents pc = te::xsd::LAX;
    value == "skip" ? pc = te::xsd::SKIP : pc = te::xsd::STRICT;
    any->setProcessContents(pc);
  }

  reader.next();

// Grammar: (annotation?)

// Annotation
  ReadAnnotated(any.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return any.release();
}

void te::xsd::serialize::Save(Any* any, te::xml::AbstractWriter& writer)
{
}

te::xsd::AnyAttribute* te::xsd::serialize::ReadAnyAttribute(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "anyAttribute");

  std::auto_ptr<te::xsd::AnyAttribute> anyAttribute(new te::xsd::AnyAttribute);

  // Id
  ReadIdentifiable(anyAttribute.get(), reader);

  // Namespace
  std::size_t pos = reader.getAttrPosition("namespace");
  if(pos != std::string::npos)
    anyAttribute->setNamespace(new std::string(reader.getAttr(pos)));

  // ProcessContents
  pos = reader.getAttrPosition("processContents");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    te::xsd::ProcessContents pc = te::xsd::LAX;
    value == "skip" ? pc = te::xsd::SKIP : pc = te::xsd::STRICT;
    anyAttribute->setProcessContents(pc);
  }

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(anyAttribute.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return anyAttribute.release();
}

void te::xsd::serialize::Save(AnyAttribute* anyAttribute, te::xml::AbstractWriter& writer)
{
}

te::xsd::Attribute* te::xsd::serialize::ReadAttribute(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "attribute");

  std::auto_ptr<te::xsd::Attribute> attribute(new te::xsd::Attribute);

  // Id
  ReadIdentifiable(attribute.get(), reader);

  // Default
  std::size_t pos = reader.getAttrPosition("default");
  if(pos != std::string::npos)
    attribute->setDefault(new std::string(reader.getAttr(pos)));

  // Fixed
  pos = reader.getAttrPosition("fixed");
  if(pos != std::string::npos)
    attribute->setFixed(new std::string(reader.getAttr(pos)));

  // Form
  pos = reader.getAttrPosition("Form");
  if(pos != std::string::npos)
    attribute->setForm(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // Name
  pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    attribute->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    attribute->setRef(CreateQName(reader.getAttr(pos)));

  // Type
  pos = reader.getAttrPosition("type");
  if(pos != std::string::npos)
    attribute->setType(CreateQName(reader.getAttr(pos)));

  // Use
  pos = reader.getAttrPosition("use");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    te::xsd::AttributeUse use = te::xsd::OPTIONAL;
    value == "prohibited" ? use = te::xsd::PROHIBITED : use = te::xsd::REQUIRED;
    attribute->setUse(use);
  }

  reader.next();

  // Grammar: (annotation?,(simpleType?))

  // Annotation
  ReadAnnotated(attribute.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
    attribute->setInnerType(ReadSimpleType(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return attribute.release();
}

void te::xsd::serialize::Save(Attribute* attribute, te::xml::AbstractWriter& writer)
{
}

te::xsd::AttributeGroup* te::xsd::serialize::ReadAttributeGroup(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "attributeGroup");

  std::auto_ptr<te::xsd::AttributeGroup> attributeGroup(new te::xsd::AttributeGroup);

  // Id
  ReadIdentifiable(attributeGroup.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    attributeGroup->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    attributeGroup->setRef(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?),((attribute|attributeGroup)*,anyAttribute?))

  // Annotation
  ReadAnnotated(attributeGroup.get(), reader);

  std::set<std::string> children;
  children.insert("attribute");
  children.insert("attributeGroup");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "attribute")
    {
      attributeGroup->addAttribute(ReadAttribute(reader));
      continue;
    }

    attributeGroup->addAttribute(ReadAttributeGroup(reader));
  }

  // AnyAttribute
  if(reader.getElementLocalName() == "anyAttribute")
    attributeGroup->setAnyAttribute(ReadAnyAttribute(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return attributeGroup.release();
}

void te::xsd::serialize::Save(AttributeGroup* attributeGroup, te::xml::AbstractWriter& writer)
{
}

te::xsd::Choice* te::xsd::serialize::ReadChoice(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "choice");

  std::auto_ptr<te::xsd::Choice> choice(new te::xsd::Choice);

  // Id
  ReadIdentifiable(choice.get(), reader);

  // MinOccurs and MaxOccurs
  ReadOccurs(choice.get(), reader);

  reader.next();

  // Grammar: (annotation?,(element|group|choice|sequence|any)*)

  // Annotation
  ReadAnnotated(choice.get(), reader);

  std::set<std::string> children;
  children.insert("element");
  children.insert("group");
  children.insert("choice");
  children.insert("sequence");
  children.insert("any");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "element")
    {
      choice->addElement(ReadElement(reader));
      continue;
    }

    if(tag == "group")
    {
      choice->addContent(ReadGroup(reader));
      continue;
    }

    if(tag == "choice")
    {
      choice->addContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      choice->addContent(ReadSequence(reader));
      continue;
    }

    if(tag == "any")
      choice->addAny(ReadAny(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return choice.release();
}

void te::xsd::serialize::Save(Choice* choice, te::xml::AbstractWriter& writer)
{
}

te::xsd::ComplexContent* te::xsd::serialize::ReadComplexContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "complexContent");

  std::auto_ptr<te::xsd::ComplexContent> cc(new te::xsd::ComplexContent);

  // Id
  ReadIdentifiable(cc.get(), reader);

  // Mixed
  std::size_t pos = reader.getAttrPosition("mixed");
  if(pos != std::string::npos)
    cc->setAsMixed(reader.getAttr(pos) == "true" ? true : false);

  reader.next();

  /* Grammar: (annotation?,(restriction|extension)) */

  // Annotation
  ReadAnnotated(cc.get(), reader);

  // Restriction
  if(reader.getElementLocalName() == "restriction")
  {
    cc->setTypeDerivation(ReadRestriction4ComplexContent(reader));
    return cc.release();
  }

  assert(reader.getElementLocalName() == "extension");
  cc->setTypeDerivation(ReadExtension(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return cc.release();
}

void te::xsd::serialize::Save(ComplexContent* cc, te::xml::AbstractWriter& writer)
{
}

te::xsd::ComplexType* te::xsd::serialize::ReadComplexType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "complexType");

  std::auto_ptr<te::xsd::ComplexType> ct(new te::xsd::ComplexType);

  // Id
  ReadIdentifiable(ct.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    ct->setName(new std::string(reader.getAttr(pos)));

  // Abstract
  pos = reader.getAttrPosition("abstract");
  if(pos != std::string::npos)
    ct->setAsAbstract(reader.getAttr(pos) == "true" ? true : false);

  // Mixed
  pos = reader.getAttrPosition("mixed");
  if(pos != std::string::npos)
    ct->setAsMixed(reader.getAttr(pos) == "true" ? true : false);

  // TODO: Block and Final ?

  reader.next();

  /* Grammar: (annotation?,(simpleContent|complexContent|((group|all|
              choice|sequence)?,((attribute|attributeGroup)*,anyAttribute?)))) */

  // Annotation
  ReadAnnotated(ct.get(), reader);

  std::set<std::string> children;
  children.insert("simpleContent");
  children.insert("complexContent");
  children.insert("group");
  children.insert("all");
  children.insert("choice");
  children.insert("sequence");
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "simpleContent")
    {
      ct->setSimpleContent(ReadSimpleContent(reader));
      continue;
    }

    if(tag == "complexContent")
    {
      ct->setComplexContent(ReadComplexContent(reader));
      continue;
    }

    if(tag == "group")
    {
      ct->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      ct->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      ct->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      ct->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      ct->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      ct->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      ct->setAnyAttribute(ReadAnyAttribute(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return ct.release();
}

void te::xsd::serialize::Save(ComplexType* ct, te::xml::AbstractWriter& writer)
{
}

te::xsd::Element* te::xsd::serialize::ReadElement(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "element");

  std::auto_ptr<te::xsd::Element> element(new te::xsd::Element);

  // Id
  ReadIdentifiable(element.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    element->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    element->setRef(CreateQName(reader.getAttr(pos)));

  // Type
  pos = reader.getAttrPosition("type");
  if(pos != std::string::npos)
    element->setType(CreateQName(reader.getAttr(pos)));

  // SubstitutionGroup
  pos = reader.getAttrPosition("substitutionGroup");
  if(pos != std::string::npos)
    element->setSubstitutionGroup(CreateQName(reader.getAttr(pos)));

  // Default
  pos = reader.getAttrPosition("default");
  if(pos != std::string::npos)
    element->setDefaultValue(new std::string(reader.getAttr(pos)));

  // Fixed
  pos = reader.getAttrPosition("fixed");
  if(pos != std::string::npos)
    element->setFixedValue(new std::string(reader.getAttr(pos)));

  // MinOccurs and MaxOccurs
  ReadOccurs(element.get(), reader);

  // Nillable
  pos = reader.getAttrPosition("nillable");
  if(pos != std::string::npos)
    element->setAsNillable(reader.getAttr(pos) == "true" ? true : false);

  // Abstract
  pos = reader.getAttrPosition("abstract");
  if(pos != std::string::npos)
    element->setAsAbstract(reader.getAttr(pos) == "true" ? true : false);

  // TODO: Block and Final ?

  reader.next();

  // Grammar: annotation?,((simpleType|complexType)?,(unique|key|keyref)*))

  ReadAnnotated(element.get(), reader);

  if(reader.getElementLocalName() == "simpleType")
    element->setContentType(ReadSimpleType(reader));
  else if(reader.getElementLocalName() == "complexType")
    element->setContentType(ReadComplexType(reader));

  std::set<std::string> children;
  children.insert("unique");
  children.insert("key");
  children.insert("keyref");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "unique")
    {
      element->addIdentityConstraint(ReadUnique(reader));
      continue;
    }

    if(tag == "key")
    {
      element->addIdentityConstraint(ReadKey(reader));
      continue;
    }

    if(tag == "keyref")
      element->addIdentityConstraint(ReadKeyRef(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return element.release();
}

void te::xsd::serialize::Save(Element* element, te::xml::AbstractWriter& writer)
{
}

te::xsd::Extension* te::xsd::serialize::ReadExtension(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "extension");

  std::auto_ptr<te::xsd::Extension> extension(new te::xsd::Extension);

  // Id
  ReadIdentifiable(extension.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    extension->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,((group|all|choice|sequence)?,
              ((attribute|attributeGroup)*,anyAttribute?))) */

  // Annotation
  ReadAnnotated(extension.get(), reader);

  std::set<std::string> children;
  children.insert("group");
  children.insert("all");
  children.insert("choice");
  children.insert("sequence"); 
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "group")
    {
      extension->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      extension->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      extension->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      extension->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      extension->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      extension->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      extension->setAnyAttribute(ReadAnyAttribute(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return extension.release();
}

void te::xsd::serialize::Save(Extension* extension, te::xml::AbstractWriter& writer)
{
}

te::xsd::Field* te::xsd::serialize::ReadField(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "field");

  std::auto_ptr<te::xsd::Field> field(new te::xsd::Field(0));

  // Id
  ReadIdentifiable(field.get(), reader);

  // XPath
  std::size_t pos = reader.getAttrPosition("xpath");
  assert(pos != std::string::npos);
  field->setXPath(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(field.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return field.release();
}

void te::xsd::serialize::Save(Field* field, te::xml::AbstractWriter& writer)
{
}

te::xsd::Group* te::xsd::serialize::ReadGroup(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "group");

  std::auto_ptr<te::xsd::Group> group(new te::xsd::Group);

  // Id
  ReadIdentifiable(group.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    group->setName(new std::string(reader.getAttr(pos)));

  // Ref
  pos = reader.getAttrPosition("ref");
  if(pos != std::string::npos)
    group->setRef(CreateQName(reader.getAttr(pos)));

  // MinOccurs and MaxOccurs
  ReadOccurs(group.get(), reader);

  reader.next();

  // Grammar: (annotation?,(all|choice|sequence)?)

  // Annotation
  ReadAnnotated(group.get(), reader);

  if(reader.getElementLocalName() == "all")
    group->setContent(ReadAll(reader));
  else if(reader.getElementLocalName() == "choice")
    group->setContent(ReadChoice(reader));
  else if(reader.getElementLocalName() == "sequence")
    group->setContent(ReadSequence(reader));
  else throw; // TODO: Add an exception here...

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return group.release();
}

void te::xsd::serialize::Save(Group* group, te::xml::AbstractWriter& writer)
{
}

te::xsd::Import* te::xsd::serialize::ReadImport(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "import");

  std::auto_ptr<te::xsd::Import> import(new te::xsd::Import(0, 0));

  // Id
  ReadIdentifiable(import.get(), reader);

  // Namespace
  std::size_t pos = reader.getAttrPosition("namespace");
  if(pos != std::string::npos)
    import->setNamespace(new std::string(reader.getAttr(pos)));

  // SchemaLocation
  pos = reader.getAttrPosition("schemaLocation");
  if(pos != std::string::npos)
    import->setSchemaLocation(new std::string(reader.getAttr(pos)));

  reader.next();

  // Annotation
  ReadAnnotated(import.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return import.release();
}

void te::xsd::serialize::Save(Import* import, te::xml::AbstractWriter& writer)
{
}

te::xsd::Include* te::xsd::serialize::ReadInclude(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "include");

  std::auto_ptr<te::xsd::Include> include(new te::xsd::Include(""));

  // Id
  ReadIdentifiable(include.get(), reader);

  // SchemaLocation
  std::size_t pos = reader.getAttrPosition("schemaLocation");
  if(pos != std::string::npos)
    include->setSchemaLocation(reader.getAttr(pos));

  reader.next();

  // Annotation
  ReadAnnotated(include.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return include.release();
}

void te::xsd::serialize::Save(Include* include, te::xml::AbstractWriter& writer)
{
}

te::xsd::Key* te::xsd::serialize::ReadKey(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "key");

  std::auto_ptr<te::xsd::Key> key(new te::xsd::Key(0));

  // Id
  ReadIdentifiable(key.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  assert(pos != std::string::npos);
  key->setName(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(selector,field+))

  // Annotation
  ReadAnnotated(key.get(), reader);

  // Selector
  if(reader.getElementLocalName() == "selector")
  {
    key->setSelector(ReadSelector(reader));
    return key.release();
  }

  // Fields
  while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "field")
    key->addField(ReadField(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return key.release();
}

void te::xsd::serialize::Save(Key* key, te::xml::AbstractWriter& writer)
{
}

te::xsd::KeyRef* te::xsd::serialize::ReadKeyRef(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "keyref");

  std::auto_ptr<te::xsd::KeyRef> keyRef(new te::xsd::KeyRef(0, 0));

  // Id
  ReadIdentifiable(keyRef.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  assert(pos != std::string::npos);
  keyRef->setName(new std::string(reader.getAttr(pos)));

  // Refer
  pos = reader.getAttrPosition("refer");
  assert(pos != std::string::npos);
  keyRef->setRef(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(selector,field+))

  // Annotation
  ReadAnnotated(keyRef.get(), reader);

  // Selector
  if(reader.getElementLocalName() == "selector")
  {
    keyRef->setSelector(ReadSelector(reader));
    return keyRef.release();
  }

  // Fields
  while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "field")
    keyRef->addField(ReadField(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return keyRef.release();
}

void te::xsd::serialize::Save(KeyRef* keyRef, te::xml::AbstractWriter& writer)
{
}

te::xsd::List* te::xsd::serialize::ReadList(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "list");

  std::auto_ptr<te::xsd::List> list(new te::xsd::List);

  // Id
  ReadIdentifiable(list.get(), reader);

  // ItemType
  std::size_t pos = reader.getAttrPosition("itemType");
  if(pos != std::string::npos)
    list->setItemType(CreateQName(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(simpleType?))

  // Annotation
  ReadAnnotated(list.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
     list->setSimpleType(ReadSimpleType(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return list.release();
}

void te::xsd::serialize::Save(List* list, te::xml::AbstractWriter& writer)
{
}

te::xsd::Redefine* te::xsd::serialize::ReadRedefine(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "redefine");

  std::auto_ptr<te::xsd::Redefine> redefine(new te::xsd::Redefine(""));

  // Id
  ReadIdentifiable(redefine.get(), reader);

  // SchemaLocation
  std::size_t pos = reader.getAttrPosition("schemaLocation");
  if(pos != std::string::npos)
    redefine->setSchemaLocation(reader.getAttr(pos));

  reader.next();

  // Grammar: (annotation|(simpleType|complexType|group|attributeGroup))*

  std::set<std::string> children;
  children.insert("annotation");
  children.insert("simpleType");
  children.insert("complexType");
  children.insert("group");
  children.insert("attributeGroup");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "annotation")
    {
      redefine->addAnnotation(ReadAnnotation(reader));
      continue;
    }

    if(tag == "simpleType")
    {
      redefine->addType(ReadSimpleType(reader));
      continue;
    }

    if(tag == "complexType")
    {
      redefine->addType(ReadComplexType(reader));
      continue;
    }

    if(tag == "group")
    {
      redefine->addGroup(ReadGroup(reader));
      continue;
    }

    if(tag == "attributeGroup")
      redefine->addAttributeGroup(ReadAttributeGroup(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return redefine.release();
}

void te::xsd::serialize::Save(Redefine* redefine, te::xml::AbstractWriter& writer)
{
}

te::xsd::Restriction4ComplexContent* te::xsd::serialize::ReadRestriction4ComplexContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "restriction");

  std::auto_ptr<te::xsd::Restriction4ComplexContent> restriction(new te::xsd::Restriction4ComplexContent(0, 0));

  // Id
  ReadIdentifiable(restriction.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    restriction->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,(group|all|choice|sequence)?,
              ((attribute|attributeGroup)*,anyAttribute?)) */

  std::set<std::string> children;
  children.insert("group");
  children.insert("all");
  children.insert("choice");
  children.insert("sequence");
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  // Annotation
  ReadAnnotated(restriction.get(), reader);

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;

    if(tag == "group")
    {
      restriction->setContent(ReadGroup(reader));
      continue;
    }

    if(tag == "all")
    {
      restriction->setContent(ReadAll(reader));
      continue;
    }

    if(tag == "choice")
    {
      restriction->setContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      restriction->setContent(ReadSequence(reader));
      continue;
    }

    if(tag == "attribute")
    {
      restriction->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      restriction->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
      restriction->setAnyAttribute(ReadAnyAttribute(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return restriction.release();
}

void te::xsd::serialize::Save(Restriction4ComplexContent* restriction, te::xml::AbstractWriter& writer)
{
}

te::xsd::Restriction4SimpleContent* te::xsd::serialize::ReadRestriction4SimpleContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "restriction");

  std::auto_ptr<te::xsd::Restriction4SimpleContent> restriction(new te::xsd::Restriction4SimpleContent(0, 0));

  // Id
  ReadIdentifiable(restriction.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    restriction->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,(simpleType?,(minExclusive |minInclusive|
              maxExclusive|maxInclusive|totalDigits|fractionDigits|
              length|minLength|maxLength|enumeration|whiteSpace|pattern)*)?,
              ((attribute|attributeGroup)*,anyAttribute?)) */

  std::set<std::string> children;
  children.insert("minExclusive");
  children.insert("minInclusive");
  children.insert("maxExclusive");
  children.insert("maxInclusive");
  children.insert("totalDigits");
  children.insert("fractionDigits");
  children.insert("length");
  children.insert("minLength");
  children.insert("maxLength");
  children.insert("enumeration");
  children.insert("whiteSpace");
  children.insert("pattern");
  children.insert("attribute");
  children.insert("attributeGroup");
  children.insert("anyAttribute");

  // Annotation
  ReadAnnotated(restriction.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
    restriction->setSimpleType(ReadSimpleType(reader));

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;

    if(tag == "attribute")
    {
      restriction->addAttribute(ReadAttribute(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      restriction->addAttribute(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "anyAttribute")
    {
      restriction->setAnyAttribute(ReadAnyAttribute(reader));
      continue;
    }

    // Facet
    pos = reader.getAttrPosition("value");
    assert(pos != std::string::npos);
    restriction->addFacet(GetFacetType(tag), reader.getAttr(pos));

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Facet

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return restriction.release();
}

void te::xsd::serialize::Save(Restriction4SimpleContent* restriction, te::xml::AbstractWriter& writer)
{
}

te::xsd::Restriction4SimpleType* te::xsd::serialize::ReadRestriction4SimpleType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "restriction");

  std::auto_ptr<te::xsd::Restriction4SimpleType> restriction(new te::xsd::Restriction4SimpleType);

  // Id
  ReadIdentifiable(restriction.get(), reader);

  // Base
  std::size_t pos = reader.getAttrPosition("base");
  if(pos != std::string::npos)
    restriction->setBase(CreateQName(reader.getAttr(pos)));

  reader.next();

  /* Grammar: (annotation?,(simpleType?,(minExclusive|minInclusive|maxExclusive|maxInclusive|
              totalDigits|fractionDigits|length|minLength|maxLength|enumeration|whiteSpace|pattern)*)) */

  std::set<std::string> children;
  children.insert("minExclusive");
  children.insert("minInclusive");
  children.insert("maxExclusive");
  children.insert("maxInclusive");
  children.insert("totalDigits");
  children.insert("fractionDigits");
  children.insert("length");
  children.insert("minLength");
  children.insert("maxLength");
  children.insert("enumeration");
  children.insert("whiteSpace");
  children.insert("pattern");

  // Annotation
  ReadAnnotated(restriction.get(), reader);

  // Simple Type
  if(reader.getElementLocalName() == "simpleType")
     restriction->setSimpleType(ReadSimpleType(reader));

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    // Facet
    pos = reader.getAttrPosition("value");
    assert(pos != std::string::npos);
    restriction->addFacet(GetFacetType(*it), reader.getAttr(pos));

    reader.next();
    assert(reader.getNodeType() == te::xml::END_ELEMENT); // End of Facet

    reader.next();
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return restriction.release();
}

void te::xsd::serialize::Save(Restriction4SimpleType* restriction, te::xml::AbstractWriter& writer)
{
}

te::xsd::Schema* te::xsd::serialize::ReadSchema(const std::string& path)
{
  std::auto_ptr<te::xml::Reader> reader(te::xml::ReaderFactory::make("XERCES"));
  reader->setValidationScheme(false);
  reader->setIgnoreWhiteSpaces(true);
  reader->read(path);

  if(!reader->next())
    throw Exception((boost::format(TE_TR("Could not read the XSD Schema in file: %1%.")) % path).str());

  if(reader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception((boost::format(TE_TR("Error reading the document %1%, the start element wasn't found.")) % path).str());

  return ReadSchema(*reader);
}

te::xsd::Schema* te::xsd::serialize::ReadSchema(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "schema");

  std::auto_ptr<te::xsd::Schema> schema(new te::xsd::Schema(0));

  // Namespaces
  std::size_t numberOfNamespaces = reader.getNumberOfNamespaces();
  for(std::size_t i = 0; i < numberOfNamespaces; ++i)
  {
    std::pair<std::string, std::string> ns;
    reader.getNamespace(i, ns);
    schema->getNamespaces().insert(boost::bimap<std::string, std::string>::value_type(ns.first, ns.second));
  }

  // Id
  ReadIdentifiable(schema.get(), reader);

  // AttributeFormDefault
  std::size_t pos = reader.getAttrPosition("attributeFormDefault");
  if(pos != std::string::npos)
    schema->setAttributeFormDefault(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // ElementFormDefault
  pos = reader.getAttrPosition("elementFormDefault");
  if(pos != std::string::npos)
    schema->setElementFormDefault(reader.getAttr(pos) == "qualified" ? te::xsd::Qualified : te::xsd::Unqualified);

  // TODO: BlockDefault and FinalDefault ?

  // TargetNamespace
  pos = reader.getAttrPosition("targetNamespace");
  if(pos != std::string::npos)
    schema->setTargetNamespace(reader.getAttr(pos));

  // Version
  pos = reader.getAttrPosition("version");
  if(pos != std::string::npos)
    schema->setVersion(reader.getAttr(pos));

  reader.next();

  /* Grammar: ((include|import|redefine|annotation)*,(((simpleType|complexType|
               group|attributeGroup)|element|attribute|notation),annotation*)*) */

  /* TODO: Using a set to find the element's children. Temporary solution!
     Suggestion: we can put this information on a static member of te::xsd classes. - Uba, 2013 */
  std::set<std::string> children;
  children.insert("include");
  children.insert("import");
  children.insert("redefine");
  children.insert("annotation");
  children.insert("simpleType");
  children.insert("complexType"); 
  children.insert("group");
  children.insert("attributeGroup");
  children.insert("element");
  children.insert("attribute");
  children.insert("notation");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "include")
    {
      schema->addInclude(ReadInclude(reader));
      continue;
    }

    if(tag == "import")
    {
      schema->addImport(ReadImport(reader));
      continue;
    }

    if(tag == "redefine")
    {
      schema->addRedefine(ReadRedefine(reader));
      continue;
    }

    if(tag == "annotation")
    {
      schema->addAnnotation(ReadAnnotation(reader));
      continue;
    }

    if(tag == "simpleType")
    {
      schema->addSimpleType(ReadSimpleType(reader));
      continue;
    }

    if(tag == "complexType")
    {
      schema->addComplexType(ReadComplexType(reader));
      continue;
    }

    if(tag == "group")
    {
      schema->addGroup(ReadGroup(reader));
      continue;
    }

    if(tag == "attributeGroup")
    {
      schema->addAttributeGroup(ReadAttributeGroup(reader));
      continue;
    }

    if(tag == "element")
    {
      schema->addElement(ReadElement(reader));
      continue;
    }

    if(tag == "attribute")
      schema->addAttribute(ReadAttribute(reader));
  }

  assert(reader.getNodeType() == te::xml::END_DOCUMENT);

  return schema.release();
}

void te::xsd::serialize::Save(Schema* schema, te::xml::AbstractWriter& writer)
{
}

te::xsd::Selector* te::xsd::serialize::ReadSelector(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "selector");

  std::auto_ptr<te::xsd::Selector> selector(new te::xsd::Selector(0));

  // Id
  ReadIdentifiable(selector.get(), reader);

  // XPath
  std::size_t pos = reader.getAttrPosition("xpath");
  assert(pos != std::string::npos);
  selector->setXPath(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?)

  // Annotation
  ReadAnnotated(selector.get(), reader);

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return selector.release();
}

void te::xsd::serialize::Save(Selector* selector, te::xml::AbstractWriter& writer)
{
}

te::xsd::Sequence* te::xsd::serialize::ReadSequence(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "sequence");

  std::auto_ptr<te::xsd::Sequence> sequence(new te::xsd::Sequence);

  // Id
  ReadIdentifiable(sequence.get(), reader);

  // MinOccurs and MaxOccurs
  ReadOccurs(sequence.get(), reader);

  reader.next();

  // Grammar: (annotation?,(element|group|choice|sequence|any)*)

  // Annotation
  ReadAnnotated(sequence.get(), reader);

  std::set<std::string> children;
  children.insert("element");
  children.insert("group");
  children.insert("choice");
  children.insert("sequence");
  children.insert("any");

  std::set<std::string>::iterator it;
  while(reader.getNodeType() == te::xml::START_ELEMENT &&
       (it = children.find(reader.getElementLocalName())) != children.end())
  {
    std::string tag = *it;
    if(tag == "element")
    {
      sequence->addElement(ReadElement(reader));
      continue;
    }

    if(tag == "group")
    {
      sequence->addContent(ReadGroup(reader));
      continue;
    }

    if(tag == "choice")
    {
      sequence->addContent(ReadChoice(reader));
      continue;
    }

    if(tag == "sequence")
    {
      sequence->addContent(ReadSequence(reader));
      continue;
    }

    if(tag == "any")
      sequence->addAny(ReadAny(reader));
  }

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sequence.release();
}

void te::xsd::serialize::Save(Sequence* sequence, te::xml::AbstractWriter& writer)
{
}

te::xsd::SimpleContent* te::xsd::serialize::ReadSimpleContent(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "simpleContent");

  std::auto_ptr<te::xsd::SimpleContent> sc(new te::xsd::SimpleContent);

  // Id
  ReadIdentifiable(sc.get(), reader);

  reader.next();

  // Grammar: (annotation?,(restriction|extension))

  // Annotation
  ReadAnnotated(sc.get(), reader);

  if(reader.getElementLocalName() == "restriction")
    sc->setTypeDerivation(ReadRestriction4SimpleContent(reader));
  else if(reader.getElementLocalName() == "extension")
    sc->setTypeDerivation(ReadExtension(reader));
  else throw; // TODO: Add an exception here...

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return sc.release();
}

void te::xsd::serialize::Save(SimpleContent* sc, te::xml::AbstractWriter& writer)
{
}

te::xsd::SimpleType* te::xsd::serialize::ReadSimpleType(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "simpleType");

  std::auto_ptr<te::xsd::SimpleType> st(new te::xsd::SimpleType);

  // Id
  ReadIdentifiable(st.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  if(pos != std::string::npos)
    st->setName(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(restriction|list|union))

  // Annotation
  ReadAnnotated(st.get(), reader);
  
  if(reader.getElementLocalName() == "restriction")
    st->setConstructor(ReadRestriction4SimpleType(reader));
  else if(reader.getElementLocalName() == "list")
    st->setConstructor(ReadList(reader));
  else if(reader.getElementLocalName() == "union")
    st->setConstructor(ReadUnion(reader));
  else throw; // TODO: Add an exception here...

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return st.release();
}

void te::xsd::serialize::Save(SimpleType* st, te::xml::AbstractWriter& writer)
{
}

te::xsd::Union* te::xsd::serialize::ReadUnion(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "union");

  std::auto_ptr<te::xsd::Union> u(new te::xsd::Union);

  // Id
  ReadIdentifiable(u.get(), reader);

  // Member Types (List of QNames)
  std::size_t pos = reader.getAttrPosition("memberTypes");
  if(pos != std::string::npos)
  {
    std::string value = reader.getAttr(pos);
    std::vector<std::string> tokens;
    te::common::Tokenize(value, tokens, " ");
    for(std::size_t i = 0; i < tokens.size(); ++i)
      u->addMemberType(CreateQName(tokens[i]));
  }

  reader.next();

  // Grammar: (annotation?, (simpleType*))

  // Annotation
  ReadAnnotated(u.get(), reader);

  // SimpleTypes
  while(reader.getNodeType() == te::xml::START_ELEMENT && (reader.getElementLocalName() == "simpleType"))
    u->addSimpleType(ReadSimpleType(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return u.release();
}

void te::xsd::serialize::Save(Union* u, te::xml::AbstractWriter& writer)
{
}

te::xsd::Unique* te::xsd::serialize::ReadUnique(te::xml::Reader& reader)
{
  assert(reader.getNodeType() == te::xml::START_ELEMENT);
  assert(reader.getElementLocalName() == "unique");

  std::auto_ptr<te::xsd::Unique> unique(new te::xsd::Unique(0));

  // Id
  ReadIdentifiable(unique.get(), reader);

  // Name
  std::size_t pos = reader.getAttrPosition("name");
  assert(pos != std::string::npos);
  unique->setName(new std::string(reader.getAttr(pos)));

  reader.next();

  // Grammar: (annotation?,(selector,field+))

  // Annotation
  ReadAnnotated(unique.get(), reader);

  // Selector
  if(reader.getElementLocalName() == "selector")
  {
    unique->setSelector(ReadSelector(reader));
    return unique.release();
  }

  // Fields
  while(reader.getNodeType() == te::xml::START_ELEMENT && reader.getElementLocalName() == "field")
    unique->addField(ReadField(reader));

  assert(reader.getNodeType() == te::xml::END_ELEMENT);
  reader.next();

  return unique.release();
}

void te::xsd::serialize::Save(Unique* unique, te::xml::AbstractWriter& writer)
{
}

void te::xsd::serialize::ReadIdentifiable(Identifiable* identifiable, te::xml::Reader& reader)
{
  assert(identifiable);

  std::size_t pos = reader.getAttrPosition("id");
  if(pos != std::string::npos)
    identifiable->setId(new std::string(reader.getAttr(pos)));
}

void te::xsd::serialize::ReadAnnotated(Annotated* annotated, te::xml::Reader& reader)
{
  assert(annotated);

  if((reader.getNodeType() != te::xml::START_ELEMENT) ||
     (reader.getElementLocalName() != "annotation"))
    return;

  te::xsd::Annotation* annotation = ReadAnnotation(reader);
  annotated->setAnnotation(annotation);
}

void te::xsd::serialize::ReadOccurs(Occurs* occurs, te::xml::Reader& reader)
{
  assert(occurs);

  // MinOccurs
  std::size_t pos = reader.getAttrPosition("minOccurs");
  if(pos != std::string::npos)
    occurs->setMinOccurs(static_cast<unsigned int>(reader.getAttrAsInt32(pos)));

  // MaxOccurs
  pos = reader.getAttrPosition("maxOccurs");
  if(pos != std::string::npos)
    reader.getAttr(pos) == "unbounded" ? occurs->setMaxOccurs(te::xsd::Occurs::unbounded) : occurs->setMaxOccurs(reader.getAttrAsInt32(pos));
}

te::xsd::QName* te::xsd::serialize::CreateQName(const std::string& name)
{
  std::vector<std::string> tokens;
  te::common::Tokenize(name, tokens, ":");
  assert(tokens.size() >= 2);
  
  return new te::xsd::QName(tokens[0], tokens[1]);
}

te::xsd::FacetType te::xsd::serialize::GetFacetType(const std::string& name)
{
  if(name == "minExclusive")
    return te::xsd::MinExclusive;
  if(name == "minInclusive")
    return te::xsd::MinInclusive;
  if(name == "maxExclusive")
    return te::xsd::MaxExclusive;
  if(name == "maxInclusive")
    return te::xsd::MaxInclusive;
  if(name == "totalDigits")
    return te::xsd::TotalDigits;
  if(name == "fractionDigits")
    return te::xsd::FractionDigits;
  if(name == "length")
    return te::xsd::Length;
  if(name == "minLength")
    return te::xsd::MinLength;
  if(name == "maxLength")
    return te::xsd::MaxLength;
  if(name == "enumeration")
    return te::xsd::Enumeration;
  if(name == "whiteSpace")
    return te::xsd::WhiteSpace;

  return te::xsd::Pattern;
}

//#endif
