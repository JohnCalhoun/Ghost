#ifndef JULIA_GHOST_H
#define JULIA_GHOST_H

#include"complex.h"
#include"julia.h"

#include<qdbmp/qdbmp.h>
#include<thrust/device_vector.h>
#include<thrust/host_vector.h>
#include<thrust/execution_policy.h>
#include<thrust/iterator/counting_iterator.h>
#include<thrust/sequence.h>
#include<thrust/transform.h>
#include<thrust/functional.h>
#include<thrust/remove.h>
#include<thrust/tuple.h>
#include<thrust/sort.h>
#include<thrust/copy.h>
#include<thrust/iterator/zip_iterator.h>
#include<algorithm>
#include<thrust/extrema.h>

namespace ghost{
	template<typename T,typename S>
	class ghostRender {
		private:
		typedef unsigned int	Integer;
		typedef S				Size;
		typedef T				Decimal; 
		typedef Complex<Decimal> complex;

		typedef thrust::device_vector<Integer>		Device_int_vector;
		typedef thrust::host_vector<Integer>		Host_int_vector;

		typedef thrust::device_vector<Decimal>		Device_decimal_vector;
		typedef thrust::host_vector<Decimal>		Host_decimal_vector;
		
		typedef thrust::device_vector<bool>		Device_bool_vector;

		typedef JuliaBase<Decimal,Size>			juliaOperation;
	
		typedef typename Device_decimal_vector::iterator	dec_it_dev;
		typedef typename Host_decimal_vector::iterator	dec_it_host;
		typedef typename Device_int_vector::iterator		int_it_dev;
		typedef typename Host_int_vector::iterator		int_it_host;
				
		typedef thrust::tuple<dec_it_dev,dec_it_dev>		itdecTupledev;
		typedef thrust::tuple<dec_it_host,dec_it_host>	itdecTuplehost;
		typedef thrust::tuple<int_it_dev,int_it_dev>		itintTupledev;
		typedef thrust::tuple<int_it_host,int_it_host>	itintTuplehost;
				
		typedef thrust::zip_iterator<itdecTupledev>	zip_itdecdev;
		typedef thrust::zip_iterator<itdecTuplehost>	zip_itdechost;
		typedef thrust::zip_iterator<itintTupledev>	zip_itintdev;
		typedef thrust::zip_iterator<itintTuplehost> zip_itinthost;

		private:
		Host_int_vector	histogram_host;

		Host_int_vector	histogram_tmp_host;
		Host_int_vector	histogram_out_host;

		Device_int_vector	histogram_device;
		Device_int_vector	histogram_out;

		Device_bool_vector	results_dev;	

		Host_int_vector	points_host;
		Device_int_vector	points_dev;

		Host_decimal_vector		real_host;
		Host_decimal_vector		imag_host;

		Device_decimal_vector	real_dev;
		Device_decimal_vector	imag_dev;

		juliaOperation juliaop; 
		complex constant;
		
		complex center; 
		Size	pixels;	
		Size height;
		Size width;


		const Size dwellLimmit=1000;
		const Size juliaLevels=5;
		const Size blockSize=50000;
		const Size escape=2;
		Size numberOfBlocks; 

		public:
		ghostRender(S); 
		~ghostRender();
		private:
		Size blocksize(Size);
		
		Size blockJulia(const Size); 
		Size findPoints();
		void initializeDecimals(Size);
		void iterate(int);
		void updatehistogram(int);

		public: 
		void setRange(const T,const T);
		void setConstant(const T,const T);
		void setCenter(const T,const T);

		
		void calculateJulia(); 
		void calculateGhost(); 

		void renderGhostImage(std::string)const;		
		void renderJuliaImage(std::string)const;
	};
	#include"ghost.inl"
};//end namespace ghost
#endif

















