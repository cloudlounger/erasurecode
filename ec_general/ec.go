package ec_general

/*
#cgo LDFLAGS: -L${SRCDIR} -lrs
#include <rs.h>
#include <stdlib.h>
*/
import "C"
import (
	"errors"
	"unsafe"
)

func init() {

	var c int
	var d C.size_t
	if unsafe.Sizeof(c) != unsafe.Sizeof(d) {
		panic("sizeof(int) != sizeof(C.size_t)")
	}
}

var ptr uintptr

const ptrSize = int(unsafe.Sizeof(ptr))

type BadLocations struct {
	Impl *C.ErrorLocationInfo
}

func OpenBadLocations(badIdxs []int) BadLocations {

	info := C.createErrorLocationInfo((*C.size_t)(unsafe.Pointer(&badIdxs[0])), C.size_t(len(badIdxs)))
	return BadLocations{info}
}

func (p BadLocations) Close() error {

	C.deleteErrorLocationInfo(p.Impl)
	return nil
}

// -----------------------------------------

type Instance struct {
	data *C.DataSeg
	n, m int
}

func initParameter(n, m int) error {

	ret := C.initializeRS(C.size_t(n), C.size_t(m))
	if ret != 0 {
		return errors.New(C.GoString(C.errorToStr(ret)))
	}
	return nil
}

func Open(bytes, n, m int) Instance {

	N, M := n, m
	if err := initParameter(N, M); err != nil {
		panic(err)
	}
	alignSize := getAlignSize(bytes)
	var (
		data  *C.gfExp
		dataS C.DataSeg
	)
	ret := C.posix_memalign((*unsafe.Pointer)(unsafe.Pointer(&data)), 32, C.size_t((N+M)*alignSize))
	if ret != 0 {
		panic("alloc memory failed")
	}

	dataS.dataLen = C.int(N + M)
	dataS.dataSeg = (**C.gfExp)(C.malloc(C.size_t((N + M) * ptrSize)))
	if dataS.dataSeg == nil {
		panic("malloc failed")
	}
	for i := 0; i < N+M; i++ {
		*(**C.gfExp)(unsafe.Pointer(uintptr(unsafe.Pointer(&(*dataS.dataSeg))) + uintptr(i*ptrSize))) = (*C.gfExp)(unsafe.Pointer(uintptr(unsafe.Pointer(data)) + uintptr(i*alignSize)))
	}
	return Instance{data: &dataS, n: N, m: M}
}

func (p Instance) GenSyndrome(bytes int) {

	alignSize := getAlignSize(bytes)
	C.encodeRS(p.data.dataSeg, (**C.gfExp)(unsafe.Pointer(uintptr(unsafe.Pointer(&(*p.data.dataSeg)))+uintptr(p.n*ptrSize))), C.size_t(alignSize))
	return
}

func (p Instance) Recover(bytes int, bads BadLocations) error {

	alignSize := getAlignSize(bytes)
	C.decodeRS(p.data.dataSeg, bads.Impl, C.size_t(alignSize))
	return nil
}

func (p Instance) Data(bytes int) [][]byte { // bytes <= 16M

	slices := make([][]byte, p.n+p.m)
	for i := range slices {
		arr := *(**C.gfExp)(unsafe.Pointer(uintptr(unsafe.Pointer(&(*p.data.dataSeg))) + uintptr(i*ptrSize)))
		slices[i] = (*[1 << 30]byte)(unsafe.Pointer(arr))[:bytes]
	}
	return slices
}

func (p Instance) Close() error {

	C.free(unsafe.Pointer(*p.data.dataSeg))
	C.free(unsafe.Pointer(p.data.dataSeg))
	return nil
}

func getAlignSize(bytes int) int {
	return ((bytes-1)/32 + 1) * 32
}
