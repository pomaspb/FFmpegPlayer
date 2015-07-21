//  Created by Roman Petrov on 21/07/15.
//  Copyright (c) 2015 Kentor. All rights reserved.

#ifndef __FFmpegPlayer__NativeVideo__
#define __FFmpegPlayer__NativeVideo__

#include "avcodec.h"
#include <stdio.h>
#include <stdbool.h>

bool native_video_init(const char* videoPath);
bool native_get_video_res(int videoRes[2]);
bool native_setup_scaler(int widthParam, int heightParam);
bool native_set_seek_time(double seconds);
bool native_step_frame();
AVPicture* native_get_current_frame();

#endif /* defined(__FFmpegPlayer__NativeVideo__) */
