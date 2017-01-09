#include <string>
#include <vector>
#include <deque>
using namespace std;
vector<vector<int16_t>> readWave(const string &path);
void writeWave(const string &path,const deque<deque<int16_t>> &data);
