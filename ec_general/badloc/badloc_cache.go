package badloc

import (
	"sync"

	ec "qiniu.com/kodo/ec/ec_general"
)

type Cache struct {
	badLocationsCache map[int]ec.BadLocations
	mutex             sync.RWMutex
	N, M              int
}

func NewCache(n, m int) *Cache {
	return &Cache{
		badLocationsCache: make(map[int]ec.BadLocations),
		N:                 n,
		M:                 m,
	}
}

func (p *Cache) Get(badIdxs []int) ec.BadLocations {

	v := badIdxs[0]
	for i := 1; i < len(badIdxs); i++ {
		v += v*(p.N+p.M) + badIdxs[i]
	}

	p.mutex.RLock()
	badLocations, ok := p.badLocationsCache[v]
	p.mutex.RUnlock()
	if !ok {
		t := ec.OpenBadLocations(badIdxs)
		p.mutex.Lock()
		badLocations, ok = p.badLocationsCache[v]
		if !ok {
			badLocations = t
			p.badLocationsCache[v] = t
		} else {
			t.Close()
		}
		p.mutex.Unlock()
	}

	// badLocations do not need to be closed because all of them will be cached.
	return badLocations
}
