/*
版权所有 (c) 2016, 深圳市天双科技有限公司
保留所有权利.

V 1.0
作者: tianshuang

描述: CommonlibAPI

历史: 1. 2016/12/29, 初稿
      2. 2017/1/15，优化
      3. 2017/1/18，修改TshComm_SetTouchEvent的参数x和y为归一化的坐标值。
      4. 2017/2/3，修改了TlvPacket_ReadBody的实现。
      5. 2017/2/6，增加了几个API函数:
            void TshComm_SetCaliState(uint32_t state);
            uint32_t TshComm_GetCaliState();
            void TshComm_SetLastCaliIfSuccess(BOOL bSuccess);
            BOOL TshComm_GetLastCaliIfSuccess();
            uint32_t TShComm_GetCaliErrorCode()
      6. 2017/2/16，增加了：
            void TshComm_SetCarInfomation(uint32_t type, int32_t value);
            void TshComm_GetCarInfomation(stCarInfo_t* carInfo)
      7. 2017/4/25，修改了TshComm_SetViewingMode的注释文字，主要是说明参数的取值场景；
            添加了内部App调用的函数TshComm_InformXXXFlag
      8. 2017/5/5，增加了外部使用的API：
            void TshComm_SetCommandEvent(uint32_t command);
            void TshComm_SetStraightCaliParams(uint32_t type, int32_t value)
            增加了内部调用的API：
            void TshComm_GetStraightCaliParams(stStraightCaliParams_t *param)
      9. 2017/5/11，增加了外部调用API：
            void TshComm_SetRadarInfo(uint32_t position, uint32_t state)
            增加了内部调用API：
            void TshComm_GetRadarInfo(stRadarInfo_t *info)
      10. 2017/5/12，增加了外部调用API：
            void TshComm_SetChessParams(stChessParams_t *param)；
            void TshComm_SetCalibration(uint32_t type, uint32_t vision);
      11. 2017/5/27，增加了外部调用API：
            void TshComm_SetPowerState(uint32_t type, uint32_t state);
      12. 2017/6/2，增加了内部和外部可调用App：
            void TshComm_SetScreenBrightness(int32_t brightness);
            增加了内部调用App：
            int32_t TshComm_GetScreenBrightness();
            void TshComm_SendScreenBrightness(int32_t brightness);
*/

#ifndef __TSHCOMMUNICATION_H__
#define __TSHCOMMUNICATION_H__

#include "tshinnerdef.h"

typedef void (*MsgProcessCb_t)(void* param, void* data, uint32_t len);

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  内部App和外部App都要调用的API

// 功能：资源初始化
// appType：app分类类型，取值参考枚举类型EN_APP_TYPE，支持按位或
// cb：线程回调函数
// param：回调函数的一个参数
// 返回：0 成功；-1 失败
int TshComm_Init(uint32_t appType, MsgProcessCb_t cb, void* param);

// 功能：检查内部通信资源是否准备好
// 返回： TRUE 内部通信就绪；FALSE 内部通信未就绪
BOOL TshComm_IsReady();

// 功能：资源释放
void TshComm_Exit();

// 功能：获取错误码，在调用TshComm_Init失败时可以获取错误码
// 返回：参考枚举类型EN_ERROR_CODE
uint32_t TshComm_GetInnerErrorCode();

// 功能：设置屏幕亮度值 
// 参数brightness：亮度值，值的含义后定
// 说明：
void TshComm_SetScreenBrightness(int32_t brightness);


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  外部App调用的API

// 功能：设置tsh程序通知功能，由EN_APP_OUTER类型的app调用
// flag：取值参考枚举类型EN_TSH_INFORM_MODE，支持按位或
void TshComm_SetInformFlag(uint32_t flag);

// 功能：设置键盘事件
// eventType：按键事件类型，取值参考枚举EN_TSH_KEYEVENT_TYPE
// keyType：按键类型，取值参考枚举类型EN_TSH_KEY_TYPE
void TshComm_SetKeyEvent(uint32_t eventType, uint32_t keyType);

// 功能：设置触摸事件
// eventType：触摸事件类型，取值参考EN_TSH_TOUCHEVENT_TYPE
// x：归一化的横坐标(值范围0.0~1.0)
// y：归一化的纵坐标(值范围0.0~1.0)
void TshComm_SetTouchEvent(uint32_t eventType, float x, float y);

