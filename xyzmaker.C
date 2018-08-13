#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

double* findxyz(double,double,int);
double findtheta(double,double,double,double,int);

int main()
{
  ofstream angout;
  fout.open("/home/ryan/nuclear/be9/stripped/xyzheader.h");

  cout<<fixed<<setprecision(4);
  fout<<fixed<<setprecision(6);
  
  fout.close();
  return 0;
}

double findtheta(double t1, double p1, double t2, double p2, double det)
{
  double theta = 0.;
  if(det!=1&&det!=3)
  {
	cout<<"Error, det must be the dE detector ID"<<endl;
	return(-100.);
  }
  
  double *vals1 = new double[3];
  double *vals2 = new double[3];
  double *deltavals = new double[3];

  vals1 = findxyz(t1,p1,det);
  vals2 = findxyz(t2,p2,det+1);

  for(int coord;coord<3;coord++)
  {
	deltavals[coord]=vals2[coord]-vals1[coord];
  }

  //sad attempt to do a dot product
  double mag = sqrt(deltavals[0]*deltavals[0]+deltavals[1]*deltavals[1]+deltavals[2]*deltavals[2]);
  double dot = deltavals[0]*0.+deltavals[1]*0.+deltavals[2]*1.;

  theta = arccos(mag/dot);

  return(theta);
}

double* findxyz(double theta, double phi, int det)
{
	double r;
	
	switch(det)
	{
		case 0:
		case 5:
		r=70.91;
		break;
		
		case 1:
		case 3:
		r=65.13;
		break;
		
		case 2:
		case 4:
		r=71.13;
		break;
	}

  double *vals = new double[3];

	vals[0] = r*cos(theta)*sin(phi);
	vals[1] = r*sin(theta)*sin(phi);
	vals[2] = r*cos(phi);

	return(vals);
}

