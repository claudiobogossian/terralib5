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
        const te::color::RGBAColor& getDefaultColor() const;

        /*!
          \brief It sets the color of the deselected rows.

          \param color The color of the deselected rows.
         */
        void setDefaultColor(const te::color::RGBAColor& color);

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

        /*!
          \brief Sets the point marker type.

          \param t The point marker type.
        */
        void setPointMarkerType(PtMarkerType t);

        /*!
          \brief Gets the point marker type.

          \param The point marker type.
        */
        PtMarkerType getPointMarkerType();

        /*!
          \brief Sets the point icon.

          \param icon The point icon.
        */
        void setPointIcon(char* icon);

        /*!
          \brief Gets the point icon.

          \return The point icon.
        */
        char* getPointIcon();

        /*!
          \brief Sets the point icon size.

          \param s The point icon size.
        */
        void setPointIconSize(int s);

        /*!
          \brief Gets the point icon size.

          \return The point icon size.
        */
        int getPointIconSize();

        /*!
          \brief Sets the point icon image type.

          \param t The point icon image type.
        */
        void setPointIconImageType(ImageType t);

        /*!
          \brief Gets the point icon image type.

          \return The point icon image type.
        */
        ImageType getPointIconImageType();

        /*!
          \brief Sets the point width.

          \param w The point width.
        */
        void setPointWidth(int w);

        /*!
          \brief Sets the point color.

          \param cor The point color.
        */
        void setPointColor(const te::color::RGBAColor& cor);

        /*!
          \brief Gets the point color.

          \return The point color.
        */
        te::color::RGBAColor getPointColor();

        /*!
          \brief Gets the point width.

          \return The point width.
        */
        int getPointWidth();

        /*!
          \brief Sets the point pattern RGBA colors.

          \param patternRGBA The RGBA colors.
        */
        void setPointPatternRGBA(te::color::RGBAColor** patternRGBA);

        /*!
          \brief Gets the point pattern RGBA colors.

          \return The RGBA colors.
        */
        te::color::RGBAColor** getPointPatternRBGA();

        /*!
          \brief Sets the point pattern RGBA number of columns.

          \param n The point pattern RGBA number of columns.
        */
        void setPointPatternRGBANCols(int n);

        /*!
          \brief Gets the point pattern RGBA number of columns.

          \return The point pattern RGBA number of columns.
        */
        int getPointPatternRGBANCols();

        /*!
          \brief Sets the point pattern RGBA number of rows.

          \param n The point pattern RGBA number of rows.
        */
        void setPointPatternRGBANRows(int n);

        /*!
          \brief Gets the point pattern RGBA number of rows.

          \return The point pattern RGBA number of rows.
        */
        int getPointPatternRGBANRows();

        /*!
          \brief Sets the line width.

          \param w The line width.
         */
        void setLineWidth(int w);

        /*!
          \brief Gets the line width.

          \return The line width.
         */
        int getLineWidth();

        /*!
          \brief Sets the line color.

          \param cor The line color.
        */
        void setLineColor(const te::color::RGBAColor& cor);

        /*!
          \brief Gets the line color.

          \return The line color.
        */
        te::color::RGBAColor getLineColor();

        /*!
          \brief Sets the line pattern icon.

          \param icon The line pattern icon.
        */
        void setLinePatternIcon(char* icon);

        /*!
          \brief Gets the line pattern icon.

          \return The line pattern icon.
        */
        char* getLinePatternIcon();

        /*!
          \brief Sets the line pattern icon size.

          \param s The line pattern icon size.
        */
        void setLinePatternIconSize(int s);

        /*!
          \brief Gets the line pattern icon size.

          \return The line pattern icon size.
        */
        int getLinePatternIconSize();

        /*!
          \brief Sets the line pattern icon image type.

          \param t The line pattern icon image type.
        */
        void setLinePatternIconImageType(ImageType t);

        /*!
          \brief Gets the line pattern image type.

          \return The line pattern image type.
        */
        ImageType getLinePatternIconImageType();

        /*!
          \brief Sets the line pattern RGBA colors.

          \param patternRGBA The RGBA colors.
        */
        void setLinePatternRGBA(te::color::RGBAColor** patternRGBA);

        /*!
          \brief Gets the line pattern RGBA colors.

          \return The RGBA colors.
        */
        te::color::RGBAColor** getLinePatternRBGA();

        /*!
          \brief Sets the line pattern RGBA number of columns.

          \param n The line pattern RGBA number of columns.
        */
        void setLinePatternRGBANCols(int n);

        /*!
          \brief Gets the line pattern RGBA number of columns.

          \return The line pattern RGBA number of columns.
        */
        int getLinePatternRGBANCols();

        /*!
          \brief Sets the line pattern RGBA number of rows.

          \param n The line pattern RGBA number of rows.
        */
        void setLinePatternRGBANRows(int n);

        /*!
          \brief Gets the line pattern RGBA number of rows.

          \return The line pattern RGBA number of rows.
        */
        int getLinePatternRGBANRows();

        /*!
          \brief Sets the polygon contour width.

          \param w The polygon contour width.
        */
        void setPolygonContourWidth(int w);
 
        /*!
          \brief Gets the polygon contour width.

          \return The polygon contour width.
         */
        int getPolygonContourWidth();

        /*!
          \brief Sets the polygon contour color.

          \param cor The polygon contour color.
        */
        void setPolygonContourColor(const te::color::RGBAColor& cor);

        /*!
          \brief Gets the polygon contour color.

          \return The polygon contour color.
        */
        te::color::RGBAColor getPolygonContourColor();

        /*!
          \brief Sets the polygon contour pattern icon image type.

          \param t The polygon contour pattern icon image type.
        */
        void setPolygonContourPatternIconImageType(ImageType t);

        /*!
          \brief Gets the polygon contour pattern icon image type.

          \return The polygon contour pattern icon image type.
        */
        ImageType getPolygonContourPatternIconImageType();

        /*!
          \brief Sets the polygon contour pattern icon.

          \param icon The polygon contour pattern icon.
        */
        void setPolygonContourPatternIcon(char* icon);

        /*!
          \brief Gets the polygon contour pattern icon.

          \return The polygon contour pattern icon.
        */
        char* getPolygonContourPatternIcon();

        /*!
          \brief Sets the polygon contour image type.

          \param s The polygon contour icon size.
        */
        void setPolygonContourPatternIconSize(int s);

        /*!
          \brief Gets the polygon contour pattern icon size.

          \return The polygon contour pattern icon size.
        */
        int getPolygonContourPatternIconSize();

        /*!
          \brief Sets the polygon contour pattern RGBA colors.

          \param patternColors The polygon contour Pattern RGBA colors.
        */
        void setPolygonContourPatternRGBA(te::color::RGBAColor** patternColors);

        /*!
          \brief Gets the polygon contour pattern RGBA.

          \return The polygon contour Pattern RGBA colors.
        */
        te::color::RGBAColor** getPolygonContourPatternRGBA();

        /*!
          \brief Sets the polygon contour pattern RGBA number of columns.

          \param n The polygon contour pattern RGBA number of columns.
        */
        void setPolygonContourPatternRGBANCols(int n);

        /*!
          \brief Gets the polygon contour pattern RGBA number of columns.

          \return The polygon contour pattern RGBA number of columns.
        */
        int getPolygonContourPatternRGBANCols();

        /*!
          \brief Sets the polygon contour pattern RGBA number of rows.

          \param n The polygon contour pattern RGBA number of rows.
        */
        void setPolygonContourPatternRGBANRows(int n);

        /*!
          \brief Gets the polygon contour pattern RGBA number of rows.

          \return The polygon contour pattern RGBA number of rows.
        */
        int getPolygonContourPatternRGBANRows();

        /*!
          \brief Sets the polygon fill color.

          \param cor The polygon fill color.
        */
        void setPolygonFillColor(const te::color::RGBAColor& cor);

        /*!
          \brief Gets the polygon fill color.

          \return The polygon fill color.
        */
        te::color::RGBAColor getPolygonFillColor();

        /*!
          \brief Sets the polygon fill marker type.

          \param t The polygon fill marker type.
        */
        void setPolygonMarkerType(PtMarkerType t);

        /*!
          \brief Gets the polygon fill marker type.

          \param The polygon fill marker type.
        */
        PtMarkerType getPolygonMarkerType();

        /*!
          \brief Sets the polygon fill marker color.

          \param cor The polygon fill marker color.
        */
        void setPolygonFillMarkerColor(const te::color::RGBAColor& cor);

        /*!
          \brief Gets the polygon fill marker color.

          \param The polygon fill marker color.
        */
        te::color::RGBAColor getPolygonFillMarkerColor();

        /*!
          \brief Sets the polygon fill pattern icon.

          \param icon The polygon fill pattern icon.
        */
        void setPolygonPatternIcon(char* icon);

        /*!
          \brief Gets the polygon fill pattern icon.

          \return The polygon fill pattern icon.
        */
        char* getPolygonPatternIcon();

        /*!
          \brief Sets the polygon fill pattern icon size.

          \param s The polygon fill pattern icon size.
        */
        void setPolygonPatternIconSize(int s);

        /*!
          \brief Gets the polygon fill pattern icon size.

          \return The polygon fill pattern icon size.
        */
        int getPolygonPatternIconSize();

        /*!
          \brief Sets the polygon fill pattern icon image type.

          \param t The polygon fill pattern icon image type.
        */
        void setPolygonPatternIconImageType(ImageType t);

        /*!
          \brief Gets the polygon fill pattern icon image type.

          \return The polygon fill pattern icon image type.
        */
        ImageType getPolygonPatternIconImageType();

        /*!
          \brief Sets the polygon fill pattern width.

          \param w The polygon fill pattern width.
        */
        void setPolygonPatternWidth(int w);

        /*!
          \brief Gets the polygon fill pattern width.

          \return The polygon fill pattern width.
        */
        int getPolygonPatternWidth();

        /*!
          \brief Sets the polygon fill pattern RGBA colors.

          \param patternRGBA The RGBA colors.
        */
        void setPolygonPatternRGBA(te::color::RGBAColor** patternRGBA);

        /*!
          \brief Gets the polygon fill pattern RGBA colors.

          \return The RGBA colors.
        */
        te::color::RGBAColor** getPolygonPatternRBGA();

        /*!
          \brief Sets the polygon fill pattern RGBA number of columns.

          \param n The polygon fill pattern RGBA number of columns.
        */
        void setPolygonPatternRGBANCols(int n);

        /*!
          \brief Gets the polygon fill pattern RGBA number of columns.

          \return The polygon fill pattern RGBA number of columns.
        */
        int getPolygonPatternRGBANCols();

        /*!
          \brief Sets the polygon fill pattern RGBA number of rows.

          \param n The polygon fill pattern RGBA number of rows.
        */
        void setPolygonPatternRGBANRows(int n);

        /*!
          \brief Gets the polygon fill pattern RGBA number of rows.

          \return The polygon fill pattern RGBA number of rows.
        */
        int getPolygonPatternRGBANRows();

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

        te::color::RGBAColor m_defaultColor;            //!< The color cell when it is deselected.
        te::color::RGBAColor m_pointedColor;            //!< The color cell when it is pointed.
        te::color::RGBAColor m_queriedColor;            //!< The color cell when it is queried.
        te::color::RGBAColor m_pointedAndQueriedColor;  //!< The color cell when it is pointed and queried.

        PtMarkerType m_pointMarkerType;            //!< Point marker type.
        char* m_pointIcon;                         //!< Point pattern icon.
        std::size_t m_pointIconSize;               //!< Point pattern icon size.
        ImageType m_pointIconImageType;            //!< Point pattern icon image type.
        int m_pointWidth;                          //!< Point width.
        te::color::RGBAColor m_pointColor;         //!< Point color.
        te::color::RGBAColor** m_pointPatternRGBA; //!< Point pattern RGBA.
        int m_pointPatternRGBANCols;               //!< Point pattern RGBA number of columns.
        int m_pointPatternRGBANRows;               //!< Point pattern RGBA number of rows.

        int m_lineWidth;                          //!< Line width.
        te::color::RGBAColor m_lineColor;         //!< Line color.
        char* m_linePatternIcon;                  //!< Line icon.
        std::size_t m_linePatternIconSize;        //!< Line icon size.
        ImageType m_linePatternIconImageType;     //!< Line icon image type.
        te::color::RGBAColor** m_linePatternRGBA; //!< Line pattern RGBA.
        int m_linePatternRGBANCols;               //!< Line pattern RGBA number of columns.
        int m_linePatternRGBANRows;               //!< Line pattern RGBA number of rows.

        int m_polygonContourWidth;                          //!< Polygon contour width.
        te::color::RGBAColor m_polygonContourColor;         //!< Polygon contour color.
        ImageType m_polygonContourPatternIconImageType;     //!< Polygon contour pattern icon image type.
        char* m_polygonContourPatternIcon;                  //!< Polygon contour pattern icon.
        std::size_t m_polygonContourPatternIconSize;        //!< Polygon contour pattern icon size.
        te::color::RGBAColor** m_polygonContourPatternRGBA; //!< Polygon contour pattern RGBA.
        int m_polygonContourPatternRGBANCols;               //!< Polygon contour pattern RGBA number of columns.
        int m_polygonContourPatternRGBANRows;               //!< Polygon contour pattern RGBA number of rows.

        te::color::RGBAColor m_polygonFillColor;            //!< Polygon contour color.
        PtMarkerType m_polygonMarkerType;                   //!< Polygon fill mark.
        te::color::RGBAColor m_polygonFillMarkerColor;      //!< Polygon fill marker color.
        char* m_polygonPatternIcon;                         //!< Polygon fill pattern icon.
        std::size_t m_polygonPatternIconSize;               //!< Polygon fill pattern icon size.
        ImageType m_polygonPatternIconImageType;            //!< Polygon fill pattern icon image type.
        int m_polygonPatternWidth;                          //!< Polygon fill pattern icon width or mark width.
        te::color::RGBAColor** m_polygonPatternRGBA;        //!< Polygon fill pattern pattern RGBA.
        int m_polygonPatternRGBANCols;                      //!< Polygon fill pattern pattern RGBA number of columns.
        int m_polygonPatternRGBANRows;                      //!< Polygon fill pattern pattern RGBA number of rows.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_DATAGRIDOPERATION_H
