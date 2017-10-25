#define NUM_THREADS 2

struct toRGB565_thread_data
{
	unsigned short *_yuvs;
	int _widthIn;
	int _heightIn;
	unsigned int *_rgbs;
	int _widthOut;
	int _heightOut;
	unsigned short *_gr;
	int _range_start;
	int _range_end;
    long t_id;
};

void *toRGB565_thread_func(void *threadarg)
//void toRGB565_thread_func(unsigned short *yuvs, int widthIn, int heightIn, unsigned int *rgbs, int widthOut, int heightOut, unsigned short *gr, int range_start, int range_end)
{
   struct toRGB565_thread_data *my_data;

	my_data = (struct toRGB565_thread_data *) threadarg;

	long thread_id = (long)my_data->t_id;

	unsigned short *yuvs = (unsigned short*)my_data->_yuvs;
	int widthIn = (int)my_data->_widthIn;
	int heightIn = (int)my_data->_heightIn;
	unsigned int *rgbs = (unsigned int*)my_data->_rgbs;
	int widthOut = (int)my_data->_widthOut;
	int heightOut = (int)my_data->_heightOut;
	unsigned short *gr = (unsigned short *)my_data->_gr;


	int range_start = (int)my_data->_range_start;
	int range_end = (int)my_data->_range_end;

	//__android_log_print(ANDROID_LOG_DEBUG,"YUV420toRGB565","threads id %d, from %d to %d", thread_id, range_start, range_end);

  int half_widthIn = widthIn >> 1;

  //the end of the luminance data
  int lumEnd = (widthIn * heightIn) >> 1;
  //points to the next luminance value pair
  //int lumPtr = 0;
  int lumPtr = range_start * half_widthIn;

  //points to the next chromiance value pair
  //int chrPtr = lumEnd  ;
  //TSZ LUNG ADD
  int chrPtr=  lumEnd + ((lumPtr  >> 1) / half_widthIn) * half_widthIn;
  //the end of the current luminance scanline
  int lineEnd = half_widthIn;

  int x,y;
  for (y=range_start;y<range_end;y++) {
    int yPosOut=(y*widthOut) >> 1;
    for (x=0;x<half_widthIn;x++) {
      //read the luminance and chromiance values
      //int Y1 = yuvs[lumPtr++];
      int Y1 = yuvs[lumPtr]; //increament later
      int Y2 = (Y1 >> 8) & 0xff;
      Y1 = Y1 & 0xff;
      int Cr = yuvs[chrPtr++];
      int Cb = ((Cr >> 8) & 0xff) - 128;
      Cr = (Cr & 0xff) - 128;

      int R, G, B;
      //generate first RGB components
      B = Y1 + ((454 * Cb) >> 8);
      if (B < 0) B = 0; if (B > 255) B = 255;
      G = Y1 - ((88 * Cb + 183 * Cr) >> 8);
      if (G < 0) G = 0; if (G > 255) G = 255;
      R = Y1 + ((359 * Cr) >> 8);
      if (R < 0) R = 0; if (R > 255) R = 255;
      int val = ((R & 0xf8) << 8) | ((G & 0xfc) << 3) | (B >> 3);

      //generate second RGB components
      B = Y2 + ((454 * Cb) >> 8);
      if (B < 0) B = 0; if (B > 255) B = 255;
      G = Y2 - ((88 * Cb + 183 * Cr) >> 8);
      if (G < 0) G = 0; if (G > 255) G = 255;
      R = Y2 + ((359 * Cr) >> 8);
      if (R < 0) R = 0; if (R > 255) R = 255;
      rgbs[yPosOut+x] = val | ((((R & 0xf8) << 8) | ((G & 0xfc) << 3) | (B >> 3)) << 16);
      //handling for the gray Mat
      gr[lumPtr] = yuvs[lumPtr];
      lumPtr++;
    }
    //skip back to the start of the chromiance values when necessary
    chrPtr = lumEnd + ((lumPtr  >> 1) / half_widthIn) * half_widthIn;
    lineEnd += half_widthIn;
  }
    return 0;
}

