// +build ec28p4

package ec_remix

import (
	ec "qiniu.com/kodo/ec/ec_general"
)

const (
	N = 28
	M = 4
)

type Instance struct {
	ec.Instance
}

func Open(bytes int) Instance {
	return Instance{ec.Open(bytes, N, M)}
}
