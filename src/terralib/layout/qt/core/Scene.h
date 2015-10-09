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
  \file Scene.h
   
   \brief Class representing the scene. This scene is child of QGraphicsScene, part of Graphics View Framework. 
   The scene is not drawn, but the items that are inserted into it. She is responsible for managing items QGraphicsItem and know when and how to redrawing them.
   The scene has a coordinate system that is, by default, the same for all items that are inserted. 
   The coordinate system of this representation is millimeters and your point 0.0 is in the bottom left(Cartesian coordinate system).
   The default settings of the layout module are in the object Context (singleton).

  \ingroup layout
*/

#ifndef  __TERRALIB_LAYOUT_INTERNAL_SCENE_H 
#define __TERRALIB_LAYOUT_INTERNAL_SCENE_H

// TerraLib
#include "../../../geometry/Envelope.h"
#include "../../core/AbstractScene.h"
#include "../../core/Config.h"
#include "../item/MovingItemGroup.h"
#include "AlignItems.h"

// STL
#include <string>
#include <vector>
#include <map>

// Qt
#include <QGraphicsScene>
#include <QTransform>
#include <QColor>
#include <QPointF>
#include <QRectF>
#include <QSize>

class QUndoCommand;
class QUndoStack;
class QGraphicsItemGroup;
class QPainter;
class QWidget;
class QStyleOptionGraphicsItem;
class QGraphicsSceneMouseEvent;
class QKeyEvent;

namespace te
{
  namespace layout
  {
    class EnumType;
    class Properties;
    class VisualizationArea;
    class PaperConfig;
    class View;
    class AbstractItemView;
    class ItemObserver;
    class AbstractItemController;
    class ContextObject;

  /*!
    \brief Class representing the scene. This scene is child of QGraphicsScene, part of Graphics View Framework. 
    The scene is not drawn, but the items that are inserted into it. She is responsible for managing items QGraphicsItem and know when and how to redrawing them.
    The scene has a coordinate system that is, by default, the same for all items that are inserted. 
    The coordinate system of this representation is millimeters and your point 0.0 is in the bottom left(Cartesian coordinate system).
    The default settings of the layout module are in the object Context (singleton).
    
    \ingroup layout

    \sa te::layout::AbstractScene
  */
    class TELAYOUTEXPORT Scene : public QGraphicsScene, public AbstractScene
    {
      Q_OBJECT //for slots/signals

      public:

        /*!
          \brief Constructor
        */ 
        Scene(QObject* object = (QObject*)0);

        /*!
          \brief Constructor. The ownership of objects passed via parameter becomes the scene.

          \param align applying the alignment of one or more objects
        */ 
        Scene(AlignItems* align, PaperConfig* paperConfig, QObject* object = 0);

        /*!
          \brief Destructor
        */ 
        virtual ~Scene();

        /*!
        \brief Method that inserts a graphic object in the scene. Inverts the matrix of the object if necessary, ex.: TextItem.

        \param item graphic object
        */
        virtual void insertItem(ItemObserver* item);

        /*!
          \brief Method that inserts a graphic object in the scene. Inverts the matrix of the object if necessary, ex.: TextItem.
      
          \param item graphic object      
        */ 
        virtual void insertItem(AbstractItemView* item);

        /*!
          \brief Method that inserts a graphic object in the scene. Inverts the matrix of the object if necessary, ex.: TextItem.
      
      \param item graphic object      
        */ 
        virtual void insertItem(QGraphicsItem* item);
               
    /*!
          \brief Method that starts the scene and configures. Calculates the transformation matrix of the scene and calculates the ratio of the size of the scene with the paper size.
      
      \param screenWMM width of physical screen in millimeters
      \param screenHMM height of physical screen in millimeters
        */
        virtual void init(double screenWMM, double screenHMM, ContextObject context);

    /*!
          \brief Method that returns the matrix transformation scene.
            
      \return transformation matrix
        */
        virtual QTransform sceneTransform();
        
    /*!
          \brief Method that deletes all selected items in the scene.
        */
        virtual void deleteItems();

        /*!
          \brief Method that delete item in the scene.
        */
        virtual bool deleteItem(QGraphicsItem *item);

