#include"julia.h"
#define __both__ __device__ __host__

template<typename T, typename S>
void JuliaBase<T,S>::setPixels(S p){
	pixels=p;
};
template<typename T, typename S>
void JuliaBase<T,S>::setDimensions(S h,S w){
	height=h;
	width=w;
};
template<typename T, typename S>
void JuliaBase<T,S>::setIterations(S i){
	iterations=i;
};
template<typename T, typename S>
void JuliaBase<T,S>::setConstant(const Complex<T>& c){
	constant=c;
};
template<typename T, typename S>
void JuliaBase<T,S>::setEscape(S e){
	_escape=e;
};
template<typename T, typename S>
void JuliaBase<T,S>::setRange(const T r,const T i){
	realRange=r;
	imagRange=i;
};
template<typename T, typename S>
void JuliaBase<T,S>::setCenter(const Complex<T>& c){
	center=c;
};
template<typename T, typename S>
__both__ Complex<T> JuliaBase<T,S>::op(const Complex<T>& z)const{
	return z*z+constant;
};
template<typename T, typename S>
__both__ Complex<T> JuliaBase<T,S>::IDtoComplex(const S id)const{
	T scaleReal=realRange/T(pixels);
	T scaleImag=imagRange/T(pixels);
	
	T real=(id%width)*scaleReal;
	T imag=(id/width)*scaleImag; 

	Complex<T> tmp(real,imag);
	tmp=tmp-center; 

	return tmp;
};
template<typename T, typename S>
__both__ bool JuliaBase<T,S>::escape(const Complex<T>& z)const{
	complex tmp=z; 
	for(int i=0; i<iterations;i++){
		tmp=op(tmp);
	}
	return tmp.abs()>_escape; 
};

template<typename T, typename S>
__both__ S JuliaBase<T,S>::ComplextoID(const Complex<T>& z)const{
	return S(z.Real())*width+S(z.Imag()); 
};





#undef __both__
