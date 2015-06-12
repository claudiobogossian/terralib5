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
  \file terralib/qt/widgets/se/SymbolInfoDialog.h

  \brief A dialog used to configure informations about a symbol.
*/

#ifndef __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLINFODIALOG_H
#define __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLINFODIALOG_H

// TerraLib
#include "../Config.h"
#include "SymbolInfo.h"

// Qt
#include <QDialog>

// STL
#include <memory>

// Forward declaraion
namespace Ui { class SymbolInfoDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class SymbolInfoDialog

        \brief A dialog used to configure informations about a symbol.
      */
      class TEQTWIDGETSEXPORT SymbolInfoDialog : public QDialog
      {
        public:

          /** @name Initializer Methods
           *  Methods related to instantiation and destruction.
           */
          //@{

          /*! \brief Constructs a symbol info dialog which is a child of parent, with widget flags set to f. */
          SymbolInfoDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

          /*! \brief Destructor. */
          ~SymbolInfoDialog();

          //@}

        public:

          /*!
            \brief Gets the configured symbol information.

            \return The configured symbol information.
          */
          SymbolInfo getSymbolInfo() const;

          /*!
            \brief Sets the symbol information to this dialog.

            \param info The symbol information.

            \note The dialog form will be update based on given information parameters.
          */
          void setSymbolInfo(const SymbolInfo& info);

          void setReadMode(bool enable = true);

        private:

          /*! \brief Updates the dialog form based on internal symbol info object. */
          void updateUi();

        private:

          std::auto_ptr<Ui::SymbolInfoDialogForm> m_ui; //!< Dialog form.
          SymbolInfo m_symbolInfo;                      //!< Symbol information that will be configured by the dialog.
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_SE_INTERNAL_SYMBOLINFODIALOG_H
