#include "G_Particles_IO.h"
#include <omp.h>
#define ATT_LENGTH 20;
g_particles_io::g_particles_io()
{
	name ="fuck ok";
	
}

g_particles_io::~g_particles_io()
{
	
}

// int method settings
void g_particles_io::GIO_setINTAttributeList( int arr[],int arraySize )
{
	per_int_attList.clear();
	per_int_attList.resize(arraySize);
	#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_int_attList[i]=arr[i];
	}
}

void g_particles_io::GIO_setINTAttributeList( vector <int> &intAttList )
{
	per_int_attList.clear();
	per_int_attList=intAttList;
}

void g_particles_io::GIO_setINTAttributeList( int value )
{
	per_int_attList.clear();
	per_int_attList.push_back(value);
}
// float settings
void g_particles_io::GIO_setFLTAttributeList( float arr[],int arraySize )
{
	per_float_attList.clear();
	per_float_attList.resize(arraySize);
	#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_float_attList[i]=arr[i];
	}
}

void g_particles_io::GIO_setFLTAttributeList( vector <float> &intAttList )
{
	per_float_attList.clear();
	
	per_float_attList=intAttList;
}

void g_particles_io::GIO_setFLTAttributeList( float value )
{
	per_float_attList.clear();
	per_float_attList.push_back(value);
}

void g_particles_io::GIO_setINTVecAttributeList( int x,int y,int z )
{
	per_int_vector per_int;
	per_int.g_x=x;
	per_int.g_y=y;
	per_int.g_z=z;
	per_int_vec_attList.push_back(per_int);
}

void g_particles_io::GIO_setINTVecAttributeList( vector<per_int_vector> intVecAttList )
{
	per_int_vec_attList.clear();
	int arraySize=  intVecAttList.size();
	per_int_vec_attList.resize(arraySize);

	#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_int_vec_attList[i]=intVecAttList[i];
	}
}

void g_particles_io::GIO_setINTVecAttributeList( per_int_vector arr[],int arraySize )
{
	per_int_vec_attList.clear();
	per_int_vec_attList.resize(arraySize);
	#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_int_vec_attList[i]=arr[i];
	}
}
// float vector settings
void g_particles_io::GIO_setFLTVecAttributeList( float x,float y,float z )
{
	per_float_vector per_flt;
	per_flt.g_x=x;
	per_flt.g_y=y;
	per_flt.g_z=z;
	per_float_vec_attList.push_back(per_flt);
}

void g_particles_io::GIO_setFLTVecAttributeList( vector<per_float_vector> fltVecAttList )
{
	per_float_vec_attList.clear();
	int arraySize=  fltVecAttList.size();
	per_float_vec_attList.resize(arraySize);

#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_float_vec_attList[i]=fltVecAttList[i];
	}
}

void g_particles_io::GIO_setFLTVecAttributeList( per_float_vector arr[],int arraySize )
{
	per_float_vec_attList.clear();
	per_float_vec_attList.resize(arraySize);
	#pragma omp parallel for
	for(int i=0;i<arraySize;i++)
	{
		per_float_vec_attList[i]=arr[i];
	}
}

void g_particles_io::saveData( ofstream &fout )
{
	//save type and save size
	//save size into the fork funcion
	//save data
	//save name
	

	fout.write((char*)&type,sizeof(type));  // write type
	
	switch (type)
	{
	case attType::INT_TYPE:
		//cout<<"writing the int type attribute\n";
		writeINT_AttListData(fout);
		break;
	case attType::FLT_TYPE:
		//cout<<"writing the flt type attribute\n";
		writeFLT_AttListData(fout);
		break;
	case attType::INT_VEC_TYPE:
		//cout<<"writing the INT_VEC_TYPE type attribute\n";
		writeINT_VecAttListData(fout);
		break;
	case attType::FLT_VEC_TYPE:
		//cout<<"writing the FLT_VEC_TYPE type attribute\n";
		writeFLT_VecAttListData(fout);
		break;
	default:
		break;
	}
	// next to save name
	// and save the text length
	
	//1->save text length ,2->save the attribute name


	char w_name[40];   //sizeof(w_name) load the 40 ,very important
	strcpy(w_name,name.c_str());
	
	int text_length = strlen(w_name);  // str length for read used
	//std::cout<<"->->->->->write the attribute "<<w_name<<" length is "<<text_length<<std::endl;
	fout.write((char*)&text_length,sizeof(int));// write the attribute name length to disk
	fout.write((char*)&w_name,sizeof(w_name));  // write the attribute name to disk
	
}

