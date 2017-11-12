This code is a direct translation of the Tcl code in
ImagePlaneWidget.tcl.  It could easily be written using a nice class
to do the job but the present code should definitely make for an
illustrative example.

This example demonstrates how to use the vtkImagePlaneWidget
to probe a 3D image dataset with three orthogonal planes.
Buttons are provided to:
a) capture the render window display to a tiff file
b) x,y,z buttons reset the widget to orthonormal
    positioning, set the horizontal slider to move the
    associated widget along its normal, and set the
    camera to face the widget
c) right clicking on x,y,z buttons pops up a menu to set
    the associated widget's reslice interpolation mode
