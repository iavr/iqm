#ifndef __LOPQ_IO_HPP__
#define __LOPQ_IO_HPP__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//---------------------------------------------------------
//loads array from binary file
template < class T >
void load_array(const std::string& bin_filename, std::vector<T>& data)
{
	std::ifstream file(bin_filename.c_str(), std::ios::binary);

	if (!(file.is_open())) {
		std::cerr << "load_array: Error: cant open binary file: " << bin_filename << std::endl;
		data.resize(0);
		return;
	}

	double elements; //no of arrays
	file.read(reinterpret_cast<char *>(&elements), sizeof(elements));

	if (elements<1){
		std::cerr << "load_array: Error: binary file: " << bin_filename << " contains "
			<< elements << " elements!" << std::endl;
		data.resize(0);
		return;
	}

	data.resize((size_t)elements);

	T *buf = new T[(size_t)elements];
	file.read(reinterpret_cast<char *>(buf), (std::streamsize)elements * sizeof(T));
	for (size_t i=0; i < (size_t)elements; i++)
		data[i] = buf[i];

	delete buf;

	file.close();
}

//---------------------------------------------------------
//loads array of arrays of equal size from binary file
template<class T>
void load_double_array(const std::string& bin_filename, std::vector< std::vector<T> >& data)
{
	std::ifstream file(bin_filename.c_str(), std::ios::binary);

	if (!(file.is_open())) {
		std::cerr << "load_double_array: Error: cant open binary file: " << bin_filename << std::endl;
		data.resize(0);
		return;
	}

	double no_of_arrays; //no of arrays
	file.read(reinterpret_cast<char *>(&no_of_arrays), sizeof(no_of_arrays));

	if (no_of_arrays<1){
		std::cerr << "load_double_array: Error: binary file: " << bin_filename << " contains "
			<< no_of_arrays << " elements!" << std::endl;
		data.resize(0);
		return;
	}

	data.resize((size_t)no_of_arrays);
	double dim; //size of each array

	for (size_t m=0; m < (size_t)no_of_arrays; m++) {
		file.read(reinterpret_cast<char *>(&dim),sizeof(dim));
		std::vector<T>& dat = data[m];
		dat.resize((size_t)dim);

		T *buf = new T[(size_t)dim];
		file.read(reinterpret_cast<char *>(buf), (std::streamsize)dim * sizeof(T));
		for (size_t i=0; i < dim; i++)
			dat[i] = buf[i];

		delete buf;
	}
	file.close();
}


//---------------------------------------------------------
//loads array of arrays of equal size from binary stream
template<class T>
void load_double_array_transposed(const std::string& bin_filename, std::vector< std::vector<T> >& data, bool print_stuff = false)
{
	std::ifstream file(bin_filename.c_str(), std::ios::binary);

	if (!(file.is_open())) {
		std::cerr << "load_double_array: Error: cant open binary file: " << bin_filename << std::endl;
		return;
	}

	double dimensions; //no of arrays
	file.read(reinterpret_cast<char *>(&dimensions), sizeof(dimensions));

	double no_of_arrays; //size of each array
	file.read(reinterpret_cast<char *>(&no_of_arrays),sizeof(no_of_arrays));

	if (print_stuff)
		std::cout << "loading...number of arrays: " << no_of_arrays << std::endl << "array dimensions: " << dimensions <<std::endl;

	data.resize((size_t)no_of_arrays, std::vector<T>(size_t(dimensions)) );
	if (print_stuff)
		std::cout << "resized...number of arrays: " << no_of_arrays << std::endl << "array dimensions: " << dimensions <<std::endl;

	for (size_t m=0; m < dimensions; m++) {

		T *buf = new T[(size_t)no_of_arrays];
		file.read(reinterpret_cast<char *>(buf), (std::streamsize)no_of_arrays * sizeof(T));

		for (size_t i=0; i < no_of_arrays; i++)
			data[i][m] = buf[i];

		delete buf;

		//! read again the num of arrays
		file.read(reinterpret_cast<char *>(&no_of_arrays),sizeof(no_of_arrays));
	}
	file.close();
	if (print_stuff)
		std::cout << "load_double_array: data loaded ok!" << std::endl;
}
//---------------------------------------------------------
//saves array/vector into binary file
template < template < typename, typename > class CONT, class T, class K >
void save_array(const CONT<T, K>& data, const std::string& bin_filename, bool print_stuff = false)
{
	double elements = data.size(); //no of arrays

	std::ofstream file(bin_filename.c_str(), std::ios::binary);

	file.write(reinterpret_cast<char *>(&elements), sizeof(elements));

	if (print_stuff)
		std::cout << "save_array: saving array, number of elements: " << elements << std::endl;

	T *buf = new T[(size_t)elements];
	for (size_t i=0; i < data.size(); i++) {
		*(buf + i) = data[i];
	}

	file.write( reinterpret_cast<char *>(buf), (std::streamsize)elements * sizeof(T));
	delete buf;

	file.close();
	if (print_stuff)
		std::cout <<"save_array: data saved ok!" << std::endl;
}

//---------------------------------------------------------
//saves array of arrays into binary file
template < 
template <typename, typename > class CONT_EX, 
template <typename, typename> class CONT_IN, 
class T, class K, class D >
	void save_double_array(const CONT_EX<CONT_IN<T, K>, D>& data, const std::string& bin_filename, bool print_stuff = false)
{
	double no_of_arrays = data.size(); //no of arrays

	std::ofstream file(bin_filename.c_str(), std::ios::binary);

	file.write(reinterpret_cast<char *>(&no_of_arrays), sizeof(no_of_arrays));

	double dim;

	if (print_stuff)
		std::cout << "save_double_array: number of arrays: " << no_of_arrays << std::endl << "array dimensions: "  ;

	for (size_t m = 0; m < no_of_arrays; m++) {
		const CONT_IN<T, K>& dat = data[m];

		dim = dat.size(); //size of each array

		if (print_stuff)
			std::cout <<  "[" << m << ":" << dim << "], ";

		file.write(reinterpret_cast<char *>(&dim),sizeof(dim));

		T *buf = new T[(size_t)dim];
		for (size_t i=0; i < dat.size(); i++) {
			*(buf + i) = dat[i];
		}

		file.write( reinterpret_cast<char *>(buf), (std::streamsize)dim * sizeof(T));
		delete buf;
	}

	file.close();
	if (print_stuff)
		std::cout <<"save_double_array: data saved ok!" << std::endl;
}

#endif //__LOPQ_IO_HPP__
