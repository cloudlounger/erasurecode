package rscode

import (
	"testing"
)

func Test_generatorPoly2(t *testing.T) {

	var baseUnit = [3]Gf_Exp{1, 3, 2}

	for sub, value := range pGenerator[2] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[2][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly3(t *testing.T) {

	var baseUnit = []Gf_Exp{1, 7, 14, 8}

	for sub, value := range pGenerator[3] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[3][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly4(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 15, 54, 120, 64}

	for sub, value := range pGenerator[4] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[4][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly5(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 31, 198, 63, 147, 116}

	for sub, value := range pGenerator[5] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[5][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly6(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 63, 1, 218, 32, 227, 38}

	for sub, value := range pGenerator[6] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[6][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly7(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 127, 122, 154, 164, 11, 68, 117}

	for sub, value := range pGenerator[7] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[7][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly8(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 255, 11, 81, 54, 239, 173, 200, 24}

	for sub, value := range pGenerator[8] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[8][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly9(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 226, 207, 158, 245, 235, 164, 232, 197, 37}

	for sub, value := range pGenerator[9] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[9][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly10(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 216, 194, 159, 111, 199, 94, 95, 113, 157, 193}

	for sub, value := range pGenerator[10] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[10][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly11(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 172, 130, 163, 50, 123, 219, 162, 248, 144, 116, 160}

	for sub, value := range pGenerator[11] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[11][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly12(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 68, 119, 67, 118, 220, 31, 7, 84, 92, 127, 213, 97}

	for sub, value := range pGenerator[12] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[12][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly13(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 137, 73, 227, 17, 177, 17, 52, 13, 46, 43, 83, 132, 120}

	for sub, value := range pGenerator[13] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[13][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly14(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 14, 54, 114, 70, 174, 151, 43, 158, 195, 127, 166, 210, 234, 163}

	for sub, value := range pGenerator[14] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[14][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly15(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 29, 196, 111, 163, 112, 74, 10, 105, 105, 139, 132, 151, 32, 134, 26}

	for sub, value := range pGenerator[15] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[15][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}

func Test_generatorPoly16(t *testing.T) {

	var baseUnit = [...]Gf_Exp{1, 59, 13, 104, 189, 68, 209, 30, 8, 163, 65, 41, 229, 98, 50, 36, 59}

	for sub, value := range pGenerator[16] {
		if baseUnit[sub] != value {
			t.Errorf("pGenerator[16][%v] value is %v , want is %v", sub, value, baseUnit[sub])
		}
	}
}
