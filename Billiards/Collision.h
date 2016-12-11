#pragma once

#include "Func.h"

struct Model_HitResult {
	vec pos;
	vec norm;
	vec hit_poly[3];
};

struct Model_HitResult_Dim {

	int hit_num;
	std::vector<struct Model_HitResult *> data;
	Model_HitResult_Dim(){
		data.reserve(DEFAULT_ALLOC);
	}

private:
	static const int DEFAULT_ALLOC = 32;

};

bool Hit_Poly_Line(vec _face[], vec _lpos1, vec _lpos2, vec *_hit_pos = NULL);