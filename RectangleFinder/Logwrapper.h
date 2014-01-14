//
//  Logwrapper.h
//  Classifer
//
//  Created by VIEW Laboratory on 25/7/13.
//  Copyright (c) 2013 ASTRI. All rights reserved.
//

#ifndef Classifer_Logwrapper_h
#define Classifer_Logwrapper_h

#ifdef _DEBUG_

void TimeInit(uint64_t *time)
{
    struct timeval _tv;
    struct timezone _tz;
    gettimeofday(&_tv, &_tz);
    *time = _tv.tv_sec * 1000000 + _tv.tv_usec;
}

void TimeLaps(uint64_t *time)
{
    struct timeval _tv;
    struct timezone _tz;
    
    uint64_t _curTime;
    gettimeofday(&_tv, &_tz);
    _curTime = _tv.tv_sec * 1000000 + _tv.tv_usec;
    *time = _curTime - *time;
    
}


#ifdef _ANDROID_

#include <android/log.h>
#define LOGTAG "ARCore"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOGTAG, __VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOGTAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOGTAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOGTAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOGTAG, __VA_ARGS__)

#else

#include <iostream>

#define LOGD(...) printf(__VA_ARGS__)
#define LOGV(...) printf(__VA_ARGS__)
#define LOGI(...) printf(__VA_ARGS__)
#define LOGE(...) printf(__VA_ARGS__)
#define LOGW(...) printf(__VA_ARGS__)

#endif
#endif
#endif

#ifndef _DEBUG_

#define LOGD(...) 
#define LOGV(...) 
#define LOGI(...) 
#define LOGE(...) 
#define LOGW(...) 

#define TimeInit(...)
#define TimeLaps(...)

#endif