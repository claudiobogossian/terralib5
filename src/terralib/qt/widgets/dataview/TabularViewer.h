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
  \file TabularViewer.h
  \brief Defines a high-level viewer of te::da::DataSet objects as Table.
  \details This component also has mechanisms for presentation of data such as highlight different kind of objects.
  Four types of data highlighted can be used there:
  <ol>
    <li>Data without highlight.</li>
    <li>Pointed data.</li>
    <li>Queried data.</li>
    <li>Pointed and queried data.</li>
  </ol>

  Its possible to add any data to any group. To remove highlight, just remove the id from the group. Its, also, possible to
  change presentation parameters of any group.
  \version 5.0
  \author Frederico Augusto Bed&ecirc; &lt;frederico.bede@funcate.org.br&gt;
  \date 2001-2012
  \copyright GNU Lesser General Public License.
 */

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TABULARVIEWER_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TABULARVIEWER_H

#include <terralib/qt/widgets/Config.h>

//Qt include files
#include <QTableView>

//STL include files
#include <set>

namespace te
{
  namespace map
  {
    //Forward declarations
    class AbstractTable;
  }

  namespace da
  {
    //Forward declarations
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {
      //Forward declarations
      class DataSetModel;
      class HLDelegateDecorator;
      class DataViewPopupFilter;

      /*!
        \class TabularViewer
        \copydoc TabularViewer.h
      */
      class TEQTWIDGETSEXPORT TabularViewer : public QTableView
      {
        Q_OBJECT

      public:

        /*!
          \enum HLGroups
          \brief Defines the kinds of highlighted items.
        */
        enum HLGroups
        {
          Query_Items,          //!< Queried items.
          Point_Items,          //!< Pointed items.
          Query_and_Point_Items //!< Pointed and queried itenm.
        };

        /*!
          \enum 
          \brief
        */
        enum MenuType
        {
          Header,     //!< Header popup menu.
          Table       //!< Data popup menu.
        };

        /*! 
          \name Initializer methods.
          Methods related to instantiation and destruction.
        */
        //@{

        /*!
          \brief Contructor.
          \param parent QWidget parent.
        */
        TabularViewer(QWidget* parent=0);

        /*!
          \brief Virtual destructor.
        */
        ~TabularViewer();
        //@}

        /*!
          \brief Returns a set containing the identifiers of the groups promoted.
          \return Set of identifiers of groups promoted.
        */
        std::set<size_t> getPromotedGroups() const;

      public slots:

        /*! 
          \name Data configuration methods.
          \details Methods related to the configuration of data to be presented.
        */
        //@{

        /*!
          \brief Presents the data set.
          \param dset The te::da::DataSet to be presented.
          \note This class does NOT gets the ownership of the pointer of the data set.
        */
        void showData(te::da::DataSet* dset);

        /*!
          \overload void te::qt::widgets::TabularViewer::showData(te::map::AbstractTable* table) 	
          \param table A table object mapping the data to be presented.
          \note This class gets the ownership of the pointer \a table.
        */
        void showData(te::map::AbstractTable* table);

        /*!
          \brief Updates the primary keys.
          \param pkeys Positions of the primary keys columns.
          \exception If any of the positions is out of table boundaries an te::common::Exception will be raised.
        */
        void setPrimaryKeys(const std::vector<size_t>& pkeys);

        /*!
          \brief Returns the primary keys positions.
          \return The vector containing the positions of primary keys.
        */
        std::vector<size_t> getPrimaryKeys() const;
        //@}

        /*! 
          \name Highlight object methods.
          \details Methods related to configuration of basic highlight groups. These methods are used for updating highlight groups and presentation colors.
          By default, it uses tree groups of highlighted objects:

          <ol>
            <li>Pointed objects (objects pointed by a mouse click, for example).</li>
            <li>Queried objects (result of a query).</li>
            <li>Queried and pointed (objects that are in both groups previously mentioned).</li>
          </ol>

          \note When using composite primary keys, it is necessary concatenate the values  separated by a separator. By default the \"#\" is used as separator.
        */
        //@{

        /*!
          \brief Updates the color of pointed objects.
          \param c New color for highlight pointed objects.
        */
        void setPointedObjectsColor(const QColor& c);

        /*!
          \brief Adds a set of objects to the existing set of pointed objects
          \param objIds The collection of object identifiers to be added to the set of the pointed objects.
        */
        void addPointedObjects(const std::set<std::string>& objIds);

        /*!
          \overload void TabularViewer::addPointedObjects(const std::vector<int> rows)
          \param rows Rows to be pointed.
          \exception If any row is out of table boundaries a te::common::Exception will be raised.
        */
        void addPointedObjects(const std::vector<int>& rows);

        /*!
          \brief Updates the color of pointed objects.
          \param c New color for highlight pointed objects.
        */
        void setQueriedObjectsColor(const QColor& c);

        /*!
          \brief Adds a set of objects to the existing set of pointed objects
          \param objIds The collection of object identifiers to be added to the set of the pointed objects.
        */
        void addQueriedObjects(const std::set<std::string>& objIds);

        /*!
          \brief Updates the color of pointed objects.
          \param c New color for highlight pointed objects.
        */
        void setPointedAndQueriedObjectsColor(const QColor& c);
        //@}

        /*! 
          \name Clear highlighted objects.
          \details Methods used to removing objects from highlighted groups.
          \note The primary keys pre-processed still remain in memory, the promotion NOT. 
          See the section \ref promotion_section "Methods of promotion of highlighted groups" for details about the functionality of promote. 
        */
        //@{

        /*!
          \brief Clear all highlight of each group.
        */
        void resetHighlights();

        /*!
          \brief Clear pointed objects.
        */
        void resetPointed();

        /*!
          \brief Clear queried objects.
        */
        void resetQueried();

        /*!
          \brief Clear pointed and queried objects.
        */
        void resetPointedAndQueried();
        //@}

        /*! 
          \name Methods of promotion.
          \details Methods related to promotion of the highlighted objects. Promotion is a way of grouping highlighted objects. The operation just get the selected group,
          and presents its contents on top of table. It only reorder the table just by moving the highlighted. Promotion has an aditional cost, when analizing the 
          comsuption of computational resources. First of all, it is necessary to pre-proccess the primary keys. It consumes proccessing of CPU and time. The time required
          for pre-proccess depends on CPU power and size of the table. Greater table, more proccessing of the CPU and more time spent. But this pre-proccess is only required
          on first time, the subsequent requirements of promotion is instantaneously. The second aspect is the memory consuption. All pre-proccessed primary keys MUST be stored,
          in main memory.
          \anchor promotion_section
          \note DO NOT use promotion if you do not have aditional computational resources available. It may compromise your system.
          \note Resetting promotion DO NOT clears the primary keys pre-proccessed set.
        */
        //@{

        /*!
          \brief Enable or disable promotion.
          \details Enabling promotion may costs time. The primary keys must be pre-processed if it did not.
          \param enable True enables promotion, False disables it.
        */
        void setPromoteEnabled(const bool& enable);

        /*!
          \brief Promote all groups.
        */
        void promoteHighlighted();

        /*!
          \brief Promote pointed objects.
        */
        void promotePointed();

        /*!
          \brief Promote queried objects.
        */
        void promoteQueried();

        /*!
          \brief Promote pointed and queried objects.
        */
        void promotePointedAndQueried();

        /*!
          \brief Removes promotion of all groups.
        */
        void resetPromote();

        /*!
          \brief Removes promotion of pointed objects.
        */
        void resetPointedPromote();

        /*!
          \brief Removes promotion of queried objects.
        */
        void resetQueriedPromote();

        /*!
          \brief Removes promotion of pointed and queried objects.
        */
        void resetPointedAndQueriedPromote();
        //@}

        /*!
          \brief Returns the menu for the given \a type.
          \details This used for manage popup menus for each type available(header and data). 
          The user may add, remove or change actions in each menu. The ownership of the \a QMenu pointer stays in this object.
          Note that there's NO mechanism that prevent clients of deallocate memory of pointer of the menu. 
          If delete the pointers of menus, the system can crash.
          Make sure to NOT delete the pointer of the menu, after calling this method.
          \param
        */
        QMenu* getMenu(const MenuType& type);

        /*!
          \brief Resets state of all columns.
          \details Shows all hidden columns and changes the positions to original state.
        */
        void resetColumns();

      protected:

        /*!
          \brief Updates the objects in a group of highlighted items.
          \param g The highlight group.
          \param ids List of identifiers to be the group.
        */
        void setHighlightObjects(const HLGroups& g, const std::set<std::string>& ids);

        /*!
          \brief Removes objects from the group of highlighted.
          \param g Group to be reset.
        */
        void clearHighlighted(const HLGroups& g);

        /*!
          \brief Updates the color of highlight of a group of items.
          \param g The highlight group.
          \param c The new color of the group.
        */
        void setHighlightColor(const HLGroups& g, const QColor& c);

        /*!
          \brief Removes the promotion of group \a g.
          \param g Group identifier.
        */
        void resetPromoteHighlight(const HLGroups& g);

        /*!
          \brief Adds promotion to the group \a g.
          \param g Group identifier.
        */
        void addPromoteHighlight(const HLGroups& g);

        te::qt::widgets::DataSetModel* m_model;           //!< Model for te::da::DataSet.
        te::qt::widgets::HLDelegateDecorator* m_delegate; //!< Delegate for te::da::DataSet.
        te::map::AbstractTable* m_table;                  //!< Table to use with model.
        std::set<size_t> m_promotedGroups;                //!< Contains the groups that have been promoted.
        DataViewPopupFilter* m_menu_filter;               //!< Menus selector.

      private:
        /*! 
          \name Copy methods.
          No copy allowed.
        */
        //@{

        /*!
          \brief Copy constructor.
        */
        TabularViewer (const TabularViewer&);

        /*!
          \brief Copy operator.
        */
        TabularViewer& operator=(const TabularViewer&);
        //@}
      };
    }
  }
}

#endif //__TERRALIB_QT_WIDGETS_INTERNAL_TABULARVIEWER_H
