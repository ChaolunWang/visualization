# This file use the vtk to show the 3D lobe model, modified using the deciFran.py file
import vtk

# We start by reading some data that was originally captured from
# a Cyberware laser digitizing system.
#
fran = vtk.vtkPolyDataReader()
fran.SetFileName("Cort_lobe_poly.vtk")

# We want to preserve topology (not let any cracks form). This may limit
# the total reduction possible, which we have specified at 90%.
#
deci = vtk.vtkDecimatePro()
deci.SetInputConnection(fran.GetOutputPort())
deci.SetTargetReduction(0.9)
deci.PreserveTopologyOn()

normals = vtk.vtkPolyDataNormals()
normals.SetInputConnection(fran.GetOutputPort())
normals.FlipNormalsOn()
franMapper = vtk.vtkPolyDataMapper()
franMapper.SetInputConnection(normals.GetOutputPort())
franActor = vtk.vtkActor()
franActor.SetMapper(franMapper)
franActor.GetProperty().SetColor(1.0, 0.49, 0.25)

# create a rendering window and renderer
ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
 
# create a renderwindowinteractor
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
 
# Add the actors to the renderer, set the background and size
ren.AddActor(franActor)
ren.SetBackground(1, 1, 1)
renWin.SetSize(700, 700)


cam1 = vtk.vtkCamera()                            #changed to set the camera position properly
cam1.SetClippingRange(0.01, 10000)
cam1.SetFocalPoint(100, 100, 100)
cam1.SetPosition(100, 100, 600)
cam1.SetViewUp(0, 1, 0)
ren.SetActiveCamera(cam1)

# enable user interface interactor
iren.Initialize()
renWin.Render()
iren.Start()

