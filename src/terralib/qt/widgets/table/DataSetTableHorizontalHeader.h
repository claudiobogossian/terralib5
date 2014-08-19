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

// STL
#include <map>
#include <string>

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

      struct LinkTableInfo
      {
        std::string m_dataSourceId;                     //!< Data Source Id.
        std::string m_dataSourceType;                   //!< Data Source Type.
        std::map<std::string, std::string> m_connInfo;  //!< Data Source Connection Info.
        std::string m_layerId;                          //!< Layer Id.
        std::string m_layerName;                        //!< Layer Name.
        std::string m_layerTitle;                       //!< Layer Title.
        std::string m_layerType;                        //!< Layer Type.
        size_t m_fieldNumber;                           //!< Field Number.
        std::string m_fieldName;                        //!< Field Name.
        size_t m_fieldType;                             //!< Field Type.
      };
      
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
            \brief Gets the drag drop flag.
          
            \return True: is drag drop. False: not is drag drop.
          */
          bool getDragDrop();

          /*!
            \brief Sets accept drop.
          
            \param b True: accept drop. False: not accept drop.
          */
          void setAcceptDrop(bool b);

          /*!
            \brief Gets accept drop flag.
          
            \return True: accept drop. False: not accept drop.
          */
          bool getAcceptDrop();

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
          
            \param firstFieldName Field name from first table.
            \param secondFieldName Field name from the secont table.
          */
          void linkTable(std::string firstFieldName, std::string secondFieldName);

          /*!
            \brief Signal emitted when the drop event runs.
          
            \param firstInfo Link information from the first table.
            \param secondInfo Link information from the second table.
          */
          void linkTable(const LinkTableInfo& firstInfo, const LinkTableInfo& secondInfo);

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
          DataSetTableView* m_view;                       //!< data set table view pointer.
          bool  m_doDragDrop;                             //!< True: do drag and drop. False: do not drag and drop.
          bool  m_acceptDrop;                             //!< True: accept drop. False: not accept drop.
          const te::map::AbstractLayer* m_layer;          //!< To get information for drag and drop.
          te::da::DataSet* m_dset;                        //!< To get information for drag and drop.
          LinkTableInfo m_firstLinkInfo;                  //!< First link info.
          LinkTableInfo m_secondLinkInfo;                 //!< Second link info.
          std::map<std::string, std::string> m_connInfo;  //!< connection info used for QMimeData.
      };
      
    } // namespace widgets
  } // namespace qt
} // namespace te

#endif // TE_QT_WIDGETS_DATASETTABLEHORIZONTALHEADER_H
