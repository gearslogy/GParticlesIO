#ifndef WENDY_GIO_ROP_H_
#define WENDY_GIO_ROP_H_
#include <ROP/ROP_Node.h>
#include <G_Global_IO.h>
#include <vector>
class OP_TemplatePair;
class OP_VariablePair;
class GU_Detail;
class SOP_Node;
class OP_Node;
class wendy_GIO_ROP:public ROP_Node
{
public:
	static OP_TemplatePair	*getTemplatePair(); //parms all in this function
	static OP_VariablePair	*getVariablePair();  /// Provides access to our variables.
	static OP_Node		*myConstructor(OP_Network *net, const char*name,
		 OP_Operator *op);
protected:
	inline wendy_GIO_ROP(OP_Network *net, const char *name, OP_Operator *entry):ROP_Node(net,name,entry)
	{
		
	}
	inline virtual ~wendy_GIO_ROP(){}


	virtual int			 startRender(int nframes, fpreal s, fpreal e);

	virtual ROP_RENDER_CODE	 renderFrame(fpreal time, UT_Interrupt *boss);

	virtual ROP_RENDER_CODE	 endRender();
	fpreal		 myEndTime;
	fpreal now;
	void save_gdp_to_disk(char *arnold_dso_path,OP_Context &,char *savePath);
	void save_mata_ass(string arDsoPath,string dpCachePath,GU_Detail *gdp);

	
};
#endif