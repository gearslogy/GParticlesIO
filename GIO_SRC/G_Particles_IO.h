#ifndef G_PARTICLES_IO
#define G_PARTICLES_IO

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include "G_Particle_type.h"
#include <string>
using namespace  std;
class g_particles_io
{
public:
	enum attType{INT_TYPE=900000000,FLT_TYPE=900000001,INT_VEC_TYPE=900000002,FLT_VEC_TYPE=900000003};
	g_particles_io();
	~g_particles_io();
	//set and get
	//get
	inline string GIO_getCurrentAttributeName(){return name;}
	inline vector<int> GIO_getINTAttributeList(){return per_int_attList;} //get int attributeList
	inline vector<float> GIO_getFLTAttributeList(){return per_float_attList;} //get float attributeList
	inline vector<per_int_vector> GIO_getINTVecAttributeList(){return per_int_vec_attList;} //get int vec attributeList
	inline vector<per_float_vector> GIO_getFLTVecAttributeList(){return per_float_vec_attList;} //get float vec attributeList
	inline attType GIO_getAttributeType(){return type;} //get float vec attributeList
	//set
	inline void GIO_SetCurrentAttributeName(char *attName)
	{
		name=attName;
	}
	inline void GIO_SetCurrentAttributeType(attType attribType){type = attribType;}
	// int attribute list set
	void GIO_setINTAttributeList(int arr[],int arraySize); // arr is a list
	void GIO_setINTAttributeList(vector <int> &intAttList);
	void GIO_setINTAttributeList(int value); // this method very slow
	// float attribute list set
	void GIO_setFLTAttributeList(float arr[],int arraySize); // arr is a list
	void GIO_setFLTAttributeList(vector <float> &floatAttList);
	void GIO_setFLTAttributeList(float value); // this method very slow
	// int vec attribute list set
	void GIO_setINTVecAttributeList(int x,int y,int z);// this method very slow
	void GIO_setINTVecAttributeList(vector<per_int_vector> intVecAttList);
	void GIO_setINTVecAttributeList(per_int_vector arr[],int arraySize);
	//flt vec attribute list set
	void GIO_setFLTVecAttributeList(float x,float y,float z);// this method very slow
	void GIO_setFLTVecAttributeList(vector<per_float_vector> fltVecAttList);
	void GIO_setFLTVecAttributeList(per_float_vector arr[],int arraySize);
	//save and load
	void saveData(ofstream &fout);
	void loadData(ifstream &fin);
private:
	attType type;
	string name;

	vector <int> per_int_attList;
	vector <float> per_float_attList;
	vector <per_int_vector> per_int_vec_attList;
	vector <per_float_vector> per_float_vec_attList;

	//write
	void writeINT_AttListData(ofstream &fout);
	void writeFLT_AttListData(ofstream &fout);
	void writeINT_VecAttListData(ofstream &fout);
	void writeFLT_VecAttListData(ofstream &fout);
	//load
	void loadINT_AttListData(ifstream &fin);
	void loadFLT_AttListData(ifstream &fin);
	void loadINT_VecAttListData(ifstream &fin);
	void loadFLT_VecAttListData(ifstream &fin);


};
#endif