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

// decode packets into frames
static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame);
// save a frame into a .pgm file
static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename);

int main(int argc, char *argv[])
{
    if (argc != 2)
        err_exit("usage: ./a.out filename");
    
    AVFormatContext *pFormatContext = NULL;
    if (avformat_open_input(&pFormatContext, argv[1], NULL, NULL) != 0)
        err_exit("avformat_open_input fail");

    printf("格式: %s, 时长: %lld min\n", pFormatContext->iformat->long_name, pFormatContext->duration / (1000 * 1000 * 60)); // 单位 us

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
    // while (av_read_frame(pFormatContext, pPacket) >= 0 && num < 8) {
    //     if (pPacket->stream_index == video_stream_index) {
    //         printf("AVPacket->pts %lld\t", pPacket->pts);
    //         if (decode_packet(pPacket, pCodecContext, pFrame) < 0)
    //             break;
    //         num++;
    //     }
    //     av_packet_unref(pPacket);
    // }

    avformat_close_input(&pFormatContext);
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);
    avcodec_free_context(&pCodecContext);
    
    exit(0);
}

static int decode_packet(AVPacket *pPacket, AVCodecContext *pCodecContext, AVFrame *pFrame)
{
    int ret = 0;
    ret = avcodec_send_packet(pCodecContext, pPacket);
    if (ret < 0) {
        fprintf(stderr, "Error while sending a packet to the decoder: %s", av_err2str(ret));
        return ret;
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(pCodecContext, pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            break;
        else if (ret < 0) {
            fprintf(stderr, "Error while receiving a frame from the decoder: %s", av_err2str(ret));
            return ret;
        }

        printf("Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]\n", pCodecContext->frame_number,
            av_get_picture_type_char(pFrame->pict_type), pFrame->pkt_size, pFrame->format, pFrame->pts, 
            pFrame->key_frame, pFrame->coded_picture_number);

        char frame_filename[1024];
        snprintf(frame_filename, sizeof(frame_filename), "%s-%d.pgm", "frame", pCodecContext->frame_number);
        if (pFrame->format != AV_PIX_FMT_YUV420P)
            fprintf(stderr, "Warning: the generated file may not be a grayscale image, but could e.g. be just the R component if the video format is RGB");
    
        // save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, frame_filename);
    }
  
    return 0;
}

static void save_gray_frame(unsigned char *buf, int wrap, int xsize, int ysize, char *filename)
{
    FILE *f = fopen(filename,"w");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (int i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}