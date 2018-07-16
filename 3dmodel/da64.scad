hole = 14;
depth = 2;
key_distance = 5;
height = 15;
wall = 1;

pcb_margin = 0.3;
crinch_z = 1;
crinch = 0.2;

/* intersection() { */
rotate(a=[180, 0, 0]) {
    /* intersection() { */
    /* kb_da64(); */
    /* kb_da64_left(); */
    kb_da64_right();
    /* { */
    /*     size = key_distance + hole; */
    /*     clearance = 10; */
    /*     translate ([size * 10, 0, -height + clearance - depth]) { */
    /*         atm32u4_deeprobot_micro(height - clearance, yoffset=-0.5, ytoptrim=0); */
    /*     } */
    /* } */

    /* micro_usb(); */
}

/* translate ([+110, -25, -2]) { */
/*     cube ([45, 30, 25]); */
/* } */
/* } */

/* rotate(a=[180, 0, 0]) { */
/* /\* micro_usb(); *\/ */
    /* micro_usb(true, 10); */
/* } */


module micro_usb(holder=false, height) {
    x = 13 + pcb_margin;
    y = 14 + pcb_margin;
    z = 1.5 + pcb_margin;
    usb_x = 8 + pcb_margin;
    usb_y = 6 + pcb_margin;
    usb_y2 = 2 + pcb_margin;
    usb_z = 3 + pcb_margin;
    usb_dz = 0.5;
    holder_z = 4;

    translate ([0, -hole -0.5, 0]) {
        if (!holder) {
            board();
        } else {
            wall = 1;

            difference() {
                translate ([-wall, -wall - key_distance, - crinch_z]) {
                    cube ([x + 2*wall, y + 2*wall + key_distance, height + crinch_z]);
                }
                translate ([-wall, -wall, holder_z]) {
                    cube ([x + 2*wall, y + 2*wall, height - holder_z]);
                }
                translate ([wall, 0, 0]) {
                    cube ([x - 2*wall, y - wall, height]);
                }
                board();

                translate ([crinch, crinch, -crinch_z]) {
                    cube ([x - 2*crinch, y - 2*crinch, crinch_z]);
                }
            }
        }
    }

    module board() {
        cube ([x, y, z]);
        translate ([x/2 - usb_x/2, y - usb_y + usb_y2, z - usb_dz]) {
            cube ([usb_x, usb_y, usb_z]);
        }
    }
}


module atm32u4_deeprobot_micro(support_depth, yoffset=0, ytoptrim=0) {
    height = 48 + pcb_margin;
    width = 18 + pcb_margin;
    depth = 1.5 + pcb_margin;
    topdepth = 3.5;
    botdepth = 2.5;
    holesize = 2.5;
    microusb_width = 8;
    microusb_depth = 2;
    microusb_outhand = 2;
    support_size = 2*holesize;

    difference() {
        difference() {
            union() {
                support();
                translate ([0, width, 0]) {
                    support();
                }
                translate ([height, 0, 0]) {
                    support();
                }
                translate ([height, width, 0]) {
                    support();
                }
            }

            translate ([0, yoffset, crinch_z]) {
                cube ([height, width, depth]);

                translate ([0, holesize, -botdepth]) {
                    cube ([height, width - 2*holesize, botdepth + depth + topdepth]);
                }

                translate ([height, width / 2 - microusb_width / 2, depth]) {
                    cube ([microusb_outhand, microusb_width, microusb_depth]);
                }

                translate ([crinch, crinch, -crinch_z]) {
                    cube ([height - 2*crinch, width - 2*crinch, crinch_z]);
                }
            }
        }
        
        translate ([-support_size/2, width + support_size / 2 - ytoptrim, 0]) {
            cube ([height + support_size, ytoptrim, support_depth]);
        }
    }

    module support() {
        translate ([-support_size/2, -support_size/2, 0]) {
            cube ([support_size, support_size, support_depth]);
        }
    }
}

module kb_da64_left(genderswap=false) {
    female_margin = genderswap ? 0 : 0.2;
    male_margin = genderswap ? 0.2 : 0;
    
