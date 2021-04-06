//Example showing how to call IRBEM fortran subroutines
// F.Lei (flie@radmod.co.uk)
// 19/12/2020
//
#include "irbem_ex.h"

using namespace std;

int main(int argc, char* argv[]){
  time_t tempo=time(NULL);
  
  int t,f;
  //IRBEM linked variables
  int NTIMEMAX=100000,NTIME=1,MODEL=3,WHATF =1,SYSAXESIN=0,*IYEAR,*IDOY,NENERGY=100;
  double *X1,*X2,*X3,*UT,*AP15,*ENERGY,*OFLUX,*gei;
  IYEAR=new int[NTIMEMAX]();
  IDOY=new int[NTIMEMAX]();
  X1=new double[NTIMEMAX]();
  X2=new double[NTIMEMAX]();
  X3=new double[NTIMEMAX]();
  UT=new double[NTIMEMAX]();
  AP15=new double[NTIMEMAX](); 
  ENERGY=new double[2*25]();
  OFLUX=new double[NENERGY*NTIMEMAX]();
  gei=new double[3];
 
  int PATHLEN; 
  unsigned char const* UPATH;
  
  //read the input data file
  string line;
  char var[63];
  ifstream dat;
  double val3;
  float val2;
  vector<double>v_amjd;
  vector<float>v_flux,v_dflux,v_xyz;
  int Nbepp=0;
  dat.open(argv[1],ios::in);
  //read header
  for(t=0;t<9;t++)getline(dat,line);
  //read time series
  while(1){
    dat>>var;
    val3=strtod(var,NULL);
    if(dat.eof())break;
    else if(val3<0){
      cout<<"AMJD<0: "<<val3<<endl;
      return 1;
    }
    v_amjd.push_back(val3);
    dat>>var;
    val2=strtod(var,NULL);
    if(val2<0 || dat.eof()){
      cout<<"Flux<0: "<<val2<<endl;
      return 1;
    }
    v_flux.push_back(val2);
    dat>>var;
    val2=strtod(var,NULL);
    if(val2<0 || dat.eof()){
      cout<<"dFlux<0: "<<val2<<endl;
      return 1;
    }
    v_dflux.push_back(val2);
    for(t=0;t<3;t++){
      dat>>var;
      val2=strtod(var,NULL);
      if(dat.eof()){
	cout<<"Unexpected end of file."<<endl;
	return 2;
      }
      v_xyz.push_back(val2);
    }
  }
  //Set the number of points for fly_in_nasa calculation = to inupts
  Nbepp=v_amjd.size();
  NTIME=Nbepp;
  //AP8 integral flux PIF
  for(f=0;f<Nbepp;f++){
    IYEAR[f]=1996;
    IDOY[f]=int(v_amjd.at(f))-50082;
    UT[f]=86400.*(v_amjd.at(f)-double(int(v_amjd.at(f))));
    //convert input GEI cartesian into GEO
    for(t=0;t<3;t++){
      gei[t]=v_xyz.at(3*f+t);
      //      printf(" %6.2f",gei[t]);
    }
    //gei2geo1_(IYEAR+f,IDOY+f,UT+f,gei,geo);
    for(t=0;t<3;t++)gei[t]/=6371.2; // in Re
    X1[f]=gei[0];
    X2[f]=gei[1];
    X3[f]=gei[2];
    AP15[f] = 0.;
  }
  
  // call AP8, integral flux 
  SYSAXESIN=5; //GEI
  MODEL=-3; // AP8 MIN with ESA interpolation
  WHATF=3; // Integral fluxes
  NENERGY= 1;
  ENERGY[0] = 10.; // > 10 MeV integral flux only	
  fly_in_nasa_aeap1_(NTIME,SYSAXESIN,MODEL,WHATF,NENERGY,ENERGY,IYEAR,IDOY,UT,X1,X2,X3,OFLUX);
  cout<<"AP8 done"<<endl;
  
  //create output file with both data with  GEI positions
  FILE *BR;
  BR=fopen("output.txt","w");
  if(BR==NULL){
    cout<<"Output file cannot be created."<<endl;
    return 4;
  }

  fprintf(BR,"AMJD time         X1      X2      X3   I_cts  AP8(>10MeV)\n");
  for(f=0;f<NTIME;f++){
    fprintf(BR,"%16.10f %6.1f %6.2f %7.2f %7.1f %10.3e\n",v_amjd.at(f),X1[f],X2[f],X3[f],v_flux.at(f),OFLUX[f]);
  }

  // call CRRES, integral flux 
  SYSAXESIN=5; //GEI
  MODEL=1; // CRRESPRO Quiet
  WHATF=3; // Integral fluxes
  NENERGY= 1;
  ENERGY[0] = 10.; // > 10 MeV integral flux only
  auto PATH = string{"../IRBEM-4.4.0/data/"} ;	
  PATHLEN = PATH.length();
  auto CPATH= PATH.c_str() ;
  UPATH = reinterpret_cast<unsigned char const*>(CPATH); 
  fly_in_afrl_crres1_(NTIME,SYSAXESIN,MODEL,WHATF,NENERGY,ENERGY,IYEAR,IDOY,UT,X1,X2,X3,AP15,OFLUX, UPATH, PATHLEN);
  cout<<"CRRES done"<<endl;

  fprintf(BR,"AMJD time         X1      X2      X3   I_cts  AP8(>10MeV)\n");
  for(f=0;f<NTIME;f++){
    fprintf(BR,"%16.10f %6.1f %6.2f %7.2f %7.1f %10.3e\n",v_amjd.at(f),X1[f],X2[f],X3[f],v_flux.at(f),OFLUX[f]);
  }  
  
  fclose(BR);
  cout<<"Finished successfully.    T(s): "<<long(time(NULL)-tempo)<<endl;
  return 0;
}
