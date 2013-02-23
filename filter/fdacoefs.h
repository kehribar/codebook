/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 *
 * Generated by MATLAB(R) 7.11 and the Filter Design Toolbox 4.7.1.
 *
 * Generated on: 23-Feb-2013 12:33:11
 *
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 251
 * Stable            : Yes
 * Linear Phase      : Yes (Type 1)
 * Arithmetic        : single
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * /Applications/MATLAB_R2010b.app/extern/include/tmwtypes.h 
 */
const int BL = 251;
const real32_T B[251] = {
  -2.498618723e-19,-5.124169911e-05,-0.0001006147722,-0.0001454387384,-0.000183127544,
  -0.0002112914808,-0.0002278488828,-0.0002311457065,-0.0002200788149,-0.0001942156086,
  -0.0001539004879,-0.0001003362559,-3.562829806e-05,3.722086694e-05,0.0001143767295,
  0.0001913047745,0.0002629716182,0.0003241121012,0.0003695527266, 0.000394575356,
  0.0003952994593,0.0003690557496,0.0003147212847,0.0002329852723,0.0001265154569,
  -5.242482539e-19,-0.0001399539324,-0.0002850744931,-0.0004258310655,-0.0005519784754,
  -0.000653213996,-0.0007199094398,-0.0007438728935,-0.000719084288,-0.0006423490704,
  -0.0005138117704,-0.0003372782085,-0.0001203037391, 0.000125981911,0.0003873243695,
  0.0006470623775,0.0008870848687, 0.001088980236, 0.001235310454, 0.001310929772,
   0.001304255216, 0.001208395697, 0.001022043754,0.0007500490756,0.0004036043247,
  -1.242601159e-18,-0.0004380697501,-0.0008836371708,-0.001306981081,-0.001677460852,
  -0.001965560019,-0.002145011909,-0.002194864908,-0.002101340098,-0.001859335345,
  -0.001473446144,-0.0009583967039,-0.000338808808, 0.000351723691, 0.001072240877,
   0.001776642399, 0.002416424919, 0.002943779808, 0.003314868081, 0.003493062919,
   0.003451935481, 0.003177770879, 0.002671409398, 0.001949244412, 0.001043254044,
  -2.130534155e-18,-0.001121412031,-0.002252342645,-0.003318466479,-0.004244262818,
  -0.004957894329,-0.005396182649,-0.005509368144,-0.005265324842,-0.004652928561,
  -0.003684306284,-0.002395748394,-0.0008471420733,0.0008801377262, 0.002686842345,
   0.004460842349,  0.00608327752, 0.007435553242,  0.00840681605, 0.008901479654,
   0.008846344426, 0.008196876384, 0.006942204665, 0.005108478479,  0.00276029529,
  -2.848886957e-18,-0.003035210771,-0.006178335752,-0.009239592589, -0.01201512199,
   -0.01429696102, -0.01588380896, -0.01659200899, -0.01626617089, -0.01478883065,
   -0.01208860148,-0.008146304637,-0.002998698968, 0.003260485595,  0.01048234105,
    0.01846739836,  0.02697291039,  0.03572247177,  0.04441758618,  0.05275055394,
    0.06041813642,  0.06713522971,  0.07264793664,  0.07674530149,  0.07926914841,
    0.08012148738,  0.07926914841,  0.07674530149,  0.07264793664,  0.06713522971,
    0.06041813642,  0.05275055394,  0.04441758618,  0.03572247177,  0.02697291039,
    0.01846739836,  0.01048234105, 0.003260485595,-0.002998698968,-0.008146304637,
   -0.01208860148, -0.01478883065, -0.01626617089, -0.01659200899, -0.01588380896,
   -0.01429696102, -0.01201512199,-0.009239592589,-0.006178335752,-0.003035210771,
  -2.848886957e-18,  0.00276029529, 0.005108478479, 0.006942204665, 0.008196876384,
   0.008846344426, 0.008901479654,  0.00840681605, 0.007435553242,  0.00608327752,
   0.004460842349, 0.002686842345,0.0008801377262,-0.0008471420733,-0.002395748394,
  -0.003684306284,-0.004652928561,-0.005265324842,-0.005509368144,-0.005396182649,
  -0.004957894329,-0.004244262818,-0.003318466479,-0.002252342645,-0.001121412031,
  -2.130534155e-18, 0.001043254044, 0.001949244412, 0.002671409398, 0.003177770879,
   0.003451935481, 0.003493062919, 0.003314868081, 0.002943779808, 0.002416424919,
   0.001776642399, 0.001072240877, 0.000351723691,-0.000338808808,-0.0009583967039,
  -0.001473446144,-0.001859335345,-0.002101340098,-0.002194864908,-0.002145011909,
  -0.001965560019,-0.001677460852,-0.001306981081,-0.0008836371708,-0.0004380697501,
  -1.242601159e-18,0.0004036043247,0.0007500490756, 0.001022043754, 0.001208395697,
   0.001304255216, 0.001310929772, 0.001235310454, 0.001088980236,0.0008870848687,
  0.0006470623775,0.0003873243695, 0.000125981911,-0.0001203037391,-0.0003372782085,
  -0.0005138117704,-0.0006423490704,-0.000719084288,-0.0007438728935,-0.0007199094398,
  -0.000653213996,-0.0005519784754,-0.0004258310655,-0.0002850744931,-0.0001399539324,
  -5.242482539e-19,0.0001265154569,0.0002329852723,0.0003147212847,0.0003690557496,
  0.0003952994593, 0.000394575356,0.0003695527266,0.0003241121012,0.0002629716182,
  0.0001913047745,0.0001143767295,3.722086694e-05,-3.562829806e-05,-0.0001003362559,
  -0.0001539004879,-0.0001942156086,-0.0002200788149,-0.0002311457065,-0.0002278488828,
  -0.0002112914808,-0.000183127544,-0.0001454387384,-0.0001006147722,-5.124169911e-05,
  -2.498618723e-19
};