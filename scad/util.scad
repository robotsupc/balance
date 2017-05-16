module rounded_square(size = [1,1] , r=0.25, corners = [true,true,true,true]) {
    difference() {
        square(size);
           
        corner(r);
    }
}

module corner(r = 1) {
    rotate(180)
    translate([1,1]*-r)
    difference() {
        square(r);
        circle(r);
    }
}


rounded_square($fn=30);