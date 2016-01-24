//#include"ghost.h"
//*******************CONSTRUCTORS/DESTRUCTORS***************
template<typename T,typename S>
ghostRender<T,S>::ghostRender(const S size){
	height=size;
	width=size; 
	pixels=height*width;
	
	numberOfBlocks=pixels/blockSize+1; 

	juliaop.setPixels(width);
	juliaop.setDimensions(height,width);
	juliaop.setIterations(400/juliaLevels); 
	juliaop.setEscape(escape); 
}
template<typename T,typename S>
ghostRender<T,S>::~ghostRender(){
}
//*****************CONSTRUCTORS/DESTRUCTORS*****************
//*****************SETTERS**********************************
template<typename T,typename S>
void ghostRender<T,S>::setConstant(const T real,const T imag){
	constant.Real(real);
	constant.Imag(imag);
	juliaop.setConstant(constant);
};
template<typename T,typename S>
void ghostRender<T,S>::setRange(const T real,const T imag){
	juliaop.setRange(real,imag); 
};
template<typename T,typename S>
void ghostRender<T,S>::setCenter(const T real,const T imag){
	center.Real(real);
	center.Imag(imag); 
	juliaop.setCenter(center); 
};
//*****************SETTERS**********************************
//*****************PRIVATE**********************************
template<typename T,typename S>
ghostRender<T,S>::Size ghostRender<T,S>::blockJulia(const ghostRender<T,S>::Size i){
	typedef thrust::counting_iterator<Size>			count_it;	
	
	const Size offset=i*blocksize(i-1);
	const Size block=blocksize(i);

	int_it_dev points_begin	=points_dev.begin();
	int_it_dev points_end	=points_begin+block;
	thrust::sequence(	thrust::device,
					points_begin,
					points_end,
					offset
				);
	for(int i=0; i<juliaLevels; i++){
		thrust::transform(	
					thrust::device,
					points_begin,
					points_end,
					results_dev.begin(),
					juliaEscape<T,S>(juliaop) 
					);
		points_end=thrust::remove_if(		
					thrust::device,
					points_begin,
					points_end,
					results_dev.begin(),
					thrust::identity<bool>()
					);
	}
	return points_end-points_begin;
};
template<typename T,typename S>
ghostRender<T,S>::Size ghostRender<T,S>::findPoints(){
	results_dev.resize(blocksize(0));	
	Size num;
	int_it_host end=points_host.begin();

	printf("finding points,blocks=%d \n <",numberOfBlocks);
	int stages=10;
	int stagesize=numberOfBlocks/stages;

	for(int i=0; i<numberOfBlocks; i++){
		num=blockJulia(i);
		
		thrust::copy_n(	points_dev.begin(),
						num,
						end);
		end+=num;
		if(i%stagesize==0){
			printf("=");
		}
	}
	printf(">\n");
	results_dev.resize(0); 
	Size results=end-points_host.begin(); 
	printf("%d points found \n",results);
	return results;
};
template<typename T,typename S>
void ghostRender<T,S>::initializeDecimals(ghostRender<T,S>::Size num){
	zip_itdecdev complex_begin_dev(
				thrust::make_tuple(	real_dev.begin(),
								imag_dev.begin()
								)
					);
	thrust::transform(	thrust::device,
					points_dev.begin(),
					points_dev.begin()+num,
					complex_begin_dev,
					juliaInitialize<T,S>(juliaop)
					);
};
template<typename T,typename S>
void ghostRender<T,S>::iterate(int i){
	zip_itdechost complex_begin_host(
				thrust::make_tuple(	real_host.begin(),
								imag_host.begin()
								)
					);
	zip_itdechost complex_end_host(
				thrust::make_tuple(	real_host.end(),
								imag_host.end()
								)
					);
	zip_itdecdev complex_begin_dev(
				thrust::make_tuple(	real_dev.begin(),
								imag_dev.begin()
								)
					);
	zip_itdecdev complex_end_dev(
				thrust::make_tuple(	real_dev.end(),
								imag_dev.end()
								)
					);

	Size block=blocksize(i-1);
	Size offset=i*block;

	thrust::copy_n(	complex_begin_host+offset,
					blocksize(i),
					complex_begin_dev);
	
	thrust::tabulate(	thrust::device,
					complex_begin_dev,
					complex_end_dev,
					juliaIterate<T,S>(juliaop)
					);
	
	thrust::copy_n(	complex_begin_dev,	
					blocksize(i),
					complex_begin_host+offset);	

};
template<typename T,typename S>
void ghostRender<T,S>::updatehistogram(int i){
		zip_itdecdev complex_begin_dev(
					thrust::make_tuple(	real_dev.begin(),
									imag_dev.begin()
									)
						);
		zip_itdecdev complex_end_dev(
					thrust::make_tuple(	real_dev.end(),
									imag_dev.end()
									)
						);
		zip_itdechost complex_begin_host(
					thrust::make_tuple(	real_host.begin(),
									imag_host.begin()
									)
						);
		zip_itdechost complex_end_host(
					thrust::make_tuple(	real_host.end(),
									imag_host.end()
									)
						);	
	
		Size block=blocksize(i);
		Size offset=i*blocksize(0);

		thrust::copy_n(	complex_begin_host+offset,
						blocksize(i),
						complex_begin_dev);
		
		thrust::transform(	thrust::device,
						complex_begin_dev,
						complex_end_dev,
						histogram_device.begin(),
						juliaOutput<T,S>(juliaop)
						);
		//use Output as key to histogram
		thrust::sort(		histogram_device.begin(),
						histogram_device.end()
						);

		thrust::counting_iterator<Size> search_begin(0);

		auto reduce_out=thrust::reduce_by_key(	
						thrust::device,
						histogram_device.begin(),
						histogram_device.end(),
						search_begin,
						histogram_device.begin(),
						histogram_out.begin()
						);
		Size count_out=	thrust::get<1>(reduce_out) -
						histogram_out.begin();
		//create zip iterator (histogram,historgramout)
		zip_itintdev histDevBegin(	
						thrust::make_tuple(	histogram_device.begin(),
										histogram_out.begin()
										)
							);
		zip_itinthost histHostBegin(
						thrust::make_tuple( histogram_tmp_host.begin(),
										histogram_out_host.begin()
										)
							);
		
		thrust::copy_n(	histDevBegin,
						count_out,
						histHostBegin); 
		//update histogram on host

		zip_itinthost it;
		for(int i=0; i<count_out; i++){
			it=histHostBegin+i;
			Size id=thrust::get<0>(*it);
			Size num=thrust::get<1>(*it);

			histogram_host[id]+=num;
		}
};

