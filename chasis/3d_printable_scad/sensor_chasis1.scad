$fn = 128;


difference() {
			// 6.5mm above Zero pane, 8.5 below 
			// 5.0mm between board and bearing
			// 1.5mm to fit the board into
			translate([0,0,-1]) cylinder(r=25, h=15, center=true);
	union() {
		translate([0,0,3]) linear_extrude(file="cut-out.dxf", 
                                            layer="umriss", 
                                            height=6,
                                            center=true);

		translate([5,0,5+2]) cube(size=[40,37,4], center=true);
		
		translate([0,0,-1]) linear_extrude(file="cut-out.dxf",
                                            layer="holes",
                                            height=16,
                                            center=true);

		translate([0,0,-4.5]) cylinder(r=15, h=9, center=true);

		

	}
}	
