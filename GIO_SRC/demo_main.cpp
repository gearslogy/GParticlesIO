#include "G_Particle_type.h"
#include "G_Particles_IO.h"
#include "G_Global_IO.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <omp.h>
using namespace std;
char *path = "c:/mydata.st";

void writeList()
{
	ofstream fout;
	fout.open(path,ios_base::binary);
	// first 
	
	char *attname="houdini";
	g_particles_io gparticles_int;
	gparticles_int.GIO_SetCurrentAttributeName(attname);
	gparticles_int.GIO_SetCurrentAttributeType(g_particles_io::INT_TYPE);
	vector <int> arrays;
	arrays.clear();
	arrays.resize(10000000);
	for(int i=0;i<10000000;i++)
	{
		arrays[i]=i;
	}
	gparticles_int.GIO_setINTAttributeList(arrays);
	

	// first add new int 
	char *attname_soft="realflow";
	g_particles_io gparticles_int_2;
	gparticles_int_2.GIO_SetCurrentAttributeName(attname_soft);
	gparticles_int_2.GIO_SetCurrentAttributeType(g_particles_io::INT_TYPE);
	vector <int> arrays_2;
	arrays_2.clear();
	arrays_2.resize(4);
	for(int i=0;i<4;i++)
	{
		arrays_2[i]=i;
	}
	gparticles_int_2.GIO_setINTAttributeList(arrays_2);

	//secend
	char *attname2="maya";
	g_particles_io gparticles_vec;
	gparticles_vec.GIO_SetCurrentAttributeName(attname2);
	gparticles_vec.GIO_SetCurrentAttributeType(g_particles_io::INT_VEC_TYPE);
	vector <per_int_vector> arrays_int_vec;
	arrays_int_vec.clear();
	arrays_int_vec.resize(4);

	for(int i=0;i<4;i++)
	{
		per_int_vector intVec;
		intVec.g_x=i;
		intVec.g_y=i;
		intVec.g_y=i;
		arrays_int_vec[i]=intVec;
	}
	gparticles_vec.GIO_setINTVecAttributeList(arrays_int_vec);

	// write to disk
	g_global_io global_io;
	//global_io.GIO_installParticleHandle(gparticles_int);
	global_io.GIO_installParticleHandle(gparticles_vec);
	global_io.GIO_installParticleHandle(gparticles_int_2);   // "realflow attribute"
	global_io.GIO_installParticleHandle(gparticles_int);  //"houdini attribute"
	
	clock_t start,finish;
	start=clock();
	global_io.save(fout);
	finish=clock();
	float final_time = (finish-start)/CLOCKS_PER_SEC;
	cout <<"write time is : " <<final_time<< " (s) "<< endl;
	fout.close();

}

void readList()
{
	ifstream fin;
	fin.open(path,ios_base::binary);
	fin.clear();
	clock_t start,finish;
	start=clock();

	g_global_io io;
	io.load(fin,path);
	std::cout<<"get the num attribute is"<<io.GIO_getNumAttributes()<<std::endl;



	finish=clock();
	float final_time = (finish-start)/CLOCKS_PER_SEC;
	cout << "read time is : "<<final_time<< " (s) "<< endl;
	fin.close();
}

int main()
{


    writeList();
	readList();
	std::cout<<"read end\n";
	cin.get();
	return 0;
}