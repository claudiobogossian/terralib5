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
  \file terralib/xsd/XSDSerializer.h

  \brief Data serialization for the XSD module.
*/

#ifndef __TERRALIB_XSD_INTERNAL_XSDSERIALIZER_H
#define __TERRALIB_XSD_INTERNAL_XSDSERIALIZER_H

// TerraLib
#include "../../Config.h"
#include "../../Enums.h"

//STL
#include <string>

//#ifdef TE_ENABLED_XML

namespace te
{
  namespace xml
  {
    class Reader;
    class Writer;
  }

  namespace xsd
  {
    class All;
    class Annotation;
    class Any;
    class AnyAttribute;
    class Attribute;
    class AttributeGroup;
    class Choice;
    class ComplexContent;
    class ComplexType;
    class Element;
    class Extension;
    class Field;
    class Group;
    class Import;
    class Include;
    class Key;
    class KeyRef;
    class List;
    class Redefine;
    class Restriction4ComplexContent;
    class Restriction4SimpleContent;
    class Restriction4SimpleType;
    class Schema;
    class Selector;
    class Sequence;
    class SimpleContent;
    class SimpleType;
    class Union;
    class Unique;
    class Identifiable;
    class Annotated;
    class Occurs;
    class QName;

    namespace serialize
    {
      TEXSDEXPORT All* ReadAll(te::xml::Reader& reader);

      TEXSDEXPORT void Save(All* all, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Annotation* ReadAnnotation(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Annotation* annotation, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Any* ReadAny(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Any* any, te::xml::AbstractWriter& writer);

      TEXSDEXPORT AnyAttribute* ReadAnyAttribute(te::xml::Reader& reader);

      TEXSDEXPORT void Save(AnyAttribute* anyAttribute, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Attribute* ReadAttribute(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Attribute* attribute, te::xml::AbstractWriter& writer);

      TEXSDEXPORT AttributeGroup* ReadAttributeGroup(te::xml::Reader& reader);

      TEXSDEXPORT void Save(AttributeGroup* attributeGroup, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Choice* ReadChoice(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Choice* choice, te::xml::AbstractWriter& writer);

      TEXSDEXPORT ComplexContent* ReadComplexContent(te::xml::Reader& reader);

      TEXSDEXPORT void Save(ComplexContent* cc, te::xml::AbstractWriter& writer);

      TEXSDEXPORT ComplexType* ReadComplexType(te::xml::Reader& reader);

      TEXSDEXPORT void Save(ComplexType* ct, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Element* ReadElement(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Element* element, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Extension* ReadExtension(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Extension* extension, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Field* ReadField(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Field* field, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Group* ReadGroup(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Group* group, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Import* ReadImport(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Import* import, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Include* ReadInclude(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Include* include, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Key* ReadKey(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Key* key, te::xml::AbstractWriter& writer);

      TEXSDEXPORT KeyRef* ReadKeyRef(te::xml::Reader& reader);

      TEXSDEXPORT void Save(KeyRef* keyRef, te::xml::AbstractWriter& writer);

      TEXSDEXPORT List* ReadList(te::xml::Reader& reader);

      TEXSDEXPORT void Save(List* list, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Redefine* ReadRedefine(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Redefine* redefine, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Restriction4ComplexContent* ReadRestriction4ComplexContent(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Restriction4ComplexContent* restriction, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Restriction4SimpleContent* ReadRestriction4SimpleContent(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Restriction4SimpleContent* restriction, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Restriction4SimpleType* ReadRestriction4SimpleType(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Restriction4SimpleType* restriction, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Schema* ReadSchema(const std::string& path);

      TEXSDEXPORT Schema* ReadSchema(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Schema* schema, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Selector* ReadSelector(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Selector* selector, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Sequence* ReadSequence(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Sequence* sequence, te::xml::AbstractWriter& writer);

      TEXSDEXPORT SimpleContent* ReadSimpleContent(te::xml::Reader& reader);

      TEXSDEXPORT void Save(SimpleContent* sc, te::xml::AbstractWriter& writer);

      TEXSDEXPORT SimpleType* ReadSimpleType(te::xml::Reader& reader);

      TEXSDEXPORT void Save(SimpleType* sc, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Union* ReadUnion(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Union* u, te::xml::AbstractWriter& writer);

      TEXSDEXPORT Unique* ReadUnique(te::xml::Reader& reader);

      TEXSDEXPORT void Save(Unique* unique, te::xml::AbstractWriter& writer);

      void ReadIdentifiable(Identifiable* identifiable, te::xml::Reader& reader);

      void ReadAnnotated(Annotated* annotated, te::xml::Reader& reader);

      void ReadOccurs(Occurs* occurs, te::xml::Reader& reader);

      QName* CreateQName(const std::string& name);

      FacetType GetFacetType(const std::string& name);

    }  // end namespace serialize
  }    // end namespace xsd
}      // end namespace te

//#endif // TE_ENABLED_XML

#endif  // __TERRALIB_XSD_INTERNAL_XSDSERIALIZER_H
