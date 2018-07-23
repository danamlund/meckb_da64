


/* u3_75_keycap(); */

translate ([0, 10, 0]) {
    /* u3_75_keycap(); */
    translate ([-112, 40, 0]) {
        da64_space_keycap();
    }
}

module u3_75_keycap() {
    size = 66;
    height = 16;
    depth = 1;

    crosshole3x();

    translate ([-size/2, -height/2, -depth]) {
        cube ([size, height, depth]);
    }
}

module crosshole3x() {
    crosshole();
    translate([-12, 0, 0]) {
        crosshole();
    }
    translate([12, 0, 0]) {
        crosshole();
    }
}

module crosshole() {
    extra = 0.12;

    height = 4.5;
    radius = 5.5 / 2;
    difference() {
        union() {
            cylinder(height, radius, radius);
            cylinder(1, 2*radius, radius);
            strut_size = 12;
            strut_width = 2;
            strut_height = 0.5;
            translate ([-strut_size/2, -strut_width/2, 0]) {
                cube ([strut_size, strut_width, strut_height]);
            }
        }

        cross_size = 4 + extra;
        cross_width = 1 + extra;
        translate ([-cross_size / 2, -cross_width / 2, 0]) {
            cube ([cross_size, cross_width, height]);
        }
        translate ([-cross_width / 2, -cross_size / 2, 0]) {
            cube ([cross_width, cross_size, height]);
        }
    }
}

module da64_space_keycap() {
    /* da64_image(only_space=true); */

    
    def_total = 3 + 10 + 3;
    def_space_x = 5 * def_total;
    def_space_y = -4 * def_total;
    translate ([def_space_x+13, def_space_y, -1]) {
        cube([37, 13, 1]);
    }
    min_total = 1 + 0.3 + 1;
    min_depth = 0.5;
    translate ([def_space_x + 15, def_space_y + def_total - 5.5, min_depth]) {
        da64_image(size=0.3, depth=min_depth, margin=1);
        min_space_x = 5 * min_total;
        min_space_y = -4 * min_total;
        translate ([min_space_x+4, min_space_y+1, 0.1]) {
            da64_image(size=0.01, depth=0.2, margin=0.1);
        }
    }

    module da64_image(size=10, depth=1, margin=3, only_space=false) {
        if (!only_space) {
            for (x = [0 : 12]) {
                key(x, 0);
            }
            key(13, 0, mul=2);

            key(0, 1, mul=1.5);
            for (x = [1 : 12]) {
                key(x + 0.5, 1);
            }
            key(13.5, 1, mul=1.5);

            key(0, 2, mul=1.75);
            for (x = [1 : 11]) {
                key(x + 0.75, 2);
            }
            key(12.75, 2, mul=2.25);

            key(0, 3, mul=2.25);
            for (x = [1 : 11]) {
                key(x + 1.25, 3);
            }
            key(12 + 1.25, 3, mul=1.75);

            key(0, 4, mul=1.25);
            key(1 + 0.25, 4, mul=1.25);
            key(2 + 0.5, 4, mul=1.25);
            key(3 + 0.75, 4, mul=1.25);
        }
        key(4 + 1, 4, mul=4.25);
        if (!only_space) {
            key(5 + 3.75, 4, mul=1.25);
            key(6 + 4, 4, mul=1.25);
            key(7 + 4.25, 4);
            key(8 + 4.25, 4);
            key(9 + 4.25, 4);
        }

        module key(x, y, mul=1) {
            total = margin + size + margin;
            translate ([x * total, -y * total, -depth]) {
                cube ([total*mul - margin, total - margin, depth]);
            }
        }
    }
}
