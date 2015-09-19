#define  SYS_HAS_NO_PI
#define BOOST_ALL_NO_LIB 1
#include "wendy_GIO_ROP.h"
#include <ROP/ROP_Error.h>
#include <ROP/ROP_Templates.h>
#include <SOP/SOP_Node.h>
#include <omp.h>
#include <OP/OP_Director.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <CH/CH_LocalVariable.h>

#include <UT/UT_DSOVersion.h>
#include  <UT/UT_String.h>
#include <sstream>
#include <cstdlib>

#include <GA/GA_AttributeFilter.h>
#include <GA/GA_AttributeDict.h>
#include <GA/GA_AttributeSet.h>
#include <GEO/GEO_AttributeHandleList.h>
#include <fstream>
#include "g_proc_ass_info.h"



static PRM_Name		 theFileName("file", "Save to file");
static PRM_Default	 theFileDefault(0, "particle.$F4.gp");

static PRM_Name theArDso("ArDso","Arnold dso path");  //this is the seed parm node~
static PRM_Default theArDsoDefault(0,"C:/solidangle/mtoadeploy/2014/procedurals/GIO_Load_proc.dll");



static PRM_Template *
	getTemplates()
{
	static PRM_Template	*theTemplate = 0;

	if (theTemplate)
		return theTemplate;
	theTemplate = new PRM_Template[22];

	theTemplate[0] = PRM_Template(PRM_FILE, 1, &theFileName, &theFileDefault);// this is the save seed geopath
	theTemplate[1] = PRM_Template(PRM_STRING,1,&theArDso,&theArDsoDefault);


	theTemplate[2] = theRopTemplates[ROP_TPRERENDER_TPLATE];                   // it's  must appear in every rop
	theTemplate[3] = theRopTemplates[ROP_PRERENDER_TPLATE];
	theTemplate[4] = theRopTemplates[ROP_LPRERENDER_TPLATE];
	theTemplate[5] = theRopTemplates[ROP_TPREFRAME_TPLATE];
	theTemplate[6]= theRopTemplates[ROP_PREFRAME_TPLATE];
	theTemplate[7] = theRopTemplates[ROP_LPREFRAME_TPLATE];
	theTemplate[8] = theRopTemplates[ROP_TPOSTFRAME_TPLATE];
	theTemplate[9] = theRopTemplates[ROP_POSTFRAME_TPLATE];
	theTemplate[10] = theRopTemplates[ROP_LPOSTFRAME_TPLATE];
	theTemplate[11] = theRopTemplates[ROP_TPOSTRENDER_TPLATE];
	theTemplate[12] = theRopTemplates[ROP_POSTRENDER_TPLATE];
	theTemplate[13] = theRopTemplates[ROP_LPOSTRENDER_TPLATE];
	theTemplate[14] = theRopTemplates[ROP_INITSIM_TPLATE];
	theTemplate[15] = theRopTemplates[ROP_MKPATH_TPLATE];
	return theTemplate;
}

OP_VariablePair	* wendy_GIO_ROP::getVariablePair()
{
	static OP_VariablePair *pair = 0;
	if (!pair)
		pair = new OP_VariablePair(ROP_Node::myVariableList);
	return pair;
}

OP_Node		* wendy_GIO_ROP::myConstructor( OP_Network *net, const char*name, OP_Operator *op )
{
	 return new wendy_GIO_ROP(net, name, op);
}

static void forkTheINTData(vector <int> &arrayReal,GA_Attribute *attrib,fpreal maxPt,fpreal startPt)
{
	arrayReal.clear();
	GA_RWHandleI ihandle(attrib);
	for(int i=0+startPt;i<maxPt+startPt;i++)
	{
		int value = ihandle.get(i);
		arrayReal.push_back(value);
	}


}
static void forkTheFLTData(vector <float> &arrayReal,GA_Attribute *attrib,fpreal maxPt,fpreal startPt)
{
	GA_RWHandleF fhandle(attrib);
	arrayReal.clear();
	for(int i=0+startPt;i<maxPt+startPt;i++)
	{

		float value = fhandle.get(i);
		arrayReal.push_back(value);
	}
	
}
static void forkTheINTVectorData(vector <per_int_vector> &arrayReal,GA_Attribute *attrib,fpreal maxPt,fpreal startPt)
{
	arrayReal.clear();
	GA_RWHandleV3 vhandle(attrib);
	for(int i=0+startPt;i<maxPt+startPt;i++)
	{

		per_int_vector t_int;
		t_int.g_x= vhandle.get(i).x();
		t_int.g_y= vhandle.get(i).y();
		t_int.g_z= vhandle.get(i).z();
		arrayReal.push_back(t_int);
	}

}
static void forkTheFLTVectorData(vector <per_float_vector> &arrayReal,GA_Attribute *attrib,fpreal maxPt,fpreal startPt)
{
	arrayReal.clear();
	GA_RWHandleV3 handle(attrib);
	for(int i=0+startPt;i<maxPt+startPt;i++)
	{
		GA_RWHandleV3 vhandle(attrib);
		
		per_float_vector t_flt;
		t_flt.g_x= float(vhandle.get(i).x());
		t_flt.g_y= float(vhandle.get(i).y());
		t_flt.g_z= float(vhandle.get(i).z());
		arrayReal.push_back(t_flt);
	}

	
}


