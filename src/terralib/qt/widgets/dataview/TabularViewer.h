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

  There is also an option for customize delegates. The te::qt::widgets::HLDelegateDecorator can be used. This way it is possible to change 
  the delegate behavior in run-time. To do that, the code looks like:
  \snippet examples/dataview/TabularViewerEx.cpp Adding custom delegate decorator
  The code above, presents a customization of the delegate of the TabularViewer \a tv. In the example, a StarDelegate is created over the
  clone of the delegate being used. It's not necessary cloning it, because the pointer to the older delegate is not owned by the TabularViewer.
  Note that if a clone of the delegate was used, then is necessary to delete the older pointer as we do in the last line of the example.

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
  namespace gm
  {
    //! Forward declarations
    class Geometry;
  }
  namespace map
  {
    //! Forward declarations
    class AbstractTable;
  }

  namespace da
  {
    //! Forward declarations
    class DataSet;
  }

  namespace qt
  {
    namespace widgets
    {
      //! Forward declarations
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
          Query_Items,            //!< Queried items.
          Point_Items,            //!< Pointed items.
          Query_and_Point_Items,  //!< Pointed and queried items.
          UserType                //!< Types defined by user.
        };

        /*!
          \enum MenuType
          \brief Types of menu.
          \details Two kinds of menu are used: header menu and data menu. Each one is called when right mouse button is 
          clicked on header area or data area, respectively.
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
          \brief Updates the popup menus. This method MUST BE called when a new item delegate has been set, 
          otherwise the popups may not reflect the viewer attributes.
        */
        void updatePopupMenus();

        /*!
          \brief Returns a set containing the identifiers of the groups promoted.
          \return Set of identifiers of groups promoted.
        */
        std::set<size_t> getPromotedGroups() const;

        /*!
          \brief
          \param o
          \param e
        */
        bool eventFilter(QObject* o, QEvent* e);

      public slots:

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
          \brief Updates the objects in a group of highlighted items.
          \param g The highlight group.
          \param ids List of identifiers to be the group.
        */
        void setHighlightObjects(const int& g, const std::set<std::string>& ids);

        /*!
          \brief Removes objects from the group of highlighted.
          \param g Group to be reset.
        */
        void clearHighlighted(const int& g);

        /*!
          \brief Updates the color of highlight of a group of items.
          \param g The highlight group.
          \param c The new color of the group.
        */
        void setHighlightColor(const int& g, const QColor& c, const bool& emitSignal=true);

        /*!
          \brief Points the rows in the vector.
          \param rows Rows to be pointed.
          \exception If any row is out of table boundaries a te::common::Exception will be raised.
        */
        void pointObjects(const std::vector<int>& rows);

        /*!
          \brief Clear all highlight of each group.
        */
        void resetHighlights();
        //@}

        /*! 
          \name Methods of promotion.
          \details Methods related to promotion of the highlighted objects. Promotion is a way of grouping highlighted objects. The operation just get the selected group,
          and presents its contents on top of table. It only reorder the table just by moving the highlighted. The promotion has additional costs, when analyzing the 
          consumption of computational resources. First of all, it is necessary to pre-process the primary keys. It consumes processing of CPU and time. The time required 
          for pre-processing the primary keys, depends on the CPU power and size of the table. Greatest tables requires more processing of the CPU and more time spent. 
          But this pre-processing is only required at first time, the subsequent requests of promotion are instantaneous. The second aspect is the memory consumption. 
          All pre-processed primary keys MUST be stored, in main memory.
          \anchor promotion_section
          \note DO NOT use promotion if you do not have additional computational resources available. It may compromise your system.
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
          \brief Removes promotion of all groups.
        */
        void resetPromote();

        /*!
          \brief Removes the promotion of group \a g.
          \param g Group identifier.
        */
        void resetPromoteHighlight(const int& g);

        /*!
          \brief Adds promotion to the group \a g.
          \param g Group identifier.
        */
        void addPromoteHighlight(const int& g);
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
          \details Shows all hidden columns and changes the positions to the original state.
        */
        void resetColumns();

      signals:
          
        void pointObjects(const std::map<std::string, te::gm::Geometry*>&);
        
        void groupColorChanged(const int&, const QColor&);

      protected:

        /*!
          \brief Sets the color of a group
        */
        void setGroupColor(const int& g, const QColor& color);

        te::qt::widgets::DataSetModel* m_model;           //!< Model for te::da::DataSet.
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
