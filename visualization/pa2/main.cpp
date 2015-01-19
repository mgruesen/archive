/*=========================================================================

  Program: Visualization Program 2
  Author:  Michael Gruesen
  Description: Displays a uniform grid using Isosurfaces, Cutting Planes,
  and Contour Lines.


=========================================================================*/

#include <windows.h>
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkContourFilter.h"
#include "vtkStructuredPointsReader.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCommand.h"
#include "vtkTransform.h"
#include <vtkContourWidget.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkWidgetEvent.h>
#include <vtkWidgetEventTranslator.h>

#include <vtkLookupTable.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkTextActor.h>
#include <vtkTextMapper.h>
#include <vtkTextSource.h>
#include <vtkTextProperty.h>

int main()
{
  int dimX, dimY, dimZ;
  std::cout << "please enter x dimension: ";
  std::cin >> dimX;
  std::cout << "\nplease enter y dimension: ";
  std::cin >> dimY;
  std::cout << "\nplease enter z dimension: ";
  std::cin >> dimZ;
  //setup renderers
  vtkRenderer* rendA = vtkRenderer::New();
  vtkRenderer* rendB = vtkRenderer::New();
  vtkRenderer* rendC = vtkRenderer::New();
  vtkRenderer* rendD = vtkRenderer::New();
  //setup render window
  vtkRenderWindow* renWin = vtkRenderWindow::New();
  //add renderers
  renWin->AddRenderer(rendA);
  renWin->AddRenderer(rendB);
  renWin->AddRenderer(rendC);
  renWin->AddRenderer(rendD);
  renWin->SetSize(1024, 640);
  

  //set renderA viewport
  rendA->SetBackground(1,1,1);
  rendA->SetViewport(0.0,0.5,0.5,1.0);
  //set renderB viewport
  rendB->SetBackground(1,1,1);
  rendB->SetViewport(0.5,0.5,1.0,1.0);
  //set renderC viewport
  rendC->SetBackground(1,1,1);
  rendC->SetViewport(0.0,0.0,0.5,0.5);
  //set viewport (x_min, y_min, x_max, y_max)
  rendD->SetBackground(1,1,1);
  rendD->SetViewport(0.5, 0.0, 1.0, 0.5);

  //make render interactive
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  //setup lookup table
  vtkLookupTable* lookup = vtkLookupTable::New();
  lookup->SetHueRange(0.25, 0.75);
  lookup->SetSaturationRange(0.25, 0.5);
  lookup->SetValueRange(0.0, 1.0);
  lookup->SetAlphaRange(1, 1);
  lookup->SetNumberOfColors(256);
  lookup->Build();

  //set quadric coefficients
  vtkQuadric* quad = vtkQuadric::New();
  quad->SetCoefficients(3,-2,1,1,2,3,3,2,1,1);
  //setup sample function
  vtkSampleFunction* func = vtkSampleFunction::New();
  func->SetImplicitFunction(quad);
  func->SetSampleDimensions(dimX, dimY, dimZ);

  //contour filter
  vtkContourFilter* contourFilter = vtkContourFilter::New();
  contourFilter->SetInputConnection(func->GetOutputPort());
  contourFilter->GenerateValues(5, 0, 1);

  //1st viewport - Iso surface
  vtkPolyDataMapper* isoMapper = vtkPolyDataMapper::New();
  isoMapper->SetInputConnection(contourFilter->GetOutputPort());
  isoMapper->SetLookupTable(lookup);

  vtkActor* isoActor = vtkActor::New();
  isoActor->SetMapper(isoMapper);
	
  //2nd viewport - Cutting plane
  //create plane
  vtkPlane* plane = vtkPlane::New();
  plane->SetOrigin(0,0,0);
  plane->SetNormal(1,0,1);

  //create cutter
  vtkCutter* cutter = vtkCutter::New();
  cutter->SetCutFunction(plane);
  cutter->SetInputConnection(func->GetOutputPort());
  cutter->GenerateValues(5, 0, 1);
  cutter->Update();

  vtkPolyDataMapper* cutMapper = vtkPolyDataMapper::New();
  cutMapper->SetInputConnection(cutter->GetOutputPort());
  cutMapper->SetLookupTable(lookup);

  vtkActor* cutActor = vtkActor::New();
  cutActor->SetMapper(cutMapper);

  //3rd viewport - contour lines
  vtkPlane* contPlane = vtkPlane::New();
  contPlane->SetOrigin(0,0,0);
  contPlane->SetNormal(1,1,1);

  vtkCutter* contourCutter = vtkCutter::New();
  contourCutter->SetCutFunction(contPlane);
  contourCutter->SetInputConnection(contourFilter->GetOutputPort());
  contourCutter->GenerateValues(1, 0, 1);
  contourCutter->Update();

  vtkPolyDataMapper* contourMapper = vtkPolyDataMapper::New();
  contourMapper->SetInputConnection(contourCutter->GetOutputPort());
  contourMapper->SetLookupTable(lookup);

  vtkActor* contourActor = vtkActor::New();
  contourActor->SetMapper(contourMapper);
  contourActor->GetProperty()->SetLineWidth(2);

  //4th viewport - info
  vtkTextProperty* textProp = vtkTextProperty::New();
  textProp->SetFontFamily(VTK_COURIER);
  textProp->SetFontSize(18);
  textProp->SetColor(0,0,0);
  
  vtkTextActor* textActor = vtkTextActor::New();
  textActor->SetInput("Top Left: Isosurface from Contour Filter\nTop Right: Cutting Planes\nBottom Left: Contour Lines from Contour Filter\n and Cutting Plane");
  textActor->SetTextProperty(textProp);
  textActor->SetPosition(0,200);

  //camera A setup
  vtkCamera* camA = vtkCamera::New();
  camA->SetViewUp (0, -1, 0);
  camA->SetPosition (0, 1, 1);

  //camera B setup 
  vtkCamera* camB = vtkCamera::New();
  camB->SetViewUp (0, 0, 0);
  camB->SetPosition (0, 1, 1);

  //camera C setup
  vtkCamera* camC = vtkCamera::New();
  camC->SetViewUp (0, 0, 0);
  camC->SetPosition (-2, 0, 0);

  //camera D setup
  vtkCamera* camD = vtkCamera::New();
  camD->SetViewUp(0,0,0);
  camD->SetPosition(0,0,1);
  

  //add 1st actor to renderer A
  rendA->AddActor(isoActor);
  rendA->SetActiveCamera(camA);
  rendA->ResetCamera();
  //add 2nd actor to renderer B
  rendB->AddActor(cutActor);
  rendB->SetActiveCamera(camB);
  rendB->ResetCamera();
  //add 3rd actor to renderer C
  rendC->AddActor(contourActor);
  rendC->SetActiveCamera(camC);
  rendC->ResetCamera();
  //add 4th actor to renderer D
  rendD->AddActor(textActor);
  rendD->SetActiveCamera(camD);
  rendD->ResetCamera();

  //setup interactor
  vtkInteractorStyleTrackballCamera* interactor = vtkInteractorStyleTrackballCamera::New();
  iren->SetInteractorStyle(interactor);    
  //init
  iren->Initialize();
  //change window name after init
  renWin->SetWindowName("Michael Gruesen - Visualization PA 2");
  //start
  iren->Start(); 

  //cleanup
  
  contourFilter->Delete();
  contourMapper->Delete();
  contourActor->Delete();
  textActor->Delete();
  quad->Delete();
  func->Delete();
  rendA->Delete();
  rendB->Delete();
  rendC->Delete();
  rendD->Delete();
  camA->Delete();
  camB->Delete();
  camC->Delete();
  camD->Delete();
  interactor->Delete();
  renWin->Delete();
  iren->Delete();

  return 0;
}
