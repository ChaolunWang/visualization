# This file use the vtk to show the 3D head model, modified using the head_6.tcl file
import vtk

############################## Read the Head Data#########################

reader1 = vtk.vtkStructuredPointsReader()
reader1.SetFileName("head.120.vtk")

############################## Look up Table Bone ##############################

lut_bone=vtk.vtkLookupTable()
lut_bone.SetNumberOfColors(256)
lut_bone.SetTableRange(0, 255)
lut_bone.SetHueRange(0.0, 1.0)
lut_bone.SetSaturationRange(0.0, 0.0)
lut_bone.SetValueRange(0.0, 1.0)
lut_bone.SetAlphaRange(1.0, 1.0)
lut_bone.Build()


############################## Look up Table Skin ##############################

lut_skin=vtk.vtkLookupTable()
lut_skin.SetNumberOfColors(256)
lut_skin.SetTableRange(0, 255)
lut_skin.SetHueRange(0.5, 3)
lut_skin.SetSaturationRange(0.0, 1.0)
lut_skin.SetValueRange(0.0, 1.0)
lut_skin.SetAlphaRange(0.6, 0.6)
lut_skin.Build()


############################### ImageMapToColors Module Bone #################### 
iMtC_bone=vtk.vtkImageMapToColors() 
iMtC_bone.SetInputConnection(reader1.GetOutputPort())
iMtC_bone.SetLookupTable(lut_bone)


############################## ImageMapToColors Module Skin #################### 

iMtC_skin=vtk.vtkImageMapToColors() 
iMtC_skin.SetInputConnection(reader1.GetOutputPort())
iMtC_skin.SetLookupTable(lut_skin)


########################### Contour Filter for Head Bone #######################
  
contour_bone=vtk.vtkContourFilter()
contour_bone.SetInputConnection(reader1.GetOutputPort())
contour_bone.SetNumberOfContours(1)
contour_bone.SetValue(0, 75.0)


########################### Contour Filter for Head Skin #######################
  
contour_skin=vtk.vtkContourFilter()
contour_skin.SetInputConnection(reader1.GetOutputPort())
contour_skin.SetNumberOfContours(1)
contour_skin.SetValue(0, 25.0) 
 
############################## Probe Filter for Bone ######################### 

probe_bone=vtk.vtkProbeFilter() 
probe_bone.SetInputConnection(contour_bone.GetOutputPort())
probe_bone.SetSourceConnection(iMtC_bone.GetOutputPort())

############################## Probe Filter for Skin ######################### 

probe_skin=vtk.vtkProbeFilter() 
probe_skin.SetInputConnection(contour_skin.GetOutputPort())
probe_skin.SetSourceConnection(iMtC_skin.GetOutputPort())

############################## PolyData Mapper Bone ############################## 

mapper_bone=vtk.vtkPolyDataMapper()
mapper_bone.SetInputConnection(probe_bone.GetOutputPort())
mapper_bone.SetLookupTable(lut_bone)
mapper_bone.SetColorModeToMapScalars()



############################## PolyData Mapper Skin ############################## 

mapper_skin=vtk.vtkPolyDataMapper()
mapper_skin.SetInputConnection(probe_skin.GetOutputPort())
mapper_skin.SetLookupTable(lut_skin)
mapper_skin.SetColorModeToMapScalars()


############################## ColorModeToMap Scalar######################## 

actor_bone=vtk.vtkActor() 
actor_bone.SetMapper(mapper_bone)

actor_skin=vtk.vtkActor()
actor_skin.SetMapper(mapper_skin)

########################### Actor and Graphics Stuff ###########################


############################Standard Rendering Stuff ##########################

style=vtk.vtkInteractorStyleTrackballCamera() 
ren = vtk.vtkRenderer()
renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
renWin.SetSize(800, 800)

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
iren.SetInteractorStyle(style)

ren.SetBackground(1, 1, 1)
ren.AddActor(actor_bone)
ren.AddActor(actor_skin)
ren.ResetCamera()
iren.Initialize()
renWin.Render()
iren.Start()

