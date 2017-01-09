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

    DUMP_VAR(header.riffID);
    DUMP_VAR(header.size);
    DUMP_VAR(header.wavID);
    DUMP_VAR(header.fmtID);
    DUMP_VAR(header.fmtSize);
    DUMP_VAR(header.format);
    DUMP_VAR(header.channels);
    DUMP_VAR(header.sampleRate);
    DUMP_VAR(header.bytePerSec);
    DUMP_VAR(header.blockSize);
    DUMP_VAR(header.bit);
    DUMP_VAR(header.dataID);
    DUMP_VAR(header.dataSize);
    
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


#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

namespace little_endian_io
{
    template <typename Word>
    std::ostream& write_word( std::ostream& outs, Word value, unsigned size = sizeof( Word ) )
    {
        for (; size; --size, value >>= 8)
            outs.put( static_cast <char> (value & 0xFF) );
        return outs;
    }
}
using namespace little_endian_io;

void writeWave(const string &path,const deque<deque<int16_t>> &data){
    ofstream f( path, ios::binary );
    // Write the file headers
    f << "RIFF";     // (chunk size to be filled in later)
    write_word( f,     data.size()*sizeof(int16_t) + sizeof(wavHeader), 4 );
    f << "WAVEfmt ";     // (chunk size to be filled in later)
    write_word( f,     16, 4 );  // no extension data
    write_word( f,      1, 2 );  // PCM - integer samples
    write_word( f,      data.size(), 2 );  // 1 channels (stereo file)
    write_word( f,  44100, 4 );  // samples per second (Hz)
    write_word( f,  88200 * data.size(), 4 );  // (Sample Rate * BitsPerSample * Channels) / 8
    write_word( f,      2*data.size(), 2 );  // data block size (size of two integer samples, one for each channel, in bytes)
    write_word( f,     16, 2 );  // number of bits per sample (use a multiple of 8)
    
    
    int dataSize = 0;
    for (auto channel:data) {
        dataSize += channel.size()*sizeof(int16_t);
    }
    // Write the data chunk header
    f << "data";  // (chunk size to be filled in later)
    write_word( f,  dataSize, 4 );
    for(auto channel:data) {
        for(auto wave:channel) {
            write_word( f,      wave, 2 );
        }
    }
}

