# This file use the vtk to show the 3D mummy model by using maximum intensity projection method, modified using the vol_mip.tcl file
#The GUI have to be modified using Tkinter library, since the iren.Start() and root.mainloop() both cause stall of the program, which leads to significant change in Graphic part. 
#A bug was fixed when doing save-after-rotation multiple times. The original code will get the same picture, which is incorrect

import vtk
from Tkinter import *
from vtk.tk.vtkTkRenderWindowInteractor import \
vtkTkRenderWindowInteractor



root = Tk()

############################## Read the Data#########################

reader = vtk.vtkStructuredPointsReader()
reader.SetFileName("mummy.128.vtk")

style= vtk.vtkInteractorStyleTrackballCamera()

############# Create transfer functions for opacity and color ################


opacityTransferFunction=vtk.vtkPiecewiseFunction()
opacityTransferFunction.AddPoint(0, 0.0)
opacityTransferFunction.AddPoint(75, 0.0)
opacityTransferFunction.AddPoint(95, 0.0)
opacityTransferFunction.AddPoint(110, 0.4)
opacityTransferFunction.AddPoint(120, 0.6)
opacityTransferFunction.AddPoint(145, 1.0)
opacityTransferFunction.AddPoint(255, 1.0)

colorTransferFunction=vtk.vtkColorTransferFunction()
colorTransferFunction.AddRGBPoint(75.0, 0.4, 0.4, 0.4)
colorTransferFunction.AddRGBPoint(95.0, .8, .8, .8)
colorTransferFunction.AddRGBPoint(110.0, .6, .6, .6)
colorTransferFunction.AddRGBPoint(120.0, .6, .6, .6)
colorTransferFunction.AddRGBPoint(145.0, .4, .4, .4) 
colorTransferFunction.AddRGBPoint(255.0, .2, .2, .2)




########## Create properties, mappers, volume actors, and ray cast function ##############
volumeProperty=vtk.vtkVolumeProperty()
volumeProperty.SetColor(colorTransferFunction)
volumeProperty.SetScalarOpacity(opacityTransferFunction)
volumeProperty.ShadeOn()


MIPFunction=vtk.vtkVolumeRayCastMIPFunction()

volumeMapper=vtk.vtkVolumeRayCastMapper()
volumeMapper.SetInput(reader.GetOutput())
volumeMapper.SetVolumeRayCastFunction(MIPFunction)
volumeMapper.SetSampleDistance(0.5)

############ create volume ################## 
volume=vtk.vtkVolume()
volume.SetMapper(volumeMapper)
volume.SetProperty(volumeProperty)



############################## Graphics stuff#################### 

ren1=vtk.vtkRenderer()
iren=vtkTkRenderWindowInteractor(root, height=512, width=512)
renWin=iren.GetRenderWindow()
renWin.AddRenderer(ren1)
iren.SetInteractorStyle(style)

#ren1.AddActor(outlineActor)             not used in the original given file
ren1.AddVolume(volume)
ren1.SetBackground(0, 0, 0)



def TkCheckAbort(obj, event):             #check abortion
	foo=renWin.GetEventPending()
	if(foo!=0):
		renWin.SetAbortRender(1)
	return


#observers
renWin.AddObserver("AbortCheckEvent", TkCheckAbort)


frame=0                                   #global variable


def saveFrame():                          #save image
	w2if=vtk.vtkWindowToImageFilter()
	writer=vtk.vtkPNGWriter()

	global frame
	name="frame%03d.png"%(frame)
	w2if.SetInput(renWin)
	writer.SetInput(w2if.GetOutput())
	writer.SetFileName(name)
	writer.Write()
	frame=frame+1
	save.configure(text="save frame %03d.png"%(frame))



save =Button(root, text="save frame %03d.png"%(frame),command=saveFrame)
quit =Button(root, text="Quit",command=sys.exit)
quit.pack()
save.pack()
iren.pack()


iren.Initialize()
root.mainloop()


