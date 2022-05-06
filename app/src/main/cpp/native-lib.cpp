#include <jni.h>
#include <string>

//使用cpp时需要对应的头文件是hpp，如果没有hpp再使用h。
#include <fmod.hpp> //尖括号表示是从打出的库中寻找头文件.
#include <unistd.h>
//#include "inc/fmod.hpp" //表示从这个路径下寻找头文件

//对应java层的六个常量，所使用的宏
//#define : 表示定义一个宏.在编译时会进行宏替换。
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_NORMAL 0L
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_LUOLI 1L
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_DASHU 2L
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_JINGSONG 3L
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_GAOGUAI 4L
#define com_lxc_jni1fmod_voicechange_MainActivity_MODE_KONGLING 5L

#define NULL nullptr

using namespace FMOD; // 添加FMOD的命名空间

//是否正在播放 (通过判断playStatus对象是否为NULL，来确定是否正在播放。)
jclass playStatus;

extern "C"
JNIEXPORT void JNICALL
Java_com_lxc_jni_1fmod_MainActivity_voiceChangeNative(JNIEnv *env, jobject thiz, jint mode,
                                                      jstring path) {

    const char *_play_finish;

    if(playStatus) { //playStatus != nullptr
        _play_finish = "正在播放,请稍后重试.";
        jclass mainCls = env->GetObjectClass(thiz);
        jmethodID toastMethodId = env->GetMethodID(mainCls, "playFinish", "(Ljava/lang/String;)V");
        auto value = env->NewStringUTF(_play_finish);
        env->CallVoidMethod(thiz, toastMethodId, value);
        env->DeleteLocalRef(mainCls);
        return;
    }

    jclass temp = env->FindClass("java/lang/Boolean");
    playStatus = (jclass)env->NewGlobalRef(temp);
    env->DeleteLocalRef(temp);

    //把java的路径转换成C++的路径。String -> char *
    const char *_path = env->GetStringUTFChars(path, NULL);

    //音效引擎系统指针
    System *system = NULL;

    //声音指针
    Sound *sound = NULL;

    //通道，音轨，声音在上面跑 跑道指针
    Channel *channel = NULL;

    //数字信号处理DSP
    DSP *dsp = NULL;

    //第一步: 创建系统
    System_Create(&system);//指针的指针
    //第二步: 系统初始化 参数1: 最大音轨数; 参数2: 系统初始化标记; 参数3: 额外标记不使用
    system->init(32, FMOD_INIT_NORMAL, 0);
    //第三步: 创建声音 参数1: 声音文件的路径; 参数2: 声音初始化标记; 参数3: 额外数据; 参数4: 声音指针
    system->createSound(_path, FMOD_DEFAULT, 0, &sound);
    //第四步: 播放声音 音轨声音 参数1: 声音; 参数2: 分组音轨; 参数3: 是否控制; 参数4: 通道。
    system->playSound(sound, 0, false, &channel);

    //第五步: 增加特效
    switch (mode) {
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_NORMAL:
            _play_finish = "【原生】播放完成";
            break;
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_LUOLI:
            _play_finish = "【萝莉】播放完成";

            //通过数字信号调整音调
            //1，创建DSP类型的Pitch 音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);

            //2，通过Pitch调节音调
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 2.0f);

            //3，把DSP的结果添加到音轨，进行播放.
            channel->addDSP(0, dsp);

            break;
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_DASHU:
            _play_finish = "【大叔】播放完成";

            //通过数字信号调整音调
            //1，创建DSP类型的Pitch 音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);

            //2，通过Pitch调节音调
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);

            //3，把DSP的结果添加到音轨，进行播放.
            channel->addDSP(0, dsp);
            break;
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_JINGSONG:
            _play_finish = "【惊悚】播放完成";

            //通过调整不同的声音，并添加多个音轨进行播放.

            //低音调
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7f);
            channel->addDSP(0, dsp);

            //回音
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10);
            channel->addDSP(1, dsp);

            //颤抖
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_FREQUENCY, 20);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 10);
            channel->addDSP(2, dsp);

            break;
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_GAOGUAI:
            _play_finish = "【搞怪】播放完成";

            //通过音轨调整声音播放频率。

            //1，从音轨获取当前帧率
            float mFrequency;
            channel->getFrequency(&mFrequency);

            //2，修改当前帧率
            channel->setFrequency(mFrequency * 1.5f);

            break;
        case com_lxc_jni1fmod_voicechange_MainActivity_MODE_KONGLING:
            _play_finish = "【空灵】播放完成";

            //通过增加音轨添加颤抖音、回音
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 200);//回音延时。默认500
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 10);//回音衰减度 默认50 0表示完全衰减了

            // 把DSP的结果添加到音轨，进行播放.
            channel->addDSP(0, dsp);

            break;
        default:
            _play_finish = "非法参数";
            break;
    }

    //第六步: 监听音轨通道，播放完成后释放。如果没有播放完成，则等待.
    bool isPlaying = -1;
    while (playStatus != NULL) {//playStatus != nullptr
        //从音轨获取播放状态
        channel->isPlaying(&isPlaying);//如果播放完成，内部会把isPlaying地址的值修改为0
        if(!isPlaying && playStatus) {
            env->DeleteGlobalRef(playStatus);
            playStatus = NULL;
        }
        usleep(1000 * 1000);
    }

    //第七步: 回收资源(需要在播放完成后回收，不然还没有播放就回收，是没有声音的。)
    sound->release();
    system->close();
    system->release();
    env->ReleaseStringUTFChars(path, _path);

    //第八步: 播放完成，告知Java。通过调用 java 方法的方式。
    jclass mainCls = env->GetObjectClass(thiz);
    jmethodID toastMethodId = env->GetMethodID(mainCls, "playFinish", "(Ljava/lang/String;)V");
    jstring value = env->NewStringUTF(_play_finish);
    env->CallVoidMethod(thiz, toastMethodId, value);
    env->DeleteLocalRef(mainCls);
}

//#undef : 表示取消宏的定义
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_NORMAL
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_LUOLI
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_DASHU
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_JINGSONG
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_GAOGUAI
#undef com_lxc_jni1fmod_voicechange_MainActivity_MODE_KONGLING