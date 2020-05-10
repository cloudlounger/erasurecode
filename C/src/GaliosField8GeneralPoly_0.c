/******************************************************************************* 
 *  @file      GaliosField8GeneralPoly_0.c 2013\9\12 15:15:13 $
 *  @author    Wang Xiaotao<wangxiaotao1980@gmail.com> (中文编码测试)
 ******************************************************************************/

#include "GaliosField8GeneralPoly_0.h"

/******************************************************************************/
/*2 x^0 + 3 x^1 + 1 x^2*/
const unsigned char g_generatorPoly2[3]  = {  1,   3,   2, };
/*8 x^0 + 14 x^1 + 7 x^2 + 1 x^3*/
const unsigned char g_generatorPoly3[4]  = {  1,   7,  14,   8, };
/*64 x^0 + 120 x^1 + 54 x^2 + 15 x^3 + 1 x^4*/
const unsigned char g_generatorPoly4[5]  = {  1,  15,  54, 120,  64, };
/*116 x^0 + 147 x^1 + 63 x^2 + 198 x^3 + 31 x^4 + 1 x^5*/
const unsigned char g_generatorPoly5[6]  = {  1,  31, 198,  63, 147, 116, };
/*38 x^0 + 227 x^1 + 32 x^2 + 218 x^3 + 1 x^4 + 63 x^5 + 1 x^6*/
const unsigned char g_generatorPoly6[7]  = {  1,  63,   1, 218,  32, 227,  38, };
/*117 x^0 + 68 x^1 + 11 x^2 + 164 x^3 + 154 x^4 + 122 x^5 + 127 x^6 + 1 x^7*/
const unsigned char g_generatorPoly7[8]  = {  1, 127, 122, 154, 164,  11,  68, 117, };
/*24 x^0 + 200 x^1 + 173 x^2 + 239 x^3 + 54 x^4 + 81 x^5 + 11 x^6 + 255 x^7 + 1 x^8*/
const unsigned char g_generatorPoly8[9]  = {  1, 255,  11,  81,  54, 239, 173, 200,  24, };
/*37 x^0 + 197 x^1 + 232 x^2 + 164 x^3 + 235 x^4 + 245 x^5 + 158 x^6 + 207 x^7 + 226 x^8 + 1 x^9*/
const unsigned char g_generatorPoly9[10] = {  1, 226, 207, 158, 245, 235, 164, 232, 197,  37, };
/*193 x^0 + 157 x^1 + 113 x^2 + 95 x^3 + 94 x^4 + 199 x^5 + 111 x^6 + 159 x^7 + 194 x^8 + 216 x^9 + 1 x^10*/
const unsigned char g_generatorPoly10[11] = {  1, 216, 194, 159, 111, 199,  94,  95, 113, 157, 193, };
/*160 x^0 + 116 x^1 + 144 x^2 + 248 x^3 + 162 x^4 + 219 x^5 + 123 x^6 + 50 x^7 + 163 x^8 + 130 x^9 + 172 x^10 + 1 x^11*/
const unsigned char g_generatorPoly11[12] = {  1, 172, 130, 163,  50, 123, 219, 162, 248, 144, 116, 160, };
/*97 x^0 + 213 x^1 + 127 x^2 + 92 x^3 + 84 x^4 + 7 x^5 + 31 x^6 + 220 x^7 + 118 x^8 + 67 x^9 + 119 x^10 + 68 x^11 + 1 x^12*/
const unsigned char g_generatorPoly12[13] = {  1,  68, 119,  67, 118, 220,  31,   7,  84,  92, 127, 213,  97, };
/*120 x^0 + 132 x^1 + 83 x^2 + 43 x^3 + 46 x^4 + 13 x^5 + 52 x^6 + 17 x^7 + 177 x^8 + 17 x^9 + 227 x^10 + 73 x^11 + 137 x^12 + 1 x^13*/
const unsigned char g_generatorPoly13[14] = {  1, 137,  73, 227,  17, 177,  17,  52,  13,  46,  43,  83, 132, 120, };
/*163 x^0 + 234 x^1 + 210 x^2 + 166 x^3 + 127 x^4 + 195 x^5 + 158 x^6 + 43 x^7 + 151 x^8 + 174 x^9 + 70 x^10 + 114 x^11 + 54 x^12 + 14 x^13 + 1 x^14*/
const unsigned char g_generatorPoly14[15] = {  1,  14,  54, 114,  70, 174, 151,  43, 158, 195, 127, 166, 210, 234, 163, };
/*26 x^0 + 134 x^1 + 32 x^2 + 151 x^3 + 132 x^4 + 139 x^5 + 105 x^6 + 105 x^7 + 10 x^8 + 74 x^9 + 112 x^10 + 163 x^11 + 111 x^12 + 196 x^13 + 29 x^14 + 1 x^15*/
const unsigned char g_generatorPoly15[16] = {  1,  29, 196, 111, 163, 112,  74,  10, 105, 105, 139, 132, 151,  32, 134,  26, };
/*59 x^0 + 36 x^1 + 50 x^2 + 98 x^3 + 229 x^4 + 41 x^5 + 65 x^6 + 163 x^7 + 8 x^8 + 30 x^9 + 209 x^10 + 68 x^11 + 189 x^12 + 104 x^13 + 13 x^14 + 59 x^15 + 1 x^16*/
const unsigned char g_generatorPoly16[17] = {  1,  59,  13, 104, 189,  68, 209,  30,   8, 163,  65,  41, 229,  98,  50,  36,  59, };

const unsigned char* const pGenerator[17] =
{
    0 , 0,
    g_generatorPoly2,  g_generatorPoly3,  g_generatorPoly4,  g_generatorPoly5,
    g_generatorPoly6,  g_generatorPoly7,  g_generatorPoly8,  g_generatorPoly9,
    g_generatorPoly10, g_generatorPoly11, g_generatorPoly12, g_generatorPoly13,
    g_generatorPoly14, g_generatorPoly15, g_generatorPoly16,
};
// 
// -----------------------------------------------------------------------------
