// +build ec14p10

package ec_remix

import (
	ec "qiniu.com/kodo/ec/ec_general"
)

const (
	N = 14
	M = 10
)

type Instance struct {
	ec.Instance
}

func Open(bytes int) Instance {
	return Instance{ec.Open(bytes, N, M)}
}
