#include "ai.h"
#include <sstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "G_Global_IO.h"
#include <fstream>
using namespace std;

// global variables for procedural
string GIO_loadPath;
float GIO_radius;
int numPt;
// recursive function that creates random cloud with fractal clumping

static void initMyIo(g_global_io &io)
{
	//char *path ="C:/Users/Administrator/Desktop/gp_cache/gpcache.0073.gp";
	ifstream fin;
	fin.open(GIO_loadPath.c_str(),ios_base::binary);
	io.load(fin,(char*)GIO_loadPath.c_str());
	fin.close();
}
static void calNumPt(g_global_io &io)
{
	 vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();
	 numPt=flt_vecHandle[0].GIO_getFLTVecAttributeList().size();
}


static void makecloud(g_global_io &io,AtArray *pointarr, AtArray *radarr,float rad)
{
	

	 vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();

	// create points!
	
	// std::cout<<"has xxxxxxxx pt num is"<<numPt<<std::endl;
	 int vecAttSize=flt_vecHandle.size();
	// std::cout<<"flt_vecHandle size"<<flt_vecHandle.size()<<std::endl;
	 for(int i=0;i<vecAttSize;i++)
	 {
		 string attname =  flt_vecHandle[i].GIO_getCurrentAttributeName();
		 vector <per_float_vector> PtPosHandle = flt_vecHandle[i].GIO_getFLTVecAttributeList();
		 //std::cout<<"att name is"<<attname<<std::endl;
		 if(attname =="P")  // create points
		 {
			 std::cout<<"GIO_POINTS:Gearslogy.....Create the Points Num is .."<<PtPosHandle.size()<<std::endl;
			 int eval_step=0;
			 for(int k=0;k<PtPosHandle.size();k++)
			 {

				 per_float_vector vecPos = PtPosHandle[k];
				 AtVector vec;
				 vec.x=vecPos.g_x;
				 vec.y=vecPos.g_y;
				 vec.z=vecPos.g_z;
				//std::cout<<vec.x<<" "<<vec.y<<" "<<vec.z<<std::endl;
				AiArraySetPnt(pointarr, k, vec);
				AiArraySetFlt(radarr, k, 0.02);
			
			 }
		 }
	 }


}
static void forkTheVecFltAttribute(g_global_io &io,vector<AtArray*> ArFltVecList,AtNode *node)
{
	 vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();
	 ArFltVecList.resize(flt_vecHandle.size());
	 // sendAllocation
	 for(int i=0;i<ArFltVecList.size();i++)
	 {
		 ArFltVecList[i]=AiArrayAllocate(numPt,1,AI_TYPE_RGBA);
	 }
	 //now set the attribute for each point
	 for(int i=0;i<ArFltVecList.size();i++)
	 {
		 string attname =  flt_vecHandle[i].GIO_getCurrentAttributeName();
		 vector <per_float_vector> PtPosHandle = flt_vecHandle[i].GIO_getFLTVecAttributeList();
		 AiNodeDeclare(node, attname.c_str(), "uniform RGBA");  // add attribute into arnold for user data color api
		 for(int k=0;k<PtPosHandle.size();k++)
		{
			per_float_vector vecAtt = PtPosHandle[k];
			AtRGBA vec;
			vec.r=vecAtt.g_x;
			vec.g=vecAtt.g_y;
			vec.b=vecAtt.g_z;
			vec.a=1.0f;
			AiArraySetRGBA(ArFltVecList[i], k, vec);
		 }
		 AiNodeSetArray(node, attname.c_str(), ArFltVecList[i]);
	 }

}
static void forkTheVecIntAttribute(g_global_io&io,vector<AtArray*>arIntVecList,AtNode*node)
{

	vector <g_particles_io> int_vecHandle=io.GIO_getINTVecParticlesHandle();
	arIntVecList.resize(int_vecHandle.size());
	// sendAllocation
	for(int i=0;i<arIntVecList.size();i++)
	{
		arIntVecList[i]=AiArrayAllocate(numPt,1,AI_TYPE_RGBA);
	}
	//now set the attribute for each point
	for(int i=0;i<arIntVecList.size();i++)
	{
		string attname =  int_vecHandle[i].GIO_getCurrentAttributeName();
		vector <per_int_vector> PtPosHandle = int_vecHandle[i].GIO_getINTVecAttributeList();
		AiNodeDeclare(node, attname.c_str(), "uniform RGBA");  // add attribute into arnold for user data color api
		for(int k=0;k<PtPosHandle.size();k++)
		{
			per_int_vector vecAtt = PtPosHandle[k];
			AtRGBA vec;
			vec.r=float(vecAtt.g_x);
			vec.g=float(vecAtt.g_y);
			vec.b=float(vecAtt.g_z);
			vec.a=1.0f;
			AiArraySetRGBA(arIntVecList[i], k, vec);
		}
		AiNodeSetArray(node, attname.c_str(), arIntVecList[i]);
	}
}
// we just wan't use the uesr data color,not the user the data float/int and so on
static void forkTheIntAttribute(g_global_io&io,vector<AtArray*>arIntList,AtNode*node)  
{
	vector <g_particles_io> int_Handle=io.GIO_getINTParticlesHandle();
	arIntList.resize(int_Handle.size());
	for(int i=0;i<arIntList.size();i++)
	{
		arIntList[i]=AiArrayAllocate(numPt,1,AI_TYPE_RGBA);
	}
	for(int i=0;i<arIntList.size();i++)
	{
		string attname =  int_Handle[i].GIO_getCurrentAttributeName();
		vector <int> PtPosHandle = int_Handle[i].GIO_getINTAttributeList();
		AiNodeDeclare(node, attname.c_str(), "uniform RGBA");  // add attribute into arnold for user data color api
		for(int k=0;k<PtPosHandle.size();k++)
		{
			int vecAtt = PtPosHandle[k];
			AtRGBA vec;
			vec.r=float(vecAtt);
			vec.g=float(vecAtt);
			vec.b=float(vecAtt);
			vec.a=1.0f;
			AiArraySetRGBA(arIntList[i], k, vec);
		}
		AiNodeSetArray(node, attname.c_str(), arIntList[i]);
	}

}

