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
  \file terralib/qt/widgets/se/RuleProperty.h

  \brief A widget used to define the rule properties, such as:
           - scale
           - description
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_RULEPROPERTY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_RULEPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>


namespace te
{
// Forward declarations
  namespace se
  {
    class Rule;
  }

  namespace qt
  {
    namespace widgets
    {
      class ScalePropertyItem;

      /*!
        \class RuleProperty

        \brief A widget used to define the rule properties, such as:
           - scale
           - description

      */

      class TEQTWIDGETSEXPORT RuleProperty : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          RuleProperty(QWidget* parent);

          /*! \brief Destructor. */
          ~RuleProperty();

          //@}

        public:

          /*!
            \brief Sets a polygon symbolizer element to this widget.

            \param symb A valid polygon symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setRule(te::se::Rule* rule);

          /*!
            \brief Gets the configured polygon symbolizer element.

            \return The configured polygon symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Rule* getRule() const;

        protected slots:

          void onScaleMinChanged();
          void onScaleMaxChanged();

        signals:

          /*! This signal is emitted when the internalrule element is changed. */
          void ruleChanged();


        protected:
          te::qt::widgets::ScalePropertyItem* m_sp;

          te::se::Rule* m_rule;                   //!< Rule element that will be configured by this widget.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_RULEPROPERTY_H
