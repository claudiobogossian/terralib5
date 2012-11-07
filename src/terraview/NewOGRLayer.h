#ifndef __TERRAVIEW_NEWOGRLAYER_H
#define __TERRAVIEW_NEWOGRLAYER_H

//! Qt include files
#include <QDialog>

//! Forward declarations
namespace te
{
  namespace map
  {
    class AbstractLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class FileChooser;
    }
  }
}

class NewOGRLayer : public QDialog
{
public:

  static te::map::AbstractLayer* getNewLayer(QWidget* parent=0);

protected:

  NewOGRLayer(QWidget* parent=0);

  ~NewOGRLayer();

  void makeDialog();

  te::qt::widgets::FileChooser* m_fc;
};

#endif //__TERRAVIEW_NEWOGRLAYER_H
