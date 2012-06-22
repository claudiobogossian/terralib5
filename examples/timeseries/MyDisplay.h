#ifndef __MY_DISPLAY_H_
#define __MY_DISPLAY_H_

#include "TimeSlider.h"
#include "MouseHandler.h"
#include<terralib/qt/widgets.h>
#include<terralib/maptools/DataGridOperation.h>

#include<map>

class MyLayer;
class MyDisplay : public te::qt::widgets::MapDisplay
{
  Q_OBJECT

public:
  MyDisplay(int w, int h, te::map::AbstractLayer* root, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~MyDisplay();

  void paintEvent(QPaintEvent*);
  void closeEvent(QCloseEvent*);
  void dragEnterEvent(QDragEnterEvent*);
  void dropEvent(QDropEvent*);
  void changeTree(te::map::AbstractLayer*);
  void contextMenuEvent(QContextMenuEvent*);
  void changeObjectStatus(QRect rec, const std::string& mode);
  virtual void draw();
  virtual void draw(te::map::AbstractLayer* layerTree);
  virtual void setExtent(const te::gm::Envelope& e);
  virtual void setSRID(const int& srid);
  void reorderDrawing(std::vector<te::map::AbstractLayer*>); // nao redesenha. Muda a ordem do desenho usando o conteudo do pixmap do canvas.
  void drawTemporalData(te::map::AbstractLayer* layer, std::vector<te::gm::Geometry*>& geoms, bool drawLines = false);
  void clearTemporalPixmaps(std::vector<te::map::AbstractLayer*>);
  void clearTemporalCanvas(te::map::AbstractLayer*);
  void setTimeSlider(TimeSlider*);
  void removeDrawOnlyChanged(te::map::AbstractLayer*);
  void addDrawOnlyChanged(te::map::AbstractLayer*);

public Q_SLOTS:
  void selectionChangedSlot(te::map::DataGridOperation*);
  void drawAllPointedsSlot();
  void drawAllQueriedsSlot();
  void drawAllPointedsAndQueriedsSlot();
  void mouseTooltipSlot(QPoint);
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
  void showRootFolderSlot();
  void printSlot();
  void printFileSlot();

Q_SIGNALS:
  void selectionChanged(te::map::DataGridOperation*);
  void closed(MyDisplay*);

protected:
  virtual te::qt::widgets::Canvas* getCanvas(te::map::AbstractLayer* layer);
  virtual void setCanvas(te::map::AbstractLayer* layer);
  void print(QPrinter*);

private:
  MouseHandler* m_mouseHandler;
  std::set<te::map::AbstractLayer*> m_drawOnlyChanged;
  te::map::AbstractLayer* m_rootFolderLayer;
  QMenu* m_mouseOperationMenu;
  QAction* m_mouseZoomInAction;
  QAction* m_mouseZoomOutAction;
  QAction* m_mousePanAction;
  QAction* m_mouseSelectionAction;
  QAction* m_mouseAddSelectionAction;
  QAction* m_mouseToggleSelectionAction;
  QAction* printAction;
  QAction* printFileAction;

  QRect m_rec;
  QPixmap* m_temporalVectorialDisplayPixmap;     //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible vectorial layers.
  QPixmap* m_temporalImageDisplayPixmap;         //!< This pixmap will be the result of all temporal drawing, i. e., the result of drawing all visible image layers.
  TimeSlider* m_timeSlider;
};

#endif
