/*  Copyright (C) 2001-2010 National Institute For Space Research (INPE) - Brazil.

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
  \file DataViewPopupFilter.h
  \brief 
  \details 
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_DATAVIEWPOPUPFILTER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_DATAVIEWPOPUPFILTER_H

#include <terralib/qt/widgets/Config.h>

//! Qt include files
#include <QObject>

//! STL include files
#include <vector>

//! Forward declarations
class QMenu;
class QAction;

namespace te
{
  namespace qt
  {
    namespace widgets
    {

      //! Forward declarations
      class TabularViewer;

      /*!
        \class DataViewPopupFilter
        \copydoc DataViewPopupFilter.h
      */
      class TEQTWIDGETSEXPORT DataViewPopupFilter : public QObject
      {
        Q_OBJECT

      public:
        DataViewPopupFilter(TabularViewer* viewer);

        ~DataViewPopupFilter();

        bool eventFilter(QObject* watched, QEvent* event);

        QMenu* getHeaderMenu();

        QMenu* getDataMenu();

      protected slots:

        void hideCurrentColumn();

        void showColumn(QAction* act);

        void promote(QAction* act);

        void resetPromote(QAction* act);

        void clearGroup(QAction* act);

      signals:

        void hideColumn(int column);

        void showColumn(int column);

        void promotePointed();

        void promoteQueried();

        void promotePointedAndQueried();

        void resetPromotion();

        void resetPointedPromotion();

        void resetQueriedPromotion();

        void resetPointedAndQueriedPromotion();

        void addPointed(const std::vector<int>&);

        void resetPointed();

        void resetQueried();

        void resetPointedAndQueried();

        void resetAll();

      protected:

        TabularViewer* m_viewer;    //!< Pointer to viewer that is being observed.

        QMenu* m_head_menu;         //!< Header menu.                     
        QMenu* m_data_menu;         //!< Data menu.
        QMenu* m_hiddenCols;        //!< Hide columns menu.
        QMenu* m_resetPrm;          //!< Reset promoted groups menu.
        QMenu* m_setPrm;            //!< Set promoted groups menu.
        QMenu* m_rsetHL;            //!< Reset group highlights menu.
        
        QAction* m_colGroup;        //!< Separator of columns menu.
        QAction* m_prmGroup;        //!< Separator of promote menu.
        QAction* m_resetPrmGroup;   //!< Separator of reset promote menu.
        QAction* m_prm_all;         //!< Promote all action.
        QAction* m_reset_prm_all;   //!< Reset all promotion action.
        QAction* m_rset_all;         //!< Reset all highlighted action.

        int m_col;
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATAVIEWPOPUPFILTER_H