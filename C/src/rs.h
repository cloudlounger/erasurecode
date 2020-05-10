/*******************************************************************************
 *               rs.h defines the RS编码的函数
 * @file         rs.h 2013\08\30 17:40:04 $
 * @author       Wang Xiaotao<wangxiaotao1980@gmail.com>
 *******************************************************************************/

#ifndef RS_264AB086_1158_11E3_801A_A41F7251BDE1_H__
#define RS_264AB086_1158_11E3_801A_A41F7251BDE1_H__
/*******************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif

    typedef unsigned char gfExp;


    typedef struct tagErrorLocationInfo
    {
        int     m_nErrorCount;              // 错误长度
        int*    m_pErrorLocationList;       // 错误位置列表           长度 为m_nErrorCount        
        gfExp*  m_pGamma;                   // Gamm多项式列表，       长度 为m_nErrorCount + 1  
        gfExp*  m_pGammaDerivative;         // Gamm求导后的多项式列表  长度 为m_nErrorCount       
    } ErrorLocationInfo;


    int initializeRS(int N, int K);

    void encodeRS(gfExp* dataSeg[], gfExp* checkSeg[], const int count);

    ErrorLocationInfo* createErrorLocationInfo( const int* locationList
                                                           , const int locationLength);
    void deleteErrorLocationInfo(ErrorLocationInfo* errorInfo);

    void decodeRS(gfExp* pDatas[], const ErrorLocationInfo* pErInfo, const int count);


#if defined(__cplusplus)
}
#endif

/*******************************************************************************/
#endif// RS_264AB086_1158_11E3_801A_A41F7251BDE1_H__
