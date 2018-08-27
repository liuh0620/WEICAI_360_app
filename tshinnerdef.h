/*
版权所有 (c) 2016, 深圳市天双科技有限公司
保留所有权利.

V 1.0
作者: tianshuang

描述: CommonlibAPI

历史: 1. 2016/12/29, 初稿
      2. 2017/1/15，优化

*/
#ifndef __TSHINNERDEF_H__
#define __TSHINNERDEF_H__


#include <stdint.h>
#include <string.h>

#ifndef BOOL
typedef int BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned int  uint32_t;
typedef  int int32_t;
typedef unsigned short uint16_t;
#define MSG_QUEUE_KEY_VAL           0x1234  // 消息队列的key
#define MESSAGE_MAX_SIZE            819200  // bytes
#define QUEUE_MAX_SIZE              1638400  // bytes

#define SEMAPHORE_KEY1_VAL          0x1236   // 信号量的key
#define SEMAPHORE_KEY2_VAL          0x1237
#define SHARED_MEMORY_KEY_VAL       0x1235  // 共享内存的key

#define MSG_MAX_LEN                 2048

#define PID_BUFFER_MAX_NUM  128

//  错误码
typedef enum {
    EN_CREATE_MSGQUE_ERROR          = 1,
    EN_CREATE_SEMPHORE_ERROR        = 2,
    EN_CREATE_SHAREDMEMORY_ERROR    = 3,
    EN_CREATE_RECVMSGTHREAD_ERROR   = 4
}EN_ERROR_CODE;

// APP类型(uint32_t)
typedef enum {
    EN_APP_NONE         = 0,
    EN_APP_INNER        = 0x00000001, // 内部app，比如360系app
    EN_APP_OUTER        = 0x00000002, // 外部app，比如读取串口的app
    EN_APP_SERVICE      = 0x00000004, // 守护进程service
}EN_APP_TYPE;

// 版本号信息
typedef struct Version {
    uint32_t nHighV;
    uint32_t nLowV;
    int32_t nYear;
    int32_t nMonth;
    int32_t nMday;
}__attribute__((packed)) stVersion_t;

// app的标记位(uint32_t)
typedef enum {
    EN_TSH_360APP_NONE     = 0,
    EN_TSH_360APP_TSHMAIN  = 0x00000001, // 主界面
    EN_TSH_360APP_TSHCALI  = 0x00000002, // 标定界面
    EN_TSH_360APP_TSHCONF  = 0x00000004, // 配置界面
    EN_TSH_360APP_TSHPLAY  = 0x00000008, // 播放界面
    EN_TSH_360APP_SWITCH   = 0x00000010,
}EN_TSH_360APP_TYPE;

// 消息类型(uint16_t) 0-0xffff
typedef enum {
    EN_TSH_MSG_NONE             = 0x0000,
    EN_TSH_MSG_SWITCHAPP        = 0x0001, // 切换app，只能由tshmain,tshconf,tshcali,tshplay发送给switch
    EN_TSH_MSG_EXITAPP          = 0x0002, // 由switch发送给tshconf或tshcali或tshplay；由tshconf直接发给tshmain
    EN_TSH_MSG_SHOWAPP          = 0x0003, // 只能由switch发送给tshconf或tshcali或tshplay
    EN_TSH_MSG_HIDEAPP          = 0x0004, // 只能由switch发送给tshconf或tshcali或tshplay
    EN_TSH_MSG_HIDEAPP_RESP     = 0x0005, // 只由tshmain发送给switch
        
    EN_TSH_MSG_REQUEST          = 0x0006, // 请求消息
    EN_TSH_MSG_RESPONE          = 0x0007, // 应答消息
    EN_TSH_MSG_INFORM           = 0x0008, // 通知消息
    EN_TSH_MSG_INVALID          = 0xffff
}EN_TSH_MSG_TYPE;

// 字段类型(uint16_t) 0-0xffff
enum {
    EN_TSH_FIELD_NONE                           = 0x0000,
    // switch 内部App使用       
    EN_TSH_FIELD_SRCAPP                         = 0x0001, // 源app(uint32_t)
    EN_TSH_FIELD_DSTAPP                         = 0x0002, // 目的app(uint32_t)
    EN_TSH_FIELD_OPTAPP                         = 0x0003, //  要操作的app(uint32_t)
    
