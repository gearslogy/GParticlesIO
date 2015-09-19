#define  SYS_HAS_NO_PI
#define BOOST_ALL_NO_LIB 1
#include "copy_node.h"
#include <UT/UT_DSOVersion.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Interrupt.h>
#include <GU/GU_Detail.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <SYS/SYS_Math.h>
#include <GEO/GEO_PrimPoly.h>
#include <GU/GU_PrimPoly.h>
#include <omp.h>
#include <OP/OP_AutoLockInputs.h>
#include <OP/OP_Director.h>
#include <fstream>
#include <vector>
#include <UT/UT_Interrupt.h>

PRM_Name load_str("load","load Points");
PRM_Default load_str_default(0,"particle.$F4.gp");


PRM_Template GIO_Load::myTemplateList[]={PRM_Template(PRM_FILE,1,&load_str,&load_str_default),PRM_Template(),};





void newSopOperator(OP_OperatorTable *table)
{
	OP_Operator *op=new OP_Operator("GIO_Load","GIO_Load",GIO_Load::myConstructor,GIO_Load::myTemplateList,0,0,0,OP_FLAG_GENERATOR);
	table->addOperator(op);
}

OP_Node * GIO_Load::myConstructor( OP_Network *net,const char*name,OP_Operator * op)
{
	return new GIO_Load(net,name,op);
}

GIO_Load::GIO_Load( OP_Network *net,const char*name,OP_Operator * op):SOP_Node(net,name,op)
{

}
OP_ERROR GIO_Load::cookMySop( OP_Context &context )
{
	
	
	fpreal now = context.getTime();
	
	gdp->clearAndDestroy();

	UT_AutoInterrupt boss("Copying Object");
	if(boss.wasInterrupted())
	{
		return error();
	}
	
	




	//GU_Detail *newgdp = new GU_Detail
	//newgdp->duplicate(*gdp);
	/*
	fpreal startPointNum=0;
	
	std::cout<<input_geo1->getPointRange().getMaxEntries()<<std::endl;
	std::cout<<input_geo1->getPointRange().getEntries()<<std::endl;

	std::cout<<offset<<std::endl;
	*/

	/*
	GA_RWHandleV3 ph(input_geo1, GA_ATTRIB_POINT, "P");
	if (ph.isValid())
	{
		UT_Vector3 N;
		for (GA_Iterator it(input_geo1->getPointRange()); !it.atEnd(); ++it)
		{
			GA_Offset offset = *it;
			
			UT_Vector3 P = ph.get(offset);
			std::cout<<"Att is" << N <<std::endl;
		
		}
	}
	*/


	/*
	GA_Offset ptoff2;
	ptoff2=0;

	GA_RWHandleV3 Phandle(input_geo1->findAttribute(GA_ATTRIB_POINT, "P"));
	std::cout<<"input_geo_1 0 position is"<<Phandle.get(ptoff2)<<std::endl;

	GA_Offset ptoff;
	
	GA_FOR_ALL_PTOFF(input_geo1, ptoff)
	{
		std::cout<<ptoff<<std::endl;
	
		UT_Vector3 Pvalue = Phandle.get(ptoff);
		std::cout<<Pvalue<<std::endl;
	
	}
	*/
	g_global_io io;
	gdp->clearAndDestroy();
	UT_String dataPath;
	this->evalString(dataPath,"load",0,now);
	loadData(io,dataPath); //load io
	forkTheData(io,gdp);  // create points
	setIntAttrib(io,gdp);
	setFloatAttrib(io,gdp);
	setIntVecAttrib(io,gdp);
	setFltVecAttrib(io,gdp);


	//unlockInputs();
	return error();
}

void GIO_Load::loadData(g_global_io &io ,char *path )
{
	ifstream fin;
	fin.open(path,std::ios_base::binary);
	io.load(fin,path);//this path just for the fork the thread
	fin.close();
}

