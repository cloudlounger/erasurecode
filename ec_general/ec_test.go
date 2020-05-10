package ec_general

import (
	"bytes"
	"crypto/sha256"
	"fmt"
	"io"
	"math/rand"
	"runtime"
	"sync"
	"testing"
	"time"
)

const SUMaxLen = 16776168

func init() {
	rand.Seed(time.Now().Unix())
}

type FastRandReader struct {
}

func (p *FastRandReader) Read(b []byte) (n int, err error) {
	if len(b) < 1024 {
		for i := range b {
			b[i] = byte(rand.Int31n(256))
		}
		return len(b), nil
	}
	step := 1 << 20
	for start := 0; start < len(b); start += step {
		x := byte(rand.Int31n(256))
		end := start + step
		if end > len(b) {
			end = len(b)
		}
		for i := start; i < end; i++ {
			b[i] = x
		}
	}
	return len(b), nil
}

func doTest(t *testing.T, p Instance, suSize int, bads []int, N, M int) {
	fmt.Printf("doTest: suSize: %v, bads: %v", suSize, bads)

	r := &FastRandReader{}

	fmt.Println("fill random data")
	now := time.Now()
	data := p.Data(suSize)
	for i := 0; i < N; i++ {
		io.ReadFull(r, data[i])
	}
	fmt.Println(time.Since(now))

	fmt.Println("gensyndrome")
	now = time.Now()
	p.GenSyndrome(suSize)
	fmt.Println(time.Since(now))

	fmt.Println("fill bads")
	now = time.Now()
	ori := make([][]byte, 4)
	for i, badi := range bads {
		ori[i] = make([]byte, len(data[badi]))
		copy(ori[i], data[badi])
		io.ReadFull(r, data[badi])
	}
	fmt.Println(time.Since(now))

	fmt.Println("recover")
	now = time.Now()
	p.Recover(suSize, OpenBadLocations(bads))
	fmt.Println(time.Since(now))

	fmt.Println("compare")
	now = time.Now()
	for i, badi := range bads {
		if !bytes.Equal(ori[i], data[badi]) {
			t.Fatal("bytes not equal")
		}
	}
	fmt.Println(time.Since(now))
}

type ecTester struct {
	t      *testing.T
	p      Instance
	suSize int
	bads   []int
}

func newEcTester(t *testing.T, suSize int) *ecTester {
	p := Open(suSize, 28, 4)
	return &ecTester{
		t:      t,
		p:      p,
		suSize: suSize,
		bads:   make([]int, 0, 4),
	}
}

func (self *ecTester) test(lostCount int, N, M int) {
	self.testLost(0, lostCount, 28, 4)
}

func (self *ecTester) testLost(i int, lostCount int, N, M int) {
	if len(self.bads) == lostCount {
		doTest(self.t, self.p, self.suSize, self.bads, N, M)
		return
	}
	for k := i; k < N+M; k++ {
		self.bads = append(self.bads, k)
		self.testLost(k+1, lostCount, N, M)
		self.bads = self.bads[:len(self.bads)-1]
	}
}

func TestLost(t *testing.T) {
	tester := newEcTester(t, 32)
	fmt.Println("TestLost1 ------------------------------")
	tester.test(1, 28, 4)
	fmt.Println("TestLost2 ------------------------------")
	tester.test(2, 28, 4)
	fmt.Println("TestLost3 ------------------------------")
	tester.test(3, 28, 4)
	fmt.Println("TestLost4 ------------------------------")
	tester.test(4, 28, 4)
}

func Test16M(t *testing.T) {
	fmt.Println("Test16M ------------------------------")
	suSize := 16 * 1024 * 1024
	p := Open(suSize, 28, 4)
	defer p.Close()

	bads := []int{3, 8, 29, 30}
	doTest(t, p, suSize, bads, 28, 4)
}