static void forkTheFltAttribute(g_global_io&io,vector<AtArray*>arFltList,AtNode*node)  
{
	vector <g_particles_io> flt_Handle=io.GIO_getFLTParticlesHandle();
	arFltList.resize(flt_Handle.size());
	for(int i=0;i<arFltList.size();i++)
	{
		arFltList[i]=AiArrayAllocate(numPt,1,AI_TYPE_RGBA);
	}
	for(int i=0;i<arFltList.size();i++)
	{
		string attname =  flt_Handle[i].GIO_getCurrentAttributeName();
		vector <float> PtPosHandle = flt_Handle[i].GIO_getFLTAttributeList();
		AiNodeDeclare(node, attname.c_str(), "uniform RGBA");  // add attribute into arnold for user data color api
		for(int k=0;k<PtPosHandle.size();k++)
		{
			float vecAtt = PtPosHandle[k];
			AtRGBA vec;
			vec.r=vecAtt;
			vec.g=vecAtt;
			vec.b=vecAtt;
			vec.a=1.0f;
			AiArraySetRGBA(arFltList[i], k, vec);
		}
		AiNodeSetArray(node, attname.c_str(), arFltList[i]);
	}

}

static void forkTheSingleVec(g_global_io &io,AtArray *array)  // this is just test one attribute for proc
{
	vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();
	 for(int i=0;i<flt_vecHandle.size();i++)
	 {
		 string attname =  flt_vecHandle[i].GIO_getCurrentAttributeName();
		 vector <per_float_vector> PtPosHandle = flt_vecHandle[i].GIO_getFLTVecAttributeList();
		 if(attname=="P")
		 {
			  AtRGBA vec;
			 for(int k=0;k<PtPosHandle.size();k++)
			 {
				 per_float_vector vecAtt = PtPosHandle[k];
				 vec.r=vecAtt.g_x;
				 vec.g=vecAtt.g_y;
				 vec.b=vecAtt.g_z;
				 vec.a = 1.0f;
				// std::cout<<"setting the rgb \n";
				 AiArraySetRGBA(array, k, vec);
			 }
		 }
	 }
}
static int MyInit(AtNode *mynode, void **user_ptr)
{
	//AiNodeDeclare(mynode, "aaa", "constant ARRAY RGB");
	*user_ptr = mynode; // make a copy of the parent procedural   

	GIO_loadPath = string(AiNodeGetStr(mynode,"GIO_loadPath"));
	std::cout<<"get load path is "<<GIO_loadPath<<std::endl;
	GIO_radius = AiNodeGetFlt(mynode,"GIO_radius");
	return true;
}

static int MyCleanup(void *user_ptr)
{
	return true;
}

static int MyNumNodes(void *user_ptr)
{
	return 1;
}

static AtNode *MyGetNode(void *user_ptr, int i)
{
	g_global_io io;
	initMyIo(io);
	calNumPt(io);
	AtArray *pointarray  = AiArrayAllocate(numPt,1,AI_TYPE_POINT);
	AtArray *radiusarray=AiArrayAllocate(numPt,1,AI_TYPE_FLOAT); 
	//AtArray *Parray=AiArrayAllocate(numPt,1,AI_TYPE_RGBA);  //test att

	
	makecloud(io,pointarray, radiusarray,GIO_radius);
	

	AtNode *node = AiNode("points");
	//AiNodeDeclare(node, "mycolor", "uniform RGBA");
	AiNodeSetArray(node, "points", pointarray);
	AiNodeSetArray(node, "radius", radiusarray);
	//forkTheSingleVec(io,Parray);
	//AiNodeSetArray(node, "mycolor", Parray);

	//now set the flt vector attribute list
	std::vector <AtArray*> extraFLT_VecAttList;
	forkTheVecFltAttribute(io,extraFLT_VecAttList,node);
	//
	std::vector <AtArray*> extraINT_VecAttList;
	forkTheVecIntAttribute(io,extraINT_VecAttList,node);
	//
	std::vector <AtArray*> extraINT_AttList;
	forkTheIntAttribute(io,extraINT_AttList,node);
	//
	std::vector <AtArray*> extraFLT_AttList;
	forkTheFltAttribute(io,extraFLT_AttList,node);
	AiNodeSetStr(node, "mode", "sphere");
	
	return node;
}

// vtable passed in by proc_loader macro define
proc_loader 
{
	vtable->Init     = MyInit;
	vtable->Cleanup  = MyCleanup;
	vtable->NumNodes = MyNumNodes;
	vtable->GetNode  = MyGetNode;
	strcpy(vtable->version, AI_VERSION);
	return true;
}