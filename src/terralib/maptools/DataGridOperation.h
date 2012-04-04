/*  Copyright (C) 2001-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file GridDataOperation.h

  \brief This class represents basically the operations that can be accomplished
         on a data set organized as a data grid.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_DATAGRIDOPERATION_H
#define __TERRALIB_MAPTOOLS_INTERNAL_DATAGRIDOPERATION_H

// TerraLib
#include "Config.h"
#include "Enums.h"
#include "../color/RGBAColor.h"

// STL
#include <vector>
#include <set>

namespace te
{
// Forward declarations
  namespace da
  {
    class DataSet;
    class DataSetType;
  }

  namespace map
  {
    /*!
      \class GridDataOperation

      \brief This class represents basically the operations that can be accomplished
             on a data set organized as a data grid.
    
     */
    class TEMAPEXPORT DataGridOperation
    {
      public:

        /*!
          \enum SelectionStatus

          \brief Each data grid row has one of the following status.
         */
        enum SelectionStatus
        {
          DESELECTED,           //!< The row is deselected.
          POINTED,              //!< The row is pointed.
          QUERIED,              //!< The row is queried.
          POINTED_AND_QUERIED   //!< The row is pointed and queried.
        };

        /** @name Initializer Methods
         *  Methods related to instantiation, destruction, and initialization
         */
        //@{

        /*!
          \brief It constructs a GridDataOperation.          

          \param dataSet The data set whose data will be the data grid contents.
         */
        DataGridOperation();

        /*! \brief Virtual destructor. */
        ~DataGridOperation();

        /*!
          \brief It initializes a GridDataOperation.          

          \param dt      The dataset type associated to a dataset.
          \param dataSet The data set whose data will be the data grid contents.
         */
        void init(te::da::DataSetType* dt, te::da::DataSet* dataSet);

        //@}

        /** @name Accessor methods
         *  Methods used to get or set the class attributes.
         */
        //@{

        /*!
          \brief It returns the data set type containing the schema.

          \return The data set type.
         */
        te::da::DataSetType* getDataSetType();

        /*!
          \brief It sets the data set type containing the schema.

          \param d The data set type.
         */
        void setDataSetType(te::da::DataSetType* dt);

        /*!
          \brief It returns the data set associated to the data grid.

          \return The data set of the data grid.
         */
        te::da::DataSet* getDataSet();

        /*!
          \brief It sets the data set associated to the data grid.

          \param d The data set to be associated to the data grid.
         */
        void setDataSet(te::da::DataSet* d);

        /*!
          \brief It returns the number of rows of the data grid.

          \return The number of rows of the data grid.
         */
        int getNumberOfRows() const;

        /*!
          \brief It returns the number of columns of the data grid.

          \return The number of columns of the data grid.
         */
        int getNumberOfColumns() const;

        /*!
          \brief It returns the selection status of the visual row.

          \param visualRow The visual row.

          \return The visual row selection status.
         */
        te::map::DataGridOperation::SelectionStatus getVisualRowStatus(int visualRow) const;

        /*!
          \brief It sets the selection status of the visual row.

          \param visualRow The visual row.
          \param status    The selection status to be set to the given row.
         */
        void setVisualRowStatus(int visualRow, SelectionStatus status);

        /*!
          \brief It returns the logical row associated to a visual row.

          \param visualRow The visual row.

          \return The logical row associated to the given visual row.
         */
        int getLogicalRow(int visualRow) const;

        /*!
          \brief It sets the logical row associated to the visual row.

          \param visualRow  The visual row.
          \param logicalRow The logical row.
         */
        void setLogicalRow(int visualRow, int logicalRow);

        /*!
          \brief It returns the visual row associated to a logical row.

          \param logicalRow The logical row.

          \return The visual row associated to the given logical row.
         */
        int getVisualRow(int logicalRow) const;

        /*!
          \brief It sets the visual row associated to the logical row.

          \param logicalRow The logical row.
          \param visualRow  The visual row.
         */
        void setVisualRow(int logicalRow, int visualRow);

        /*!
          \brief It returns the selection status of the visual column.

          \param The visual column.

          \return The column selection status: false (deselected), true (selected).
         */
        bool getVisualColumnStatus(int visualColumn) const;

        /*!
          \brief It sets the selection status of the visual column.

          \param visualColumn The column whose selection status will be set.
          \param status       False, to deselect the column; true, to select the column.
         */
        void setVisualColumnStatus(int visualColumn, bool status);

        /*!
          \brief It returns the logical column associated to a visual column.

          \param visualColumn The visual column.

          \return The logical column associated to the given visual column.
         */
        int getLogicalColumn(int visualColumn) const;

        /*!
          \brief It sets the logical column associated to the visual column.

          \param visualColumn  The visual column.
          \param logicalColumn The logical column.
         */
        void setLogicalColumn(int visualColumn, int logicalColumn);

        /*!
          \brief It returns the visual column associated to a logical column.

          \param logicalColumn The logical column.

          \return The visual column associated to the given logical column.
         */
        int getVisualColumn(int logicalColumn) const;

        /*!
          \brief It sets the visual column associated to the logical column.

          \param logicalColumn The logical column.
          \param visualColumn  The visual column.
         */
        void setVisualColumn(int logicalColumn, int visualColumn);

        /*!
          \brief It returns the logical column that contains geometry data.

          \return The logical column that contains geometry data.
         */
        size_t getLogicalGeometryColumn() const;

        /*!
          \brief It returns the logical row of a given feature.

          \param featureId The feature id.

          \return The logical row of the given feature.
         */
        int getLogicalRow(const std::string& featureId) const;

        /*!
          \brief It returns the feature id of a logical row.

          \param logicalRow The feature logical row.

          \return The feature id of the given logical row.
         */
        const std::string& getDataSetId(int logicalRow) const;

        /*!
          \brief It returns the color of the deselected rows.

          \return The color of the deselected rows.
         */
        const te::color::RGBAColor& getDeselectedColor() const;

        /*!
          \brief It sets the color of the deselected rows.

          \param color The color of the deselected rows.
         */
        void setDeselectedColor(const te::color::RGBAColor& color);

        /*!
          \brief It returns the color of the pointed rows.

          \return The color of the pointed rows.
         */
        const te::color::RGBAColor& getPointedColor() const;

        /*!
          \brief It sets the color of the pointed rows.

          \param color The color of the pointed rows.
         */
        void setPointedColor(const te::color::RGBAColor& color);

        /*!
          \brief It returns the color of the queried rows.

          \return The color of the queried rows.
         */
        const te::color::RGBAColor& getQueriedColor() const;

        /*!
          \brief It sets the color of the queried rows.

          \param color The color of the queried rows.
         */
        void setQueriedColor(const te::color::RGBAColor& color);

        /*!
          \brief It returns the color of the pointed and queried rows.

          \return The color of the pointed and queried rows.
         */
        const te::color::RGBAColor& getPointedAndQueriedColor() const;

        /*!
          \brief It sets the color of the pointed and queried rows.

          \param color The color of the pointed and queried rows.
         */
        void setPointedAndQueriedColor(const te::color::RGBAColor& color);

        //@}

        /** @name General methods
         *  General methods that are related to the operations of selecting
            or deselecting the rows and columns of the data grid.
         */
        //@{

        /*!
          \brief It returns the feature selection status resulting from the combination
                 of the selection status of the rows that are associated to this feature.

          \param featureId The feature id.

          \return The selection status of the feature:
                  <ul>
                  <li>If all the feature rows are deselected, the return value is DESELECTED</li>;
                  <li>If there is a feature row pointed, and all the others are deselected, the return value is POINTED</li>;
                  <li>If there is a feature row queried, and all the others are deselected, the return value is QUERIED</li>;
                  <li>If any of the feature rows is pointed and queried, the return value is POINTED_AND_QUERIED</li>;
                  <li>If there is a feature row pointed and another queried, the return value is POINTED_AND_QUERIED</li>.
                  </ul>
         */
        SelectionStatus getDataSetSelectionStatus(const std::string& featureId);

        /*!
          \brief It gets the rows having the selection status POINTED, QUERIED, or POINTED_AND_QUERIED

          \return The list of all the rows that are selected.
         */
        std::vector<int> getSelectedRows();

        /*!
          \brief It gets the columns that are selected.

          \return The list of all the columns that are selected.
         */
        std::vector<int> getSelectedColumns();

        /*!
          \brief It toggles the pointing selection status of the given visual rows.

          \param visualRows The visual rows whose pointing selection status will be toggled.
         */
        void toggleRowsPointingStatus(const std::vector<int>& visualRow);

        /*!
          \brief It toggles the pointing selection status of the given visual column.

          \param visualColumn The visual column whose pointing selection status will be toggled.
         */
        void toggleColumnPointingStatus(int visualColumn);

        /*!
          \brief It adds to POINTED the selection status of the given visual rows.

          \param visualRows The visual rows whose selection status will be added as POINTED.
         */
        void addRowsToPointed(const std::vector<int>& visualRows);

        /*!
          \brief It sets as POINTED the selection status of the given visual rows. The others are set as not indicated

          \param visualRows The visual rows whose selection status will be set as POINTED.
         */
        void setRowsAsPointed(const std::vector<int>& visualRows);

        /*!
          \brief It sets as pointed (selected) the selection status of the given visual columns.

          \param visualColumns The visual columns whose selection status will be set as pointed.
         */
        void setColumnsAsPointed(const std::vector<int>& visualColumns);

        /*!
          \brief It removes the POINTED status of the given visual rows.

          \param visualRows The visual rows whose selection status will be removed as POINTED.
         */
        void removePointedStatusOfRows(const std::vector<int>& visualRows);

        /*! \brief It removes the POINTED status of all the rows. */
        void removePointedStatusOfAllRows();

        /*!
          \brief It removes the pointed (selected) status of the given visual columns.

          \param visualColumns The visual columns whose selection status will be removed as pointed.
         */
        void removePointedStatusOfColumns(const std::vector<int>& visualColumns);

        /*!
          \brief It sets as QUERIED the selection status of the given visual rows.

          \param visualRows The visual rows whose selection status will be set as QUERIED.
         */
        void setRowsAsQueried(const std::vector<int>& visualRows);

        /*!
          \brief It add rows to QUERIED the selection status of the given visual rows.

          \param visualRows The visual rows whose selection status will be added to QUERIED.
         */
        void addRowsToQueried(const std::vector<int>& visualRows);

        /*!
          \brief It removes the QUERIED status of the given visual rows.

          \param visualRows The visual rows whose selection status will be removed as QUERIED.
         */
        void removeQueriedStatusOfRows(const std::vector<int>& visualRows);

        /*! \brief It removes the QUERIED status of all the rows. */
        void removeQueriedStatusOfAllRows();

        /*! \brief It sets all the rows as DESELECTED. */
        void removeRowSelections();

        /*! \brief It sets all the columns as DESELECTED. */
        void removeColumnSelections();

        /*!
          \brief It promotes the rows having the given selection status to occupy the first positions on the data grid.

          \param status The selection status (POINTED, QUERIED, or POINTED_AND_QUERIED).
         */
        void promoteRows(SelectionStatus status);

        /*!
          \brief It updates the logical to visual mapping of the grid rows according to the given dataset sorted.

          \param sortedDataSet The sorted dataset.
         */
        void sortColumns(te::da::DataSet* sortedDataSet);

        /*!
          \brief It moves the visual column from a position to another position.

          \param fromPos The original position of the visual column.
          \param toPos   The final position of the visual column.

          \return The logical geometry column before performing the operation.
         */
        int moveColumn(int fromPos, int toPos);

        /*!
          \brief It gets the full name of the logical column located at the given position.
                 The full name will be given by the logical column name followed by a point
                 and the logical column name.

          \param logicalColumn The logical column number.
         */
        std::string getColumnFullName(size_t logicalColumn);

        /*!
          \brief It gets the last changed objects.

          \return The last changed objects.
         */
        const std::set<std::string>& getTheChanged();

        /*!
          \brief Add to changed objects.

          \param ob The changed object.
         */
        void addToChanged(std::string ob);

        // os metodos abaixo  vao ser substituidos por style  (POR ENQUANTO FICA ASSIM PARA TESTES)
        void setLineWidth(int);
        int getLineWidth();
        void setContourWidth(int);
        void setContourColor(const te::color::RGBAColor&);
        te::color::RGBAColor getContourColor();
        int getContourWidth();
        void setPtMarkerType(PtMarkerType);
        PtMarkerType getPtMarkerType();
        void setPtWidth(int);
        int getPtWidth();
        void setPolygonOpacity(int);

        //@}

      protected:

        /*! \It clears the mapping variables. */
        void clear();
        
        /*! \It builds the relationship between the dataset items and the logical rows. */
        void buildDataSetItemToLogicalRowRelation();

      private:

        /** @name Copy Constructor and Assignment Operator
         *  Copy constructor and assignment operator not allowed.
         */
        //@{

        /*!
          \brief Copy constructor not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.
         */
        DataGridOperation(const DataGridOperation& rhs);

        /*!
          \brief Assignment operator not allowed.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
         */
        DataGridOperation& operator=(const DataGridOperation& rhs);

        //@}

      private:
        te::da::DataSetType* m_dt;                      //!< The data set type containing the schema.
        te::da::DataSet* m_dataSet;                     //!< The data set whose data will be the data grid contents.

        int m_numRows;                                  //!< The number of rows of the data grid.
        int m_numColumns;                               //!< The number of columns of the data grid.

        std::vector<SelectionStatus> m_visualRowStatus; //!< Status of the visual rows.
        std::vector<int> m_visualToLogicalRows;         //!< Mapping between the visual rows and the logical rows. 
        std::vector<int> m_logicalToVisualRows;         //!< Mapping between the logical rows and the visual rows.

        std::vector<bool> m_visualColumnStatus;         //!< Status of the visual column.
        std::vector<int> m_visualToLogicalColumns;      //!< Mapping between the visual columns and the logical columns.
        std::vector<int> m_logicalToVisualColumns;      //!< Mapping between the logical columns and the visual columns.

        size_t m_logicalGeometryColumn;                 //!< The logical column containing geometry data.

        std::map<std::string, int> m_dataSetItemToLogicalRowMap; //!< The mapping between the dataset item and the logical row.
        std::vector<std::string> m_logicalRowToDataSetItemMap;   //!< The mapping between the logical row and the dataset item.

        std::set<std::string> m_theChanged;             //!< The last changed objects.

        // as variaveis abaixo vao ser substituidos por style (POR ENQUANTO FICA ASSIM PARA TESTES)
        te::color::RGBAColor m_deselectedColor;         //!< The color cell when it is deselected.
        te::color::RGBAColor m_pointedColor;            //!< The color cell when it is pointed.
        te::color::RGBAColor m_queriedColor;            //!< The color cell when it is queried.
        te::color::RGBAColor m_PointedAndQueriedColor;  //!< The color cell when it is pointed and queried.
        int m_lineWidth;
        int m_contourWidth;
        te::color::RGBAColor m_contourColor;
        PtMarkerType m_ptMarkerType;
        int m_ptWidth;
        int m_polygonOpacity;
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_DATAGRIDOPERATION_H
