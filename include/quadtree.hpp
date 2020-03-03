#pragma once

#include <cstdlib>
#include <iostream>
#include <stdlib.h>

#define DEBUG_LEAF		false
#define DEBUG_LEAF_AREA	12

class Pixel
{
public:
	Pixel() {
		r = 0;
		g = 0;
		b = 0;
	}
	Pixel(uint8_t red, uint8_t green, uint8_t blue) {
		r = red;
		g = green;
		b = blue;
	}
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
};

/*
_________
| A | B |
|---|---|
| C | D |
`````````
*/

/*
	result: https://docs.google.com/spreadsheets/d/17MNseuZ7234wSgKaGH4x13Qxni6oD9iySb00nfcebaU/edit?usp=sharing
	this can be improved by fusing areas on a second pass in the tree
*/

class QuadNode
{
public:
	QuadNode(Pixel** arr, int x, int y, int width, int height, unsigned int threshold);
	bool		isLeaf() const;

	QuadNode **		children;
	Pixel			pixel;
	unsigned int	detail;//todo
	unsigned int	x;
	unsigned int	y;
	unsigned int	width;
	unsigned int	height;
	/*
		Obj3d *	cube;
		sera en fait un descriptif du cube et pas un ptr:
			texture
			is breakable
			...
	*/
private:
};