    /*!
          \brief Method that removes all selected items in the scene and creates a Command to Undo/Redo of type DeleteCommand. The item is removed from the scene, but is not deleted.
        */
        virtual void removeSelectedItems();
    
    /*!
          \brief Groups objects and creates a QGraphicsItemGroup object. A command Undo/Redo of type AddCommand is created.
      
      \param list of objects
      
      \return items group of objects
        */
        virtual QGraphicsItemGroup* createItemGroup( const QList<QGraphicsItem *> & items );

        
        virtual te::layout::MovingItemGroup* createMovingItemGroup( const QList<QGraphicsItem*>& items);

    /*!
          \brief Method that delete object grouping, but the individual objects continue to exist.
      
      \param group list of objects
        */
        virtual void destroyItemGroup( QGraphicsItemGroup *group );
        
    /*!
          \brief Method that insert command Undo/Redo of type AddCommand in the Undo/Redo stack.
      
      \param command command
        */
        virtual void addUndoStack( QUndoCommand* command );

    /*!
          \brief Method that limits the size of the stack of Undo/Redo.
      
      \param limit limit
        */
        virtual void setUndoStackLimit( int limit );

    /*!
          \brief Method that returns the size limit of the stack Undo/Redo.
      
      \return limit
        */
        virtual int getUndoStackLimit();

    /*!
          \brief Method that return stack of Undo/Redo.
      
      \return stack
        */
        virtual QUndoStack* getUndoStack();
    
    /*!
          \brief Method that resets the scene and calculates again.
      
      \param widthMM width of physical screen in millimeters
      \param heightMM height of physical screen in millimeters
        */
        virtual void calculateSceneMeasures(double widthMM, double heightMM);

    /*!
          \brief Method that exports all the objects in the scene to a template. Ex.: JSON.
      
      \param type type of template. Ex .: JSON type
      \param fileName Full path where the template is saved.
      
      \return true if exported, false otherwise
        */
        virtual bool exportPropertiesToTemplate(EnumType* type, std::string fileName);

    /*!
          \brief Method that imports a template and turns it into properties. Ex.: JSON.
      
      \param type type of template. Ex .: JSON type
      \param fileName Full path where the template is saved
      
      \return true list of properties, false list of empty properties
        */
        virtual std::vector<te::layout::Properties> importTemplateToProperties(EnumType* type, std::string fileName);
        
    /*!
          \brief Method that clears the scene and the stack of Undo/Redo.
        */
        virtual void reset();

        /*!
          \brief Method that delete the paper item.
        */
        virtual void deletePaperItem();

        /*!
          \brief Method that delete the paper item.
        */
        virtual QGraphicsItem* getPaperItem();
    
    /*!
          \brief Method that import a template and build all objects. Ex.: JSON.
      
      \param type type of template. Ex .: JSON type
      \param fileName Full path where the template is saved.
      
      \return true if exported, false otherwise
        */
        virtual bool buildTemplate(VisualizationArea* vzArea, EnumType* type, std::string fileName);

    /*!
          \brief Redraws the Layer of the selected map.
        */
        virtual void redrawSelectionMap();

    /*!
          \brief Saves each item in the scene as image. Ex .: .png.
      
      \param dir Full path where the images will be saved
        */
        virtual void exportItemsToImage(std::string dir);
        
    /*!
       \brief Select an item an item by name.
      
      \param name graphic object name
        */
        virtual void selectItem(std::string name);
    /*!
      \brief Select items by name.
      
      \param names vector with object names
        */
        virtual void selectItems(std::vector<std::string> names);

    /*!
      \brief Select items.
      
      \param items vector with QGraphicsItems
        */
        virtual void selectItems(QList<QGraphicsItem*> items);

   /*!
      \brief Select specified item.
      
      \param item instance of QGraphicsItem
        */
        virtual void selectItem(QGraphicsItem* item);

        virtual void redrawItems();

        /*
          \brief Add to stack that stores the items that are not entered into the scene.
          To undo/redo operations, where the item is removed from the scene, 
          a single place to store it is necessary, and if necessary, delete the items that are not entered into the scene (scene destructor).

          \param item 
        */

        virtual bool addItemStackWithoutScene(QGraphicsItem* item);