var (
	Seed        = int64(2017)
	StripeCount = len(Sha1Map)
	// 优化前算法的校验块生成的sum, 共20个条带
	Sha1Map = map[int]string{
		0:  "e98862faf35a9a60681b59f172715b2960f6d600c74df601aa12a6071e3bf2b8",
		1:  "1dc7be5983c0ad3b1439adcce3e898786fe1fca1ab1a647dcc89d4d847f90bd0",
		2:  "0f13a165bbf5f082b9d67967cb48204a4befb6b98ee3233ee492e03828aeee5d",
		3:  "b5db2b3f83c30d07d0f34754656bde8cbdf1f69cec484357dfa0b0eb94b187f7",
		4:  "501d18edd3861e1f22db58c36e0a528b04750c3481e19a22547e14bb92a7bc2c",
		5:  "33eb016445cab93079d15df1ba5f01a479e5f1957a1125141476f6611ecccfd3",
		6:  "7cad3af9c71ee5d20ce56d23b1af96afe2b40df668a0fbe9478ce96a959f9b9b",
		7:  "a8b85a2698980b09fde77c076eea21894da08a0d51175cdf6a6cf20fa8bc150e",
		8:  "36f99ae5301f3888be93f67f8f8b960dc4a946f505a0c5d63377e4d7c16c2c08",
		9:  "557935fd3c1ec329834f730be4cfdb9e9b3199e5b5ca094e19c8c462cc9ec696",
		10: "7813407a7b74f69f3cc7b6c57b65b421ccb1e74aca06f16edf57c1a05e61cc9a",
		11: "03991a37b894b92f6107cd1ec7aab782d166029be9c48d2b4e5b576f17bbc6b3",
		12: "33e14dd905e6b2f1c9aeb7ef9b6ac201d1fefa06be41f94c2086cc96fa6515f0",
		13: "bc0aeb803d3033033a71cd09977e83dc23b5aeea384d14ad169509a9de08600b",
		14: "ce6eaf6bc3403f38e20ca7596ff5ab419e2a28e5e57beed6c946b8113c9de67a",
		15: "cfae35b7fcec6722ec380e928031054cd56cf0c0f56eef24fe75cc2893a3ca61",
		16: "3cc2ff89968f967a142e7cd3be8231a11ae143c345e7f774473d28f6b6eb8dc6",
		17: "3fd7f1337b3b818e67d59d405e01ba4e1118ade3f15144624d8ff33eab40b9a4",
		18: "8dfd22b6c508453b4fc8f8fe5b4beb7009417aa4fac9a55ee316d809efd90285",
		19: "52d0007b6f79e76fc82ddc4d74a397f1cc68b4f33545b6ba24d1afdd20f58eb9",
	}
	Sha2Map = map[int]string{
		0:  "d7da8b74202ed9a0d6aae8d5461ede0a3a931196e669da312fd83c5c36dd7f51",
		1:  "67f6bf6a9c787fb8b4eb22f71b667c952e6618e66156a2bca1fce60878ba22f7",
		2:  "47f9b8f7bf8f52db5470b08d4c9cda672c5312234008d71454b8d27fd8cc7c0f",
		3:  "716f174020a5ec8156cc6c6dc75a06b07b24103f5d7f8dcccb0bec59224b3d5b",
		4:  "28e3090cc28e0a7e2a8021016cd3a71b97246df5e31336c9002fbe3fceb43336",
		5:  "a2ccb732612a9077e60c07dd5871a97faaff6c18ccb68ef8c1ec762d047ad3f8",
		6:  "fadc9d8b4dc3ea05cc31101eee7b0dd9996aee5295664a476afdc82bb822b24c",
		7:  "e827ecc95312f9e15243e28339f9365f1b7d1127eb8d3169831f7c4cb2a65a34",
		8:  "e1e3b46dbc93725fb5037f23f2d88663ba28e29a1e96f7a6b14cc3829245ae85",
		9:  "9a6953cbbf8b60de0e7d73a66b585c74daa5711c29a2dd3b82ed122ee4ea07d1",
		10: "1162c3eb10c5843685af0948eb0a13e088844971d2eeafe37f56f5f64a99bad4",
		11: "1b9e824eeadfc1e70ccd33c16c69ac231387c9dd89f0dffa9bbe1e551b66db4a",
		12: "f4208292130d4d712e911c2e25045eba8a42c0d8b7dba046a62ba25902d134bf",
		13: "1f623a7d6d7a988688a760710ff59839a29e613de9330404487da441a65c1367",
		14: "bb5a4cf73b8a4591d2ef1d2d20183e79de84ecd66e2d64fbbcd61b649f6f3696",
		15: "9c7ea1cd528c277ae2ba31a125fb7cc0fb0e72562fcc03efe2c2a6088e72a82a",
		16: "c007e959fb058c3cb34089cd7846c586b96b8816dfbc25aff0737d2515fc7bb9",
		17: "725ce5d517f8d9522418c026613d559c63baa60aaf702819a2f5422b5329815c",
		18: "5f485fc4989940b6a4a8a2452fd9972e3bc571ea31b6dea400a7016383febb44",
		19: "5f713f1a5434dd873e550ed540b0ccbf7d217d7124e0194f99bc7be77fc3e6dc",
	}
)

func TestConsistency28p4(t *testing.T) {
	fmt.Println("Verity data consistency ---------")
	rand.Seed(Seed)
	r := &FastRandReader{}
	for i := 0; i < StripeCount; i++ {
		var instance = Open(SUMaxLen, 28, 4)
		verifyOneStripe(t, r, i, instance, Sha1Map, 28, 4)
	}
	fmt.Println("consistency pass ----------------")
}

func TestConsistency16p4(t *testing.T) {
	fmt.Println("Verity data consistency ---------")
	rand.Seed(Seed)
	r := &FastRandReader{}
	for i := 0; i < StripeCount; i++ {
		var instance = Open(SUMaxLen, 16, 4)
		verifyOneStripe(t, r, i, instance, Sha2Map, 16, 4)
	}
	fmt.Println("consistency pass ----------------")
}

