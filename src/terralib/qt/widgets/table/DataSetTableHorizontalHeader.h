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


#ifndef TE_QT_WIDGETS_DATASETTABLEHORIZONTALHEADER_H
#define TE_QT_WIDGETS_DATASETTABLEHORIZONTALHEADER_H

#include "../../../dataaccess/datasource/DataSource.h"

#include <QHeaderView>

namespace te
{
  namespace da
  {
    class DataSet;
  }

  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      // Forward declaration
      class DataSetTableView;
      
      class DataSetTableHorizontalHeader : public QHeaderView
      {
          Q_OBJECT

        public:
          /*!
            \brief Constructor.

            \param parent DataSetTableView parent.
          */
          DataSetTableHorizontalHeader(DataSetTableView* view);

          /*!
            \brief Sets the drag drop.
          
            \param b True: does the drag drop. False: do not drag drop.
          */
          void setDragDrop(bool b);

          /*!
            \brief Sets the layer to get drag and drop information.
          
            \param layer Pointer to the layer to be presented.
          */
          void setLayer(const te::map::AbstractLayer* layer);

          /*!
            \brief Sets the data set to get drag and drop information.
          
            \param layer Pointer to the layer to be presented.
          */
          void setDataSet(te::da::DataSet* dset);

        signals:

          /*!
            \brief Signal emitted when the drop event runs.
          
            \param fromFieldName field name of the input table.
            \param toFieldName field name of the output table.
          */
          void linkTable(std::string fromFieldName, std::string toFieldName);

      protected:
          /*!
            \brief Handles the mouse pres event e initiates the drag and drop if the flag is active.
          
            \param e The mouse event.
          */
          void mousePressEvent(QMouseEvent* e);

          /*!
            \brief Handles the drag enter event.
          
            \param e The drag enter event.
          */
          void dragEnterEvent(QDragEnterEvent *e);

          /*!
            \brief Handles the drag move event.
          
            \param e The drag move event.
          */
          void dragMoveEvent(QDragMoveEvent *e);

          /*!
            \brief Handles the drop Event.
          
            \param e The drag enter event.
          */
          void dropEvent(QDropEvent *e);

          /*!
            \brief It gets the layer data source.
          
            \param layer The layer from which we obtain the source.
          */
          te::da::DataSourcePtr GetDataSource(const te::map::AbstractLayer* layer);

        protected:
          DataSetTableView* m_view;               //!< data set table view pointer.
          bool  m_doDragDrop;                     //!< True: do drag and drop. False: do not drag and drop.
          const te::map::AbstractLayer* m_layer;  //!< To get information for drag and drop.
          te::da::DataSet* m_dset;                //!< To get information for drag and drop.
          size_t m_linkFromFieldNumber;           //!< To drag drop link information.
          std::string m_linkFromFieldName;        //!< To drag drop link information.
          size_t m_linkFromFieldType;             //!< To drag drop link information.
          std::string m_linkFromDataSourceId;     //!< To drag drop link information.
          std::string m_linkFromLayerId;          //!< To drag drop link information.
          std::string m_linkFromLayerName;        //!< To drag drop link information.
          std::string m_linkFromLayerTitle;       //!< To drag drop link information.
          std::string m_linkFromLayerType;        //!< To drag drop link information.
          size_t m_linkToFieldNumber;             //!< To drag drop link information.
          std::string m_linkToFieldName;          //!< To drag drop link information.
          size_t m_linkToFieldType;               //!< To drag drop link information.
          std::string m_linkToDataSourceId;       //!< To drag drop link information.
          std::string m_linkToLayerId;            //!< To drag drop link information.
          std::string m_linkToLayerName;          //!< To drag drop link information.
          std::string m_linkToLayerTitle;         //!< To drag drop link information.
          std::string m_linkToLayerType;          //!< To drag drop link information.
      };
      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_DATASETTABLEHORIZONTALHEADER_H
