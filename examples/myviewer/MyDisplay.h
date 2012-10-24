#ifndef __MY_DISPLAY_H_
#define __MY_DISPLAY_H_

#include "TimeSlider.h"
#include "MouseHandler.h"
#include<terralib/geometry/Envelope.h>
#include<terralib/qt/widgets.h>
#include<terralib/maptools/DataGridOperation.h>

//QT
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLineEdit>

#include<map>

class MyLayer;
class MyDisplay : public te::qt::widgets::MapDisplay
{
  Q_OBJECT

public:
  MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~MyDisplay();

  void paintEvent(QPaintEvent*);
  void resizeEvent(QResizeEvent*);
  void closeEvent(QCloseEvent*);
  void dragEnterEvent(QDragEnterEvent*);
  void dropEvent(QDropEvent*);
  void setLayerTree(te::map::AbstractLayer* layer);
  te::gm::Envelope MyDisplay::getLayerExtent(te::map::AbstractLayer* al);
  void changeTree(te::map::AbstractLayer*);
  void contextMenuEvent(QContextMenuEvent*);
  void changeObjectStatus(QRect rec, const std::string& mode);
  virtual void draw();
  void draw(std::list<te::map::AbstractLayer*>& layerList);
  virtual void draw(te::map::AbstractLayer* layerTree);
  void setExtent();

  /*!            
    \brief Recalculates the extension of all visible layers.

    \return The envelope of all visible layers.
  */
  te::gm::Envelope getAllExtent();

  /*!            
    \brief Recalculates the extension of all visible layers.

    \layerList List of layers to be calculated.

    \return The envelope of all visible layers.
  */
  te::gm::Envelope getAllExtent(std::list<te::map::AbstractLayer*>& layerList);

  virtual void setSRID(const int& srid);
  void reorderDrawing(std::vector<te::map::AbstractLayer*>); // nao redesenha. Muda a ordem do desenho usando o conteudo do pixmap do canvas.
  void drawTemporalData(te::map::AbstractLayer* layer, std::vector<te::gm::Geometry*>& geoms, bool drawLines = false);
  void clearTemporalPixmaps(std::vector<te::map::AbstractLayer*>);
  void clearTemporalCanvas(te::map::AbstractLayer*);
  void removeDrawOnlyChanged(te::map::AbstractLayer*);
  void addDrawOnlyChanged(te::map::AbstractLayer*);
  void getLayerList(te::map::AbstractLayer* al, std::list<te::map::AbstractLayer*>& layerList);
  void removeAllTemporalLayers();
  void addTemporalLayer(te::map::AbstractLayer*);
  void setTitle(QString&);
  QWidget* getWidget();
  void setTimeSliderIcon(QPixmap*);
  void clearTimeLineEdit();
  void showOrHideTimeSlider(std::list<te::map::AbstractLayer*>& layerList);
  void setRepaint(bool s);
  te::map::AbstractLayer* getLayerTree();
  void fit(std::list<te::map::AbstractLayer*>& layerList);
  bool transform(te::gm::Envelope& e, int oldsrid, int newsrid);

public Q_SLOTS:
  void selectionChangedSlot(te::map::DataGridOperation*);
  void drawAllPointedsSlot();
  void drawAllQueriedsSlot();
  void drawAllPointedsAndQueriedsSlot();
  void mouseTooltipSlot(QPoint);
  void mouseShowCoordSlot(QPoint);
  /*!            
    \brief Executes zoom area.

    \Param rec The area to be magnified.
  */
  void execZoomAreaSlot(const QRect& rec);

  /*!            
    \brief Executes zoom in from point.

    \Param p The central point of the zoom in 
  */
  void execZoomInSlot(const QPoint& p);
 
  /*!            
    \brief Executes zoom out from point.

    \Param p The central point of the zoom out.
  */
  void execZoomOutSlot(const QPoint& p);
 
  /*!            
    \brief Executes pan.

    \Param from The initial point of the pan. 
    \Param to The final point of the pan. 
  */
  void execPanSlot(const QPoint& from, const QPoint& to);

  /*!            
    \brief Sets mouse operation to zoom in.
  */
  void setMouseOperationToZoomInSlot();

  /*!            
    \brief Sets mouse operation to zoom out.
  */
  void setMouseOperationToZoomOutSlot();

  /*!            
    \brief Sets mouse operation to pan.
  */
  void setMouseOperationToPanSlot();
  void mouseSelectionSlot(QRect);
  void mouseAddSelectionSlot(QRect);
  void mouseToggleSelectionSlot(QRect);
  void setMouseOperationToObjectSelectionSlot();
  void setMouseOperationToAddObjectSelectionSlot();
  void setMouseOperationToToggleObjectSelectionSlot();
  void setMouseOperationToTooltipSlot();
  void setMouseOperationToShowCoordSlot();
  //void clearTooltipPixmap();
  void showRootFolderSlot();
  void initTemporal();
  void printSlot();
  void printFileSlot();
  void timeSliderContextMenuSlot(const QPoint&);
  void configTemporalPlaySlot();
  void showTimeLineEditSlot();
  void fitAllLayersSlot();
  void setSRIDSlot();
  void onResizeTimeout();

Q_SIGNALS:
  void selectionChanged(te::map::DataGridOperation*);
  void closed(MyDisplay*);
  void sizeChanged(QSize s);

protected:
  virtual te::qt::widgets::Canvas* getCanvas(te::map::AbstractLayer* layer);
  virtual void setCanvas(te::map::AbstractLayer* layer);
  void print(QPrinter*);

private:
  te::gm::Envelope m_envelope;      //!< Envelope.
  MouseHandler* m_mouseHandler;
  std::set<te::map::AbstractLayer*> m_drawOnlyChanged;
  te::map::AbstractLayer* m_rootFolderLayer;
  te::map::AbstractLayer* m_layerTree;        //!< The layer to be displayed.
  bool m_resize;                    //!< flag to draw with zoom due to the resize in progress
  QRect m_resizeRec;                //!< rectangle used for display resize
  QRect m_resizeWRec;               //!< rectangle used for display resize
  bool m_repaint;                   //!< repaint control.
  QVBoxLayout* m_layout;
  QWidget* m_widget;
  QGroupBox* m_timeGroupBox;
  QMenu* m_menu;                    //!< Menu.
  QMenu* m_timeSliderMenu;
  QAction* m_showTimeLineEditAction;
  QMenu* m_mouseOperationMenu;
  QAction* m_mouseZoomInAction;
  QAction* m_mouseZoomOutAction;
  QAction* m_mousePanAction;
  QAction* m_mouseSelectionAction;
  QAction* m_mouseAddSelectionAction;
  QAction* m_mouseToggleSelectionAction;
  QAction* m_mouseTooltipAction;
  QAction* m_mouseShowCoordAction;
  QAction* printAction;
  QAction* printFileAction;
  QAction* m_fitAllLayersAction;    //!< Action to fit all layers.
  QAction* m_srsAction;             //!< Action to change srid.

  //QPixmap* m_tooltipDisplayPixmap;               //!< tootip display pixmap
  QRect    m_tooltipRect;                        //!< tootip rect

  QPixmap* m_temporalVectorialDisplayPixmap;     //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible vectorial layers.
  QPixmap* m_temporalImageDisplayPixmap;         //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible image layers.
  QPushButton* m_playPauseButton;
  TimeSlider* m_timeSlider;
  QLineEdit* m_timeLineEdit;
  QHBoxLayout* m_timeLayout;
};

#endif
