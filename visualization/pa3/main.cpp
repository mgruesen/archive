/**
*	Visualization PA3-A
*	Scattered Data
*	Michael Gruesen
**/
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkMath.h"
#include <vtkPolyData.h>
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"

#include "vtkTextProperty.h"
#include "vtkTextActor.h"
#include "vtkContourFilter.h"
#include "vtkLookupTable.h"
#include "vtkStructuredPointsReader.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRendererCollection.h"
#include "vtkObjectFactory.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>


#define inputFileName "scaled_scattered_data.txt"
using namespace std;
int* dim = new int[3];
float filterVal;

class KeyInteractorStyle : public vtkInteractorStyleTrackballCamera {
public:
	static KeyInteractorStyle* New();
	void setFilter(vtkAlgorithm* filter) { this->filter = (vtkContourFilter*)filter; }
	vtkTypeMacro(KeyInteractorStyle, vtkInteractorStyleTrackballCamera);
	virtual void OnKeyPress(){
		vtkRenderWindowInteractor* iren = this->GetInteractor();
		string key = iren->GetKeySym();
		if (key == "i"){			
			filterVal += 0.005;
		}
		if(key == "d"){
			if (filterVal > 0.01) filterVal -= 0.005;
		}
		this->filter->SetValue(0, filterVal);
		iren->GetRenderWindow()->Render();
		vtkInteractorStyleTrackballCamera::OnKeyPress();
	}
	vtkContourFilter* filter;
}; 
vtkStandardNewMacro(KeyInteractorStyle);

struct Point{
	Point(int a, int b, int c, float d):x(a),y(b),z(c),v(d){}
	int x,y,z;
	float v;
};

void getInput(){
	std::cout << "Please enter X dimension size: ";
	std::cin >> dim[0];
	std::cout << "\nPlease enter Y dimension size: ";
	std::cin >> dim[1];
	std::cout << "\nPlease enter Z dimension size: ";
	std::cin >> dim[2];
}

void determineIndex(float* raw, int* real, int size){
	for(int i = 0; i < size; i++){
		float ftemp = raw[i] * dim[i];
		int itemp = ftemp;
		if (abs(ftemp - itemp) < 0.5){
			real[i] = itemp;
		}
		else{
			real[i] = itemp + 1;
		}
		if(real[i] == dim[i])
			real[i] -= 1;
	}
}

vector<Point> readAndBuildData(){
  vector<Point> points;
  std::ifstream f(inputFileName); 
  std::string line;
  while (std::getline(f, line)){
	  std::stringstream lineStream;
	  lineStream << line;
	  float* raw = new float[4];// x, y, z, v;
	  int* real = new int[3];
	  
	  lineStream >> raw[0] >> raw[1] >> raw[2] >> raw[3];
	  determineIndex(raw, real, 3);
	  Point p(real[0], real[1], real[2], raw[3]);
	  points.push_back(p);
  }
  f.close();
  return points;
}

float isKnown(int i, int j, int k, vector<Point>& points){
	float ret = -1;
	for(vector<Point>::iterator iter = points.begin(); iter != points.end(); ++iter){
		if (i == iter->x && j == iter->y && k == iter->z){
			ret = iter->v;
			break;
		}
	}
	return ret;
}

float getDistance3D(int a[], int b[]){
	int x, y, z;
	x = a[0] - b[0];
	y = a[1] - b[1];
	z = a[2] - b[2];
	return sqrt((float)(x*x + y*y + z*z));
}

float getWeight(vector<Point>::iterator iter, int i, int j, int k){
	int a[3] = {i,j,k};
	int b[3] = {iter->x, iter->y, iter->z};
	float dist = getDistance3D(a,b);
	return 1/pow(dist, 2.0f);
}

void printInput(float* input){
	for(int k = 0; k < dim[2]; k++){
		int koff = k * dim[0] * dim[1];
		for(int j = 0; j < dim[1]; j++){
			int joff = j * dim[0];
			for(int i = 0; i < dim[0]; i++){
				cout << "x:" << i << " y:" << j << " z:" << k << " v:" << input[koff + joff + i] << endl;
			}
		}
	}
}

