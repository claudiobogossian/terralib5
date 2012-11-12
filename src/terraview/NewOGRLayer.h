#ifndef __TERRAVIEW_NEWOGRLAYER_H
#define __TERRAVIEW_NEWOGRLAYER_H

//! Qt include files
#include <QDialog>
#include <QLineEdit>

//! Forward declarations
namespace te
{
  namespace map { class Layer; }

  namespace da { class DataSource; }

  namespace gm { class Envelope; }

  namespace qt
  {
    namespace widgets
    {
      class SRSManagerDialog;
    }
  }
}

class NewOGRLayer : public QDialog
{
  Q_OBJECT

  public:

   NewOGRLayer(QWidget* parent=0);

  ~NewOGRLayer();

  te::map::Layer* getNewLayer();

  protected:

    std::string getDataSetName(te::da::DataSource* ds, te::gm::Envelope*& box);

    void makeDialog();

  protected slots:

    void onFileSelected(QString s);

    void showProjDlg();

  protected:

    std::auto_ptr<te::map::Layer> m_layer;

    QLineEdit* m_sridLnEdt;

    te::qt::widgets::SRSManagerDialog* m_srs;
};

#endif //__TERRAVIEW_NEWOGRLAYER_H
