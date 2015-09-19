#ifndef  COPY_NODE_H_
#define  COPY_NODE_H_

#include <SOP/SOP_Node.h>
#include "G_Global_IO.h"
class GIO_Load:public SOP_Node
{
public:
	static OP_Node *myConstructor(OP_Network *,const char*,OP_Operator *);
	static PRM_Template myTemplateList[];

	GIO_Load(OP_Network *,const char*,OP_Operator *);
	virtual ~GIO_Load() {}
	
	void loadData(g_global_io &io,char *path);
	void forkTheData(g_global_io &io,GU_Detail *gdp);
	
	void setIntAttrib(g_global_io &io,GU_Detail *gdp);
	void setFloatAttrib(g_global_io &io,GU_Detail *gdp);
	void setIntVecAttrib(g_global_io &io,GU_Detail *gdp);
	void setFltVecAttrib(g_global_io &io,GU_Detail *gdp);
protected:
	virtual OP_ERROR cookMySop(OP_Context &context);


};
#endif