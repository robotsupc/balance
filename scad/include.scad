/*
This file contains all variables of the project that may be interesting to change
*/


// motor-base union
function mbu_padding() = 5; // radius around the screws
function mbu_size_m() = 8;
function mbu_size_b() = 11;
function mbu_r() = 0;
function mbu_th() = 3;
function mbu_screw_dst() = 17.5;
function mbu_num_screws_m() = 3;
function mbu_num_screws_b() = 3;


// base
function base_sep() = 40; // separation between top and bottom bases
function base_type() = "B"; // A or B
function base_size() = 2;
function base_length(bs=base_size()) = bs*30+10;
function base_num_extra_screws(bs=base_size()) = bs*3+1;
function base_width() = mbu_screw_dst();
function base_th() = 5;
function base_num_screws_mbu() = mbu_num_screws_b();
function base_padding() = mbu_padding();
function base_total_width() = base_width() + screw_r()*2 + base_padding();

// Battery case holder
function acc_dst() = 15.4;
function bch_base_screw_dst() = mbu_screw_dst();
function bch_case_screw_dst() = 44.5;
function bch_th() = mbu_th();
function bch_r() = 3;

// LM2596
function lm2596_dst_x() = 30;
function lm2596_dst_y() = 16;




echo("mbu screw2screw:", mbu_screw_dst());
echo("base width (screw to screw):", base_length());



// distance from screw to screw on a motor
function base_screw_dst() = 15;
// screw radius
function screw_r() = 3/2+0.2;
// distance from screw to screw on the base
// (in the wheel axis direction)
function base_screw_dst() = 100;
    