    // business  内部App使用        
    EN_TSH_FIELD_TOUCHEVENT                     = 0x03e9, // 触摸事件(stPoint_t)
    EN_TSH_FIELD_KEYEVENT                       = 0x03ea, // 键盘事件(uint32_t)
    EN_TSH_FIELD_VEHICLE_SIGNAL                 = 0x03eb, // 车辆信号(uint32_t)
    EN_TSH_FIELD_VEHICLE_EVENT                  = 0x03ec, // 车辆事件(uint32_t)
    EN_TSH_FIELD_ADAS_FEATURES                  = 0x03ed, // adas功能
    EN_TSH_FIELD_CALI_FIELD_OF_VIEW             = 0x03ee, // 标定视野类型
    EN_TSH_FIELD_SYSTIME                        = 0x03ef, // 系统时间(stSysTime_t)
    EN_TSH_FIELD_SWITCH_APP                     = 0x03f0, // 360app切换命令
    EN_TSH_FIELD_VIEWMODE                       = 0x03f1, // 视角模式
    EN_TSH_FIELD_RECORDING                      = 0x03f2, // 录像
    EN_TSH_FIELD_VIDEOMODE                      = 0x03f3, // 视频
    EN_TSH_FIELD_RESET_DEFAULTCFG               = 0x03f4, // 恢复默认设置  (int)
    EN_TSH_FIELD_LANGUAGE                       = 0x03f5, // 语言信息
    EN_TSH_FIELD_COMMAND_EVENT                  = 0x03f6, // 外部命令事件
    EN_TSH_FIELD_CHESS_PARAMS                   = 0x03f7, // 棋盘参数 (stChessParams_t)
    EN_TSH_FIELD_CALIBRATION                    = 0x03f8, // 标定(stCalibration_t)
    EN_TSH_FIELD_POWER                          = 0x03f9, // 电源(stPowerState_t)
    
    // inform  外部App使用
    // 需要在回调函数中进行解包
    EN_TSH_FIELD_INFORM_CURRAPP                 = 0xfffe, // 通知外部当前app(uint32_t)  值参考EN_TSH_360APP_TYPE
    EN_TSH_FIELD_INFORM_CURRVIEWINGANGLE        = 0xfffd, // 通知当前视角(stViewingMode_t)  值参考EN_TSH_VIEWING_TYPE
    EN_TSH_FIELD_INFORM_RECORDINGSTATE          = 0xfffc, // 通知录像状态(uint32_t) 值参考EN_TSH_RECORDING_STATE
    EN_TSH_FIELD_INFORM_VIDEOEXCEPTION          = 0xfffb, // 通知视频异常(uint32_t)  值为1
    EN_TSH_FIELD_INFORM_CALIRESULT              = 0xfffa, // 通知标定结果(uint32_t)  值为标定的错误码，0表示成功，>0 表示失败
    EN_TSH_FIELD_INFORM_XXXFLAG                 = 0xfff9, // 自定义通知（uint32_t） 值后定
    EN_TSH_FIELD_RSP_SCREEN_BRIGHTNESS          = 0xfff8, // 应答屏幕亮度值（int32_t）
    
    EN_TSH_FIELD_INVALID                        = 0xffff
};

// 按键事件类型
typedef enum {
    EN_TSH_KEYEVENT_AUTO    = 1, // 一次性触发down和up
    EN_TSH_KEYEVENT_UP      = 2, // 触发弹起
    EN_TSH_KEYEVENT_DOWN    = 3  // 触发按下
}EN_TSH_KEYEVENT_TYPE;

// 触摸事件类型
typedef enum {
    EN_TSH_TOUCHEVENT_AUTO  = 1, // 一次性触发down和up
    EN_TSH_TOUCHEVENT_UP    = 2, // 触发弹起
    EN_TSH_TOUCHEVENT_DOWN  = 3, // 触发按下
    EN_TSH_TOUCHEVENT_MOVE  = 4  // 触发移动
}EN_TSH_TOUCHEVENT_TYPE;

