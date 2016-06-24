#include "svmparametersdialog.h"
#include "ui_svmparametersdialog.h"
#include <opencv2/ml.hpp>
#include <set>
SVMParametersDialog::SVMParametersDialog(QWidget *parent, SVMParameters initialParameters) :
    QDialog(parent),
    ui(new Ui::SVMParametersDialog)
{
    ui->setupUi(this);
    kernelComboBox.setComboBox((ui->KernelComboBox));
    typeComboBox.setComboBox(ui->typeComboBox);
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
    typeComboBox.add_item("C_SVC C-Support Vector Classification",SVM::C_SVC);
    typeComboBox.add_item("EPS_SVR  ϵ-Support Vector Regression",SVM::EPS_SVR);

    kernelComboBox.add_item("Linear",SVM::LINEAR);
    kernelComboBox.add_item("RBF",SVM::RBF);


}

void SVMParametersDialog::updateUI(){
    ui->CDoubleSpinBox->setValue(params.getSvm_c());
    ui->PDoubleSpinBox->setValue(params.getSvm_P());
    ui->GammaDoubleSpinBox->setValue(params.getSvm_Gamma());
    ui->KernelComboBox->setCurrentIndex(kernelComboBox.getIndex(params.getSvm_kernel()));
    ui->typeComboBox->setCurrentIndex(typeComboBox.getIndex(params.getSvm_type()));
    updateReqLabels();
}

void SVMParametersDialog::getFromUI(){
    params.setSvm_c(ui->CDoubleSpinBox->value());
    params.setSvm_P(ui->PDoubleSpinBox->value());
    params.setSvm_Gamma(ui->GammaDoubleSpinBox->value());
    params.setSvm_kernel(kernelComboBox.getSelectedValue());
    params.setSvm_type(typeComboBox.getSelectedValue());
}
bool SVMParametersDialog::reqCondition(int param){
    set<int> TypeVals;
    set<int> kernelVals;
    switch (param) {
    case SVM::C:
        TypeVals.insert({SVM::C_SVC, SVM::EPS_SVR,SVM::NU_SVR});
        return TypeVals.count(typeComboBox.getSelectedValue());
        break;
    case SVM::GAMMA:
        kernelVals.insert({SVM::POLY, SVM::RBF, SVM::SIGMOID, SVM::CHI2});
        return kernelVals.count(kernelComboBox.getSelectedValue());
        break;
    case SVM::P:
        TypeVals.insert({SVM::EPS_SVR});
        return TypeVals.count(typeComboBox.getSelectedValue());
        break;
    default:
        break;
    }
}

void SVMParametersDialog::updateReqLabels(int index){
    ui->reqC->setText(QString(" *"[reqCondition(SVM::C)]));
    ui->reqGamma->setText(QString(" *"[reqCondition(SVM::GAMMA)]));
    ui->reqP->setText(QString(" *"[reqCondition(SVM::P)]));

}


