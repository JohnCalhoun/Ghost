#ifndef GHOST_JULIA_H
#define GHOST_JULIA_H

#include <stdio.h>
#include <cuda.h>
#include "complex.h"
#include <thrust/tuple.h>
#define __both__ __device__ __host__

namespace ghost{
	template<typename T,typename S>
	class JuliaBase{
		public:
		typedef Complex<T> complex;

		private:
		complex center;
		complex constant;

		S pixels;

		S height;
		S width; 
		
		T realRange;
		T imagRange;
		
		S iterations; 
		S _escape;

		public:
		void setPixels(S);
		void setDimensions(S,S);
		void setIterations(S);
		void setEscape(S);

		void setRange(const T,const T);
		void setConstant(const Complex<T>&);
		void setCenter(const Complex<T>&); 
	
		__both__ Complex<T> op(const Complex<T>&)const; 
		__both__ Complex<T> IDtoComplex(const S)const;
		__both__ bool		escape(const Complex<T>&)const; 
		__both__ S		ComplextoID(const Complex<T>&)const;	
	};
	template<typename T,typename S>
	class juliaOutput{
		private:
		typedef typename JuliaBase<T,S>::complex complex;
		JuliaBase<T,S> julia;
		
		public:
		typedef thrust::tuple<T,T> Complex_tuple; 
		juliaOutput(const JuliaBase<T,S>& j):julia(j){};

		__both__ S operator()(const Complex_tuple& z){
			complex tmp(	thrust::get<0>(z),
						thrust::get<1>(z)
						);
			S out=julia.ComplextoID(tmp);
			return out;
		}
	};
	template<typename T,typename S>
	class juliaInitialize{
		private:
		typedef typename JuliaBase<T,S>::complex complex;
		JuliaBase<T,S> julia;
		
		public:
		typedef thrust::tuple<T,T> Complex_tuple; 
		juliaInitialize(const JuliaBase<T,S>& j):julia(j){};

		__both__ Complex_tuple operator()(const S id){
			complex tmp=julia.IDtoComplex(id);

			Complex_tuple out(	tmp.Real(),
							tmp.Imag());

			return out;
		}
		
	};
	template<typename T,typename S>
	class juliaEscape{
		private:
		typedef typename JuliaBase<T,S>::complex complex;
		JuliaBase<T,S> julia;
		
		public:
		typedef thrust::tuple<T,T> Complex_tuple; 
		juliaEscape(const JuliaBase<T,S>& j):julia(j){};

		__both__ bool operator()(S id){
			complex tmp=julia.IDtoComplex(id); 
			return julia.escape(tmp);
		}
	};
	template<typename T,typename S>
	class juliaIterate{
		private:
		typedef typename JuliaBase<T,S>::complex complex;
		JuliaBase<T,S> julia;
		
		public:
		typedef thrust::tuple<T,T> Complex_tuple; 
		juliaIterate(const JuliaBase<T,S>& j):julia(j){};

		__both__ Complex_tuple operator()(const Complex_tuple& z){
			complex tmp(	thrust::get<0>(z),
						thrust::get<1>(z)
						);

			tmp=julia.op(tmp);
		
			Complex_tuple out(	tmp.Real(),
							tmp.Imag());

			return out;
		}

	};
	#include"julia.inl"
};



//end namespace julia
#undef __both__
#endif
