#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <limits.h>
#include <vector>
#include <sys/stat.h>

//irbem subroutines that will be called from C++
extern "C"{
//position
  void gei2geo1_(int*,int*,double*,double*,double*);
  int get_doy_(int&,int&,int&);
  void car_sph_(double*,double&,double&,double&);
  void geo_gdz_(double&,double&,double&,double&,double&,double&);

  void make_lstar1_(int&,int&,int*,int&,int*,int*,double*,double*,double*,double*,double*,double*,double*,double*,double*,double*,double*);
  void get_field_multi_(int&,int&,int*,int&,int*,int*,double*,double*,double*,double*,double*,double*,double*);
  void fly_in_nasa_aeap1_(int&,int&,int&,int&,int&,double*,int*,int*,double*,double*,double*,double*,double*);
//CALL fly_in_afrl_crres1(ntime,sysaxes,whichm,whatf,Nene,energy,iyear,idoy, UT,x1,x2,x3,Ap15,flux,path,path_len)
  void fly_in_afrl_crres1_(int&,int&,int&,int&,int&,double*,int*,int*,double*,double*,double*,double*,double*,double*,unsigned char const*,int&);
}

