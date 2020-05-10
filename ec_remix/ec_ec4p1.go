// +build ec4p1

package ec_remix

import (
	ec "qiniu.com/kodo/ec/ec_general"
)

const (
	N = 4
	M = 1
)

type Instance struct {
	ec.Instance
}

func Open(bytes int) Instance {
	return Instance{ec.Open(bytes, N, M)}
}
