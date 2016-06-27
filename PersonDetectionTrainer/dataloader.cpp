#include "dataloader.h"
#include "trainermainwindow.h"
#include "detectionwindow.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <thread>
DataLoader::DataLoader()
{

}

CenterDataLoader::CenterDataLoader( vector<Mat> &posData, vector<Mat> &negData)
{
    pos=&posData;
    neg=&negData;
    imageIdx=0;
}
bool CenterDataLoader::hasNext()
{

    return imageIdx<(int)pos->size()+(int)neg->size();
}
pair<Mat, int> CenterDataLoader::next()
{
    Mat img;
    int lbl;
    if (imageIdx<(int)pos->size()){
        img=(*pos)[imageIdx];
        lbl=1;
    }
    else {
        img=(*neg)[imageIdx-(int)pos->size()];
        lbl=-1;
    }
    imageIdx++;
    return make_pair(getCentral64x128Image(img),lbl);
}


AugmentedDataLoader::AugmentedDataLoader( vector<Mat> &posData,  vector<Mat> &negData,MyPersonDetector * originalDetector)
{

    pos=&posData;
    neg=&negData;
    imageIdx=0;
    negWindows=1;
    windowInImageidx=negWindows;
    this->originalDetector=originalDetector;

}

bool AugmentedDataLoader::hasNext()
{
    return imageIdx<(int)pos->size()+(int)neg->size()+(int)hardExamples.size()||windowInImageidx<negWindows;
}

void hardExamplesthread(MyPersonDetector * originalDetector,vector<DetectionWindow> * subWindows,vector<DetectionWindow> * hardExamples,int st,int en, Mutex *mtx){
    for (int i=st;i<en;++i){
        DetectionWindow & window=(*subWindows)[i];
        if (isPositiveClass(originalDetector->predict(window.getImageWindow()))){
            mtx->lock();
            hardExamples->push_back(window);
            mtx->unlock();
        }
    }
}

pair<Mat, int> AugmentedDataLoader::next()
{
    Mat img;
    int lbl;
    if (imageIdx<(int)pos->size()){

        img=getCentral64x128Image((*pos)[imageIdx]);
        imageIdx++;
        lbl=1;
    }
    else if (imageIdx-(int)pos->size()<(int)neg->size()||windowInImageidx!=negWindows){

        if (windowInImageidx==negWindows){
            Mat fullImg=(*neg)[imageIdx-(int)pos->size()];
            subWindows=applySlidingWindow(fullImg);
            m_dbg<<subWindows.size()<< " subwindows found by sliding window";
            int sz=subWindows.size()/8;
            std::thread * threads[8];
            Mutex  mtx;
            for (int i=0;i<8;++i){
                threads[i]=new std::thread(hardExamplesthread,originalDetector,&subWindows,&hardExamples,sz*i,(i==7)?subWindows.size():sz*(i+1),&mtx);
            }
            for (int i=0;i<8;++i){
                threads[i]->join();
                delete threads[i];
            }
//            for (DetectionWindow window:subWindows){
////                if (hardExamples.size()>=1000)break;
//                if (isPositiveClass(originalDetector->predict(window.getImageWindow()))){
//                    hardExamples.push_back(window);
//                }
//            }
            m_dbg<<hardExamples.size()<<" hard examples found , up to the "<<imageIdx<<"th image";
            distribution=uniform_int_distribution<int>(0,subWindows.size()-1);
            windowInImageidx=0;
            imageIdx++;
        }
        img=subWindows[distribution(generator)].getImageWindow();
        windowInImageidx++;
        lbl=-1;
    }
    else {

        if (hardExamples.size()>20000){
            reduceHardExamplesSize();
        }
        try{
            img=hardExamples[imageIdx-(int)pos->size()-(int)neg->size()].getImageWindow();
        }
        catch(cv::Exception e){
            m_dbg<<windowInImageidx;
            m_dbg<<imageIdx;
            m_dbg<<hardExamples.size();
            m_dbg<<pos->size();
            m_dbg<<neg->size();

            throw e;

        }

        imageIdx++;
        lbl=-1;
    }
    return make_pair(img,lbl);

}
void AugmentedDataLoader::reduceHardExamplesSize(){
    vector<DetectionWindow> res;
    distribution=uniform_int_distribution<int>(0,hardExamples.size()-1);
    for (int i=0;i<20000;++i){
        res.push_back(hardExamples[distribution(generator)]);
    }
    hardExamples=res;
}