func verifyOneStripe(t *testing.T, r *FastRandReader, index int, instance Instance, sumMap map[int]string, N, M int) {
	defer instance.Close()
	for i := 0; i < N; i++ {
		io.ReadFull(r, instance.Data(SUMaxLen)[i])
	}
	instance.GenSyndrome(SUMaxLen)
	b := make([]byte, 0)
	for i := N; i < N+M; i++ {
		b = append(b, instance.Data(SUMaxLen)[i]...)
	}
	s := fmt.Sprintf("%x", sha256.Sum256(b))
	value, _ := sumMap[index]
	if value == s {
		fmt.Println("stripe", index, "verify pass")
	} else {
		t.Fatal("stripe", index, "pairy data no equal. expect:", value, "indeed:", s)
	}
}

func BenchmarkEncode16M(b *testing.B) {
	p := Open(SUMaxLen, 28, 4)
	defer p.Close()

	r := &FastRandReader{}
	data := p.Data(SUMaxLen)
	for i := 0; i < 28; i++ {
		io.ReadFull(r, data[i])
	}

	b.SetBytes(28 * SUMaxLen)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		p.GenSyndrome(SUMaxLen)
	}
}

func BenchmarkEncode16M_ConCurrency(b *testing.B) {
	count := runtime.NumCPU()
	r := &FastRandReader{}
	Instances := make([]Instance, count)
	defer func() {
		for i := 0; i < count; i++ {
			Instances[i].Close()
		}
	}()
	queue := make(chan Instance, count)
	for i := 0; i < count; i++ {
		Instances[i] = Open(SUMaxLen, 28, 4)
		for j := 0; j < 28; j++ {
			io.ReadFull(r, Instances[i].Data(SUMaxLen)[j])
		}
		queue <- Instances[i]
	}

	b.SetBytes(28 * SUMaxLen)
	b.ResetTimer()
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			select {
			case instance := <-queue:
				instance.GenSyndrome(SUMaxLen)
				queue <- instance
			default:
				b.Fatalf("can not get instance")
			}
		}
	})
}

func BenchmarkDecode16M_1_Lost(b *testing.B) {
	benchmarkDecode16M_Lost(b, 1)
}

func BenchmarkDecode16M_2_Lost(b *testing.B) {
	benchmarkDecode16M_Lost(b, 2)
}

func BenchmarkDecode16M_3_Lost(b *testing.B) {
	benchmarkDecode16M_Lost(b, 3)
}

func BenchmarkDecode16M_4_Lost(b *testing.B) {
	benchmarkDecode16M_Lost(b, 4)
}

func BenchmarkDecode16M_1_Lost_ConCurrency(b *testing.B) {
	benchmarkDecode16M_Lost_ConCurrency(b, 1)
}

func BenchmarkDecode16M_2_Lost_ConCurrency(b *testing.B) {
	benchmarkDecode16M_Lost_ConCurrency(b, 2)
}

func BenchmarkDecode16M_3_Lost_ConCurrency(b *testing.B) {
	benchmarkDecode16M_Lost_ConCurrency(b, 3)
}

func BenchmarkDecode16M_4_Lost_ConCurrency(b *testing.B) {
	benchmarkDecode16M_Lost_ConCurrency(b, 4)
}

func benchmarkDecode16M_Lost(b *testing.B, lostNumber int) {
	p := Open(SUMaxLen, 28, 4)
	defer p.Close()

	r := &FastRandReader{}
	data := p.Data(SUMaxLen)
	for i := 0; i < 28; i++ {
		io.ReadFull(r, data[i])
	}

	p.GenSyndrome(SUMaxLen)

	bads := make([]int, lostNumber)
	for i := 0; i < lostNumber; i++ {
		bads[i] = rand.Intn(32)
	}
	b.SetBytes(28 * SUMaxLen)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		p.Recover(SUMaxLen, OpenBadLocations(bads))
	}
}

func benchmarkDecode16M_Lost_ConCurrency(b *testing.B, lostNumber int) {
	count := runtime.NumCPU()
	r := &FastRandReader{}
	Instances := make([]Instance, count)
	defer func() {
		for i := 0; i < count; i++ {
			Instances[i].Close()
		}
	}()

	queue := make(chan Instance, count)
	var g sync.WaitGroup
	for i := 0; i < count; i++ {
		g.Add(1)
		go func(idx int) {
			instance := Open(SUMaxLen, 28, 5)
			for j := 0; j < 28; j++ {
				io.ReadFull(r, instance.Data(SUMaxLen)[j])
			}
			instance.GenSyndrome(SUMaxLen)
			queue <- instance
			Instances[idx] = instance
			g.Done()
		}(i)
	}
	g.Wait()

	bads := make([]int, lostNumber)
	for i := 0; i < lostNumber; i++ {
		bads[i] = rand.Intn(32)
	}

	b.SetBytes(28 * SUMaxLen)
	b.ResetTimer()
	b.RunParallel(func(pb *testing.PB) {
		for pb.Next() {
			select {
			case instance := <-queue:
				instance.Recover(SUMaxLen, OpenBadLocations(bads))
				queue <- instance
			default:
				b.Fatalf("can not get instance to run recover")
			}
		}
	})
}
