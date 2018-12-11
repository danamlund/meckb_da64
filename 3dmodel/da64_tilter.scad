wall = 1;

ysize = 100;
lifty = 20;
angle = asin(lifty / ysize);

footd = 9;
foot_sep = 4;
foot_edge = 1;
foot_size = foot_edge + 2*(footd + foot_sep) + foot_edge;
foot_inset = 0.7;

wall_wide =  wall+0.3;
w = wall_wide + 2;
ww = 3*w;

//cube ([foot_size, foot_size, 1], center=true);
//tilter_foot(lifty, ww);
//tilter_foot(2);
/* translate ([foot_size + 2, 0, 0]) { */
/*     tilter(angle, wall-0.2, wall_wide, 5); */
/* } */
corner_tilter(20, 10, wall-0.2, wall_wide);

$fn=100;

module corner_tilter(size, height, wall_small, wall_wide) {
    foot_extra = 1;
    grabber_size = size/2;
    grabber_width = wall_wide * 3;
    grabber_height = 5;
    difference() {
        union() {
            hull() {
                translate ([0, 0, height]) {
                    cylinder (0.01, d=footd + foot_extra);
                }
                translate ([-size/4, size/4, 0]) {
                    rotate ([180, 0, 0]) {
                        translate ([size/2, 0, 0]) {
                            rotate ([0, 0, 90]) {
                                grabber_base(grabber_size);
                            }
                        }
                    }
                }
            }
            hull() {
                translate ([0, 0, height]) {
                    cylinder (0.01, d=footd + foot_extra);
                }
                translate ([-size/4, size/4, 0]) {
                    rotate ([180, 0, 0]) {
                        translate ([0, size/2, 0]) {
                            rotate ([0, 0, 180]) {
                                grabber_base(grabber_size);
                            }
                        }
                    }
                }
            }
        }
        translate ([0, 0, height-foot_inset+0.1]) {
            cylinder (foot_inset, d=footd);
        }
    }

    translate ([-size/4, size/4, 0]) {
        rotate ([180, 0, 0]) {
            translate ([size/2, 0, 0]) {
                rotate ([0, 0, 90]) {
                    grabber(grabber_size);
                }
            }
            translate ([0, size/2, 0]) {
                rotate ([0, 0, 180]) {
                    grabber(grabber_size);
                }
            }
        }
    }

    module grabber_base(size) {
        translate ([-size/2, 0, 0]) {
            cube ([size, grabber_width, 0.01]);
        }        
    }
    module grabber(size) {
        translate ([-size/2, 0, 0]) {
            difference() {
                cube ([size, grabber_width, grabber_height]);
                translate ([0, grabber_width/2 - wall_wide/2, 0]) {
                    hull() {
                        translate ([0, wall_wide/2 - wall_small/2, 0]) {
                            cube ([size, wall_small, 0.01]);
                        }
                        translate([0, 0, grabber_height]) {
                            cube ([size, wall_wide, 0.01]);
                        }
                    }
                }
            }
        }
    }
}








module tilter(angle, wall_small, wall_wide, height) {

    translate ([foot_size/2, foot_size/2, sin(angle) * ww/2]) {
        union() {
            hull() {
                rotate ([angle, 0, 0]) {
                    cube ([foot_size, ww, 0.01], center=true);
                }
                translate ([0, 0, -sin(angle) * ww/2]) {
                    cube ([foot_size, ww, 0.01], center=true);
                }
            }
    
            rotate ([angle, 0, 0]) {
                difference()
                {
                    hull() {
                        cube ([foot_size, ww, 0.01], center=true);
                        translate ([0, 0, height]) {
                            cube ([foot_size, w, 0.01], center=true);
                        }
                    }
                    hull() {
                        cube([foot_size, wall_small, 0.01], center=true);
                        translate ([0, 0, height]) {
                            cube([foot_size, wall_wide, 0.01], center=true);
                        }
                    }
                }
            }
        }
    }
}

module tilter_foot(h, ww) {
    foot_inset = 1;
    translate ([foot_size/2, foot_size/2, 0]) {
        hull() {
            cube ([foot_size, ww, 0.01], center=true);
            translate ([0, 0, h]) {
                cube ([foot_size, foot_size, 0.01], center=true);
            }
        }
    }
    translate([0, 0, h]) {
        difference() {
            cube ([foot_size, foot_size, 2]);
            translate ([0, 0, 2-foot_inset]) {
                foot_center1 = foot_edge + footd/2;
                foot_center2 = foot_size - foot_center1;
                translate([foot_center1, foot_center1, 0]) {
                    cylinder (foot_inset, footd/2, footd/2);
                }
                translate([foot_center2, foot_center1, 0]) {
                    cylinder (foot_inset, footd/2, footd/2);
                }
                translate([foot_center1, foot_center2, 0]) {
                    cylinder (foot_inset, footd/2, footd/2);
                }
                translate([foot_center2, foot_center2, 0]) {
                    cylinder (foot_inset, footd/2, footd/2);
                }
            }
        }
    }
}
