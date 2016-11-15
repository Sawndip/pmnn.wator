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
const unsigned long iConstWaveRawGraphFactor = 50;
const unsigned long iConstWaveDiffGraphFactor = 10;


int main() {
    auto waves = readWave("./myRecording03.wav");
    DUMP_VAR(waves.size());
    if(waves.empty()) {
        return 0;
    }
    auto wave = waves.begin();
//	wave++;
    DUMP_VAR(wave->size());
    vector<int16_t> diffWave;
    int16_t width = std::min(iConstWaveGraphWidth,wave->size() -1);
	int16_t rawHeight = 2*(INT16_MAX/iConstWaveRawGraphFactor);
	int16_t diffHeight = 2*(INT16_MAX/iConstWaveDiffGraphFactor);
    cv::Mat mat(rawHeight + diffHeight ,width,CV_8UC3,cv::Scalar(255,255,255));
    cv::Mat white(mat);
    for(int i = 0;i < wave->size() -1;i++) {
        
        auto slip = i / iConstWaveGraphWidth;
        if(i % iConstWaveGraphWidth == 0){
            //mat = white;
            mat = cv::Scalar(255,255,255);
        }
        if(i % iConstWaveGraphWidth == iConstWaveGraphWidth-1) {
            string path = "waveform/wave.out.";
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(8) << slip;
            path += ss.str();
            path += ".png";
            cv::imwrite( path,mat);
        }
        
    
        int16_t baseLineWav =  rawHeight/2;
        int16_t baseLineDiff =  rawHeight + diffHeight/2;
        int16_t diff = wave->at(i+1) - wave->at(i);
        diffWave.push_back(diff);
        //draw waveform
    	int16_t yWave = (int16_t)wave->at(i) / (int16_t)iConstWaveRawGraphFactor + baseLineWav;
    	int16_t yDiff = (int16_t)diff/(int16_t)iConstWaveDiffGraphFactor + baseLineDiff;
        if(yWave >= rawHeight || yWave < 0) {
        	DUMP_VAR(yWave);
        	DUMP_VAR(rawHeight);
            continue;
        }
        if(yDiff >= rawHeight + diffHeight || yDiff < rawHeight) {
        	DUMP_VAR(yDiff);
        	DUMP_VAR(rawHeight);
        	DUMP_VAR(rawHeight + diffHeight);
            continue;
        }
        int x = i % iConstWaveGraphWidth;
        
    	//mat.at<cv::Vec3b>(yWave, x) = cv::Vec3b(0,0,255);
    	cv::line(mat,cv::Point(x,yWave),cv::Point(x,baseLineWav), cv::Scalar(0,0,255));
        //mat.at<cv::Vec3b>(yDiff, x) = cv::Vec3b(0,255,0);
    	cv::line(mat,cv::Point(x,yDiff),cv::Point(x,baseLineDiff), cv::Scalar(0,255,0));

        mat.at<cv::Vec3b>(baseLineWav, x) = cv::Vec3b(0,0,0);
        mat.at<cv::Vec3b>(baseLineDiff, x) = cv::Vec3b(0,0,0);
        
        
    }
    DUMP_VAR(diffWave.size());
    return 0;
}