void toRGB565(unsigned short *yuvs, int widthIn, int heightIn, unsigned int *rgbs, int widthOut, int heightOut, unsigned short *gr)
{

    pthread_t threads[NUM_THREADS];
	int rc1;
	int rc2;
	long t;
	typedef 	toRGB565_thread_data TH_DATA;
	TH_DATA		toRGB565_thread_data_array[NUM_THREADS];
	int			range_per_thread = heightIn/NUM_THREADS;

	pthread_attr_t attr;
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<NUM_THREADS; t++)
	{
		toRGB565_thread_data_array[t]._yuvs = yuvs;
		toRGB565_thread_data_array[t]._widthIn = widthIn;
		toRGB565_thread_data_array[t]._heightIn = heightIn;
		toRGB565_thread_data_array[t]._rgbs = rgbs;
		toRGB565_thread_data_array[t]._widthOut = widthOut;
		toRGB565_thread_data_array[t]._heightOut = heightOut;
		toRGB565_thread_data_array[t]._gr = gr;
		if(t==NUM_THREADS-1)
		{
			toRGB565_thread_data_array[t]._range_start = t*range_per_thread;
			toRGB565_thread_data_array[t]._range_end = heightIn;
		} else {
			toRGB565_thread_data_array[t]._range_start = t*range_per_thread;
			toRGB565_thread_data_array[t]._range_end = ((t+1)*range_per_thread);
		}
		toRGB565_thread_data_array[t].t_id = t;

        rc1 = pthread_create(&threads[t], &attr, toRGB565_thread_func, (void *)&toRGB565_thread_data_array[t]);
        if (rc1){
            //printf("ERROR; return code from pthread_create() is %d\n", rc1);
            exit(-1);
        }
    }

    void *status;
    for(t=0; t<NUM_THREADS; t++)
    {
        rc1 = pthread_join(threads[t], &status);
        if (rc1) {
            //printf("ERROR; return code from pthread_join()                   is %d\n", rc1);
            exit(-1);
        }
        //   printf("Main: First pass completed join with thread %ld having a status of %ld\n",t,(long)status);
    }

    // printf("First pass thread done\n");

    /* Free attribute and wait for the other threads */
    pthread_attr_destroy(&attr);

}

//void toRGB565(unsigned short *yuvs, int widthIn, int heightIn, unsigned int *rgbs, int widthOut, int heightOut, unsigned short *gr) {
//  int half_widthIn = widthIn >> 1;
//
//  //the end of the luminance data
//  int lumEnd = (widthIn * heightIn) >> 1;
//  //points to the next luminance value pair
//  int lumPtr = 0;
//  //points to the next chromiance value pair
//  int chrPtr = lumEnd;
//  //the end of the current luminance scanline
//  int lineEnd = half_widthIn;
//
//  int x,y;
//  for (y=0;y<heightIn;y++) {
//    int yPosOut=(y*widthOut) >> 1;
//    for (x=0;x<half_widthIn;x++) {
//      //read the luminance and chromiance values
//      //int Y1 = yuvs[lumPtr++];
//      int Y1 = yuvs[lumPtr]; //increament later
//      int Y2 = (Y1 >> 8) & 0xff;
//      Y1 = Y1 & 0xff;
//      int Cr = yuvs[chrPtr++];
//      int Cb = ((Cr >> 8) & 0xff) - 128;
//      Cr = (Cr & 0xff) - 128;
//
//      int R, G, B;
//      //generate first RGB components
//      B = Y1 + ((454 * Cb) >> 8);
//      if (B < 0) B = 0; if (B > 255) B = 255;
//      G = Y1 - ((88 * Cb + 183 * Cr) >> 8);
//      if (G < 0) G = 0; if (G > 255) G = 255;
//      R = Y1 + ((359 * Cr) >> 8);
//      if (R < 0) R = 0; if (R > 255) R = 255;
//      int val = ((R & 0xf8) << 8) | ((G & 0xfc) << 3) | (B >> 3);
//
//      //generate second RGB components
//      B = Y2 + ((454 * Cb) >> 8);
//      if (B < 0) B = 0; if (B > 255) B = 255;
//      G = Y2 - ((88 * Cb + 183 * Cr) >> 8);
//      if (G < 0) G = 0; if (G > 255) G = 255;
//      R = Y2 + ((359 * Cr) >> 8);
//      if (R < 0) R = 0; if (R > 255) R = 255;
//      rgbs[yPosOut+x] = val | ((((R & 0xf8) << 8) | ((G & 0xfc) << 3) | (B >> 3)) << 16);
//      //handling for the gray Mat
//      gr[lumPtr] = yuvs[lumPtr];
//      lumPtr++;
//    }
//    //skip back to the start of the chromiance values when necessary
//    chrPtr = lumEnd + ((lumPtr  >> 1) / half_widthIn) * half_widthIn;
//    lineEnd += half_widthIn;
//  }
//}




