/*
 * https://github.com/leandromoreira/ffmpeg-libav-tutorial/blob/master/README-cn.md
 * sudo apt install ffmpeg libavcodec-dev libavformat-dev
 * gcc 01.c -lavcodec -lavformat -lavutil
 */

#include <stdio.h>
#include <stdlib.h>

#include <libavformat/avformat.h>

#define err_exit(msg) {             \
    fprintf(stderr, "%s\n", msg);   \
    exit(1);                        \
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_exit("usage: ./a.out filename");
    
    AVFormatContext *pFormatContext = avformat_alloc_context();
    if (pFormatContext == NULL)
        err_exit("avformat_alloc_context fail")

    if (avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0)
        err_exit("avformat_open_input fail");

    printf("格式: %s, 时长: %lld us\n", pFormatContext->iformat->long_name, pFormatContext->duration);

    if (avformat_find_stream_info(pFormatContext,  NULL) < 0)
        err_exit("avformat_find_stream_info fail");

    AVCodecParameters *pCodecParameters = NULL, *pLocalCodecParameters = NULL;
    AVCodec *pCodec = NULL, *pLocalCodec = NULL;
    int video_stream_index = -1;
    for (int i = 0; i < pFormatContext->nb_streams; i++) {
        pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        if (pLocalCodec == NULL)
            continue;
        // 只要视频和音频
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            if (video_stream_index == -1) {
                video_stream_index = i;
                pCodec = pLocalCodec;
                pCodecParameters = pLocalCodecParameters;
            }
            printf("Video Codec: resolution %d x %d\n", pLocalCodecParameters->width, pLocalCodecParameters->height);
        } else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            printf("Audio Codec: %d channels, sample rate %d\n", pLocalCodecParameters->channels, pLocalCodecParameters->sample_rate);
        }
        // 通用参数
        printf("Codec %s ID %d bit_rate %lld\n\n", pLocalCodec->long_name, pLocalCodec->id, pLocalCodecParameters->bit_rate);
    }

    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if (pCodecContext == NULL)
        err_exit("avcodec_alloc_context3 fail");
    if (avcodec_parameters_to_context(pCodecContext, pCodecParameters) < 0)
        err_exit("avcodec_parameters_to_context fail");
    if (avcodec_open2(pCodecContext, pCodec, NULL) < 0)
        err_exit("avcodec_open2 fail");

    AVFrame *pFrame = av_frame_alloc();
    if (pFrame == NULL)
        err_exit("av_frame_alloc fail");

    AVPacket *pPacket = av_packet_alloc();
    if (pPacket == NULL)
        err_exit("av_packet_alloc fail");

    int num = 0;
    while (av_read_frame(pFormatContext, pPacket) >= 0 && num < 8) {
        if (pPacket->stream_index == video_stream_index) {
            printf("AVPacket->pts %" PRId64, pPacket->pts);
            if (decode_packet(pPacket, pCodecContext, pFrame) < 0)
                break;
            num++;
        }
        av_packet_unref(pPacket);
    }

    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);
    
    exit(0);
}
