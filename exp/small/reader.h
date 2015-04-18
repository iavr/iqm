#ifndef __READER_H__
#define __READER_H__

#include <fstream>
#include <vector>
#include <string>

class Reader {
public:
	static void readFvecs(std::string filename, std::vector<std::vector<float> >& features);
	static void readVector(std::string filename, std::vector<std::vector<float> >& features);

};

#endif
