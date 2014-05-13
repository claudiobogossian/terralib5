/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/se/Style.h
  
  \brief The Style defines the styling that is to be applied to a geographic dataset (vector geometries or coverage).
*/

#ifndef __TERRALIB_SE_INTERNAL_STYLE_H
#define __TERRALIB_SE_INTERNAL_STYLE_H

// TerraLib
#include "../common/BaseVisitable.h"
#include "Config.h"
#include "Visitor.h"

// STL
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declarations
  namespace xl { class SimpleLink; }

  namespace se
  {
// Forward declarations
    class Description;
    class Rule;

    /*!
      \class Style
      
      \brief The Style defines the styling that is to be applied to a geographic dataset (vector geometries or coverage).

      The ordering to use for the Rules inside of a Style
      is following the "painters model" with the first item in a list
      being the first item plotted and hence being on the "bottom".

      \ingroup se

      \sa FeatureTypeStyle, CoverageStyle, Description, Rule
    */
    class TESEEXPORT Style : public te::common::BaseVisitable<Visitor>, public boost::noncopyable
    {
      public:

        TE_DEFINE_VISITABLE

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Style. */
        Style();

        /*! \brief Virtual destructor. */
        virtual ~Style();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setName(std::string* name);

        const std::string* getName() const;

        void setDescription(Description* d);

        const Description* getDescription() const;

        void push_back(const std::string& semanticTypeIdentifier);

        const std::string& getSemanticTypeIdentifier(std::size_t i);

        const std::vector<std::string>* getSemanticTypeIdentifiers() const;

        void push_back(Rule* rule);

        const std::vector<Rule*>& getRules() const;

        Rule* getRule(std::size_t i) const;

        void push_back(te::xl::SimpleLink* onlineResource);

        const std::vector<te::xl::SimpleLink*>& getOnlineResources() const;

        const te::xl::SimpleLink* getOnlineResource(std::size_t i) const;

        void setVersion(const std::string& v);

        const std::string& getVersion() const;

        /*!
          \brief It returns the style type.

          \return The style type.
        */
        virtual const std::string& getType() const = 0;

        //@}

        /*! \brief It creates a new copy of this object. */
        virtual Style* clone() const = 0;

      protected:

        std::string* m_name;                                  //!< It allows the style to be referenced. Names must be unique in the context in which they are defined. (Optional)
        Description* m_description;                           //!< It gives an informative description of the style. (Optional)
        std::vector<std::string>* m_semanticTypeIdentifiers;  //!< The SemanticTypeIdentifier is experimental and is intended to be used to identify what the feature style is suitable to be used for using community-controlled name(s). (Optional)
        std::vector<Rule*> m_rules;                           //!< It allows conditional rendering. (Mandatory if m_onlineResource is empty, otherwise, it is optional)
        std::vector<te::xl::SimpleLink*> m_onlineResources;   //!< It refers to an external document. (Mandatory if m_rules is empty, otherwise, it is optional)
        std::string m_version;                                //!< The version is an optional attribute on the FeatureTypeStyle that identifies the SE version number that the FeatureTypeStyle corresponds to. (Optional)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_STYLE_H