    difference() {
        intersection() {
            kb_da64();
            size = (hole + key_distance);
            translate ([0, 0, -height]) {
                cube ([7 * size, size, height]);
                translate ([0, -size, 0]) {
                    cube ([7.5 * size, size, height]);
                }
                translate ([0, -2*size, 0]) {
                    cube ([7.75 * size, size, height]);
                }
                translate ([0, -3*size, 0]) {
                    cube ([8.25 * size, size, height]);
                }
                translate ([0, -5*size, 0]) {
                    cube ([8.25 * size, 2*size, height]);
                }
            }
        }
        kb_split_connector(7.5, 1, female=true, margin=female_margin);
        kb_split_connector(8.25, 3, female=true, margin=female_margin);
    }
    kb_split_connector(7, 0, margin=male_margin);
    kb_split_connector(7.75, 2, margin=male_margin);
    kb_split_connector(8.25, 4, margin=male_margin);
}

module kb_da64_right() {
    intersection() {
        kb_da64();
        size = (hole + key_distance);
        difference() {
            translate ([0, -5*size, -height]) {
                cube ([16 * size, 6 * size, height]);
            }
            kb_da64_left(genderswap=true);
        }
    }
}

module kb_split_connector(x, y, female=false, margin=0) {
    size = (hole + key_distance);
    translate ([x * size, -y * size, 0]) {
        xsize = key_distance/3 + margin;
        ysize = size/4 + margin;
        zsize = depth/2 + margin;
        x = female ? -xsize : 0;
        translate ([x, size/2 - ysize/2, -zsize]) {
            cube ([xsize, ysize, zsize]);
        }
    }
}

module kb_da64() {
    size = (hole + key_distance);
    usb_clearance = height - 10;
    chip_clearance = height - 11;
    usb_x = size * 5.5 + key_distance/2;
    strip_width = 2 + pcb_margin;
    strip_holes = 2.5;
    
    // row 0
    for (x = [0 : 12]) {
        key_xy(x, 0);
    }
    key_xy(13, 0, u=2);
    difference() {
        case(15, 0, down=false);
        translate ([usb_x, size - wall, -height + usb_clearance - depth]) {
            micro_usb(false);
        }
    }
    for (x = [1 : 2 : 9]) {
        leg(x, 0);
    }
    leg(13, 0);

    // row 1
    key_xy(0, 1, u=1.5);
    for (x = [1 : 12]) {
        key_xy(x, 1, xoffset=0.5);
    }
    key_xy(13, 1, u=1.5, xoffset=0.5);
    case(15, 1, down=false, up=false);
    for (x = [1 : 2 : 13]) {
        leg(0.5 + x, 1);
    }

    // row 2
    key_xy(0, 2, u=1.75);
    for (x = [1 : 11]) {
        key_xy(x, 2, xoffset=0.75);
    }
    key_xy(12, 2, u=2.25, xoffset=0.75);
    case(15, 2, down=false, up=false);
    for (x = [1 : 2 : 10]) {
        leg(0.75 + x, 2);
    }
    leg(12.75, 2);

    // row 3
    key_xy(0, 3, u=2.25);
    for (x = [1 : 11]) {
        key_xy(x, 3, xoffset=1.25);
    }
    key_xy(12, 3, u=1.75, xoffset=1.25);
    case(15, 3, down=false, up=false);
    for (x = [1 : 2 : 12]) {
        leg(1.25 + x, 3);
    }

    // row 4
    key_xy(0, 4, u=1.25);
    key_xy(1, 4, u=1.25, xoffset=0.25);
    key_xy(2, 4, u=1.25, xoffset=0.5);
    key_xy(3, 4, u=1.25, xoffset=0.75);
    key_xy(4, 4, u=3.75, xoffset=1.0);
    key_xy(5, 4, u=1.25, xoffset=3.75);
    key_xy(6, 4, u=1.25, xoffset=4.0);
    key_xy(7, 4, xoffset=4.25);
    key_xy(8, 4, xoffset=4.25);
    key_xy(9, 4, xoffset=4.25);

    // fill in extra space
    translate ([14.25 * size, -4 * size, -depth]) {
        cube ([0.75 * size, size, depth]);
    }
    
    case(15, 4, up=false, down=false);
    difference() {
        case(15, 4.15, up=false);
        // rgb strip
        stutterfix = 0.01;
        translate ([wall, -4.15 * size - stutterfix, -height]) {
            cube ([strip_holes * size, wall + stutterfix, height]);
        }
    }
    // make room for spacebar stabilizer
    translate ([0, -4.15 * size, -depth]) {
        cube ([15 * size, 0.15 * size, depth]);
    }
    

