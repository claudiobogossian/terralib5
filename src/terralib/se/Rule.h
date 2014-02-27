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
  \file terralib/se/Rule.h

  \brief A Rule is used to attach property/scale conditions to and group the individual symbols used for rendering.
*/

#ifndef __TERRALIB_SE_INTERNAL_RULE_H
#define __TERRALIB_SE_INTERNAL_RULE_H

// TerraLib
#include "Config.h"

// STL 
#include <string>
#include <vector>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
// Forward declaration
  namespace fe { class Filter; }

  namespace se
  {
// Forward declaration
    class Description;
    class Graphic;
    class Symbolizer;

    /*!
      \class Rule
      
      \brief A Rule is used to attach property/scale conditions to and group the individual symbols used for rendering.

      Rules are used to group rendering instructions by
      feature-property conditions and map scales. Rule
      definitions are placed immediately inside of
      featuretype- or coverage-style definitions.
      If a rule has no Filter element, the
      interpretation is that the rule condition is
      always true, i.e., all features are accepted
      and styled by the rule.
      The ElseFilter has a more complicated interpretation
      than the Filter element, and is interpreted as follows.
      The nominal scale of the map to be portrayed is
      computed (as described in the previous subclause) and
      all rules for scale ranges that do not include the computed
      nominal scale are discarded from further processing. Then, the
      specific condition for the ElseFilter is computed by "or-ing"
      together all of the other filter conditions and take the
      global "not" of that condition.

      \ingroup se

      \sa FeatureTypeStyle, CoverageStyle, Description, Graphic, Symbolizer, te::fe::Filter
    */
    class TESEEXPORT Rule : public boost::noncopyable
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief It initializes a new Rule. */
        Rule();

        /*! \brief Destructor. */
        ~Rule();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        void setName(std::string* name);

        const std::string* getName() const;

        void setDescription(Description* d);

        const Description* getDescription() const;

        void setLegendGraphic(Graphic* legendGraphic);

        const Graphic* getLegendGraphic() const;

        void removeLegendGraphic();

        /*!
          \brief

          \note The ElseFilter must be false in order to set a filter.
        */
        void setFilter(te::fe::Filter* f);

        const te::fe::Filter* getFilter() const;

        /*!
          \brief

          \note The filter must be NULL in order to call this method.
        */
        void enableElseFilter();

        void disableElseFilter();

        bool hasElseFilter() const;

        void setMinScaleDenominator(const double& minScaleDenominator);

        const double& getMinScaleDenominator() const;

        void setMaxScaleDenominator(const double& maxScaleDenominator);

        const double& getMaxScaleDenominator() const;

        void push_back(Symbolizer* s);

        void setSymbolizer(std::size_t i, Symbolizer* s);

        void setSymbolizers(const std::vector<Symbolizer*>& symbs);

        const std::vector<Symbolizer*>& getSymbolizers() const;

        const Symbolizer* getSymbolizer(std::size_t i) const;

        void removeSymbolizer(std::size_t i);

        void clearSymbolizers();

        //@}

      private:

        std::string* m_name;                     //!< It allows the rule to be referenced externally, which is needed in some methods of SE usage. (Optional)
        Description* m_description;              //!< It gives the familiar short title for display lists and longer description for the rule. (Optional)
        Graphic* m_legendGraphic;                //!< It allows an optional explicit Graphic Symbolizer to be displayed in a legend for this rule. (Optional)
        te::fe::Filter* m_filter;                //!< It allows the selection of features in rules to be controlled by attribute conditions. (Optional)
        bool m_elseFilter;                       //!< It allows rules to be specified that are activated for features that are not selected by any other rule in a feature-type style. Default: false. If true, this is a Else filter rule. (Optional)
        double m_minScaleDenominator;            //!< It defines the range of map-rendering scales for which the rule should be applied. Default: 0.0. (Optional)
        double m_maxScaleDenominator;            //!< It defines the range of map-rendering scales for which the rule should be applied. Default: TE_DOUBLE_INFINITY. (Optional)
        std::vector<Symbolizer*> m_symbolizers;  //!< A Symbolizer describes how a feature/coverage is to appear on a map. (Mandatory)
    };

  } // end namespace se
}   // end namespace te

#endif  // __TERRALIB_SE_INTERNAL_RULE_H
