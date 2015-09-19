#include "g_proc_ass_info.h"

g_particle_mataAss::g_particle_mataAss()
{
	
	infoBBox.clear();
	infoBBox.push_back(10.00);
	infoBBox.push_back(11.00);
	infoBBox.push_back(12.00);
	infoBBox.push_back(13.00);
	infoBBox.push_back(14.00);
	infoBBox.push_back(15.00);
	dso_PATH = "C:/solidangle/mtoadeploy/2013/procedurals/GIO_Load_proc.dll";
}

void g_particle_mataAss::setBBOX( float min_x,float min_y,float min_z,float max_x,float max_y,float max_z )
{
	infoBBox.clear();
	infoBBox.push_back(min_x);
	infoBBox.push_back(min_y);
	infoBBox.push_back(min_z);
	infoBBox.push_back(max_x);
	infoBBox.push_back(max_y);
	infoBBox.push_back(max_z);

}

void g_particle_mataAss::save( ofstream &fout )
{
	// predefine the mata
	string title ="procedural";
	string left_rd ="{";
	string right_rd ="}";
	string proc_name = " name GIO_Load_proc";
	//string proc_dso_path = "C:/solidangle/mtoadeploy/2013/procedurals/GIO_Load_proc.dll";

	fout<<title.c_str()<<"\n";
	fout<<left_rd.c_str()<<"\n";
	fout<<proc_name.c_str()<<"\n";
	fout<<" dso " <<"\""<<dso_PATH.c_str()<<"\""<<"\n";
	fout.setf(ios_base::fixed);
	fout<<" min "<<infoBBox[0]<<" "<<infoBBox[1]<<" "<<infoBBox[2]<<"\n";
	fout<<" max "<<infoBBox[3]<<" "<<infoBBox[4]<<" "<<infoBBox[5]<<"\n";

	string dpath = " declare GIO_loadPath constant STRING";
	string dradius=" declare GIO_radius constant FLOAT";

	fout<<dpath.c_str()<<"\n";
	fout<<dradius.c_str()<<"\n";

	string dp_path = " GIO_loadPath \"";
	string dp_radius = " GIO_radius 1";
	fout<<dp_path.c_str()<<gio_cache_path.c_str()<<"\"" <<"\n";
	fout<<dp_radius.c_str()<<"\n";


	fout<<" attName ";
	for(int i=0;i<extraInfoMat.size();i++)
	{
		fout<<extraInfoMat[i].c_str()<<" ";
	}
	fout<<"\n";
	fout<<right_rd.c_str()<<"\n";
}

g_particle_mataAss::~g_particle_mataAss()
{

}

