#ifndef __TERRAVIEW_NEWGDALLAYER_H
#define __TERRAVIEW_NEWGDALLAYER_H

//! Qt include files
#include <QDialog>
#include <QLineEdit>

//! Forward declarations
namespace te
{
  namespace map { class RasterLayer; }

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

class NewGDALLayer : public QDialog
{
  Q_OBJECT

  public:

    NewGDALLayer(QWidget* parent=0);

    ~NewGDALLayer();

    te::map::RasterLayer* getNewLayer();

  protected:

    std::string getDataSetRasterName(te::da::DataSource* ds, te::gm::Envelope*& extent);

    void makeDialog();

  protected slots:

    void onFileSelected(QString s);
    void setNoValue(QString s);
    void showProjDlg();
    void onOkPushButtonClicked();

protected:

    QLineEdit* m_sridLnEdt;
    QLineEdit* m_noValueLnEdt;

    std::auto_ptr<te::map::RasterLayer> m_layer;

    te::qt::widgets::SRSManagerDialog* m_srs;

    double m_noValue;
};

#endif //__TERRAVIEW_NEWGDALLAYER_H
