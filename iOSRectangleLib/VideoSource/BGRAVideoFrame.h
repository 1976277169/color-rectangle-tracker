
#ifndef BGRAVideoFrame_h
#define BGRAVideoFrame_h

// A helper struct presenting interleaved BGRA image in memory.
struct BGRAVideoFrame
{
    size_t width;
    size_t height;
    size_t stride;
  
    uint8_t * data;
};


#endif