        /*
          \brief Remove from stack that stores the items that are not entered into the scene.
          To undo/redo operations, where the item is removed from the scene, 
          a single place to store it is necessary, and if necessary, delete the items that are not entered into the scene (scene destructor).

          \param item 
        */
        virtual bool removeItemStackWithoutScene(QGraphicsItem* item);

        /*!
          \brief This function is called every time the context is updated. It will sign to all items that a change in the context had ocurred.
        */
        virtual void updateAllItemsContext();

        virtual void applyPaperProportion(QSize oldPaper, QSize newPaper);

        /*!
          \brief Context between te::layout::View and te::layout::Scene: zoom, dpix, dpiy, paper config and mode.
        */
        virtual const ContextObject& getContext() const;
        
        /*!
          \brief Method that returns the object responsible for aligning objects in the scene.
      
          \return align object
        */
        virtual AlignItems* getAlignItems();

        /*!
          \brief Method that returns the first view in the list.
      
          \return view object
        */
        View* getView();
        
        virtual void deselectAllItems();

        /*!
          \brief Returns paper setting. 
      
          \return A te::layout::PaperConfig pointer      
        */
        PaperConfig* getPaperConfig();

        virtual void setEditionMode(bool editionMode);

        bool isEditionMode();

        void setContext(ContextObject context = ContextObject(0,0,0,0));

      signals:

     /*!
          \brief Issued after insertion of an item in the scene.
        */
         void addItemFinalized(QGraphicsItem* item);
    
    /*!
          \brief Issued after deleting an item in the scene.
      
      \param names names of items removed
        */
         void deleteFinalized(std::vector<std::string> names);

         /*!
          \brief Issued after leaving the edition mode
          */
         void editionFinalized();

         /*!
          \brief Issued after entering on edition mode
          */
         void editionInitialized();
         /*!
          \brief Issued when the scene context changes, like Zoom and DPI
          */
         void contextUpdated();

        
      protected:

        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void  mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
        
        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void  mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
        
        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void  mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);

        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void	keyPressEvent(QKeyEvent * keyEvent);

        /*!
        \brief Reimplemented from QGraphicsScene
        */
        virtual void drawForeground(QPainter * painter, const QRectF & rect);

    /*!
          \brief Method that calculates the transformation matrix of the scene. This matrix will be set in each QGraphicsView class that watches this scene.
        */
        virtual void calculateMatrixViewScene();

    /*!
          \brief Method that calculates the ratio of the size of the scene with the paper size. This calculation is necessary so that the paper always is centered in the scene.
      
      \param wMM width of physical screen in millimeters
      \param hMM height of physical screen in millimeters
        */
        virtual void calculateWindow(double wMM, double hMM); 

    /*!
          \brief Method that returns a list of the properties of all the graphic objects in the scene.
      
      \return list of properties
        */
        virtual std::vector<te::layout::Properties> getItemsProperties();

        virtual void applyProportionAllItems(QSize oldPaper, QSize newPaper);

        virtual void updateBoxFromProperties(te::gm::Envelope box, AbstractItemController* controller);

        virtual void changeViewMode(EnumType* mode);

        virtual bool enterEditionMode();

        virtual void leaveEditionMode();

        virtual te::gm::Envelope calculateProportion(te::gm::Envelope box, QSize oldPaper, QSize newPaper);
        
    protected:

        QTransform                         m_matrix; //!< transformation matrix of the scene.
        QColor                             m_backgroundColor; //!< background color that is applied to each observer(QGraphicsView) of the scene.
        QUndoStack*                        m_undoStack; //!< Undo/Redo stack
        int                                m_undoStackLimit; //!< Undo/Redo limit size
        AlignItems*                        m_align; //!< object responsible for aligning objects in the scene.
        bool                               m_moveWatched;
        std::map<QGraphicsItem*, QPointF>  m_moveWatches;
        QList<QGraphicsItem*>              m_itemStackWithoutScene; //!< Items that are not included in any scene 
        PaperConfig*                       m_paperConfig; //!< paper settings
        AbstractItemView*                  m_currentItemEdition;
        bool                               m_isEditionMode;
        ContextObject                      m_context;
    };
  }
}

#endif




