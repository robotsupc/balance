include <include.scad>

h = mbu_screw_dst()+2*mbu_padding()+2*screw_r();

module half2d() {
    w = mbu_size();
    module cyl(x, r) {
        translate([w,x*mbu_screw_dst()/2])
        circle(r=r);
    }
    difference() {
        union() {
            translate([w/2,0,0])
            square([w,h], true);
            hull() {
                cyl(1, screw_r()+mbu_padding());
                cyl(-1, screw_r()+mbu_padding());
            }
        }
        cyl(1,screw_r(), $fn=20);
        cyl(-1, screw_r(), $fn=20);
    }
}

module half() {
    r = mbu_r();
    translate([r,0,0])
    linear_extrude(height=mbu_th())
    half2d();
}


module join(r) {
    echo(r);
    intersection() {
        translate([r/2,0,-r/2])
        cube([r,h,r], true);
        translate([r,0,-r])
        rotate([90,0,0])
        difference() {
            cylinder(r=r, h = h, center=true);
            cylinder(r=r-mbu_th(), h = h, center=true);
        }
    }
}

module motor_base_union() {
    join(r=10);
    join(r=0);
    
    
   
    translate([0,0,-mbu_th()]) half();
    rotate([0,90,0]) half();
}

motor_base_union();