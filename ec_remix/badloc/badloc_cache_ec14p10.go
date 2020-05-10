// +build ec14p10

package badloc

import (
	"qiniu.com/kodo/ec/ec_general/badloc"
)

const (
	N = 14
	M = 10
)

type Cache struct {
	*badloc.Cache
}

func NewCache() *Cache {
	return &Cache{badloc.NewCache(N, M)}
}
