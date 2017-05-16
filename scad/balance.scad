include <motor_base_union.scad>
include <base.scad>
include <battery_case_holder.scad>
include <motor_base_union.scad>


module mbus() {
    sep = base_length(base_size()) + mbu_size_b()*2;
    
    translate([-sep/2,0,0])
    motor_base_union();

    translate([sep/2,0,0])
    rotate(180)
    motor_base_union();
}


module multibase(n) {
    x = 100;
    for (i = [1:n]) {
        translate([x,0])
        translate([0,40*(i-0.5-n/2)]) base("A", i);
        
        translate([-x,0])
        rotate(180)
        translate([0,40*(i-0.5-n/2)]) base("B", i);
    }
}




module fingers2d(coords, ir = screw_r(), or = screw_r()+3) {
    
    for (i = [0:len(coords)-2]) {
        difference() {
            hull() {
                translate(coords[i]) circle(r=or);
                translate(coords[i+1]) circle(r=or);
            }
            translate(coords[i]) circle(r=ir);
            translate(coords[i+1]) circle(r=ir);
        }
    }
}

module acc_holder() {
    linear_extrude(2) fingers2d([
        [mbu_screw_dst(),0],
        [0,0],
        [0,acc_dst()],
    ]);
}


module lm2596_holder() {
    linear_extrude(2) fingers2d([
        [0,0],
        [mbu_screw_dst(),0],
        [lm2596_dst_x(),lm2596_dst_y()],
    ]);
}

module esp8266_holder() {
    dim = [22.5,3.2,5];
    translate([0,0,dim[2]/2])
    difference() {
        cube(dim+[3,2,0],true);
        cube(dim,true);
    }
    
    translate([-mbu_screw_dst()/2,5.4,0])
    linear_extrude(5) fingers2d([
        [0,0],
        [mbu_screw_dst(),0],
    ], or=screw_r()+2, $fn=30);
}


module screw(l = 15, head=true) {
    $fn = 20;
    color("red") {
        if(head)
        translate([0,0,-2])
        cylinder(r=3, h = 2);
        
        cylinder(r=screw_r(), h=l);
    }
}


module dup(v) {
    mirror(v) children();
    children();
}
module show_screws() {
    
    dup([0,1,0]) dup([1,0,0])
    translate([base_length()/2,base_width()/2,base_th()]) rotate([0,180,0]) screw();
    dup([0,1,0])
    translate([0,base_width()/2,base_th()+3]) rotate([0,180,0]) screw();
}

//show_screws();

/*
battery_case_holder();
esp8266_holder();
*/

lm2596_holder($fn=30);
translate([11,13,0])
rotate(180) lm2596_holder($fn=30);
//lm2596_holder($fn=30);
//esp8266_holder();
//battery_case_holder();


!union() {
    //lm2596_holder();
    //translate([0,20,0]) acc_holder();
    
    translate([-20,0,0]) battery_case_holder();
    //translate([0,-30,0]) esp8266_holder();
}

//base("A");
