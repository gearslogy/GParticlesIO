#ifndef G_PROC_ASS_INFO_H_
#define G_PROC_ASS_INFO_H_
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

class g_particle_mataAss
{
public:
	g_particle_mataAss();
	~g_particle_mataAss();
	void setBBOX(float min_x,float min_y,float min_z,float max_x,float max_y,float max_z);
	void setGioCachePath(string path){gio_cache_path=path;}
	void inserExtraMataInfo(string attName){extraInfoMat.push_back(attName);}
	void setDsoPath(string dso_path){dso_PATH=dso_path;}
public:
	void save(ofstream  &fout);
private:
	string dso_PATH;    //
	vector <float> infoBBox;        //BBOX
	string gio_cache_path;         //GP cache path
	vector <string> extraInfoMat;  // extra for USERINTERFACE VIEW
//ass bbox

};
#endif