// 功能：设置车速
// speed：车速
void TshComm_SetVehicleSpeed(float speed);

// 功能：设置方向盘角度
// angle: 角度，<0 表示方向右；>0 表示方向左  取值范围：-45~45
void TshComm_SetVehicleWheelAngle(float angle);

// 功能：设置时间
// year: 年，比如2016
// month：月，1~12
// mday：日，1~31
// hour：时，0~23
// minute：分，0~59
// second：秒，0~59
void TshComm_SetSysTime(int32_t year, int32_t month, int32_t mday, int32_t hour, int32_t minute, int32_t second);

// 功能：设置语言信息
// lang：取值参考枚举类型EN_TSH_LANG_TYPE
void TshComm_SetLangInfo(uint32_t lang);

// 功能：设置车辆信号，目前支持信号：驾驶门、副驾驶门、后备箱门
// signalFlag：参考枚举类型EN_TSH_SIGNAL_TYPE，支持按位或
// bOpened：TRUE 开；FALSE 关
void TshComm_SetVehicleSignal(uint32_t signalFlag, BOOL bOpened);

// 功能：设置车辆事件
// event：参考枚举类型EN_TSH_VEHICLE_EVENT
void TshComm_SetVehicleEvent(uint32_t event);

// 功能：设置ADAS功能
// adasFlag：参考枚举类型EN_TSH_ADAS_TYPE，支持按位或
// bOpened：TRUE 开；FALSE 关
void TshComm_SetAdasFeatures(uint32_t adasFlag, BOOL bOpened);

// 功能：设置标定视野类型
// field：参考枚举类型EN_TSH_CALI_TYPE
// 说明：此函数已废弃，请参考API函数TshComm_SetCalibration(uint32_t type, uint32_t field)
void TshComm_SetCaliFieldOfView(uint32_t field);

// 功能：设置切换的界面程序，仅在当前界面为主界面时生效
// app：参考枚举类型EN_TSH_360APP_TYPE，此处不支持按位或
void TshComm_SetDirectEnterApp(uint32_t app);

// 功能：设置显示视角
// viewMode：标准版360全景，取值参考枚举类型EN_TSH_VIEWING_TYPE；定制版360全景，取值后定。
// angle：仅viewMode是3D自定义角度视角时才生效；其他情况时，建议angle取0.
void TshComm_SetViewingMode(uint32_t viewMode, float angle);

// 功能：设置录像功能
// recordingFlag：参考枚举类型EN_TSH_RECORDING_TYPE，支持按位或
// bOpened：TRUE 开；FALSE 关
void TshComm_SetRecording(uint32_t recordingFlag, BOOL bOpened);

// 功能：设置视频
// videoMode：参考枚举类型EN_TSH_VIDEO_STATE
// 说明：此函数只为设定标定是否从视频抓图
void TshComm_SetVideoMode(uint32_t videoMode);

// 功能：获取摄像头状态
// 返回：四个摄像的状态，参考枚举类型EN_TSH_CAMERA_STATE，对应的比特位如果是1则正常，否则异常
uint32_t TshComm_GetCamerasState();

// 功能：获取主机状态
// 返回：参考枚举类型EN_TSH_HOST_STATE
uint32_t TshComm_GetHostState();

// 功能：获取Usb电源状态
// 返回：参考枚举类型EN_TSH_USBPOWER_STATE
uint32_t TshComm_GetUsbPwoerState();

// 获取版本信息
void TShComm_GetVersionInfo(stVersion_t* version);

// 功能：设置恢复默认配置
void TshComm_SetRecoverConfig(BOOL bRecovered);

// 功能：获取标定的错误码
// 返回：错误码，0表示成功，>0表示失败
uint32_t TShComm_GetCaliErrorCode();

// 功能：获取标定的状态
// 返回：标定的状态，参考枚举类型EN_TSH_CALI_STATE
uint32_t TshComm_GetCaliState();

// 功能：获取是否曾经标定成功
// 返回：TRUE 已标定；FALSE 未标定
BOOL TshComm_GetLastCaliIfSuccess();