    // pro micro
    {
        translate ([size * 10, 0, -height + chip_clearance - depth]) {
            atm32u4_deeprobot_micro(height - chip_clearance, yoffset=-0.5, ytoptrim=2);
        }
    }

    // micro usb connector
    {
        translate ([usb_x, size - wall, -height + usb_clearance - depth]) {
            /* micro_usb(); */
            micro_usb(true, height - usb_clearance);
        }
    }

    // rgb strip
    {
        translate ([0, -4.15 * size, -height]) {
            translate ([0, -strip_width, 0]) {
                difference() {
                    translate ([0, -wall, 0]) {
                        cube ([15 * size, strip_width + wall, height]);
                    }
                    translate ([wall, 0, wall]) {
                        cube ([15 * size - 2*wall, strip_width, height - depth - wall]);
                    }
                    translate ([wall, 0, -wall]) {
                        cube ([strip_holes * size, strip_width + wall, height - depth]);
                    }
                }
            }
        }
    }
}

module key_xy(x, y, xoffset=0, u=1) {
    size = hole + key_distance;
    xoffset2 = (xoffset == 0) ? 0 : (size * xoffset);

    translate ([xoffset2 + x * size, -y * size, 0]) {
        if (u >= 2) {
            key2u(u);
        } else {
            key(u);
        }
    }
}

module key(mul = 1) {
    translate ([0, 0, -depth]) {
        difference() {
            size = (key_distance + hole) * mul;
            cube ([size, hole + key_distance, depth]);

            translate ([size / 2 - hole / 2, key_distance/2, 0]) {
                cube ([hole, hole, depth]);
            }
        }
    }
}

module key2u(mul=2) {
    size = (hole + key_distance) * mul;
    hole_stabilizer_distance = 1.5;

    translate ([0, 0, -depth]) {
        difference() {
            cube ([size, hole + key_distance, depth]);

            translate ([size/2 - hole/2, key_distance/2, 0]) {
                cube ([hole, hole, depth]);

                translate ([-hole_stabilizer_distance, 0, 0]) {
                    stabilizer(left=true);
                }
                translate ([hole + hole_stabilizer_distance, 0, 0]) {
                    stabilizer(left=false);
                }
            }
        }
    }

    module stabilizer(left=true) {
        h = 12;
        w = 7;
        extraw = 1.5;
        extrah = 3;

        x = left ? -w : 0;
        translate ([x, 0, 0]) {
            cube ([w, h, depth]);
            translate ([0, h - 3 - extrah, 0]) {
                translate ([-extraw, 0, 0]) {
                    cube ([extraw, extrah, depth]);
                }
                translate ([w, 0, 0]) {
                    cube ([extraw, extrah, depth]);
                }
            }
            translate ([2, -extraw, 0]) {
                cube ([extrah, extraw, depth]);
            }
        }
    }
}

module case(w, y, left=true, right=true, up=true, down=true) {
    size = (hole + key_distance) * w;
    translate ([0, (hole + key_distance) * -y, -height]) {
        difference() {
            cube ([(hole + key_distance) * w, hole + key_distance, height]);

            translate ([wall, wall, 0]) {
                cube ([size - 2*wall, hole + key_distance - 2*wall, height]);
            }
            if (!left) {
                translate ([0, wall, 0]) {
                    cube ([wall, hole + key_distance - 2*wall, height]);
                }
            }
            if (!right) {
                translate ([size - wall, wall, 0]) {
                    cube ([wall, hole + key_distance - 2*wall, height]);
                }
            }
            if (!down) {
                translate ([wall, 0, 0]) {
                    cube ([size - 2*wall, wall, height]);
                }
            }
            if (!up) {
                translate ([wall, hole + key_distance - wall, 0]) {
                    cube ([size - 2*wall, wall, height]);
                }
            }
        }
    }
}

module leg(x, y) {
    translate ([x * (hole + key_distance), -y * (hole + key_distance), -height]) {
        translate ([-key_distance/2, -key_distance/2, 0]) {
            cube ([ key_distance, key_distance, height]);
        }
    }
}