void g_particles_io::loadData( ifstream &fin )
{
	//read type
	//std::cout<<"enter the g_particles_io class io loading ......\n";0
	
	attType data_type;
	fin.read((char*)&data_type,sizeof(data_type));
	type=data_type;
	///read size and read data
	//std::cout<<"the type id is : "<<data_type<<std::endl;
	
	switch (data_type)
	{
	case attType::INT_TYPE:
		//std::cout<<"loading the g_particles_io class INT ......\n";
		loadINT_AttListData(fin);
		break;
	case attType::FLT_TYPE:
		//std::cout<<"loading the g_particles_io class FLT ......\n";
		loadFLT_AttListData(fin);
		break;
	case attType::INT_VEC_TYPE:
		//std::cout<<"loading the g_particles_io class INT_VEC_TYPE ......\n";
		loadINT_VecAttListData(fin);
		break;
	case attType::FLT_VEC_TYPE:
		//std::cout<<"loading the g_particles_io class FLT_VEC_TYPE ......\n";
		loadFLT_VecAttListData(fin);
		break;
	default:
		break;
	}
	
	//read the text length 
	int text_length;
	fin.read((char*)&text_length,sizeof(int));
	//std::cout<<"read the attribute "<<" length is "<<text_length<<std::endl;
	char attName[50]={"\0"};
	fin.read((char*)&attName,text_length);

	
	name = string(attName);
	//std::cout<<"read the attribute name is"<<name<<std::endl;
	//std::cout<<"enter the g_particles_io class io loading ending ......\n";
	
}

void g_particles_io::writeINT_AttListData( ofstream &fout )
{
	int size=per_int_attList.size();
	fout.write((char*)&size,sizeof(int));
	for(int i=0;i<size;i++)
	{
		fout.write((char*)&per_int_attList[i],sizeof(per_int_attList[i]));
	}
	
}

void g_particles_io::writeFLT_AttListData( ofstream &fout )
{
	int size=per_float_attList.size();
	fout.write((char*)&size,sizeof(int));
	for(int i=0;i<size;i++)
	{
		fout.write((char*)&per_float_attList[i],sizeof(per_float_attList[i]));
	}

}

void g_particles_io::writeINT_VecAttListData( ofstream &fout )
{
	int size=per_int_vec_attList.size();
	fout.write((char*)&size,sizeof(int));
	for(int i=0;i<size;i++)
	{
		fout.write((char*)&per_int_vec_attList[i],sizeof(per_int_vec_attList[i]));
	}
	
}

void g_particles_io::writeFLT_VecAttListData( ofstream &fout )
{

	

	int size=per_float_vec_attList.size();
	
	fout.write((char*)&size,sizeof(int));
	for(int i=0;i<size;i++)
	{
		//std::cout<<"every pt num is"<<per_float_vec_attList[i].g_x<<" "<<per_float_vec_attList[i].g_y<<" "<<per_float_vec_attList[i].g_z<<std::endl;
		per_float_vector pt=per_float_vec_attList[i];
		fout.write((char*)&pt,sizeof(pt));
	}
	

	
}

void g_particles_io::loadINT_AttListData( ifstream &fin )
{
	int size;
	fin.read((char*)&size,sizeof(int));
	per_int_attList.clear();
	per_int_attList.resize(size);

	//create temp stack
	vector <int> temp_per_int_attList;
	
	for(int i=0;i<size;i++)
	{
		int value;
		fin.read((char*)&value,sizeof(value));
		temp_per_int_attList.push_back(value);
	}
	GIO_setINTAttributeList(temp_per_int_attList);
	
}

void g_particles_io::loadFLT_AttListData( ifstream &fin )
{
	int size;
	fin.read((char*)&size,sizeof(int));
	per_float_attList.clear();
	per_float_attList.resize(size);

	//create temp stack
	vector <float> temp_per_int_attList;

	for(int i=0;i<size;i++)
	{
		float value;
		fin.read((char*)&value,sizeof(value));
		temp_per_int_attList.push_back(value);
	}
	GIO_setFLTAttributeList(temp_per_int_attList);
	
	
}

void g_particles_io::loadINT_VecAttListData( ifstream &fin )
{
	int size;
	fin.read((char*)&size,sizeof(int));

	per_int_vec_attList.clear();
	per_int_vec_attList.resize(size);

	//create temp stack
	vector <per_int_vector> temp_per_int_vec_attList;
	for(int i=0;i<size;i++)
	{
		per_int_vector pt_int;
		fin.read((char*)&pt_int,sizeof(pt_int));

		temp_per_int_vec_attList.push_back(pt_int);
	}
	GIO_setINTVecAttributeList(temp_per_int_vec_attList);
	
}

void g_particles_io::loadFLT_VecAttListData( ifstream &fin )
{
	

	int size;
	fin.read((char*)&size,sizeof(int));

	per_int_vec_attList.clear();
	per_int_vec_attList.resize(size);

	//create temp stack
	vector <per_float_vector> temp_per_flt_vec_attList;
	for(int i=0;i<size;i++)
	{
		per_float_vector pt_int;
		fin.read((char*)&pt_int,sizeof(pt_int));
		//std::cout<<"get x y z"<<pt_int.g_x<<" "<<pt_int.g_y<<" "<<pt_int.g_z<<std::endl;
		temp_per_flt_vec_attList.push_back(pt_int);
	}
	GIO_setFLTVecAttributeList(temp_per_flt_vec_attList);

	
}
