#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <iomanip>
using namespace std;

#include <opencv/highgui.h>
//#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



#define DUMP_VAR(x) {cout << #x "=<" << x << ">" << endl;}
vector<vector<int16_t>> readWave(const string &path);

const unsigned long iConstWaveGraphWidth = 3600*5;
const unsigned long iConstWaveGraphFactor = 50;

int main() {
    auto waves = readWave("./myRecording03.wav");
    DUMP_VAR(waves.size());
    if(waves.empty()) {
        return 0;
    }
    auto wave = waves.begin();
    DUMP_VAR(wave->size());
    vector<int16_t> diffWave;
    auto width = std::min(iConstWaveGraphWidth,wave->size() -1);
    cv::Mat mat(4 * INT16_MAX/iConstWaveGraphFactor,width,CV_8UC3,cv::Scalar(255,255,255));
    cv::Mat white(mat);
    for(int i = 0;i < wave->size() -1;i++) {
        
        auto slip = i / iConstWaveGraphWidth;
        if(i % iConstWaveGraphWidth == 0){
            //mat = white;
            mat = cv::Scalar(255,255,255);
        }
        
    
        int32_t baseLineWav =  INT16_MAX/iConstWaveGraphFactor;
        int32_t baseLineDiff =  3*INT16_MAX/iConstWaveGraphFactor;
        auto diff = wave->at(i+1) - wave->at(i);
        diffWave.push_back(diff);
        //draw waveform
        int yWave = (INT16_MAX + wave->at(i))/iConstWaveGraphFactor;
        int yDiff = (3*INT16_MAX + diff)/iConstWaveGraphFactor;
        if(yWave >= 2 * INT16_MAX/iConstWaveGraphFactor || yWave < 0) {
            continue;
        }
        if(yDiff >= 4 * INT16_MAX/iConstWaveGraphFactor || yDiff < 2 * INT16_MAX/iConstWaveGraphFactor) {
            continue;
        }
        int x = i % iConstWaveGraphWidth;
        
        mat.at<cv::Vec3b>(yWave, x) = cv::Vec3b(0,0,255);
        mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,255,0);

        mat.at<cv::Vec3b>(INT16_MAX/iConstWaveGraphFactor, x) = cv::Vec3b(255,0,0);
        mat.at<cv::Vec3b>(3*INT16_MAX/iConstWaveGraphFactor, x) = cv::Vec3b(255,0,0);
        
        
        if(i % iConstWaveGraphWidth == iConstWaveGraphWidth-1) {
            string path = "waveform/wave.out.";
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(8) << slip;
            path += ss.str();
            path += ".png";
            cv::imwrite( path,mat);
        }
    }
    DUMP_VAR(diffWave.size());
    return 0;
}
