#ifndef SVMPARAMETERSDIALOG_H
#define SVMPARAMETERSDIALOG_H
#include <QDialog>
#include <QComboBox>
#include "svmparameters.h"
#include "my_utilties.h"
using namespace std;

namespace Ui {
class SVMParametersDialog;
}

class SVMParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SVMParametersDialog(QWidget *parent = 0,SVMParameters initialParameters=SVMParameters());
    ~SVMParametersDialog();

    SVMParameters getParameters();
private:
    Ui::SVMParametersDialog *ui;
    SVMParameters params;
    void updateUI();
    void getFromUI();
    void initUI();
    MyComboBox<int> kernelComboBox;
};

#endif // SVMPARAMETERSDIALOG_H
