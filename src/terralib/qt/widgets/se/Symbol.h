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
  \file terralib/qt/widgets/se/Symbol.h

  \brief This class represents a symbol.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOL_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOL_H

// TerraLib
#include "../Config.h"
#include "SymbolInfo.h"

// STL
#include <string>
#include <vector>

namespace te
{
// Forward declarations
  namespace se
  {
    class Symbolizer;
  }

  namespace qt
  {
    namespace widgets
    {
      /*!
        \class Symbol

        \brief This class represents a symbol. TODO: More description!
      */
      class TEQTWIDGETSEXPORT Symbol
      {
        public:

        /** @name Initializer Methods
          *  Methods related to instantiation and destruction.
          */
        //@{

        /*! \brief Default constructor. */
        Symbol();

        /*! \brief Copy constructor. */
        Symbol(const Symbol& rhs);

        /*! \brief Destructor. */
        ~Symbol();

        //@}

        public:

        /*!
          \brief It return the information associated to the symbol.

          \return The information associated to the symbol.
        */
        const SymbolInfo& getInfo() const;

        /*!
          \brief It sets the information associated to the symbol.

          \param info The information that will be associated to this symbol.
        */
        void setInfo(const SymbolInfo& info);

        /*!
          \brief It returns the number of Symbolizers that compose of the symbol.

          \return The number of Symbolizers that compose of the symbol.
        */
        std::size_t getSymbolizersCount() const;

        /*!
          \brief It returns the n-th Symbolizer.

          \param i The Symbolizer index.

          \return It returns a pointer to the specified Symbolizer.
            
          \note The caller of this method will NOT take the ownership of the symbolizer.
        */
        te::se::Symbolizer* getSymbolizer(const std::size_t& i) const;

        /*!
          \brief It returns the n-th Symbolizer.

          \param i The Symbolizer index.

          \return It returns a pointer to the specified Symbolizer.
            
          \note The caller of this method will NOT take the ownership of the symbolizer.
        */
        te::se::Symbolizer* operator[](const std::size_t& i) const;

        /*!
          \brief It returns the list of Symbolizers that compose the symbol.

          \return The list of Symbolizers that compose the symbol.
            
          \note The caller of this method will NOT take the ownership of the symbolizers.
        */
        const std::vector<te::se::Symbolizer*>& getSymbolizers() const;

        /*!
          \brief It adds the given Symbolizer to the list of Symbolizers of the symbol.

          \param symb The Symbolizer that will be added.

          \note The symbol will take the ownership of the given Symbolizer.
        */
        void addSymbolizer(te::se::Symbolizer* symb);

        /*!
          \brief It sets the given Symbolizer at the given position.

          \param i    The Symbolizer position.
          \param symb The Symbolizer that will be added.

          \note The symbol will take the ownership of the given Symbolizer.
        */
        void setSymbolizer(const std::size_t& i, te::se::Symbolizer* symb);

        /*!
          \brief It removes the specified Symbolizer from the list of Symbolizers of the symbol.

          \param i The Symbolizer index.
        */
        void removeSymbolizer(const std::size_t& i);

        /*!
          \brief It swaps the position of the Symbolizers.

          \param first The first Symbolizer index.
          \param second The second Symbolizer index.
        */
        void swapSymbolizers(const std::size_t& first, const std::size_t& second);

        /*! \brief It creates a new copy of this object. */
        Symbol* clone() const;

        private:

          SymbolInfo m_info;                        //!< Information about the symbol.
          std::vector<te::se::Symbolizer*> m_symbs; //!< Set of symbolizers that compose the symbol.
      };

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOL_H
