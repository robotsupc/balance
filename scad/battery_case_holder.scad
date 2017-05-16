include <include.scad>

module battery_case_holder() {
    bdst = bch_base_screw_dst();
    cdst = bch_case_screw_dst();
    r = bch_r();
    w = 2*(r+screw_r());
    h = 40;
    $fn = 30;
 
    module line(dst) {
        linear_extrude(height=bch_th())
        difference() {
            hull() {
                translate([-dst/2,0,0]) circle(r=r+screw_r());
                translate([+dst/2,0,0]) circle(r=r+screw_r());
            }
            translate([-dst/2,0,0]) circle(r=screw_r());
            translate([+dst/2,0,0]) circle(r=screw_r());
        }
    }
    
    
    module base() {
        translate([0,0,h])
        line(cdst);
        
        line(bdst);
        
        translate([0,0,h/2])
        cube([r*2,w,h], true);
    }
    
    rotate(90) translate([0,0,base_th()]) base();
}

//battery_case_holder();