//JNIEXPORT void JNICALL Java_au_notzed_jjmpeg_AVFrameNative_toRGB565
//(JNIEnv *env, jobject jptr, jint fmt, jint width, jint height, jobject jdst) {
//        AVFrame *cptr = PTR(jptr, AVFrame);
//        unsigned short *dst = ADDR(jdst);
//        int dwidth = (width+15)&(~15);
//        int y;
//        unsigned char *srcy = cptr->data[0];
//        unsigned char *srcu = cptr->data[1];
//        unsigned char *srcv = cptr->data[2];
//
//        for (y=0;y<height;y++) {
//                yuv420p_rgb565_neon(srcy, srcu, srcv, dst, dwidth);
//
//                srcy += cptr->linesize[0];
//                dst += dwidth;
//                if ((y&1)) {
//                        srcu += cptr->linesize[1];
//                        srcv += cptr->linesize[2];
//                }
//        }
//}


//char gstr[255];
//
//extern "C" void yuv420p_rgb565_neon(unsigned char *srcy, unsigned char *srcu, unsigned char *srcv, unsigned short *dst, int width);
//extern "C" void yuv420p_rgb565_neon_u(unsigned char *srcy, unsigned char *srcu, unsigned char *srcv, unsigned short *dst, int width);
//
//void toRGB565neon(unsigned char *yuvs, int widthIn, int heightIn, unsigned short *rgbs)
//{
//	int half_widthIn = widthIn >> 1;
//  //the end of the luminance data
//  int lumEnd = (widthIn * heightIn) >> 1;
//  //points to the next luminance value pair
//  int lumPtr = 0;
//  //points to the next chromiance value pair
//  int chrPtr = lumEnd;
//  //the end of the current luminance scanline
//  int lineEnd = half_widthIn;
//
//	unsigned char *srcy;
//	unsigned char *srcu;
//	unsigned char *srcv;
//	unsigned short *dst;
//
//	srcy = yuvs;
//	srcu = yuvs + (widthIn * heightIn);
//	srcv = yuvs + (widthIn * heightIn); //temp testing
//	dst = rgbs;
//
//	for (int y=0;y<heightIn;y++) {
////			sprintf(gstr, "y=%d srcy=%x srcu=%x srcv=%x dst=%x", y, srcy, srcu, srcv, dst);
////			__android_log_print(ANDROID_LOG_DEBUG, gstr, "");
//			yuv420p_rgb565_neon_u(srcy, srcu, srcv, dst, widthIn);
//			srcy += widthIn;
//			dst  += widthIn;
//			if ((y&1)) {
//					srcu += widthIn;
//					srcv += widthIn;
//			}
//	}
////	sprintf(gstr, "exit y loop");
////	__android_log_print(ANDROID_LOG_DEBUG, gstr, "");
//}

extern "C" void yuv420p_rgb565_neon(unsigned char *srcy, unsigned char *srcu, unsigned char *srcv, unsigned short *dst, int width);

void toRGB565neon(unsigned char *yuvs, int widthIn, int heightIn, unsigned short *rgbs, unsigned char *gr)
{
  int half_widthIn = widthIn >> 1;
  //the end of the luminance data
  int lumEnd = (widthIn * heightIn) >> 1;
  //points to the next luminance value pair
  int lumPtr = 0;
  //points to the next chromiance value pair
  int chrPtr = lumEnd;
  //the end of the current luminance scanline
  int lineEnd = half_widthIn;

	unsigned char *srcy;
	unsigned char *srcu;
	unsigned char *srcv;
	unsigned short *dst;
	unsigned char  *dst_gr;
	//unsigned char testu[76800];
	//unsigned char testv[76800];
	srcy = yuvs;
	srcu = yuvs + (widthIn * heightIn);
	srcv = yuvs + (widthIn * heightIn); //temp testing
	dst = rgbs;
	dst_gr = gr;
	/*int cnt =0, cnty =0;
	unsigned char* testy = srcy+widthIn * heightIn;
	for (int y=0;y<heightIn/2;y++)
		for (int z=0;z<widthIn/2;z++) {
			testu[cnt] = testy[cnty++];
			testv[cnt++] = testy[cnty++];
		}
	srcu = testu; srcv = testv;*/
	for (int y=0;y<heightIn;y++) {
			yuv420p_rgb565_neon(srcy, srcu, dst_gr, dst, widthIn);
			//yuv420p_rgb565_neon(srcy, srcu, srcv, dst, widthIn);
			srcy += widthIn;
			dst  += widthIn;
			if ((y&1)) {
					srcu += widthIn;
					//srcv += widthIn;

					//srcu += half_widthIn;
					//srcv += half_widthIn;
			}
			dst_gr += widthIn;
	}

}

