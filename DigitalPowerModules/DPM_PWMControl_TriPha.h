#include "DPM_PWMControl.h"
#include "../Modules/MPY32.h"
#include "../IQMath/IQmathLib.h"
#include "../IQMath/QmathLib.h"

const uint16_t data_tri_origin[604] = {256,259,261,264,266,269,272,274,277,280,282,285,287,290,293,295,298,300,303,305,308,311,313,316,318,321,323,326,328,331,333,336,338,341,343,346,348,350,353,355,358,360,362,365,367,369,372,374,376,379,381,383,386,388,390,392,394,397,399,401,403,405,407,409,411,413,415,417,419,421,423,425,427,429,431,433,435,436,438,440,442,444,445,447,449,450,452,454,455,457,458,460,461,463,464,466,467,468,470,471,473,474,475,476,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,495,496,497,497,498,499,499,500,501,501,502,502,502,503,503,504,504,504,505,505,505,505,506,506,506,506,506,506,506,506,506,506,506,506,506,505,505,505,505,504,504,504,503,503,502,502,502,501,501,500,499,499,498,497,497,496,495,495,494,493,492,491,490,489,488,487,486,485,484,483,482,481,480,479,478,476,475,474,473,471,470,468,467,466,464,463,461,460,458,457,455,454,452,450,449,447,445,444,442,440,438,436,435,433,431,429,427,425,423,421,419,417,415,413,411,409,407,405,403,401,399,397,394,392,390,388,386,383,381,379,376,374,372,369,367,365,362,360,358,355,353,350,348,346,343,341,338,336,333,331,328,326,323,321,318,316,313,311,308,305,303,300,298,295,293,290,287,285,282,280,277,274,272,269,266,264,261,259,256,253,251,248,246,243,240,238,235,232,230,227,225,222,219,217,214,212,209,207,204,201,199,196,194,191,189,186,184,181,179,176,174,171,169,166,164,162,159,157,154,152,150,147,145,143,140,138,136,133,131,129,126,124,122,120,118,115,113,111,109,107,105,103,101,99,97,95,93,91,89,87,85,83,81,79,77,76,74,72,70,68,67,65,63,62,60,58,57,55,54,52,51,49,48,46,45,44,42,41,39,38,37,36,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,17,16,15,15,14,13,13,12,11,11,10,10,10,9,9,8,8,8,7,7,7,7,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,8,8,8,9,9,10,10,10,11,11,12,13,13,14,15,15,16,17,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,36,37,38,39,41,42,44,45,46,48,49,51,52,54,55,57,58,60,62,63,65,67,68,70,72,74,76,77,79,81,83,85,87,89,91,93,95,97,99,101,103,105,107,109,111,113,115,118,120,122,124,126,129,131,133,136,138,140,143,145,147,150,152,154,157,159,162,164,166,169,171,174,176,179,181,184,186,189,191,194,196,199,201,204,207,209,212,214,217,219,222,225,227,230,232,235,238,240,243,246,248,251,253,256,259,261,264};
