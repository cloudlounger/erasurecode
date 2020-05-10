// +build ec4p1

package badloc

import (
	"qiniu.com/kodo/ec/ec_general/badloc"
)

const (
	N = 4
	M = 1
)

type Cache struct {
	*badloc.Cache
}

func NewCache() *Cache {
	return &Cache{badloc.NewCache(N, M)}
}