// 功能：设置车辆信息
// type：参考枚举类型EN_TSH_CARINFO_TYPE
// value: 值
void TshComm_SetCarInfomation(uint32_t type, int32_t value);

// 功能：设置命令事件，包含优化等
// 参数command：参考枚举EN_TSH_COMMAND_EVENT
// 说明：此API适合外部设置无参命令；直线标定前可调用TshComm_SetVideoMode是否要抓图
void TshComm_SetCommandEvent(uint32_t command);

// 功能：设置直线标定的参数；如果需要直线标定，请使用TshComm_SetCommandEvent
// 参数type：参考枚举类型EN_TSH_STRAIGHT_CALI_PARAM
// 参数value：值
// 说明：
void TshComm_SetStraightCaliParams(uint32_t type, int32_t value);

// 功能：设置雷达信息提示
// 参数position：参考枚举EN_TSH_RADAR_POSITION；如有特殊需求，可以重新定义
// 参数state：参考枚举EN_TSH_RADAR_STATE；如有特殊需求，可以重新定义
// 说明：
void TshComm_SetRadarInfo(uint32_t position, uint32_t state);

// 功能：设置棋盘参数
// 参数param：参考结构体stChessParam_t
void TshComm_SetChessParams(stChessParams_t *param);

// 功能：设置标定
// 参数type：标定类型，参考枚举EN_TSH_CALIBRATION_TYPE
// 参数vision：标定视野，参考枚举EN_TSH_CALI_VISION
void TshComm_SetCalibration(uint32_t type, uint32_t vision);

// 功能：设置电源状态
// 参数type：电源类型，参考枚举EN_TSH_POWER_TYPE
// 参数state：电源状态，参考枚举EN_TSH_POWER_STATE
void TshComm_SetPowerState(uint32_t type, uint32_t state);


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  内部App调用的API

// 功能：获取tsh程序通知功能，由EN_APP_INNER类型的app调用
// 返回：枚举类型EN_TSH_INFORM_MODE的按位或
uint32_t TshComm_GetInformFlag();

// 功能：获取语言信息
// 返回：参考枚举类型EN_TSH_LANG_TYPE
uint32_t TshComm_GetLangInfo();

// 功能：获取车速
// 返回：车速
float TshComm_GetVehicleSpeed();

// 功能：获取方向盘角度
// 返回: 角度，<0 表示方向右；>0 表示方向左
float TshComm_GetVehicleWheelAngle();

// 功能：设置摄像头状态
// cameraFlag：参考枚举类型EN_TSH_CAMERA_STATE，支持按位或
// bWorked：TRUE 正常；FALSE 异常
void TshComm_SetCamerasState(uint32_t cameraFlag, BOOL bWorked);

// 功能：设置主机状态
// hostState：参考枚举类型EN_TSH_HOST_STATE
void TshComm_SetHostState(uint32_t hostState);

// 功能：设置Usb电源状态
// usbPowerState：参考枚举类型EN_TSH_USBPOWER_STATE
void TshComm_SetUsbPwoerState(uint32_t usbPowerState);

// 设置版本号
void TshComm_SetVersionInfo(stVersion_t* version);

// 功能：通知外部当前tsh菜单，函数内部自会判断是否要发送通知消息，但菜单变化时调用者必须调用此函数
// currApp：参考枚举类型EN_TSH_360APP_TYPE，不支持按位或
void TshComm_InformCurrentMenu(uint32_t currApp);

// 通知外部当前视角，函数内部自会判断是否要发送通知消息，但视角变化时调用者必须调用此函数
// viewMode：参考枚举类型EN_TSH_VIEWING_TYPE
// angle：仅viewMode是3D自定义角度视角时才生效；其他情况时，建议angle取0
void TshComm_InformCurrentViewingAngle(uint32_t viewMode, float angle);

// 功能：通知外部录像状态，函数内部自会判断是否要发送通知消息，但录像状态变化时调用者必须调用此函数
// state：参考枚举类型EN_TSH_RECORDING_STATE
void TshComm_InformRecordingState(uint32_t state);

// 功能：通知外部视频异常，函数内部自会判断是否要发送通知消息，但视频异常时调用者必须调用此函数
void TshComm_InformVideoException();

