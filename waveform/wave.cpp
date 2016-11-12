#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
using namespace std;

#define DUMP_VAR(x) {cout << #x "=<" << x << ">" << endl;}

typedef struct {
    char riffID[4];
    unsigned int size;
    char wavID[4];
    char fmtID[4];
    unsigned int fmtSize;
    unsigned short format;
    unsigned short channels;
    unsigned int sampleRate;
    unsigned int bytePerSec;
    unsigned short blockSize;
    unsigned short bit;
    char dataID[4];
    unsigned int dataSize;
} wavHeader;

vector<vector<int16_t>> readWave(const string &path) {
    wavHeader header;
    std::ifstream ifs;
    ifs.open(path);
    if (!ifs) {
        return {{}};
    }
    ifs.read((char*)&header, sizeof(header));

    
    DUMP_VAR(header.bit);
    DUMP_VAR(header.dataSize);
    DUMP_VAR(header.channels);
    
    if(header.bit != sizeof(int16_t) * 8) {
        return {{}};
    }
    vector<vector<int16_t>> waves;
    for(int i = 0 ;i < header.channels;i++) {
        waves.push_back({});
    }
    
    do {
        int16_t buf[header.channels];
        ifs.read((char*)buf, sizeof(buf[0])*header.channels);
        for(int i = 0 ;i < header.channels;i++) {
            waves[i].push_back(buf[i]);
        }
    } while (!ifs.eof());
    for(auto & wave : waves) {
        DUMP_VAR(wave.size());
    }
    return waves;
}


