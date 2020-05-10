// +build ec6p3

package ec_remix

import (
	ec "qiniu.com/kodo/ec/ec_general"
)

const (
	N = 6
	M = 3
)

type Instance struct {
	ec.Instance
}

func Open(bytes int) Instance {
	return Instance{ec.Open(bytes, N, M)}
}