// 按键类型
typedef enum {
    EN_TSH_KEY_NONE         = 0,
    EN_TSH_KEY_UP           = 1, // 按键上
    EN_TSH_KEY_DOWN         = 2, // 按键下
    EN_TSH_KEY_LEFT         = 3, // 按键左
    EN_TSH_KEY_RIGHT        = 4, // 按键右
    EN_TSH_KEY_RETURN       = 5, // 按键返回
    EN_TSH_KEY_CONFIRM      = 6, // 按键确定
    EN_TSH_KEY_1            = 7, // 按键数字1
    EN_TSH_KEY_2            = 8  // 按键数字2
}EN_TSH_KEY_TYPE;

typedef struct TouchActor {
    uint32_t nEventType;
    float x;
    float y;
}stTouchActor_t;

typedef struct KeyActor {
    uint32_t nEventType;
    uint32_t nKey;
}stKeyActor_t;

// 视角模式(uint32_t)
// 新版：车朝向：0前  1右前  2右后  3后  4左后  5左前  6倒车
typedef enum {
    EN_TSH_VIEW_NONE                                    = 0,
    EN_TSH_VIEW_2D_FRONT_AND_SPLICING                   = 1, // 显示2D前视视角+拼接
    EN_TSH_VIEW_2D_BACK_AND_SPLICING                    = 2, // 显示2D后视视角+拼接
    EN_TSH_VIEW_2D_LEFT_AND_SPLICING                    = 3, // 显示2D左视视角+拼接
    EN_TSH_VIEW_2D_RIGHT_AND_SPLICING                   = 4, // 显示2D右视视角+拼接
    EN_TSH_VIEW_3D_FRONT_AND_SPLICING                   = 5, // 显示3D前视视角+拼接
    EN_TSH_VIEW_3D_RF_AND_SPLICING                      = 6, // 显示3D右前视角+拼接
    EN_TSH_VIEW_3D_RB_AND_SPLICING                      = 7, // 显示3D右后视角+拼接
    EN_TSH_VIEW_3D_BACK_AND_SPLICING                    = 8, // 显示3D后视视角+拼接
    EN_TSH_VIEW_3D_LB_AND_SPLICING                      = 9, // 显示3D左后视角+拼接
    EN_TSH_VIEW_3D_LF_AND_SPLICING                      = 10, // 显示3D左前视角+拼接
    EN_TSH_VIEW_3D_REVERSE_AND_SPLICING                 = 11, // 显示3D倒车视角+拼接
    EN_TSH_VIEW_3D_SPECIFIC_ANGLE_AND_SPLICING          = 12, // 显示3D自定义角度视角+拼接
    EN_TSH_VIEW_WIDE_DEFAULT                            = 13, // 显示宽视角（默认）
    EN_TSH_VIEW_WIDE_FRONT                              = 14, // 显示宽视角（前）
    EN_TSH_VIEW_WIDE_BACK                               = 15, // 显示宽视角（后）
    EN_TSH_VIEW_INVALID                                 = 0xffffffff // 非法
}EN_TSH_VIEWING_TYPE;

typedef struct ViewingMode {
    uint32_t nMode;
    float fAngle;
}__attribute__((packed)) stViewingMode_t;

// 视频
typedef enum {
    EN_TSH_VIDEO_NONE       = 0,
    EN_TSH_VIDEO_OPEN       = 1, // 视频打开
    EN_TSH_VIDEO_CLOSE      = 2  // 视频关闭
}EN_TSH_VIDEO_STATE;

// 语言信息
typedef enum {
    EN_TSH_LANG_NONE                    = 0,
    EN_TSH_LANG_SIMPLIFIED_CHINESE      = 1, // 简体中文
    EN_TSH_LANG_TRADITIONAL_CHINESE     = 2, // 繁体中文
    EN_TSH_LANG_ENGLISH                 = 3  // 英语
}EN_TSH_LANG_TYPE;

// 车辆信号
typedef enum {
    EN_TSH_VEHICLE_SIGNAL_NONE              = 0,
    EN_TSH_VEHICLE_SIGNAL_DRIVER_DOOR       = 0x00000001, // 驾驶员门信号
    EN_TSH_VEHICLE_SIGNAL_CODRIVER_DOOR     = 0x00000002, // 副驾驶员门信号
    EN_TSH_VEHICLE_SIGNAL_TRUNK_DOOR            = 0x00000004,  // 后备箱门信号
}EN_TSH_SIGNAL_TYPE;

