// +build ec6p3

package badloc

import (
	"qiniu.com/kodo/ec/ec_general/badloc"
)

const (
	N = 6
	M = 3
)

type Cache struct {
	*badloc.Cache
}

func NewCache() *Cache {
	return &Cache{badloc.NewCache(N, M)}
}
