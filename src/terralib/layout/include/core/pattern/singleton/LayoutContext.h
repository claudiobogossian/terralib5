#ifndef LAYOUTCONTEXT_H
#define LAYOUTCONTEXT_H

#include "LayoutMode.h"
#include "LayoutAbstractType.h"
#include "LayoutUtils.h"
#include "LayoutAbstractOutsideFactory.h"

#include "../../../../../maptools/Canvas.h"
#include "../../../../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    class LayoutScene;
    class LayoutAbstractItemFactory;
    class LayoutAbstractOutsideFactory;

    class LayoutContext 
    {
      private:
        LayoutContext() :
          _mode(TypeNone),
          _scene(0),
          _canvas(0),
          _zoomFactor(1.),
          _itemFactory(0),
          _outsideFactory(0),
          _utils(0)
        {
        };  // Private so that it can  not be called	
        LayoutContext(LayoutContext const&){};             // copy constructor is private	
        LayoutContext& operator=(LayoutContext const&){};  // assignment operator is private	
        static LayoutContext* _instance;

      public:
        virtual ~LayoutContext(){};
        static LayoutContext* getInstance();

        LayoutMode	 getMode();
        LayoutScene* getScene();
        void setMode(LayoutMode mode);
        void setScene(LayoutScene* scene);

        double getZoomFactor();
        void  setZoomFactor(double zoomFactor);
      
        LayoutAbstractItemFactory* getItemFactory();
        void setItemFactory(LayoutAbstractItemFactory* factory);

        LayoutAbstractOutsideFactory* getOutsideFactory();
        void setOutsideFactory(LayoutAbstractOutsideFactory* factory);
        
        te::gm::Envelope* getBoxPaper();
        void setBoxPaper(te::gm::Envelope* box);

        LayoutAbstractPaperType getTypePaper();
        void setTypePaper(LayoutAbstractPaperType type);

        te::map::Canvas* getCanvas();
        void setCanvas(te::map::Canvas* canvas);

        LayoutUtils* getUtils();
        void setUtils(LayoutUtils* utils);

      protected:
        LayoutMode						          _mode;
        LayoutScene*					          _scene;
        double							            _zoomFactor;
        te::map::Canvas*	        			_canvas;
        LayoutAbstractItemFactory*		  _itemFactory;
        LayoutAbstractOutsideFactory*	  _outsideFactory;
        te::gm::Envelope*		            _paperBox; //--->>>>>>>>>>>> Change for just object, no pointer!
        LayoutAbstractPaperType			    _typePaper;
        LayoutUtils*                    _utils;
    };
  }
}

#endif