// 功能：通知标定的结果，标定完成后必须调用此函数
// ret：标定完成后错误码，0表示成功；>0 响应的错误码
void TshComm_InformCaliResult(uint32_t ret);

// 功能：设置标定的状态
// state：参考枚举类型EN_TSH_CALI_STATE
void TshComm_SetCaliState(uint32_t state);

// 功能：设置是否曾经标定成功
// bSuccess：TRUE 已标定；FALSE 未标定
void TshComm_SetLastCaliIfSuccess(BOOL bSuccess);

// 功能：检查并进程ID列表，仅由EN_APP_SERVICE类型的app调用
void TshComm_TimerForClearMsg();

// 功能：按指定app类型广播消息，使用TLV编码时调用
// appType：参考枚举类型EN_APP_TYPE，支持按位或
// buff：消息缓存的首地址
// len：消息长度
// 返回：-1  失败，检查内部通信是否正常或参数是否非法；0 成功
int TshComm_SendMsg(uint32_t appType, void* buff, uint32_t len);

// 功能：按消息ID发送消息，点对点
// mtype：消息ID，接收者app的进程ID
// buff：消息缓存的首地址
// len：消息长度
// 返回：-1  失败，检查内部通信是否正常或参数是否非法；0 成功
int TshComm_SendMsg2(uint32_t mtype, void* buff, uint32_t len);

// 功能：获取车辆信息
// carInfo：参考结构stCarInfo_t
void TshComm_GetCarInfomation(stCarInfo_t* carInfo);

// 功能：通知自定义参数
// val：自定义值，具体含义后定
void TshComm_InformXXXFlag(uint32_t val);

// 功能：获取直线标定参数
// 参数param：参考结构体stStraightCaliParams_t
void TshComm_GetStraightCaliParams(stStraightCaliParams_t *param);

// 功能：获取雷达信息
// 参数info：请参考结构体stRadarInfo_t
void TshComm_GetRadarInfo(stRadarInfo_t *info);

// 功能：获取屏幕亮度值
// 返回：屏幕亮度值
int32_t TshComm_GetScreenBrightness();

// 功能：发送外部App屏幕亮度值
// 参数brightness：亮度值
void TshComm_SendScreenBrightness(int32_t brightness);

//////////////////////////////////////////////////////////////
///////////////////////  TLV编解码 API ///////////////////////
//// 内部App和外部App都调用的API
int TlvPacket_Init(stTlvPacket_t* packet, void* buffer, uint32_t len);
///// code /////
//// 内部App调用的API
// 返回值：0 成功；-1 错误，检查函数参数或缓存长度
int TlvPacket_WriteHead(stTlvPacket_t* packet, uint16_t type);
int TlvPacket_WriteBody_1B(stTlvPacket_t* packet, uint16_t tag, int8_t val);
int TlvPacket_WriteBody_U1B(stTlvPacket_t* packet, uint16_t tag, uint8_t val);
int TlvPacket_WriteBody_2B(stTlvPacket_t* packet, uint16_t tag, int16_t val);
int TlvPacket_WriteBody_U2B(stTlvPacket_t* packet, uint16_t tag, uint16_t val);
int TlvPacket_WriteBody_4B(stTlvPacket_t* packet, uint16_t tag, int32_t val);
int TlvPacket_WriteBody_U4B(stTlvPacket_t* packet, uint16_t tag, uint32_t val);
int TlvPacket_WriteBody_Float(stTlvPacket_t* packet, uint16_t tag, float val);
int TlvPacket_WriteBody_Double(stTlvPacket_t* packet, uint16_t tag, double val);
int TlvPacket_WriteBody_String(stTlvPacket_t* packet, uint16_t tag, char* data, uint32_t len);
int TlvPacket_WriteBody_Complex(stTlvPacket_t* packet, uint16_t tag, void* data, uint32_t len);
///// decode //////
//// 外部App调用的API
BOOL TlvPacket_ReadHead(stTlvPacket_t* packet, uint16_t* type);
char* TlvPacket_ReadBody(stTlvPacket_t* packet, uint16_t* tag, uint16_t* len);

#ifdef __cplusplus
}
#endif

#endif // __TSHCOMMUNICATION_H__
