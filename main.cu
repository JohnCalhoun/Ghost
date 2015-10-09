#include<stdio.h>
#include<cuda.h>
#include"ghost.h"
#include"complex.h"

int main(){
	typedef unsigned int	Size;
	typedef float			Decimal;

	Size size=1000;
	ghost::ghostRender<Decimal,Size> renderer(size);

	renderer.setConstant(.285,.01);
	renderer.setCenter(1,-1);
	renderer.setRange(2,2);

	renderer.calculateJulia();
	renderer.renderJuliaImage("julia.bmp"); 

	return 0;
}
