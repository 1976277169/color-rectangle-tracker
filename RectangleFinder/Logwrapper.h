//
//  Logwrapper.h
//
//  Created by VIEW Laboratory on 25/7/13.
//  Copyright (c) 2013 ASTRI. All rights reserved.
//

#ifndef LOGWRAPPER_H
#define LOGWRAPPER_H

#ifdef DEBUG

#ifdef ANDROID

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

#ifndef DEBUG

#define LOGD(...) 
#define LOGV(...) 
#define LOGI(...) 
#define LOGE(...) 
#define LOGW(...) 

#define TimeInit(...)
#define TimeLaps(...)

#endif
