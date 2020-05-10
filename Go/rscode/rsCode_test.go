package rscode

import (
	"math/rand"
	"testing"
	"time"
)

func TestInitializeRS2(t *testing.T) {
	var CodeLength int = 10
	var FetchLength int = 2

	InitializeRS(CodeLength, FetchLength)

	if CodeLength != g_RS_GLOBAL_N {
		t.Errorf("g_RS_GLOBAL_N value is %v, want %v", g_RS_GLOBAL_N, CodeLength)
	}

	if FetchLength != g_RS_GLOBAL_K {
		t.Errorf("g_RS_GLOBAL_K value is %v, want %v", g_RS_GLOBAL_K, FetchLength)
	}

	if (FetchLength + 1) != len(g_RS_pGenerator) {
		t.Errorf("array g_RS_pGenerator length is %v, want %v", len(g_RS_pGenerator), FetchLength)
	}
}

func TestInitializeRS3(t *testing.T) {
	var CodeLength int = 10
	var FetchLength int = 3

	InitializeRS(CodeLength, FetchLength)

	if CodeLength != g_RS_GLOBAL_N {
		t.Errorf("g_RS_GLOBAL_N value is %v, want %v", g_RS_GLOBAL_N, CodeLength)
	}

	if FetchLength != g_RS_GLOBAL_K {
		t.Errorf("g_RS_GLOBAL_K value is %v, want %v", g_RS_GLOBAL_K, FetchLength)
	}

	if (FetchLength + 1) != len(g_RS_pGenerator) {
		t.Errorf("array g_RS_pGenerator length is %v, want %v", len(g_RS_pGenerator), FetchLength)
	}
}

//--------------------------------------------4-2---------------------------------------------------------
func TestEncodeRS4And2(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 4
	var FetchLength int = 2

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

}

func TestEncodeDecodeRS4And2(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 4
	var FetchLength int = 2

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

	codeAll := make([][]Gf_Exp, FetchLength+CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeAll[i] = codeSlice[i]
	}
	for i := 0; i < FetchLength; i++ {
		codeAll[CodeLength+i] = fetchSlice[i]
	}

	var errCount = 2
	errList := make([]int, errCount)
	errList[0] = 0
	errList[1] = 2

	SrcList := make([][]Gf_Exp, errCount)

	for i := 0; i < errCount; i++ {
		SrcList[i] = codeAll[errList[i]]
		codeAll[errList[i]] = make([]Gf_Exp, Len)
	}

	errParam := CreateErrorLocationInfo(errList, errCount)
	DecodeRS(codeAll, errParam, Len)

	for i := 0; i < errCount; i++ {
		for j := 0; j < Len; j++ {
			if SrcList[i][j] != codeAll[errList[i]][j] {
				t.Errorf("data[%v][%v] is %v, want %v", i, j, codeAll[errList[i]][j], SrcList[i][j])
			}
		}
	}
}

//--------------------------------------------10-4---------------------------------------------------------

func TestEncodeRS10And4(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 10
	var FetchLength int = 4

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

}

func TestEncodeDecodeRS10And4(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 10
	var FetchLength int = 4

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

	codeAll := make([][]Gf_Exp, FetchLength+CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeAll[i] = codeSlice[i]
	}
	for i := 0; i < FetchLength; i++ {
		codeAll[CodeLength+i] = fetchSlice[i]
	}

	var errCount = 4
	errList := make([]int, errCount)
	errList[0] = 0
	errList[1] = 2
	errList[2] = 4
	errList[3] = 8

	SrcList := make([][]Gf_Exp, errCount)

	for i := 0; i < errCount; i++ {
		SrcList[i] = codeAll[errList[i]]
		codeAll[errList[i]] = make([]Gf_Exp, Len)
	}

	errParam := CreateErrorLocationInfo(errList, errCount)
	DecodeRS(codeAll, errParam, Len)

	for i := 0; i < errCount; i++ {
		for j := 0; j < Len; j++ {
			if SrcList[i][j] != codeAll[errList[i]][j] {
				t.Errorf("data[%v][%v] is %v, want %v", i, j, codeAll[errList[i]][j], SrcList[i][j])
			}
		}
	}
}

//--------------------------------------------28-4---------------------------------------------------------

func TestEncodeRS28And4(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 28
	var FetchLength int = 4

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

}

func TestEncodeDecodeRS28And4(t *testing.T) {
	const (
		Len = 65536
	)

	rand.Seed(time.Now().Unix())

	var CodeLength int = 28
	var FetchLength int = 4

	InitializeRS(CodeLength, FetchLength)

	codeSlice := make([][]Gf_Exp, CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeSlice[i] = make([]Gf_Exp, Len)
		for j := 0; j < Len; j++ {
			codeSlice[i][j] = Gf_Exp(rand.Int31n(256))
		}
	}

	fetchSlice := make([][]Gf_Exp, FetchLength)

	for i := 0; i < FetchLength; i++ {
		fetchSlice[i] = make([]Gf_Exp, Len)
	}

	EncodeRS(codeSlice, fetchSlice, Len)

	codeAll := make([][]Gf_Exp, FetchLength+CodeLength)

	for i := 0; i < CodeLength; i++ {
		codeAll[i] = codeSlice[i]
	}
	for i := 0; i < FetchLength; i++ {
		codeAll[CodeLength+i] = fetchSlice[i]
	}

	var errCount = 4
	errList := make([]int, errCount)
	errList[0] = 0
	errList[1] = 2
	errList[2] = 4
	errList[3] = 8

	SrcList := make([][]Gf_Exp, errCount)

	for i := 0; i < errCount; i++ {
		SrcList[i] = codeAll[errList[i]]
		codeAll[errList[i]] = make([]Gf_Exp, Len)
	}

	errParam := CreateErrorLocationInfo(errList, errCount)
	DecodeRS(codeAll, errParam, Len)

	for i := 0; i < errCount; i++ {
		for j := 0; j < Len; j++ {
			if SrcList[i][j] != codeAll[errList[i]][j] {
				t.Errorf("data[%v][%v] is %v, want %v", i, j, codeAll[errList[i]][j], SrcList[i][j])
			}
		}
	}
}

func TestCreateErrorLocationInfo1(t *testing.T) {
	var CodeLength int = 10
	var FetchLength int = 4

	InitializeRS(CodeLength, FetchLength)

	var locate = []int{0, 1, 2, 3}
	ret := CreateErrorLocationInfo(locate, len(locate))

	if ret.ErrorCount != len(locate) {
		t.Errorf("ret.ErrorCount is %v, want %v", ret.ErrorCount, len(locate))
	}

	for i := 0; i < ret.ErrorCount; i++ {
		if ret.ErrorLocationList[i] != locate[i] {
			t.Errorf("ret.ErrorLocationList[%v] is %v, want %v", i, ret.ErrorLocationList[i], locate[i])
		}
	}

}
