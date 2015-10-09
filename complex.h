#ifndef GHOST_COMPLEX_H
#define GHOST_COMPLEX_H

#include <stdio.h>
#include <cuda.h>

#define __both__ __device__ __host__

namespace ghost{
	template<typename T>
	class Complex {
		private:
		T real;
		T imag;

		public:
		__both__ Complex(T r,T i):real(r),imag(i){};
		__both__ Complex():real(0),imag(0){};


		__both__ T Real()const{return real;};
		__both__ T Imag()const{return imag;};
		__both__ void Real(T r){real=r;};
		__both__ void Imag(T i){imag=i;};
		
		__both__ Complex operator+(const Complex&)const;
		__both__ Complex operator*(const Complex&)const;
		__both__ Complex operator-(const Complex&)const;
		__both__ T abs()const;
	};
	#include"complex.inl"
};//end namespace julia
#undef __both__
#endif
