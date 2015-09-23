#include <QApplication>


class TerraViewApp : public QApplication
{
public:
    TerraViewApp(int &argc, char ** argv);
    // ~TerraViewApp();
private:
    bool notify(QObject *receiver_, QEvent *event_);
};
