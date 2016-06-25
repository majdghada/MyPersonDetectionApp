#ifndef MY_UTILTIES_H
#define MY_UTILTIES_H
#include <map>
#include <QComboBox>
#include <QTime>
#include <iostream>
#include <QtGlobal>
#include <QtDebug>
using namespace std;
extern QTime myTimer;
bool isPositiveClass(double weight);
bool isNegativeClass(double weight);
void setAppTheme(QApplication * a);

template <class T>
class MyComboBox{
private:
    QComboBox * comboBox;
    map<T,int>idx;
    map<T,QString>names;
    map<int,T>inv_idx;
public :
    void setComboBox(QComboBox * comboBox);
    void add_item(QString name, T value);
    T getValue(int index);
    int getIndex(T value);
    T getSelectedValue();
    int getSelectedIndex();
    QString getName(T value);
    void setSelectedIndex(int index);

};
template <class T>
void MyComboBox<T>::setComboBox(QComboBox *comboBox)
{
    this->comboBox=comboBox;
}

template <class T>
void MyComboBox<T>::add_item(QString name,T value){
    int cur=comboBox->count();
    idx[value]=cur;
    inv_idx[cur]=value;
    names[value]=name;
    comboBox->addItem(name);
}

template <class T>
T MyComboBox<T>::getValue(int index)
{
    return inv_idx[index];
}

template <class T>
int MyComboBox<T>::getIndex(T value)
{
    return idx[value];
}

template <class T>
T MyComboBox<T>::getSelectedValue()
{
    return getValue(comboBox->currentIndex());
}

template <class T>
QString MyComboBox<T>::getName(T value)
{
    return names[value];
}

template <class T>
int MyComboBox<T>::getSelectedIndex()
{
    return comboBox->currentIndex();
}
template <class T>
void MyComboBox<T>::setSelectedIndex(int index)
{
    comboBox->setCurrentIndex(index);
}

#define m_dbg qDebug()<<QTime::currentTime().toString("hh:mm:ss.zzz a :")<< myTimer.elapsed()<<":"

#endif // MY_UTILTIES_H