static void updateAttributeList(g_global_io &io,GU_Detail *cur_gdp)
{
	fpreal numPt= cur_gdp->getNumPoints();
	GA_Iterator iter(cur_gdp->getPointRange());
	fpreal start_point_num = *iter;
	
	 if ( cur_gdp )
	 {
		 for (GA_AttributeDict::iterator it = cur_gdp->getAttributeDict(GA_ATTRIB_POINT).begin(GA_SCOPE_PUBLIC); !it.atEnd(); ++it)
		 {
			 GA_Attribute *attrib = it.attrib();
			 UT_String attName( attrib->getName() );
			// std::cout<<"the att name is"<<attName<<std::endl;
			 int attSize = attrib->getTupleSize();
		     GA_StorageClass     storage= attrib->getStorageClass();
			 GA_TypeInfo typeInfo = attrib->getTypeInfo();

			 //save mata info
			 


			 switch(attSize)
			 {
			 case 1:                    // per  int or float
				 if(storage==GA_STORECLASS_FLOAT)
				 {
					 std::cout<<"the att name is "<<attName<< " and the type is float\n";
					 g_particles_io t_io;
					 t_io.GIO_SetCurrentAttributeName(attName);
					 t_io.GIO_SetCurrentAttributeType(g_particles_io::FLT_TYPE);
					 std::vector<float> t_value;  // create empty stack
					 forkTheFLTData(t_value,attrib,numPt,start_point_num);
					 t_io.GIO_setFLTAttributeList(t_value);
					io.GIO_installParticleHandle(t_io);  // install this attribute into our global IO

				 }
				 if(storage==GA_STORECLASS_INT)
				 {
					 std::cout<<"the att name is "<<attName<< " and the type is int\n";
					 g_particles_io t_io;
					 t_io.GIO_SetCurrentAttributeName(attName);
					 t_io.GIO_SetCurrentAttributeType(g_particles_io::INT_TYPE);
					 std::vector<int> t_value;  // create empty  int stack
					 forkTheINTData(t_value,attrib,numPt,start_point_num);
					 t_io.GIO_setINTAttributeList(t_value);
					 io.GIO_installParticleHandle(t_io);
				 }
				 break;
			 case 3:                   // int vector or float vector
				 if(storage==GA_STORECLASS_FLOAT)
				 {
					 std::cout<<"the att name is "<<attName<< " and the type is float vector\n";
					 g_particles_io t_io;
					 t_io.GIO_SetCurrentAttributeName(attName);
					 t_io.GIO_SetCurrentAttributeType(g_particles_io::FLT_VEC_TYPE);
					 std::vector<per_float_vector> t_value;  // create empty  int stack
					 forkTheFLTVectorData(t_value,attrib,numPt,start_point_num);
					 t_io.GIO_setFLTVecAttributeList(t_value);
					 io.GIO_installParticleHandle(t_io);

				 }
				 if(storage==GA_STORECLASS_INT)
				 {
					 std::cout<<"the att name is "<<attName<< " and the type is int vector\n";
					 g_particles_io t_io;
					 t_io.GIO_SetCurrentAttributeName(attName);
					 t_io.GIO_SetCurrentAttributeType(g_particles_io::INT_VEC_TYPE);
					 std::vector<per_int_vector> t_value;  // create empty  int stack
					 forkTheINTVectorData(t_value,attrib,numPt,start_point_num);
					 t_io.GIO_setINTVecAttributeList(t_value);
					 io.GIO_installParticleHandle(t_io);
				 }
				 break;
			 case 4:                   // P attrib
				 {
					 std::cout<<"the att name is "<<attName<< " and the type is float 4 \n";
					 g_particles_io t_io;
					 t_io.GIO_SetCurrentAttributeName(attName);
					 t_io.GIO_SetCurrentAttributeType(g_particles_io::FLT_VEC_TYPE);
					 std::vector<per_float_vector> t_value;  // create empty  int stack
					 forkTheFLTVectorData(t_value,it.attrib(),numPt,start_point_num);
					
					 t_io.GIO_setFLTVecAttributeList(t_value);
					 io.GIO_installParticleHandle(t_io);
				 }
				
				 break;
			 }

		 }
	 }
}

