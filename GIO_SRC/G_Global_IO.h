#ifndef G_GLOBAL_IO_H_
#define G_GLOBAL_IO_H_

#include "G_Particle_type.h"
#include "G_Particles_IO.h"
#include <vector>
#include <fstream>
using namespace std;
class g_global_io
{
public:
	g_global_io();
	~g_global_io();

	void GIO_installParticleHandle(g_particles_io &particles_io);  // this is add particles handle

	vector<g_particles_io> GIO_getParticlesHandle(){return particles_handle;}  // get all handle
	vector<g_particles_io> GIO_getINTParticlesHandle(){return _int_particles_handle;}  // get int particles attribute handle
	vector<g_particles_io> GIO_getFLTParticlesHandle(){return _flt_particles_handle;}  // get flt particles attribute handle
	vector<g_particles_io> GIO_getINTVecParticlesHandle(){return _int_vec_particles_handle;}  // get int vector particles attribute handle
	vector<g_particles_io> GIO_getFLTVecParticlesHandle(){return _flt_vec_particles_handle;}  // get float vector particles attribute handle
	int GIO_getNumAttributes(){return particles_handle.size();}
	// data load and save!
	void save(ofstream &fout); // save all attribute into disk
	void load(ifstream &fin,char *loadDataPath);
private:
	int ptr;
	vector <g_particles_io> particles_handle;  // all particle handle and for each to write so very import port!
	vector <int > handle_ptr;
	// streampos position
	vector <streampos> int_type_streamPos;
	vector <streampos> int_vec_type_streamPos;
	vector <streampos> flt_type_streamPos;
	vector <streampos> flt_vec_type_streamPos;

	// sperator type handle
	vector <g_particles_io> _int_particles_handle;
	vector <g_particles_io> _int_vec_particles_handle;
	vector <g_particles_io> _flt_particles_handle;
	vector <g_particles_io> _flt_vec_particles_handle;
};

#endif