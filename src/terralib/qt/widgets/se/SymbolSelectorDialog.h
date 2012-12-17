/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/se/SymbolSelectorDialog.h

  \brief A dialog used to select a specific symbol.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLSELECTORDIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLSELECTORDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QtGui/QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class SymbolSelectorDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
// Forward declarations
      class Symbol;
      struct SymbolInfo;

      /*!
        \class SymbolSelectorDialog

        \brief A dialog used to select a specific symbol.
      */
      class TEQTWIDGETSEXPORT SymbolSelectorDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a symbol selector dialog which is a child of parent, with widget flags set to f. */
          SymbolSelectorDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~SymbolSelectorDialog();

          //@}

          /*!
            \brief Pops up a modal symbol selector dialog with the given window title, lets the user select a symbol, 
                   and returns that symbol.

            \param parent Dialog parent.
            \param title  Dialog title.

            \note The caller will take the ownership of the returned symbol.
            \note It returns a NULL pointr if the user cancels the dialog.
            \note It return a NULL pointer if there is not a selected symbol.
          */
          static Symbol* getSymbol(QWidget* parent, const QString& title = "");

          /*!
            \brief Gets the selected symbol.

            \return The selected fill element.
            
            \note The caller will take the ownership of the returned symbol.
            \note It return a NULL pointer if there is not a selected symbol.
          */
          Symbol* getSymbol() const;

        private:

          /*! \brief Initialize the dialog. */
          void initialize();

          /*! \brief Auxiliary internal method to format a symbol info to be used on tool tips. */
          QString formatSymbolInfo(const SymbolInfo& info) const;

        private:

          std::auto_ptr<Ui::SymbolSelectorDialogForm> m_ui; //!< Dialog form.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLSELECTORDIALOG_H
