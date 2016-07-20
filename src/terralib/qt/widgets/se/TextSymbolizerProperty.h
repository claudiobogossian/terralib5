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
  \file terralib/qt/widgets/se/TextSymbolizerProperty.h

  \brief A widget used to define the text symbolizer
         properties, such as:
           - basic fill
           - font
           - halo
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTSYMBOLIZERPROPERTY_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTSYMBOLIZERPROPERTY_H

// TerraLib
#include "../Config.h"

// Qt
#include <QWidget>


namespace te
{
// Forward declarations
  namespace se
  {
    class TextSymbolizer;
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      class HaloPropertyItem;
      class LinePlacementPropertyItem;
      class PointPlacementPropertyItem;
      class TextPropertyItem;

      /*!
        \class TextSymbolizerProperty

        \brief A widget used to define the text symbolizer
        properties, such as:
        - basic fill
        - font
        - halo

      */

      class TEQTWIDGETSEXPORT TextSymbolizerProperty : public QWidget
      {
        Q_OBJECT

        public:
          /** @name Initializer Methods
            *  Methods related to instantiation and destruction.
            */
          //@{

          /*! \brief Constructor */
          TextSymbolizerProperty(QWidget* parent);

          /*! \brief Destructor. */
          ~TextSymbolizerProperty();

          //@}

        public:

          /*!
            \brief Sets a text symbolizer element to this widget.

            \param symb A valid text symbolizer element.

            \note The widget will NOT take the ownership of the given symbolizer.
            \note The widget form will be update based on given symbolizer parameters.
          */
          void setSymbolizer(te::se::TextSymbolizer* symb);

          /*!
            \brief Gets the configured polygon symbolizer element.

            \return The configured polygon symbolizer element.
            
            \note The caller will take the ownership of the returned symbolizer.
          */
          te::se::Symbolizer* getSymbolizer() const;


          /*!
          \brief Used to set all properties names to use as label
          */
          void setLabels(std::vector<std::string> names);

        protected slots:

          void onTextChanged();

          void onLinePlacementChanged();

          void onPointPlacementChanged();

          void onHaloChanged();

        signals:

          /*! This signal is emitted when the internal polygon symbolizer element is changed. */
          void symbolizerChanged();

        protected:
          te::qt::widgets::TextPropertyItem* m_text;
          te::qt::widgets::LinePlacementPropertyItem* m_lp;
          te::qt::widgets::PointPlacementPropertyItem* m_pp;
          te::qt::widgets::HaloPropertyItem* m_halo;

          bool m_setLocalSymbol;

          te::se::TextSymbolizer* m_symb;                   //!< Text symbolizer element that will be configured by this widget.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_SE_INTERNAL_TEXTSYMBOLIZERPROPERTY_H