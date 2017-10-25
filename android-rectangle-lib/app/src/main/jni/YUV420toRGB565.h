#define NUM_THREADS 2

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

	srcy = yuvs;
	srcu = yuvs + (widthIn * heightIn);
	srcv = yuvs + (widthIn * heightIn); //temp testing
	dst = rgbs;
	dst_gr = gr;

	for (int y=0;y<heightIn;y++) {
			yuv420p_rgb565_neon(srcy, srcu, dst_gr, dst, widthIn);
			srcy += widthIn;
			dst  += widthIn;
			if ((y&1)) {
					srcu += widthIn;
			}
			dst_gr += widthIn;
	}

}

