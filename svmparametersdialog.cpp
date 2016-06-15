#include "svmparametersdialog.h"
#include "ui_svmparametersdialog.h"

SVMParametersDialog::SVMParametersDialog(QWidget *parent, SVMParameters initialParameters) :
    QDialog(parent),
    ui(new Ui::SVMParametersDialog)
{
    ui->setupUi(this);
    kernelComboBox.setComboBox((ui->KernelComboBox));
    initUI();
    params=initialParameters;
    updateUI();
}

SVMParametersDialog::~SVMParametersDialog()
{
    delete ui;
}

SVMParameters SVMParametersDialog::getParameters()
{
    getFromUI();
    return params;
}

void SVMParametersDialog::initUI(){
    kernelComboBox.add_item("Linear",SVM::LINEAR);
    kernelComboBox.add_item("RBF",SVM::RBF);
}

void SVMParametersDialog::updateUI(){
    ui->CDoubleSpinBox->setValue(params.getSvm_c());
    ui->GammaDoubleSpinBox->setValue(params.getSvm_Gamma());
    ui->KernelComboBox->setCurrentIndex(kernelComboBox.getIndex(params.getSvm_kernel()));
}

void SVMParametersDialog::getFromUI(){
    params.setSvm_c(ui->CDoubleSpinBox->value());
    params.setSvm_Gamma(ui->GammaDoubleSpinBox->value());
    params.setSvm_kernel(kernelComboBox.getSelectedValue());
}

