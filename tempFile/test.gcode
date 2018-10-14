;(****Build and Slicing Parameters****)
;(Pix per mm X            = 11.16279 px/mm )
;(Pix per mm Y            = 11.19171 px/mm )
;(X Resolution            = 3840 )
;(Y Resolution            = 2160 )
;(Layer Thickness         = 0.05000 mm )
;(Layer Time              = 8000 ms )
;(Render Outlines         = False
;(Outline Width Inset     = 2
;(Outline Width Outset    = 0
;(Bottom Layers Time      = 5000 ms )
;(Number of Bottom Layers = 7 )
;(Blanking Layer Time     = 7200 ms )
;(Build Direction         = Bottom_Up)
;(Lift Distance           = 7 mm )
;(Slide/Tilt Value        = 1)
;(Anti Aliasing           = False)
;(Use Mainlift GCode Tab  = False)
;(Anti Aliasing Value     = 8 )
;(Z Lift Feed Rate        = 150.00000 mm/s )
;(Z Bottom Lift Feed Rate = 150.00000 mm/s )
;(Z Lift Retract Rate     = 150.00000 mm/s )
;(Flip X                  = False)
;(Flip Y                  = False)
;Number of Slices        =  638
;(****Machine Configuration ******)
;(Platform X Size         = 344mm )
;(Platform Y Size         = 193mm )
;(Platform Z Size         = 300mm )
;(Max X Feedrate          = 100mm/s )
;(Max Y Feedrate          = 100mm/s )
;(Max Z Feedrate          = 100mm/s )
;(Machine Type            = UV_DLP)
;********** Header Start ********
;Here you can set any G or M-Code which should be executed BEFORE the build process
G21 ;Set units to be mm
G91 ;Relative Positioning
M17 ;Enable motors
;********** Header End **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 0 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 1 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 2 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 3 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 4 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 5 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 6 
;<Delay> 5000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 7 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 8 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 9 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 10 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 11 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 12 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 13 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 14 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 15 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 16 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 17 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 18 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 19 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 20 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 21 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 22 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 23 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 24 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 25 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 26 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 27 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 28 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 29 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 30 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 31 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 32 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 33 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 34 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 35 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 36 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 37 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 38 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 39 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 40 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 41 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 42 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 43 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 44 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 45 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 46 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 47 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 48 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 49 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 50 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 51 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 52 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 53 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 54 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 55 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 56 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 57 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 58 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 59 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 60 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 61 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 62 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 63 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 64 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 65 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 66 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 67 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 68 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 69 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 70 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 71 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 72 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 73 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 74 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 75 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 76 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 77 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 78 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 79 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 80 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 81 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 82 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 83 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 84 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 85 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 86 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 87 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 88 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 89 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 90 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 91 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 92 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 93 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 94 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 95 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 96 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 97 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 98 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 99 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 100 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 101 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 102 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 103 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 104 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 105 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 106 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 107 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 108 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 109 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 110 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 111 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 112 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 113 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 114 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 115 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 116 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 117 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 118 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 119 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 120 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 121 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 122 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 123 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 124 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 125 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 126 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 127 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 128 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 129 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 130 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 131 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 132 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 133 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 134 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 135 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 136 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 137 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 138 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 139 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 140 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 141 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 142 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 143 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 144 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 145 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 146 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 147 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 148 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 149 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 150 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 151 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 152 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 153 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 154 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 155 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 156 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 157 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 158 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 159 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 160 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 161 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 162 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 163 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 164 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 165 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 166 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 167 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 168 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 169 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 170 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 171 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 172 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 173 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 174 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 175 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 176 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 177 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 178 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 179 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 180 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 181 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 182 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 183 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 184 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 185 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 186 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 187 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 188 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 189 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 190 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 191 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 192 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 193 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 194 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 195 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 196 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 197 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 198 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 199 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 200 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 201 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 202 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 203 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 204 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 205 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 206 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 207 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 208 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 209 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 210 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 211 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 212 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 213 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 214 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 215 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 216 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 217 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 218 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 219 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 220 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 221 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 222 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 223 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 224 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 225 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 226 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 227 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 228 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 229 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 230 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 231 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 232 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 233 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 234 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 235 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 236 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 237 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 238 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 239 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 240 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 241 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 242 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 243 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 244 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 245 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 246 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 247 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 248 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 249 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 250 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 251 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 252 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 253 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 254 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 255 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 256 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 257 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 258 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 259 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 260 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 261 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 262 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 263 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 264 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 265 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 266 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 267 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 268 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 269 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 270 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 271 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 272 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 273 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 274 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 275 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 276 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 277 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 278 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 279 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 280 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 281 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 282 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 283 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 284 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 285 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 286 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 287 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 288 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 289 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 290 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 291 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 292 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 293 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 294 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 295 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 296 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 297 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 298 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 299 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 300 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 301 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 302 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 303 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 304 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 305 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 306 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 307 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 308 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 309 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 310 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 311 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 312 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 313 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 314 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 315 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 316 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 317 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 318 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 319 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 320 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 321 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 322 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 323 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 324 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 325 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 326 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 327 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 328 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 329 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 330 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 331 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 332 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 333 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 334 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 335 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 336 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 337 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 338 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 339 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 340 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 341 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 342 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 343 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 344 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 345 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 346 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 347 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 348 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 349 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 350 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 351 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 352 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 353 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 354 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 355 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 356 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 357 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 358 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 359 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 360 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 361 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 362 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 363 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 364 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 365 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 366 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 367 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 368 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 369 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 370 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 371 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 372 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 373 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 374 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 375 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 376 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 377 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 378 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 379 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 380 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 381 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 382 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 383 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 384 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 385 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 386 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 387 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 388 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 389 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 390 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 391 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 392 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 393 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 394 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 395 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 396 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 397 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 398 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 399 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 400 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 401 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 402 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 403 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 404 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 405 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 406 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 407 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 408 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 409 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 410 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 411 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 412 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 413 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 414 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 415 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 416 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 417 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 418 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 419 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 420 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 421 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 422 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 423 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 424 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 425 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 426 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 427 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 428 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 429 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 430 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 431 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 432 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 433 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 434 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 435 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 436 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 437 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 438 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 439 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 440 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 441 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 442 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 443 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 444 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 445 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 446 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 447 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 448 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 449 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 450 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 451 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 452 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 453 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 454 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 455 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 456 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 457 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 458 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 459 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 460 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 461 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 462 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 463 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 464 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 465 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 466 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 467 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 468 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 469 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 470 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 471 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 472 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 473 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 474 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 475 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 476 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 477 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 478 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 479 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 480 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 481 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 482 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 483 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 484 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 485 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 486 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 487 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 488 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 489 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 490 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 491 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 492 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 493 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 494 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 495 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 496 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 497 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 498 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 499 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 500 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 501 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 502 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 503 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 504 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 505 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 506 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 507 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 508 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 509 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 510 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 511 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 512 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 513 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 514 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 515 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 516 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 517 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 518 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 519 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 520 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 521 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 522 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 523 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 524 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 525 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 526 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 527 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 528 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 529 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 530 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 531 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 532 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 533 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 534 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 535 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 536 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 537 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 538 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 539 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 540 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 541 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 542 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 543 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 544 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 545 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 546 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 547 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 548 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 549 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 550 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 551 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 552 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 553 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 554 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 555 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 556 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 557 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 558 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 559 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 560 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 561 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 562 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 563 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 564 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 565 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 566 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 567 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 568 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 569 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 570 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 571 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 572 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 573 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 574 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 575 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 576 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 577 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 578 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 579 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 580 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 581 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 582 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 583 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 584 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 585 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 586 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 587 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 588 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 589 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 590 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 591 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 592 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 593 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 594 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 595 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 596 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 597 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 598 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 599 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 600 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 601 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 602 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 603 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 604 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 605 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 606 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 607 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 608 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 609 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 610 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 611 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 612 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 613 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 614 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 615 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 616 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 617 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 618 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 619 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 620 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 621 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 622 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 623 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 624 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 625 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 626 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 627 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 628 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 629 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 630 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 631 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 632 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 633 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 634 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 635 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 636 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Pre-Slice Start ********
;Set up any GCode here to be executed before a lift
M106 S25
;********** Pre-Slice End **********
;<Slice> 637 
;<Delay> 8000 
;<Slice> Blank 
;********** Lift Sequence ********
M107
G1 X1.0 Z7.0 F150.0
G1 X-1.0 Z-6.95 F150.0
;<Delay> 7200
;********** Lift Sequence **********
;********** Footer Start ********
;Here you can set any G or M-Code which should be executed after the last Layer is Printed
G1 Z10 F150
M18 ;Disable Motors
M107
;<Completed>
;********** Footer End ********
