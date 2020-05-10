package rscode

type Gf_Exp uint8

type ErrorLocationInfo struct {
	ErrorCount        int      // 错误长度
	ErrorLocationList []int    // 错误位置列表           长度 为m_nErrorCount
	Gamma             []Gf_Exp // Gamm多项式列表，       长度 为m_nErrorCount + 1
	GammaDerivative   []Gf_Exp // Gamm求导后的多项式列表  长度 为m_nErrorCount
}

var (
	g_RS_GLOBAL_N   int      = 0
	g_RS_GLOBAL_K   int      = 0
	g_RS_GLOBAL_ALL int      = 0
	g_RS_pGenerator []Gf_Exp = pGenerator[0]
)

func InitializeRS(N, K int) {
	g_RS_GLOBAL_N = N
	g_RS_GLOBAL_K = K
	g_RS_GLOBAL_ALL = N + K
	g_RS_pGenerator = pGenerator[K]

}

func CreateErrorLocationInfo(locationList []int, locationLength int) ErrorLocationInfo {
	var ret ErrorLocationInfo
	//拷贝错误列表
	ret.ErrorCount = locationLength
	ret.ErrorLocationList = make([]int, locationLength)
	for i := 0; i < locationLength; i++ {
		ret.ErrorLocationList[i] = locationList[i]
	}

	//计算gamma
	ret.Gamma = make([]Gf_Exp, ret.ErrorCount+1)
	ret.Gamma[ret.ErrorCount] = 1
	for i := 0; i < ret.ErrorCount; i++ {
		var mutlpoly Gf_Exp = gf8AlpT[g_RS_GLOBAL_ALL-1-ret.ErrorLocationList[i]]

		for j := (ret.ErrorCount - 1 - i); j < (ret.ErrorCount); j++ {
			ret.Gamma[j] ^= gf8MulT[ret.Gamma[j+1]][mutlpoly]
		}
	}

	ret.GammaDerivative = make([]Gf_Exp, ret.ErrorCount)
	for i := 0; i < ret.ErrorCount; i++ {
		var sub int = ret.ErrorCount - 1 - i

		if 1&i == 0 {
			ret.GammaDerivative[sub] = ret.Gamma[sub]
		} else {
			ret.GammaDerivative[sub] = 0
		}

	}

	return ret
}

func EncodeRS(dataSeg [][]Gf_Exp, checkSeg [][]Gf_Exp, count int) {

	for col := 0; col < count; col++ {
		for row := 0; row < g_RS_GLOBAL_K; row++ {
			checkSeg[row][col] = dataSeg[row][col]
		}
		//}

		var quotient Gf_Exp = 0

		//for col := 0; col < count; col++ {
		for row := g_RS_GLOBAL_K; row < g_RS_GLOBAL_ALL; row++ {
			quotient = gf8DivT[checkSeg[0][col]][g_RS_pGenerator[0]]

			for j := 0; j < (g_RS_GLOBAL_K - 1); j++ {
				checkSeg[j][col] = checkSeg[j+1][col] ^ gf8MulT[quotient][g_RS_pGenerator[j+1]]
			}
			var tempUC Gf_Exp = gf8MulT[quotient][g_RS_pGenerator[g_RS_GLOBAL_K]]

			if row < g_RS_GLOBAL_N {
				checkSeg[g_RS_GLOBAL_K-1][col] = dataSeg[row][col] ^ tempUC
			} else {
				checkSeg[g_RS_GLOBAL_K-1][col] = Gf_Exp(0) ^ tempUC
			}
		}

	}
}

func DecodeRS(dataSeg [][]Gf_Exp, errInfo ErrorLocationInfo, count int) {

	var omeageLen int = g_RS_GLOBAL_K + errInfo.ErrorCount
	var syndrom []Gf_Exp = make([]Gf_Exp, g_RS_GLOBAL_K)
	var omega []Gf_Exp = make([]Gf_Exp, omeageLen)

	for col := 0; col < count; col++ {
		var returnValue Gf_Exp = 0
		for i := 0; i < g_RS_GLOBAL_K; i++ {
			syndrom[g_RS_GLOBAL_K-i-1] = 0
			for j := 0; j < g_RS_GLOBAL_ALL; j++ {
				syndrom[g_RS_GLOBAL_K-1-i] ^= gf8MulT[gf8ExpT[gf8AlpT[i]][g_RS_GLOBAL_ALL-1-j]][dataSeg[j][col]]
			}
			returnValue |= syndrom[g_RS_GLOBAL_K-1-i]
		}

		// returnValue为0 没有错误值，直接越过（当错误过多也可能产生判断错误）
		if returnValue == 0 {
			continue
		}

		for i := 0; i < omeageLen; i++ {
			omega[i] = 0
		}

		var pResult int = omeageLen - 1
		var pCurrentResult int = 0
		for i := errInfo.ErrorCount + 1; i > 0; i-- {
			pCurrentResult = pResult
			for j := g_RS_GLOBAL_K; j > 0; j-- {
				omega[pCurrentResult] ^= gf8MulT[errInfo.Gamma[i-1]][syndrom[j-1]]
				pCurrentResult--
			}
			pResult--
		}

		for errsub := 0; errsub < (errInfo.ErrorCount); errsub++ {
			var alpa_inverse Gf_Exp = gf8AlpT[255-(g_RS_GLOBAL_ALL-1-(errInfo.ErrorLocationList[errsub]))]
			var omegaValue Gf_Exp = 0
			for i := 0; i < g_RS_GLOBAL_K; i++ {
				omegaValue ^= gf8MulT[omega[i+errInfo.ErrorCount]][gf8ExpT[alpa_inverse][g_RS_GLOBAL_K-1-i]]
			}
			var numerator Gf_Exp = gf8MulT[omegaValue][gf8AlpT[g_RS_GLOBAL_ALL-1-(errInfo.ErrorLocationList[errsub])]]
			var denominator Gf_Exp = 0
			for i := 0; i < (errInfo.ErrorCount); i++ {
				denominator ^= gf8MulT[errInfo.GammaDerivative[i]][gf8ExpT[alpa_inverse][errInfo.ErrorCount-1-i]]
			}

			dataSeg[errInfo.ErrorLocationList[errsub]][col] ^= gf8DivT[numerator][denominator]
		}
	}

}
