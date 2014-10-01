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
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
class QTreeWidgetItem;
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
      class SymbolPreviewWidget;

      /*!
        \class SymbolSelectorDialog

        \brief A dialog used to select a specific symbol.
      */
      class TEQTWIDGETSEXPORT SymbolSelectorDialog : public QDialog
      {
        Q_OBJECT

        private:

          /*!
            \enum NodeType

            \brief Auxiliary internal enumeration to control the tree nodes.
          */
          enum NodeType
          {
            LIBRARY = 0, /*!< Library root node. */
            SYMBOL  = 1  /*!< Symbol node.       */
          };

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

        private slots:

          void onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

          void onShowSymbolInfoPushButtonPressed();
          
          void onLoadSymbolLibraryPushButtonPressed();

          void onSearchLineEditTextChanged(const QString& text);

        private:

          /*! \brief Initialize the dialog. */
          void initialize();

          /*! \brief  Auxiliary internal method to filter the symbols. */
          void filter(const QList<QTreeWidgetItem*>& items);

          /*! \brief Auxiliary internal method that returns the selected symbol. */
          Symbol* getSelectedSymbol() const;

          /*! \brief Auxiliary internal method that returns the symbol given a list widget item. */
          Symbol* getSymbolFromItem(QTreeWidgetItem* item) const;

          /*! \brief Auxiliary internal method to format a symbol info to be used on tool tips. */
          QString formatSymbolInfo(const SymbolInfo& info) const;

        private:

          std::auto_ptr<Ui::SymbolSelectorDialogForm> m_ui; //!< Dialog form.
          SymbolPreviewWidget* m_preview;                   //!< Preview Widget used to visualize the symbol.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLSELECTORDIALOG_H