float* shepardInterpolate(vector<Point>& knownPoints){
  float* input = new float[dim[0] * dim[1] * dim[2]];
  cout << "starting interpolation...";
  clock_t start = clock();
  for(int k = 0; k < dim[2]; k++){
	int koff = k * dim[0] * dim[1];
	for(int j = 0; j < dim[1]; j++){
	  int joff = j * dim[0];
	  for(int i = 0; i < dim[0]; i++){
	    float val = 0.0;
		int off = koff + joff + i;
		if ((val = isKnown(i,j,k, knownPoints)) < 0.0){			
		  val = 0.0;
		  float top = 0, bottom = 0;
		  for(vector<Point>::iterator iter = knownPoints.begin(); iter != knownPoints.end(); ++iter){
		    float weight = getWeight(iter, i, j, k);
			top += weight * iter->v;
			bottom += weight;			
		  }
		  val = top / bottom;
		}
		input[off] = val;
	  }
    }
  }
  clock_t end = clock();
  cout << "done!" << endl;
  cout << "total time:" << (end - start) / (double) CLOCKS_PER_SEC << " seconds" << endl;
  return input;
}

void writeOutput(float* input){
	ofstream os("shepard_output.vtk");
	os << "# vtk DataFile Version 3.0\nvtk output\nASCII\nDATASET STRUCTURED_POINTS\n";
	os << "DIMENSIONS " << dim[0] << " " << dim[1] << " " << dim[2] << endl;
	os << "SPACING 1.6 1.6 1.5\n";
	os << "ORIGIN 0 0 0\n";
	os << "POINT_DATA " << (dim[0] * dim[1] * dim[2]) << endl;
	os << "SCALARS scalars float\n";
	os << "LOOKUP_TABLE default\n";

	for(int k = 0; k < dim[2]; k++){
		int koff = k * dim[0] * dim[1];
		for(int j = 0; j < dim[1]; j++){
			int joff = j * dim[0];
			for(int i = 0; i < dim[0]; i++){
				os << input[koff + joff + i] << " ";
			}
			os << endl;
		}
	}
	os.close();
}

int main( int argc, char *argv[] )
{  
  getInput();
  vector<Point> points = readAndBuildData();
  float* input = shepardInterpolate(points);
  writeOutput(input);
  delete(input);
  vtkStructuredPointsReader* reader = vtkStructuredPointsReader::New();
  reader->SetFileName("shepard_output.vtk");

  vtkContourFilter* contourFilter = vtkContourFilter::New();
  contourFilter->SetInputConnection(reader->GetOutputPort()); 
  contourFilter->SetValue(0, (filterVal = 0.01));
  
  reader->Delete();

  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
  mapper->SetInputConnection(contourFilter->GetOutputPort());
  

  vtkActor* uActor = vtkActor::New();
  uActor->SetMapper(mapper); 
  uActor->SetPosition(1,1,1);

  vtkTextProperty* textProp = vtkTextProperty::New();
  textProp->SetFontFamilyToCourier();
  textProp->SetFontSize(18);
  textProp->SetColor(0,0,0);

  vtkTextActor* textActor = vtkTextActor::New();
  textActor->SetTextProperty(textProp);
  textActor->SetPosition(0, 400);
  textActor->SetInput("To Increase contour value,\n press 'I'.\nTo Decrease contour value,\n press 'D'.");

  //camera A setup
  vtkCamera* camA = vtkCamera::New();
  camA->SetViewUp (0, -1, 0);
  camA->SetPosition (0, 1, 1);

  //camera B setup 
  vtkCamera* camB = vtkCamera::New();
  camB->SetViewUp (0, 0, 0);
  camB->SetPosition (0, 0, 1);

  // Create the usual rendering stuff
  vtkRenderer *renderer = vtkRenderer::New();
  renderer->SetViewport(0.0, 0.0, 0.6, 1.0);
  renderer->AddActor(uActor);  
  renderer->SetBackground(1,1,1);

  vtkRenderer* rendB = vtkRenderer::New();
  rendB->SetViewport(0.6, 0.0, 1.0, 1.0);
  rendB->SetBackground(1,1,1);
  rendB->AddActor(textActor);


  vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer(renderer);
	renWin->AddRenderer(rendB);


  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	
    iren->SetRenderWindow(renWin);

	KeyInteractorStyle* interactor = KeyInteractorStyle::New();

	iren->SetInteractorStyle(interactor);
	interactor->SetCurrentRenderer(renderer);
	interactor->setFilter(contourFilter);


  renWin->SetSize(800,600);

  // interact with data
  renWin->Render();
  iren->Initialize();
  renWin->SetWindowName("Michael Gruesen - Visualization PA 3A");
  iren->Start();


  textActor->Delete();
  uActor->Delete();
  contourFilter->Delete();
  mapper->Delete();
  camA->Delete();
  camB->Delete();
  textProp->Delete();
  renderer->Delete();
  rendB->Delete();
  renWin->Delete();
  iren->Delete();  
  uActor->Delete();

  return 0;
}

