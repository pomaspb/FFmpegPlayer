//  Created by Roman Petrov on 21/07/15.
//  Copyright (c) 2015 Kentor. All rights reserved.

#include "NativeVideo.h"
#include "avcodec.h"
#include "avformat.h"
#include "swscale.h"
#include <stdbool.h>

AVCodecContext *codecCtx = NULL;
AVFormatContext *formatCtx = NULL;
AVFrame *decodedFrame = NULL;
int videoStream = -1;
struct SwsContext *img_convert_ctx;
AVPicture picture;

bool native_video_init(const char* videoPath) {
    AVDictionary *optionsDict = NULL;
   
    printf("video path %s\n", videoPath);

    // Register all formats and codecs
    av_register_all();
    
    // Open video file
    if(avformat_open_input(&formatCtx, videoPath, NULL, NULL) != 0)
        return false; // Couldn't open file
    
    // Retrieve stream information
    if(avformat_find_stream_info(formatCtx, NULL) < 0)
        return false; // Couldn't find stream information
    
    // Dump information about file into standard output
    av_dump_format(formatCtx, 0, videoPath, 0);
    
    // Find the first video stream
    for(int i = 0; i < formatCtx->nb_streams; i++) {
        if(formatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            videoStream=i;
            break;
        }
    }
    if(videoStream == -1)
        return false; // Didn't find a video stream
    
    // Get a pointer to the codec context for the video stream
    codecCtx = formatCtx->streams[videoStream]->codec;
    
    // Find the decoder for the video stream
    AVCodec *pCodec = avcodec_find_decoder(codecCtx->codec_id);
    if(pCodec == NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return false; // Codec not found
    }

    // Open codec
    if(avcodec_open2(codecCtx, pCodec, &optionsDict) < 0)
        return false; // Could not open codec
    
    // Allocate video frame
    decodedFrame = av_frame_alloc();
    
    return true;
}

bool native_get_video_res(int videoRes[2]) {
    if (codecCtx == NULL) {
        return false;
    }
    videoRes[0] = codecCtx->width;
    videoRes[1] = codecCtx->height;

    return true;
}

bool native_setup_scaler(int outputWidth, int outputHeight) {
    // Allocate RGB picture
    avpicture_alloc(&picture, PIX_FMT_RGB24, outputWidth, outputHeight);
    
    // Setup scaler
    static int sws_flags =  SWS_FAST_BILINEAR;
    img_convert_ctx = sws_getContext(codecCtx->width,
                                     codecCtx->height,
                                     codecCtx->pix_fmt,
                                     outputWidth,
                                     outputHeight,
                                     PIX_FMT_RGB24,
                                     sws_flags, NULL, NULL, NULL);
    return true;
}

bool native_set_seek_time(double seconds) {
    AVRational timeBase = formatCtx->streams[videoStream]->time_base;
    int64_t targetFrame = (int64_t)((double)timeBase.den / timeBase.num * seconds);
    avformat_seek_file(formatCtx, videoStream, targetFrame, targetFrame, targetFrame, AVSEEK_FLAG_FRAME);
    avcodec_flush_buffers(codecCtx);
    return true;
}

bool native_step_frame() {
    AVPacket packet;
    int frameFinished = 0;
    
    while (!frameFinished && av_read_frame(formatCtx, &packet) >=0 ) {
        // Is this a packet from the video stream?
        if(packet.stream_index == videoStream) {
            // Decode video frame
            avcodec_decode_video2(codecCtx, decodedFrame, &frameFinished, &packet);
        }
    }
    
    return frameFinished != 0;
}

AVPicture* native_get_current_frame() {
    if (!decodedFrame->data[0]) return NULL;

    sws_scale(img_convert_ctx,
              decodedFrame->data,
              decodedFrame->linesize,
              0,
              codecCtx->height,
              picture.data,
              picture.linesize);

    
    return &picture;
}
