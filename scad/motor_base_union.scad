include <include.scad>

h = mbu_screw_dst()+2*mbu_padding()+2*screw_r();

module half2d(w, ns) {
    
    module c(d, r) {
        translate([w,d]) circle(r=r);
    }
    module cyl(x, r) {
        c(x*mbu_screw_dst()/2, r);
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
        
        for (i = [0:ns-1]) {
            d = mbu_screw_dst()/(ns-1)*i;
            c(mbu_screw_dst()/2-d,screw_r(), $fn=30);
        }
//        cyl(1,screw_r(), $fn=20);
//        cyl(-1, screw_r(), $fn=20);
    }
}

module half(w, ns) {
    r = mbu_r();
    translate([r,0,0])
    linear_extrude(height=mbu_th())
    half2d(w, ns);
}


module join(r) {
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

module motor_base_union(b=mbu_size_b(), m=mbu_size_m()) {
    join(r=5, $fn=20);
    join(r=0);
   
    translate([0,0,-mbu_th()])
    half(b, mbu_num_screws_b());
    rotate([0,90,0])
    half(m, mbu_num_screws_m());
}

//motor_base_union();