typedef struct VehicleSignal {
    uint32_t nSignal;
    BOOL bOpened;
}__attribute__((packed)) stVehicleSignal_t;

// 车辆事件
typedef enum {
    EN_TSH_VEHICLE_EVENT_NONE                   = 0,
    EN_TSH_VEHICLE_EVENT_RFGEAR_CHANGE          = 1, // 进入倒车状态
    EN_TSH_VEHICLE_EVENT_FRGEAR_CHANGE          = 2, // 退出倒车状态
    EN_TSH_VEHICLE_EVENT_LLIGHT_ON              = 3, // left light is turned on
    EN_TSH_VEHICLE_EVENT_LLIGHT_OFF             = 4, // left light is turned off
    EN_TSH_VEHICLE_EVENT_RLIGHT_ON              = 5, // right light is turned on
    EN_TSH_VEHICLE_EVENT_RLIGHT_OFF             = 6, // right light is turned off
    EN_TSH_VEHICLE_EVENT_ELIGHT_ON              = 7, // emergency light is turned on
    EN_TSH_VEHICLE_EVENT_ELIGHT_OFF             = 8, // emergency light is turned off
    EN_TSH_VEHICLE_EVENT_FLIGHT_ON              = 9, // front light is turned on
    EN_TSH_VEHICLE_EVENT_FLIGHT_OFF             = 10, // front light is turned off
    EN_TSH_VEHICLE_EVENT_BRAKE_ON               = 11, // brake on
    EN_TSH_VEHICLE_EVENT_BRAKE_OFF              = 12  // brake off
}EN_TSH_VEHICLE_EVENT;

// adas功能
typedef enum {
    EN_TSH_ADAS_NONE    = 0,
    EN_TSH_ADAS_BSD     = 0x00000001, // 盲区检测
    EN_TSH_ADAS_LDW     = 0x00000002, // 车道偏离警示
    EN_TSH_ADAS_FCW     = 0x00000004, // 前方碰撞预警
    EN_TSH_ADAS_MOD     = 0x00000008, // 移动物体检测
    EN_TSH_ADAS_TRACE   = 0x00000010, // 轨迹线
    EN_TSH_ADAS_SOUND   = 0x00000020, // 声音提示
    EN_TSH_ADAS_DISPLAY = 0x00000040  // 显示提示
}EN_TSH_ADAS_TYPE;

typedef struct AdasFeatures {
    uint32_t nAdasFlag;
    BOOL bOpened;
}__attribute__((packed)) stAdasFeatures_t;

// 标定视野类型
typedef enum {
    EN_TSH_CALI_FIELD_NONE          = 0,     // 
    EN_TSH_CALI_FIELD_WIDE          = 1, // 大视野标定
    EN_TSH_CALI_FIELD_MIDDLE        = 2, // 中视野标定
    EN_TSH_CALI_FIELD_SMALL         = 3, // 小视野标定
    EN_TSH_CALI_FIELD_HIGH          = 4  // 超视野标定
}EN_TSH_CALI_TYPE;

// 录像类型
typedef enum {
    EN_TSH_RECORDING_NONE       = 0,
    EN_TSH_RECORDING_COMMON     = 0x00000001,   // 普通录像
    EN_TSH_RECORDING_URGENT     = 0x00000002    // 紧急录像
}EN_TSH_RECORDING_TYPE;

typedef struct Recording {
    uint32_t nRecordingFlag;
    BOOL bOpened;
}__attribute__((packed)) stRecording_t;

// 摄像头类型
typedef enum {
    EN_TSH_CAMERA_NONE      = 0,
    EN_TSH_CAMERA_1         = 0x00000001,   // 摄像头1
    EN_TSH_CAMERA_2         = 0x00000002,   // 摄像头2
    EN_TSH_CAMERA_3         = 0x00000004,   // 摄像头3
    EN_TSH_CAMERA_4         = 0x00000008    // 摄像头4
}EN_TSH_CAMERA_STATE;

