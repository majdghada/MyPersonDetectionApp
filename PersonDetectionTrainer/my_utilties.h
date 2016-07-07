#ifndef MY_UTILTIES_H
#define MY_UTILTIES_H
#include <map>
#include <QComboBox>
#include <QTime>
#include <iostream>
#include <QtGlobal>
#include <QtDebug>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/ml.hpp>
#include <set>
using namespace std;
extern QTime myTimer;
bool isPositiveClass(double weight);
bool isNegativeClass(double weight);
void setAppTheme(QApplication * a);
class MyPersonDetector;
void addNMSRects(vector<cv::Rect> rects, cv::Mat dispImg);
cv::Mat getDispImg(MyPersonDetector *detector,cv::Mat img);
cv::Mat getDispImg(cv::HOGDescriptor *hog,cv::Mat img);
void loadDetectorFromFile(MyPersonDetector &detector, string filename, QWidget *parent=0);
vector<cv::Rect>  nonMaximumSupression(vector<cv::Rect> rects, double overlapThreshold,vector<double> weights=vector<double>());
void get_svm_detector(const cv::Ptr<cv::ml::SVM>& svm, vector< float > & hog_detector );

template <typename T>
std::vector<int> compute_order(const std::vector<T>& v)
{
    std::vector<int> indices(v.size());
    std::iota(indices.begin(), indices.end(), 0u);
    std::sort(indices.begin(), indices.end(), [&](int lhs, int rhs) {
        return v[lhs] < v[rhs];
    });
    std::vector<int> res(v.size());
    for (int i = 0; i != indices.size(); ++i) {
        res[indices[i]] = i;
    }
    return res;
}
namespace ListRemover{
    template <typename T>
    void removeIndexesFromList(std::list<T> &l,std::set<int> &indexes){
        typename std::list<T>::iterator i=l.begin();
        int idx=0;
        while (i!=l.end()){
            if (indexes.count(idx)){
                l.erase(i++);
            }
            else {
                i++;
            }
            idx++;
        }
    }
    template <typename T>
    void removeElementsFromList(std::list<T> &l,std::set<T> &elements){
        typename std::list<T>::iterator i=l.begin();
        while (i!=l.end()){
            if (elements.count(*i)){
                l.erase(i++);
            }
            else {
                i++;
            }
        }
    }
}
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
