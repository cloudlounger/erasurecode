package rscode

import (
	"math/rand"
	"testing"
	"time"
)

func BenchmarkEncodeRS28And4(b *testing.B) {
	b.StopTimer()
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

	b.StartTimer()
	EncodeRS(codeSlice, fetchSlice, Len)

}

func BenchmarkEncodeDecodeRS28And4(b *testing.B) {

	b.StopTimer()
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
	b.StartTimer()
	DecodeRS(codeAll, errParam, Len)

}