template<typename T,typename S>
ghostRender<T,S>::Size ghostRender<T,S>::blocksize(ghostRender<T,S>::Size i){
	if(i<(numberOfBlocks-1)){
		return blockSize;
	}else{
		return pixels%blockSize;
	}
};
//*****************PRIVATE**********************************
//*****************PUBLIC***********************************
template<typename T,typename S>
void ghostRender<T,S>::calculateJulia(){
	points_host.resize(pixels);
	points_dev.resize(blocksize(0));

	findPoints();

	points_dev.resize(0);
};
template<typename T,typename S>
void ghostRender<T,S>::calculateGhost(){
	Size numofpoints=findPoints();
	
	real_host.resize(numofpoints);
	real_dev.resize(blocksize(0));
	
	imag_host.resize(numofpoints);
	imag_dev.resize(blocksize(0));

//	initializeDecimals(numofpoints);
		
	histogram_host.resize(numofpoints);
	histogram_tmp_host.resize(blocksize(0));
	histogram_out_host.resize(blocksize(0));

	histogram_device.resize(blocksize(0));
	histogram_out.resize(blocksize(0));
	
	int stages=10;
	int stagesize=dwellLimmit/stages;
	printf("calculating trajectories\n");
	printf("<");
	for(int i=0;i<dwellLimmit;i++){
		for(int j=0; i<numberOfBlocks;i++){
			//figure out how to overlap
			iterate(j);
			updatehistogram(j);	
		}
		if(i%stagesize==0){
			printf("=");
		}
	}
	printf(">\n"); 
	histogram_tmp_host.resize(0);
	histogram_out_host.resize(0);

	histogram_device.resize(0);
	histogram_out.resize(0);
};
//*****************PUBLIC***********************************
//*****************OUTPUT***********************************
template<typename T,typename S>
void ghostRender<T,S>::renderGhostImage(std::string file)const{
	
	auto minmax=thrust::minmax_element(	thrust::host,
									histogram_host.begin(),
									histogram_host.end()
								);
	Integer min=*(minmax.first);
	Integer max=*(minmax.second);
	
	BMP* bmp=BMP_Create(pixels,pixels,8);
	
	for(int i=0; i<histogram_host.size();i++){
		int color=(histogram_host[i]-min)*(float(255)/float(max));
		
		BMP_SetPixelIndex(	bmp,
						i/width,
						i%width,
						color
						);
	}
	for(int j=0; j<255;j++){
		BMP_SetPaletteColor(bmp,j, j,j,j);
	}
	const char* c=file.c_str();
	BMP_WriteFile(bmp,c);
};
template<typename T,typename S>
void ghostRender<T,S>::renderJuliaImage(std::string file)const{

	BMP* bmp=BMP_Create(width,height,8);
	
	for(int i=0; i<points_host.size();i++){
		
		BMP_SetPixelIndex(	bmp,
						i%width,
						i/width,
						points_host[i]
						);
	}
	BMP_SetPaletteColor(bmp,1, 0,  0,  0);
	BMP_SetPaletteColor(bmp,0, 255,255,255);

	const char* c=file.c_str();
	BMP_WriteFile(bmp,c);
};

//*****************OUTPUT***********************************