// 主机状态
typedef enum {
    EN_TSH_HOST_STATE_NONE      = 0,
    EN_TSH_HOST_STATE_WORKING   = 1,    // 主机正在工作
    EN_TSH_HOST_STATE_EXCEPTION = 2     // 主机异常
}EN_TSH_HOST_STATE;

// USB电源状态
typedef enum {
    EN_TSH_USBPOWER_STATE_NONE      = 0,
    EN_TSH_USBPOWER_STATE_OPEN      = 1, // usb电源打开
    EN_TSH_USBPOWER_STATE_CLOSE     = 2  // usb电源关闭
}EN_TSH_USBPOWER_STATE;

typedef struct Tlv {
    uint16_t tag;
    uint16_t len;
    uint8_t val[0];
}__attribute__((packed)) stTlv_t;


typedef struct TlvPacket {
    char* mData;
    char* mTempData;
    uint32_t mLen;
    uint32_t mTempLen;
    uint32_t mRealLen;
    BOOL mHeadOpted;
}stTlvPacket_t;


// tsh程序主动通知功能
typedef enum {
    EN_TSH_INFORM_NONE                  = 0,
    EN_TSH_INFORM_VIEWING_ANGLE         = 0x00000001, // 视角变化时通知当前的视角
    EN_TSH_INFORM_RECORDING_STATE       = 0x00000002, // 录像开始或停止时通知录像的状态
    EN_TSH_INFORM_CURRENT_MENU          = 0x00000004, // 菜单变化时通知
    EN_TSH_INFORM_VIDEO_EXCEPTION       = 0x00000008,  // 视频异常时通知
    EN_TSH_INFORM_CALI_RESULT           = 0x00000010,    // 标定结果通知
    EN_TSH_INFORM_XXXFLAG               = 0x00000020    // 自定义通知
}EN_TSH_INFORM_MODE;

// tsh程序录像状态变化时通知
typedef enum {
    EN_TSH_RECORDING_COMMON_OPEN        = 1,  // 普通录像状态打开
    EN_TSH_RECORDING_COMMON_CLOSE       = 2,  // 普通录像状态关闭
    EN_TSH_RECORDING_URGENT_OPEN        = 3,  // 紧急录像状态打开
    EN_TSH_RECORDING_URGENT_CLOSE       = 4   // 紧急录像状态关闭
}EN_TSH_RECORDING_STATE;

// 时间
typedef struct SysTime {
    int32_t year;
    int32_t month;
    int32_t mday;
    int32_t hour;
    int32_t minute;
    int32_t second;
}__attribute__((packed)) stSysTime_t;

// 坐标
typedef struct Point {
    int32_t x;
    int32_t y;
}__attribute__((packed)) stPoint_t;

typedef enum {
    EN_TSH_CALI_NONE        = 0, // 未进行标定
    EN_TSH_CALI_RUNNING     = 1, // 正在标定
    EN_TSH_CALI_SUCCESS     = 2, // 标定成功
    EN_TSH_CALI_FAIL        = 3 // 标定失败
}EN_TSH_CALI_STATE;

