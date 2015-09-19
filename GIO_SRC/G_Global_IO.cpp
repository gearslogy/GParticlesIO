#include "G_Global_IO.h"
#include "G_Particles_IO.h"
#include <ctime>
g_global_io::g_global_io()
{
	ptr=0;
	
}

g_global_io::~g_global_io()
{

}

void g_global_io::GIO_installParticleHandle( g_particles_io &particles_ios )
{
	ptr+=1;
	particles_handle.push_back(particles_ios);
	handle_ptr.push_back(ptr);

	g_particles_io::attType current_type=particles_ios.GIO_getAttributeType();
	switch (current_type)
	{
	case g_particles_io::INT_TYPE:
		_int_particles_handle.push_back(particles_ios);
		break;
	case g_particles_io::FLT_TYPE:
		_flt_particles_handle.push_back(particles_ios);
		break;
	case g_particles_io::INT_VEC_TYPE:
		_int_vec_particles_handle.push_back(particles_ios);
		break;
	case g_particles_io::FLT_VEC_TYPE:
		_flt_vec_particles_handle.push_back(particles_ios);
		break;
	default:
		break;
	}
	//std::cout<<"current put the attribute had "<<this->GIO_getNumAttributes()<<std::endl;
}

void g_global_io::save(ofstream &fout)
{
	// for each attribute writing into the disk
	for(int i =0;i<handle_ptr.size();i++)
	{
		std::cout<<"writing the  "<<i+1 << " attribute ,the name is "<<particles_handle[i].GIO_getCurrentAttributeName()<<" "<<std::endl;
		particles_handle[i].saveData(fout);  //write every attribute data
	}
}

void g_global_io::load(ifstream &fin,char*loadDataPath)
{

	// this part will write streampos
	clock_t start,finish;
	start=clock();
	fin.clear();
	int curattType;
	
	while(fin.read((char*)&curattType,sizeof(int)))
	{
		//std::cout<<"all current  stream function is"<<fin.tellg()<<std::endl;
		if(curattType == g_particles_io::INT_TYPE)
		{
			//std::cout<<"Read Streams------current INT stream function is"<<fin.tellg()<<std::endl;
			int t_pos=fin.tellg();
			streampos pos=t_pos-4;
			int_type_streamPos.push_back(pos);
			//std::cout<<" load int type att "<< ",stream pos is"<<pos<<std::endl;
			
		}
		
		if(curattType==g_particles_io::FLT_TYPE)
		{
			//std::cout<<" Read Streams------load flt type att\n";
			int t_pos=fin.tellg();
			streampos pos=t_pos-4;
			flt_type_streamPos.push_back(pos);
		}
	
		if(curattType==g_particles_io::INT_VEC_TYPE)
		{
			//std::cout<<"Read Streams------current INT_VEC_TYPE stream function is"<<fin.tellg()<<std::endl;
			int t_pos=fin.tellg();
			streampos pos=t_pos-4;
			//std::cout<<" load int vec type att"<< ",stream pos is"<<pos<<std::endl;
			int_vec_type_streamPos.push_back(pos);
		}

		if(curattType==g_particles_io::FLT_VEC_TYPE)
		{
			//std::cout<<"Read Streams------current FLT_VECT_TYPE stream function is"<<fin.tellg()<<std::endl;
			int t_pos=fin.tellg();
			streampos pos=t_pos-4;
			flt_vec_type_streamPos.push_back(pos);
		}
		//std::cout<<"read type is"<<curattType<<std::endl;

	}
	finish=clock();
	float final_time = (finish-start)/CLOCKS_PER_SEC;
	//cout <<"read stream time is : " <<final_time<< " (s) "<< endl;


	// process for the int attribute list
	

	//SIMD read int attribute
	int get_int_size = int_type_streamPos.size();
	_int_particles_handle.resize(get_int_size);
	#pragma omp parallel for
	for(int i=0;i<get_int_size;i++)
	{
		
		ifstream test;
		test.open(loadDataPath,ios_base::binary);
		test.clear();
		test.seekg(int_type_streamPos[i]);
		g_particles_io io_temp;
		io_temp.loadData(test);
		std::cout<<"global io read the int attribute name is "<<io_temp.GIO_getCurrentAttributeName()<<"\n";
		_int_particles_handle[i]=io_temp;
	}



	/*  this method is sisd....
	for(int i=0;i<int_type_streamPos.size();i++)
	{
		std::cout<<"read the int attribute\n";
		fin.clear();
		fin.seekg(int_type_streamPos[i]);   
		g_particles_io io_temp;
		io_temp.loadData(fin);
		particles_handle.push_back(io_temp);
		_int_particles_handle.push_back(io_temp);
	}
	*/
	
	
	//process for the float attribute list
	int get_flt_size = flt_type_streamPos.size();
	_flt_particles_handle.resize(get_flt_size);
	#pragma omp parallel for
	for(int i=0;i<get_flt_size;i++)
	{
		std::cout<<"read the flt attribute\n";
		ifstream test;
		test.open(loadDataPath,ios_base::binary);
		test.clear();
		test.seekg(flt_type_streamPos[i]);
		g_particles_io io_temp;
		io_temp.loadData(test);
		_flt_particles_handle[i]=io_temp;
		std::cout<<"global io read the int attribute name is "<<io_temp.GIO_getCurrentAttributeName()<<"\n";
	}


	
	// process for the int vector attribute list/
	int get_int_vec_size = int_vec_type_streamPos.size();
	_int_vec_particles_handle.resize(get_int_vec_size);
	#pragma omp parallel for
	for(int i=0;i<get_int_vec_size;i++)
	{
		std::cout<<"read the int vec attribute\n";
		ifstream test;
		test.open(loadDataPath,ios_base::binary);
		test.clear();
		test.seekg(int_vec_type_streamPos[i]);
		g_particles_io io_temp;
		io_temp.loadData(test);
		_int_vec_particles_handle[i]=io_temp;
		std::cout<<"global io read the int attribute name is "<<io_temp.GIO_getCurrentAttributeName()<<"\n";
	}

	
	// process for the float vector attribute list
	int get_flt_vec_size = flt_vec_type_streamPos.size();
	_flt_vec_particles_handle.resize(get_flt_vec_size);
	#pragma omp parallel for
	for(int i=0;i<get_flt_vec_size;i++)
	{
		std::cout<<"read the flt vec attribute\n";
		ifstream test;
		test.open(loadDataPath,ios_base::binary);
		test.clear();
		test.seekg(flt_vec_type_streamPos[i]);
		g_particles_io io_temp;
		io_temp.loadData(test);
		_flt_vec_particles_handle[i]=io_temp;
		std::cout<<"global io read the int attribute name is "<<io_temp.GIO_getCurrentAttributeName()<<"\n";
	}
	
}