void GIO_Load::forkTheData( g_global_io &io,GU_Detail *gdp )
{

 //vector <g_particles_io> fltHandle=io.GIO_getFLTParticlesHandle();
 //vector <g_particles_io> intHandle=io.GIO_getINTParticlesHandle();
 vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();
// vector <g_particles_io> int_vecHandle=io.GIO_getINTVecParticlesHandle();


 int vecSize =flt_vecHandle.size();

 for(int i=0;i<vecSize;i++)
 {
	string attname =  flt_vecHandle[i].GIO_getCurrentAttributeName();
	
	
	vector <GEO_Point*> points;
	vector <per_float_vector> PtPosHandle = flt_vecHandle[i].GIO_getFLTVecAttributeList();
	if(attname =="P")
	{
		//std::cout<<"create point start\n";
		//create points
		
		//std::cout<<"get the ptPosHandle size is"<<PtPosHandle.size()<<std::endl;
		for(int k=0;k<PtPosHandle.size();k++)
		{
			
			GEO_Point *pt = gdp->appendPoint();
			per_float_vector vecPos = PtPosHandle[k];
			pt->setPos(vecPos.g_x,vecPos.g_y,vecPos.g_z);
			points.push_back(pt);
		}
	}
	
	//else  // add attrib
	//{
		//gdp->addFloatTuple(GA_ATTRIB_POINT,attname.c_str(),3);
		/*
		GA_RWHandleV3 handle(gdp->findAttribute(GA_ATTRIB_POINT,attname.c_str()));
		if(handle.isValid())
		{
			std::cout<<"fount attrib name is "<<attname<<"and numpt is "<<gdp->getNumPoints()<<std::endl;
			
			for(int k=0;k<PtPosHandle.size();k++)
			{
				per_float_vector att = PtPosHandle[k];
				UT_Vector3F c_houdini(att.g_x,att.g_y,att.g_z);
				handle.set(k,c_houdini);
			}
		}
		*/
	
	//}


 }
}

void GIO_Load::setIntAttrib( g_global_io &io,GU_Detail *gdp )
{
	
	vector <g_particles_io> intHandle=io.GIO_getINTParticlesHandle();
	for(int i=0;i<intHandle.size();i++) // foreach every attrib
	{
		string attname =  intHandle[i].GIO_getCurrentAttributeName();
		gdp->addIntTuple(GA_ATTRIB_POINT,attname.c_str(),1);
		GA_RWHandleI att_handle(gdp->findPointAttribute(attname.c_str()));

		vector <int> per_Data = intHandle[i].GIO_getINTAttributeList();
		if(att_handle.isValid())// if find attrib
		{
			for(int k=0;k<gdp->getNumPoints();k++)  // set attrib
			{
				int att = per_Data[k];
				att_handle.set(k,att);
			}
		}
	}
	

}

void GIO_Load::setFloatAttrib( g_global_io &io,GU_Detail *gdp )
{
	vector <g_particles_io> fltHandle=io.GIO_getFLTParticlesHandle();
	for(int i=0;i<fltHandle.size();i++) // foreach every attrib
	{
		string attname =  fltHandle[i].GIO_getCurrentAttributeName();
		gdp->addIntTuple(GA_ATTRIB_POINT,attname.c_str(),1);
		GA_RWHandleF att_handle(gdp->findPointAttribute(attname.c_str()));

		vector <float> per_Data = fltHandle[i].GIO_getFLTAttributeList();
		if(att_handle.isValid())// if find attrib
		{
			for(int k=0;k<gdp->getNumPoints();k++)  // set attrib
			{
				float att = per_Data[k];
				att_handle.set(k,att);
			}
		}
	}

}

void GIO_Load::setIntVecAttrib( g_global_io &io,GU_Detail *gdp )
{
	vector <g_particles_io> int_vecHandle=io.GIO_getINTVecParticlesHandle();
	for(int i=0;i<int_vecHandle.size();i++) // foreach every attrib
	{
		string attname =  int_vecHandle[i].GIO_getCurrentAttributeName();
		gdp->addIntTuple(GA_ATTRIB_POINT,attname.c_str(),3);
		GA_RWHandleV3 att_handle(gdp->findPointAttribute(attname.c_str()));

		vector <per_int_vector> per_vecData = int_vecHandle[i].GIO_getINTVecAttributeList();
		if(att_handle.isValid())// if find attrib
		{
			for(int k=0;k<gdp->getNumPoints();k++)  // set attrib
			{
				per_int_vector att = per_vecData[k];
				UT_Vector3I c_houdini(att.g_x,att.g_y,att.g_z);
				att_handle.set(k,c_houdini);
			}
		}
	}
}

void GIO_Load::setFltVecAttrib( g_global_io &io,GU_Detail *gdp )
{
	vector <g_particles_io> flt_vecHandle=io.GIO_getFLTVecParticlesHandle();
	for(int i=0;i<flt_vecHandle.size();i++) // foreach every attrib
	{
		string attname =  flt_vecHandle[i].GIO_getCurrentAttributeName();
		if(attname!="P")  // because p we had create points
		{
			gdp->addFloatTuple(GA_ATTRIB_POINT,attname.c_str(),3);
			GA_RWHandleV3 att_handle(gdp->findPointAttribute(attname.c_str()));

			vector <per_float_vector> per_vecData = flt_vecHandle[i].GIO_getFLTVecAttributeList();
			if(att_handle.isValid())// if find attrib
			{
				for(int k=0;k<gdp->getNumPoints();k++)  // set attrib
				{
					per_float_vector att = per_vecData[k];
					UT_Vector3F c_houdini(att.g_x,att.g_y,att.g_z);
					att_handle.set(k,c_houdini);
				}
			}
		}
	}
}