typedef struct {
    int32_t nCarPhysicalLength; // 车长
    int32_t nCarPhysicalWidth; // 车宽
    int32_t nLRPhysicalDist; // 前距
    int32_t nWheelBase; // 轴距
    int32_t nWheel2Rear; // 尾距
    int32_t nCaliMode; // 模式
}__attribute__((packed)) stCarInfo_t;
typedef enum {
    EN_TSH_CARINFO_NONE         = 0,
    EN_TSH_CARINFO_LENGTH       = 1, // 车长
    EN_TSH_CARINFO_WIDTH        = 2, // 车宽
    EN_TSH_CARINFO_DIST         = 3, // 前距
    EN_TSH_CARINFO_WHEELBASE    = 4, // 轴距
    EN_TSH_CARINFO_WHEEL2REAR   = 5, // 尾距
    EN_TSH_CARINFO_CALIMODE     = 6 // 模式
}EN_TSH_CARINFO_TYPE;
typedef enum {
    EN_TSH_CMD_EVENT_NONE               = 0,
    EN_TSH_CMD_EVENT_OPTIMIZATION       = 1, // 优化
}EN_TSH_COMMAND_EVENT;
typedef enum {
    EN_TSH_STRAIGHT_CALI_NONE           = 0, 
    EN_TSH_STRAIGHT_CALI_LANEWIDTH      = 1, // 线距 单位mm
    EN_TSH_STRAIGHT_CALI_LEFTDIST       = 2, // 左距 单位mm
    EN_TSH_STRAIGHT_CALI_RIGHTDIST      = 3  // 右距 单位mm
}EN_TSH_STRAIGHT_CALI_PARAM;
typedef struct StraightCaliParams {
    int nLaneWidth;      // 线距 单位mm
    int nLeftDist;       // 左距 单位mm
    int nRightDist;      // 右距 单位mm
}__attribute__((packed)) stStraightCaliParams_t;
typedef enum {
    EN_TSH_RADAR_POS_LF     = 0, // 左前
    EN_TSH_RADAR_POS_RF     = 1, // 右前
    EN_TSH_RADAR_POS_LB     = 2, // 左后
    EN_TSH_RADAR_POS_RB     = 3,  // 右后
    EN_TSH_RADAR_POS_MAX
}EN_TSH_RADAR_POSITION;
typedef enum {
    EN_TSH_RADAR_STATE_NONE         = 0, // 关闭
    EN_TSH_RADAR_STATE_NORMAL       = 1, // 正常
    EN_TSH_RADAR_STATE_ATTENTION    = 2, // 注意
    EN_TSH_RADAR_STATE_WARNING      = 3  // 警告
}EN_TSH_RADAR_STATE;
typedef struct RadarInfo {
    uint32_t arrPositions[EN_TSH_RADAR_POS_MAX];
}__attribute__((packed)) stRadarInfo_t;
typedef struct ChessParams {
    int32_t nChessMode;                     // 棋盘格模式 1 后装 2 sd 3 sf
    int32_t nLRChessSpaceDist;              // 左右棋盘格之间的距离，<=0则表示不修改值
    int32_t nFBChessSpaceDist;              // 前后棋盘格之间的距离，<=0则表示不修改值
    int32_t nFrontLRDist;                   // 前与左右棋盘格之间的距离，<=0则表示不修改值
    int32_t nFrontBorderSize;               // 前棋盘格与车距离，<0则表示不修改值
    int32_t nBackBorderSize;                // 后棋盘格与车距离，<0则表示不修改值
    int32_t nLeftBorderSize;                // 左棋盘格与车距离，<0则表示不修改值
    int32_t nRightBorderSize;               // 右棋盘格与车距离，<0则表示不修改值
}__attribute__((packed)) stChessParams_t;
typedef enum {
    EN_TSH_CALIBRATION_NONE    = 0,
    EN_TSH_CALIBRATION_CHESSBOARD      = 1, // 棋盘格标定
    EN_TSH_CALIBRATION_BEELINE         = 2  // 直线标定
}EN_TSH_CALIBRATION_TYPE;
typedef enum {
    EN_TSH_CALI_VISION_NONE         = 0,
    EN_TSH_CALI_VISION_BIG          = 1, // 大视野
    EN_TSH_CALI_VISION_MIDDLE       = 2, // 中视野
    EN_TSH_CALI_VISION_SMALL        = 3, // 小视野
    EN_TSH_CALI_VISION_HIGH         = 4  // 超视野
}EN_TSH_CALI_VISION;
typedef struct Calibration {
    uint32_t nCaliType;     // 标定类型
    uint32_t nVisionMode;   // 标定视野
}__attribute__((packed)) stCalibration_t;
typedef enum {
    EN_TSH_POWER_TYP_NONE       = 0,
    EN_TSH_POWER_TYP_CAMERAS    = 1     // 四个摄像头
}EN_TSH_POWER_TYPE;
typedef enum {
    EN_TSH_POWER_STAT_NONE  = 0,
    EN_TSH_POWER_STAT_ON    = 1,    // 开
    EN_TSH_POWER_STAT_OFF   = 2     // 关
}EN_TSH_POWER_STATE;
typedef struct PowerState{
    uint32_t nType;
    uint32_t nState;
}__attribute__((packed)) stPowerState_t;
#endif // __TSHINNERDEF_H__

