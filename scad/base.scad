include <include.scad>


module base2d(type, size) {
    bl = base_length(size);
    r = base_padding() + screw_r();
    
    module c(x, y, r) {
        translate([x*bl/2,y*base_width()/2])
        circle(r=r);
    }
    
    module cc(x, y, r) {
        translate([x-bl/2,y-base_width()/2])
        circle(r=r);
    }
    
    module screws() {
        x = base_num_extra_screws(size);
        y = base_num_screws_mbu();
        l = bl / (x-1);
        w = base_width() / (y-1);
        for (i = [0:x-1]) {
            for (j = [0:y-1]) {
                if (i != floor(x/2) || type == "A")
                cc(x = l*i, y = w*j, r = screw_r(), $fn=20);
            }
        }
        
        // big gaps
        if (type == "B")
        for (i = [0:floor(x/3)-1]) {
            hull() {
                cc(x = l*i*3+l*2, y = 0, r = screw_r(), $fn=20);
                cc(x = l*i*3+l, y = 0, r = screw_r(), $fn=20);
                cc(x = l*i*3+l*2, y = base_width(), r = screw_r(), $fn=20);
                cc(x = l*i*3+l, y = base_width(), r = screw_r(), $fn=20);
            } 
        }
    }
    
    difference() {
        hull() {
            c(1,1, r); c(1,-1, r); c(-1,1, r); c(-1,-1, r);
        }
        screws();
    }
}

module base(type=base_type(), size=base_size()) {
    linear_extrude(height=base_th()) base2d(type, size);
}
