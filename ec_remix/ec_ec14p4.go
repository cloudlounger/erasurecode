// +build ec14p4

package ec_remix

import (
	ec "qiniu.com/kodo/ec/ec_general"
)

const (
	N = 14
	M = 4
)

type Instance struct {
	ec.Instance
}

func Open(bytes int) Instance {
	return Instance{ec.Open(bytes, N, M)}
}
