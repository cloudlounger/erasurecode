// +build ec14p4

package badloc

import (
	"qiniu.com/kodo/ec/ec_general/badloc"
)

const (
	N = 14
	M = 4
)

type Cache struct {
	*badloc.Cache
}

func NewCache() *Cache {
	return &Cache{badloc.NewCache(N, M)}
}
