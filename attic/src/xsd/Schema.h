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
  \file terralib/xsd/Schema.h

  \brief A class that models an XML Schema (XSD).
*/

#ifndef __TERRALIB_XSD_INTERNAL_SCHEMA_H
#define __TERRALIB_XSD_INTERNAL_SCHEMA_H

// TerraLib
#include "Annotation.h"
#include "Attribute.h"
#include "AttributeGroup.h"
#include "ComplexType.h"
#include "Element.h"
#include "Enums.h"
#include "Group.h"
#include "Identifiable.h"
#include "Import.h"
#include "Include.h"
#include "Notation.h"
#include "Redefine.h"
#include "Schema.h"
#include "SimpleType.h"

// Boost
#include <boost/bimap.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace te
{
  namespace xsd
  {
    /*!
      \class Schema

      \brief A class that models a XML schema (XSD).

      \sa Identifiable, Include, Import, Redefine, Annotation,
          SimpleType, ComplexType, Group, AttributeGroup,
          Element, Attribute, Notation
    */
    class TEXSDEXPORT Schema : public Identifiable, public boost::noncopyable
    {
      public:

        Schema(std::string* id);

        ~Schema();

        Form getAttributeFormDefault() const { return m_attributeFormDefault; }

        void setAttributeFormDefault(Form f) { m_attributeFormDefault = f; }

        Form getElementFormDefault() const { return m_elementFormDefault; }

        void setElementFormDefault(Form f) { m_elementFormDefault = f; }

        int getBlockDefault() const { return m_blockDefault; }

        void setBlockDefault(int bdf) { m_blockDefault = bdf; }

        int getFinalDefault() const { return m_finalDefault; }

        void setFinalDefault(int fdf) { m_finalDefault = fdf; }

        const std::string& getTargetNamespace() const { return m_targetNamespace; }

        void setTargetNamespace(const std::string& tns) { m_targetNamespace = tns; }

        const std::string& getVersion() const { return m_version; }

        void setVersion(const std::string& v) { m_version = v; }

        const boost::bimap<std::string, std::string>& getNamespaces() const { return m_xmlns; }

        boost::bimap<std::string, std::string>& getNamespaces() { return m_xmlns; }

        const boost::ptr_vector<Include>& getIncludes() const;

        const boost::ptr_vector<Import>& getImports() const;

        const boost::ptr_vector<Redefine>& getRedefines() const;

        const boost::ptr_vector<Annotation>& getAnnotations() const;

        const boost::ptr_vector<SimpleType>& getSimpleTypes() const;

        const boost::ptr_vector<ComplexType>& getComplexTypes() const;

        const boost::ptr_vector<Group>& getGroups() const;

        const boost::ptr_vector<AttributeGroup>& getAttributeGroups() const;

        const boost::ptr_vector<AbstractAttribute>& getAttributes() const;

        const boost::ptr_vector<Element>& getElements() const;

        const boost::ptr_vector<Notation>& getNotations() const;

        void addNamespace(const std::string& ns, const std::string& nsprefix);

        void addInclude(Include* i);

        void addImport(Import* i);

        void addRedefine(Redefine* r);

        void addAnnotation(Annotation* a);

        void addSimpleType(SimpleType* s);

        void addComplexType(ComplexType* c);

        void addGroup(Group* g);

        void addAttributeGroup(AttributeGroup* ag);

        void addAttribute(AbstractAttribute* a);

        void addElement(Element* e);

        void addNotation(Notation* n);

      private:

        Form m_attributeFormDefault;  //!< Tells if attributes declared in this schema must use the namespace prefix or not. Default: Unqualified. (Optional)
        Form m_elementFormDefault;    //!< Tells if elements declared in this schema must use the namespace prefix or not. Default: Unqualified. (Optional)

        int m_blockDefault; // (Optional)
        int m_finalDefault; // (Optional)

        std::string m_targetNamespace;        //!< The URI namespace of the schema. (Mandatory)
        std::string m_version;                //!< The schema version. (Optional)

        boost::bimap<std::string, std::string> m_xmlns;    //!< A list of one or more namespaces for use in this schema. Mandatory only if attributeFormDefault or elementFormDefault is set to Qualified.

        boost::ptr_vector<Include> m_includeVec;                 //!< The list of include declarations at schema root.
        boost::ptr_vector<Import> m_importVec;                   //!< The list of import declarations at schema root.
        boost::ptr_vector<Redefine> m_redefineVec;               //!< The list of redefine declarations at schema root.
        boost::ptr_vector<Annotation> m_annotationVec;           //!< The list of annotation declarations at schema root.
        boost::ptr_vector<SimpleType> m_simpleTypeVec;           //!< The list of simple type declarations at schema root.
        boost::ptr_vector<ComplexType> m_complexTypeVec;         //!< The list of complex type declarations at schema root.
        boost::ptr_vector<Group> m_groupVec;                     //!< The list of group declarations at schema root.
        boost::ptr_vector<AttributeGroup> m_attributeGroupVec;   //!< The list of attributes at schema root.
        boost::ptr_vector<Element> m_elementVec;                 //!< The list of elements declarations at schema root.
        boost::ptr_vector<AbstractAttribute> m_attributeVec;     //!< The list of attributes at schema root.
        boost::ptr_vector<Notation> m_notationVec;               //!< The list of notation declarations at schema root.
    };

  } // end namespace xsd
}   // end namespace te

#endif  // __TERRALIB_XSD_INTERNAL_SCHEMA_H

