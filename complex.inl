#define __both__ __device__ __host__

template<typename T>
__both__ Complex<T> Complex<T>::operator+(const Complex<T>& other)const{
	T r=real+other.Real();
	T i=imag+other.Imag();
	Complex tmp(r,i);
	return tmp; 
};
template<typename T>
__both__ Complex<T> Complex<T>::operator*(const Complex<T>& other)const{
	T r=real*(other.Real())-imag*(other.Imag());
	T i=real*(other.Imag())+imag*(other.Real()); 
	Complex tmp(r,i);
	return tmp; 
};	
template<typename T>
__both__ Complex<T> Complex<T>::operator-(const Complex<T>& other)const{
	T r=real-other.Real();
	T i=imag-other.Imag();
	Complex tmp(r,i);
	return tmp; 
};
template<typename T>
__both__ Complex<T> Complex<T>::operator/(const Complex<T>& other)const{
	T r=real-other.Real();
	T i=imag-other.Imag();
	Complex tmp(r,i);
	return tmp; 
};
/*template<typename T>
__host__ Complex<T> Complex<T>::operator<<(const Complex<T>& other)const{
	std::cout<<"Real="<<Real()<<" Imag="<<Imag()<<endl;
	return os; 
};
*/
template<typename T>
__both__ T Complex<T>::abs()const{
	T tmp=sqrt(real*real+imag*imag);
	return tmp;
};

template<typename T>
__both__ void Complex<T>::print()const{
	printf("real=%f imag=%f",this->Real(),this->Imag());
};

#undef __both__
