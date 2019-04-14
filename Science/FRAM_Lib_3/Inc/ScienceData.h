#include "stm32l1xx_hal.h"

uint32_t ScienceTestData[] = {13,119,2,222,4,468,6,572,10,671,14,912,9,940,8,975,1,1086,10,1196,8,1381,7,1625,17,1811,12,2110,5,2267,17,2462,9,2612,6,2768,5,2848,14,3133,9,3183,8,3475,8,3731,1,4000,11,4197,2,4201,11,4272,12,4338,8,4575,2,4759,12,4865,12,5034,17,5202,6,5272,14,5504,8,5722,1,6018,2,6138,6,6344,9,6545,9,6593,6,6741,9,6774,2,7041,2,7296,9,7568,10,7585,7,7656,3,7695,6,7866,11,8052,6,8308,17,8415,11,8625,4,8760,9,8927,5,9156,11,9326,17,9574,11,9791,9,9899,12,10056,1,10310,4,10409,16,10527,6,10608,8,10812,4,11107,10,11347,17,11533,16,11703,14,11943,7,12181,2,12191,8,12466,13,12611,7,12873,11,12915,14,12980,17,12984,3,13274,14,13461,2,13518,13,13681,16,13937,3,14142,7,14373,16,14435,1,14579,17,14867,5,14957,4,15226,8,15394,4,15403,15,15681,9,15942,14,16156,4,16233,4,16389,16,16438,11,16482,2,16556,15,16806,17,16932,10,17073,5,17280,5,17540,6,17549,15,17757,3,18051,4,18247,9,18399,13,18667,16,18757,17,19051,9,19260,16,19405,3,19519,2,19586,9,19627,9,19660,11,19938,14,20106,5,20351,11,20428,11,20508,1,20805,2,21093,11,21269,9,21466,10,21622,5,21662,13,21787,15,22051,1,22279,2,22470,13,22478,6,22587,9,22780,14,22803,12,23022,3,23236,5,23251,7,23506,3,23794,17,24067,15,24089,3,24256,3,24323,2,24336,11,24565,1,24798,13,24994,15,25067,5,25202,4,25218,2,25294,6,25354,13,25540,5,25639,3,25805,13,26021,2,26182,6,26244,12,26361,8,26513,2,26601,11,26731,15,26835,13,26899,12,27169,4,27377,16,27638,8,27772,13,27880,16,28090,17,28130,7,28301,12,28325,3,28476,15,28757,12,28832,14,29082,10,29312,12,29422,15,29629,2,29659,17,29806,17,30074,10,30227,16,30234,8,30421,17,30664,3,30892,10,31022,1,31062,6,31150,16,31413,6,31654,16,31884,10,31957,7,32067,1,32287,1,32391,12,32495,3,32582,13,32717,6,32954,5,33124,5,33186,16,33376,7,33659,4,33862,17,34078,9,34352,12,34595,2,34682,4,34700,1,34763,14,34858,15,35084,6,35105,7,35308,6,35539,6,35550,17,35717,6,35941,17,36001,16,36224,10,36283,13,36528,5,36763,8,36875,12,37170,4,37379,8,37551,8,37851,10,38096,12,38205,6,38220,15,38503,9,38650,4,38804,4,38872,7,39128,14,39343,13,39469,12,39627,7,39817,17,39850,4,40024,7,40107,15,40218,12,40269,5,40308,15,40592,6,40599,13,40873,5,40886,17,40958,4,41030,9,41208,6,41424,11,41605,12,41768,9,42004,11,42209,5,42389,1,42468,16,42475,16,42525,14,42536,14,42738,11,42803,8,42974,3,42975,13,43075,13,43218,12,43434,9,43691,6,43743,13,43840,7,44011,16,44275,11,44491,13,44674,1,44715,11,44901,11,44953,17,45143,7,45182,2,45246,1,45496,17,45688,14,45811,3,46044,14,46089,4,46127,1,46359,8,46501,14,46553,8,46810,4,47066,4,47204,3,47211,8,47309,2,47391,6,47656,2,47910,5,48091,8,48258,12,48324,13,48551,11,48563,8,48776,14,48902,6,48985,14,49066,12,49108,15,49149,8,49314,9,49447,11,49573,6,49654,13,49880,16,49904,5,49932,3,49954,7,49956,13,50232,16,50311,9,50353,1,50484,17,50565,17,50827,3,51098,3,51128,3,51213,5,51394,10,51513,12,51672,11,51873,7,51912,3,52207,9,52361,2,52575,12,52820,3,53004,4,53215,9,53443,11,53596,14,53671,7,53871,6,54156,16,54412,1,54603,1,54853,6,54875,5,54888,10,55091,6,55269,1,55547,4,55731,16,55750,10,55800,17,55841,8,56010,16,56101,17,56320,6,56598,15,56644,7,56902,12,57120,12,57202,12,57447,5,57712,11,57946,13,58133,12,58231,6,58424,4,58617,5,58916,13,59170,3,59285,14,59315,10,59503,1,59509,17,59693,14,59698,5,59741,10,59857,17,60018,7,60146,1,60357,12,60613,11,60733,17,61031,13,61282,2,61358,3,61462,5,61647,7,61679,9,61921,1,62176,3,62379,13,62468,14,62754,15,62810,6,63022,7,63168,16,63259,8,63455,12,63700,12,63843,15,63845,6,63977,10,64139,3,64390,10,64431,3,64731,13,64737,10,64944,17,65104,17,65242,13,65279,17,65492,15,65780,10,66049,12,66206,8,66407,14,66567,14,66637,13,66671,8,66867,12,67164,16,67304,13,67368,14,67635,17,67789,3,67931,17,68055,6,68346,12,68350,11,68582,11,68826,5,68908,10,69032,3,69111,16,69341,4,69458,3,69547,11,69739,3,69856,11,70119,15,70291,15,70550,7,70699,12,70848,11,70895,15,71109,15,71254,13,71351,2,71506,12,71731,15,71935,11,72218,5,72357,8,72497,7,72636,10,72678,4,72944,6,72958,3,73021,1,73197,8,73467,16,73606,4,73863,13,74157,15,74435,6,74507,9,74580,10,74629,17,74873,1,75129,8,75359,14,75405,2,75625,14,75818,2,75966,13,76051,17,76126,13,76246,5,76418,8,76459,17,76570,13,76856,16,76974,6,77002,8,77040,13,77116,7,77370,15,77542,16,77750,9,77985,9,78007,3,78222,4,78253,11,78435,17,78576,9,78689,4,78862,10,78986,3,79017,12,79298,6,79390,8,79512,8,79555,7,79807,16,80071,15,80324,14,80608,5,80887,16,81031,3,81259,9,81539,7,81581,7,81652,6,81746,1,81993,11,82012,9,82144,5,82345,2,82356,4,82555,5,82683,4,82819,13,83046,15,83220,1,83295,3,83456,11,83501,11,83580,3,83779,6,83878,11,84021,14,84292,1,84557,14,84633,15,84659,14,84720,10,84861,10,85069,17,85133,5,85268,9,85504,15,85670,5,85865,15,85981,11,86049,17,86327,16,86457,17,86602,2,86853,13,86955,1,87153,9,87202,17,87256,11,87335,4,87344,3,87386,4,87409,12,87534,9,87764,13,87866,11,88147,13,88169,5,88267,14,88493,7,88640,17,88784,10,88829,11,89024,12,89139,13,89331,1,89410,9,89614,4,89666,6,89741,8,89820,14,89897,17,90072,9,90368,15,90515,4,90558,12,90737,4,90961,8,91185,6,91409,14,91523,7,91779,8,92053,3,92276,15,92423,16,92621,4,92879,7,93092,3,93279,8,93301,13,93371,15,93394,11,93448,4,93470,10,93595,13,93820,7,94109,16,94153,15,94224,10,94481,12,94718,5,94829,3,95129,14,95389,10,95401,10,95592,2,95640,11,95881,4,96045,3,96087,13,96358,8,96440,15,96586,11,96830,7,96968,7,96973,4,97129,13,97197,14,97403,11,97524,14,97738,5,97742,7,97788,9,97814,11,97823,3,98030,4,98046,9,98342,10,98428,10,98712,4,98791,7,98871,12,99008,8,99270,17,99370,15,99516,13,99596,10,99870,3,100013,15,100253,8,100495,11,100753,6,100809,1,101083,15,101172,4,101185,15,101402,8,101506,7,101664,6,101949,5,102157,15,102404,14,102649,1,102790,6,103020,1,103086,2,103232,4,103350,7,103366,2,103462,1,103544,11,103748,2,103990,12,104092,7,104130,10,104399,17,104445,9,104732,2,104969,6,105167,10,105362,15,105596,9,105618,8,105785,10,105884,14,105897,11,105995,9,106189,12,106365,7,106600,15,106747,2,106951,16,107129,16,107268,11,107345,12,107521,16,107743,4,107943,12,108139,5,108419,9,108486,9,108544,14,108679,10,108695,4,108826,3,109073,4,109143,4,109248,14,109472,16,109704,12,109828,11,109883,1,110054,11,110169,10,110235,12,110523,16,110801,9,110907,1,110958,13,111097,13,111101,10,111193,1,111484,9,111748,10,111969,15,112176,5,112280,9,112338,17,112413,2,112527,4,112654,9,112781,14,113054,10,113295,14,113487,2,113660,10,113858,14,114122,14,114155,17,114187,2,114358,15,114441,15,114511,16,114691,1,114919,8,115043,12,115196,1,115485,3,115628,7,115928,8,115976,1,116120,3,116420,10,116539,10,116670,1,116725,8,116915,11,117020,8,117060,6,117224,17,117351,6,117527,9,117794,10,117800,14,117913,3,117984,14,118266,11,118544,8,118673,6,118877,12,119000,1,119254,8,119425,6,119441,13,119529,2,119604,4,119863,16,119872,12,119943,9,120180,1,120202,13,120295,7,120320,6,120446,7,120569,15,120704,9,120855,17,120945,8,121034,16,121091,7,121172,16,121400,7,121417,9,121460,4,121579,4,121819,3,122044,5,122240,8,122377,1,122462,1,122728,13,122903,13,122972,3,123223,16,123344,11,123356,3,123429,9,123537,5,123600,3,123688,13,123872,14,123956,12,124109,14,124345,13,124570,5,124854,6,124882,6,124974,15,125211,9,125432,9,125682,10,125968,13,126005,5,126284,8,126526,8,126580,1,126618,8,126886,16,126912,2,126980,12,127030,4,127276,9,127378,10,127495,9,127537,10,127699,8,127826,16,127976,12,127979,1,128077,7,128259,7,128310,13,128577,15,128843,12,129021,12,129120,12,129219,6,129324,7,129462,12,129470,11,129551,4,129827,3,130118,1,130268,3,130378,15,130588,14,130688,11,130981,7,131135,7,131246,12,131477,10,131748,14,131804,3,132003,3,132094,15,132161,1,132409,14,132525,6,132823,14,133092,10,133258,7,133292,4,133319,7,133362,12,133443,14,133468,8,133484,15,133544,3,133782,12,133951,2,134039,12,134055,8,134220,10,134277,9,134557,2,134810,12,135023,8,135246,5,135274,4,135311,6,135522,7,135722,4,135936,5,136141,10,136228,5,136344,2,136390,7,136544,7,136776,6,136823,6,136849,9,137064,7,137332,10,137432,16,137525,17,137785,13,137836,1,137849,10,138129,13,138261,7,138391,2,138530,16,138713,12,138968,12,139172,4,139450,17,139695,8,139989,17,140021,2,140035,9,140046,1,140300,3,140580,16,140604,5,140851,1,140928,15,140978,2,141073,17,141269,4,141301,17,141463,7,141691,11,141894,14,142154,7,142367,4,142442,15,142551,11,142763,10,142786,15,143074,17,143098,5,143186,8,143412,6,143423,9,143538,6,143705,1,143827,2,144050,13,144276,14,144399,14,144606,10,144893,11,144896,3,145030,9,145276,12,145333,3,145354,17,145503,17,145724,8,145854,13,146055,8,146302,9,146427,6,146460,17,146657,2,146829,14,146965,8,147243,9,147315,4,147360,5,147532,13,147643,10,147827,7,148027,1,148096,13,148374,7,148594,3,148656,6,148748,13,148898,17,149020,1,149239,13,149282,12,149332,9,149518,17,149797,10,149874,5,150120,11,150345,1,150378,3,150445,6,};
	
	


	
	