void wendy_GIO_ROP::save_mata_ass(string arDsoPath,string dpCachePath,GU_Detail *gdp )
{
	//get arnold dso path
	

	g_particle_mataAss assROP;
	ofstream fout;
	string getCacheDP=dpCachePath;

	stringstream ss(getCacheDP);
	string sub_str;
	vector <string> sp_strPath;
	sp_strPath.clear();
	while(getline(ss,sub_str,'.')) 
	{
		sp_strPath.push_back(sub_str);
	}

	string newAssPathName = sp_strPath[0]+sp_strPath[1]+string(".ass");
	fout.open(newAssPathName);
	assROP.setGioCachePath(getCacheDP);  // set Link GIO CACHE
	for (GA_AttributeDict::iterator it = gdp->getAttributeDict(GA_ATTRIB_POINT).begin(GA_SCOPE_PUBLIC); !it.atEnd(); ++it)
	{
		GA_Attribute *attrib = it.attrib();
		string attName( attrib->getName() );
		assROP.inserExtraMataInfo(attName);
	}
	//set ass bbox
	UT_BoundingBox BBOX;
	gdp->getBBox(&BBOX);
	BBOX.expandBounds(2,2,2);
	float min_x=BBOX.xmin();
	float min_y=BBOX.xmin();
	float min_z=BBOX.xmin();
	float max_x=BBOX.xmax();
	float max_y=BBOX.ymax();
	float max_z=BBOX.zmax();
	assROP.setBBOX(min_x,min_y,min_z,max_x,max_y,max_z);
	assROP.setDsoPath(arDsoPath);
	assROP.save(fout);
	fout.close();
}



int wendy_GIO_ROP::startRender( int nframes, fpreal s, fpreal e )
{
	
//first we get the node 
	myEndTime = e;
	if (error() < UT_ERROR_ABORT)
		executePreRenderScript(s);
	return 1;
}

ROP_RENDER_CODE wendy_GIO_ROP::renderFrame( fpreal time, UT_Interrupt *boss )
{
	
	executePreFrameScript(time);
	fpreal t =CHgetManager()->getTime(this->myCurrentFrame);  //this can get the time!!!!
	now=time;
	
	
	UT_String save_geo_path;
	evalString(save_geo_path,"file",0,time);
	OP_Context		 context(time);

	UT_String _ar_dso_path;
	evalString(_ar_dso_path,"ArDso",0,time);

	save_gdp_to_disk(save_geo_path,context,save_geo_path);
	
	

	if (error() < UT_ERROR_ABORT)
		executePostFrameScript(time);
	return ROP_CONTINUE_RENDER;
}

void wendy_GIO_ROP::save_gdp_to_disk( char *arnold_dso_path,OP_Context &context,char *savePath)
{
	OP_Node * node = this->getInput(0);
	if(node)
	{
		SOP_Node * getNode = node->castToSOPNode();
		
		std::cout<<"the node name is"<<getNode->getName()<<std::endl;
		GU_Detail *sop_detail = (GU_Detail *)getNode->getCookedGeo(context);
		// save the geometry into desk
		g_global_io global_io;
		updateAttributeList(global_io,sop_detail);
		ofstream fout;
		fout.open(savePath,std::ios_base::binary);
		global_io.save(fout);
		fout.close();

		// mata data for the ass
		save_mata_ass(arnold_dso_path,savePath,sop_detail);
		
	}
	
	else
	{
		std::cout<<"No Sop Geometry Connect!\n";
		return ;
	}
}

ROP_RENDER_CODE wendy_GIO_ROP::endRender()
{
	if (error() < UT_ERROR_ABORT)
		executePostRenderScript(myEndTime);
	
	//delete gdp;
	this->initSimulationOPs();
	return ROP_CONTINUE_RENDER;
}

OP_TemplatePair	* wendy_GIO_ROP::getTemplatePair()
{
	static OP_TemplatePair *ropPair = 0;
	if (!ropPair)
	{
		OP_TemplatePair	*base;

		base = new OP_TemplatePair(getTemplates());
		ropPair = new OP_TemplatePair(ROP_Node::getROPbaseTemplate(), base);
	}
	return ropPair;
}



void newSopOperator(OP_OperatorTable *table)
{
	table->addOperator(new OP_Operator("wendy_GPIO_ROP",
		"wendy_GPIO_ROP",
		wendy_GIO_ROP::myConstructor,
		wendy_GIO_ROP::getTemplatePair(),
		0,
		1,
		wendy_GIO_ROP::getVariablePair(),
		OP_FLAG_MANAGER));
}
