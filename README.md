# PlayVideo
    交流方式qq505694246。
    yuvencodedecode项目包含Android硬编码、硬解码、yuv渲染3个主要功能在Nokia6上已验证可用. 
    添加libyuv对图像的裁剪功能代码。
    20200321补充
    添加FFmpeg软解码裸h264功能。
    20200326补充
    添加ffmpeg解码流数据功能，
    同时注释掉 
    //调用硬件解码
    // Decoder.getDecoder().onFrame(u);
    //调用FFmpeg软解
    //Decoder.getDecoder().onFFmpegFrame(u);
    这几行代码关闭硬解h264和ffmpeg软解裸h264数据功能。有需要的可以在Encoder类中恢复该功能。
    20200328ffmpeg解码流数据放置在子线程中。
