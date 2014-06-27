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
 \file terralib/srs/qt/SRSManagerDialog.h
 
 \brief A dialog used to manage Spatial Reference Systems for coordinates.
 */

#ifndef __TERRALIB_SRS_INTERNAL_QT_SRSMANAGERDIALOG_H
#define __TERRALIB_SRS_INTERNAL_QT_SRSMANAGERDIALOG_H

// TerraLib
#include "../Config.h"

// Qt
#include <QDialog>
#include <QStringList>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

//STL
#include <string>

// Forward declaraion
namespace Ui { class SRSManagerDialogForm; }

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      class SRSDialog;
      
      /*!
       \class SRSManagerDialog
       
       \brief A dialog used to build a SRSManagerDialog element.

       \ingroup widgets
      */
      class TEQTWIDGETSEXPORT SRSManagerDialog : public QDialog
      {
        Q_OBJECT
        
      public:
        
        /*! 
         \brief Constructs a basic dialog which is a child of parent, with widget flags set to f.
         \param parent Pointer to the dialog parent.
         \param f The dialog flags.
         */
        SRSManagerDialog( QWidget* parent = 0, Qt::WindowFlags f = 0);
        
        /*! 
         \brief Destructor. 
         */
        ~SRSManagerDialog();
        
        /*!
         \brief Returns the selected SRS in the window. 
         \return a const pair <srid,auth_name> reference, with an SRS identification given by an authority. If the user doesn't
         select a SRS the pair will have a TE_UNKNOW_SRS and an empty auth_name.
         */
        const std::pair<int, std::string>& getSelectedSRS() const;
        
        /*!
         \brief Pops up a modal SRS selector dialog with the given window title, lets the user select a SRS, 
         and returns that SRS.
         \param parent Dialog parent.
         \param title  Dialog title.
         \return a pair <srid,auth_name>, with an SRS identification given by an authority. If the user doesn't
         select a SRS the pair will have a TE_UNKNOW_SRS and an empty auth_name.
         */
        static std::pair<int, std::string> getSRS(QWidget* parent, const QString& title = "");
        
        
        protected slots:
        
        void onSRSTreeWidgetItemClicked(QTreeWidgetItem* , int);
        void onOkPushButtonClicked();
        void onCancelPushButtonClicked();
        void onHelpPushButtonClicked();
        void onSearchLineEditTextChanged(const QString& text);
        void onSRSRecentTableWidgetItemClicked(QTableWidgetItem*);
        
      private:
        
        void filter(const QList<QTreeWidgetItem*>& items);
        
        Ui::SRSManagerDialogForm* m_ui;         //!< Dialog form.    
        std::pair<int, std::string> m_selSrsId; //!< The selected SRS
        QStringList m_recentSRS;
      }; 
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_SRS_INTERNAL_QT_SRSMANAGERDIALOG_H
