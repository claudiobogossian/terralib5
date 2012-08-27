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
  \brief Receives the te::qt::widgets::TabularViewer objects events and handles if they are context menu events or mouse clicked. 
  \details This class receives events of context menu and mouse clicks and reacts to them. Popup menus are presented in context
  case, if required, on header or data. Mouse left button clicked adds the pointed row to the "Pointed" group. 
  Another task is to highlight objects that are clicked (with left mouse button).
  \sa te::map::PromoTable, te::qt::widgets::TabularViewer
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
        
        /*!
          \brief Contructor.
          \param viewier The te::qt::qwidgets::TabularViewer to be observed.
        */
        DataViewPopupFilter(TabularViewer* viewer);

        /*!
          \brief Destructor.
        */
        ~DataViewPopupFilter();

        /*!
          \brief Reimplementation of QObject method.
          \details Three events are handled:
          <ol>
            <li>Header context menu required.</li>
            <li>Data context menu required.</li>
            <li>Point data (mouse left button clicked on a row).</li>
          </ol>
        */
        bool eventFilter(QObject* watched, QEvent* event);

        /*!
          \brief Returns the pointer to the header menu.
          \return The popup menu of the header.
        */
        QMenu* getHeaderMenu();

        /*!
          \brief Returns the pointer to the data menu.
          \return The popup menu of data.
        */
        QMenu* getDataMenu();

      protected slots:

        /*!
          \name Popup actions.
          \details Actions executed by the popup menus.
        */
        //@{

        /*!
          \brief Hides the column that where selected.
        */
        void hideCurrentColumn();

        /*!
          \brief Shows the selected column.
          \param act The QAction triggered. It carries the position of column to be showed.
        */
        void showColumn(QAction* act);

        /*!
          \brief Promotes a group of highlighted itens.
          \param act The QAction triggered. It carries the group of highlighted to be promoted.
          \note Promotion has additional costs. See documentations to learn details about that.
        */
        void promote(QAction* act);

        /*!
          \brief Resets the promotion of some group.
          \param act The QAction triggered. It carries the group of highlighted to reset the promotion.
        */
        void resetPromote(QAction* act);

        /*!
          \brief Removes objects references of some group of highlighted.
          \param act The QAction triggered. It carries the group of highlighted to be cleared.
        */
        void clearGroup(QAction* act);
        //@}

      signals:

        /*!
          \name Signals.
          \brief Signals emmited by popup actions.
          \note Columns positions are ALWAYS logical. For details about logical an visual positions see QHeaderView documentation.
        */
        //@{

        /*!
          \brief Emitted to hide column \a column.
          \param column Logical column position.
        */
        void hideColumn(int column);

        /*!
          \brief Emitted to show column \a column.
          \param column Logical column position.
        */
        void showColumn(int column);

        /*!
          \brief Emitted to promote only pointed objects.
        */
        void promotePointed();

        /*!
          \brief Emitted to promote only queried objects.
        */
        void promoteQueried();

        /*!
          \brief Emitted to promote only pointed and queried objects.
        */
        void promotePointedAndQueried();

        /*!
          \brief Emitted to reset all promotion.
        */
        void resetPromotion();

        /*!
          \brief Emitted to reset only pointed promotion.
        */
        void resetPointedPromotion();

        /*!
          \brief Emitted to reset only queried promotion.
        */
        void resetQueriedPromotion();

        /*!
          \brief Emitted to reset only pointed and queried promotion.
        */
        void resetPointedAndQueriedPromotion();

        /*!
          \brief Emitted to add objects to the pointed group.
        */
        void addPointed(const std::vector<int>&);

        /*!
          \brief Emitted to remove only objects of the pointed group.
        */
        void resetPointed();

        /*!
          \brief Emitted to remove only objects of the queried group.
        */
        void resetQueried();

        /*!
          \brief Emitted to remove only objects of the pointed and queried group.
        */
        void resetPointedAndQueried();

        /*!
          \brief Emitted remove objects of all highlight groups.
        */
        void resetAll();
        //@}

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
        QAction* m_rset_all;        //!< Reset all highlighted action.

        int m_col;                  //!< Visual column position.
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_DATAVIEWPOPUPFILTER_H