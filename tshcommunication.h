/*
��Ȩ���� (c) 2016, ��������˫�Ƽ����޹�˾
��������Ȩ��.

V 1.0
����: tianshuang

����: CommonlibAPI

��ʷ: 1. 2016/12/29, ����
      2. 2017/1/15���Ż�
      3. 2017/1/18���޸�TshComm_SetTouchEvent�Ĳ���x��yΪ��һ��������ֵ��
      4. 2017/2/3���޸���TlvPacket_ReadBody��ʵ�֡�
      5. 2017/2/6�������˼���API����:
            void TshComm_SetCaliState(uint32_t state);
            uint32_t TshComm_GetCaliState();
            void TshComm_SetLastCaliIfSuccess(BOOL bSuccess);
            BOOL TshComm_GetLastCaliIfSuccess();
            uint32_t TShComm_GetCaliErrorCode()
      6. 2017/2/16�������ˣ�
            void TshComm_SetCarInfomation(uint32_t type, int32_t value);
            void TshComm_GetCarInfomation(stCarInfo_t* carInfo)
      7. 2017/4/25���޸���TshComm_SetViewingMode��ע�����֣���Ҫ��˵��������ȡֵ������
            ������ڲ�App���õĺ���TshComm_InformXXXFlag
      8. 2017/5/5���������ⲿʹ�õ�API��
            void TshComm_SetCommandEvent(uint32_t command);
            void TshComm_SetStraightCaliParams(uint32_t type, int32_t value)
            �������ڲ����õ�API��
            void TshComm_GetStraightCaliParams(stStraightCaliParams_t *param)
      9. 2017/5/11���������ⲿ����API��
            void TshComm_SetRadarInfo(uint32_t position, uint32_t state)
            �������ڲ�����API��
            void TshComm_GetRadarInfo(stRadarInfo_t *info)
      10. 2017/5/12���������ⲿ����API��
            void TshComm_SetChessParams(stChessParams_t *param)��
            void TshComm_SetCalibration(uint32_t type, uint32_t vision);
      11. 2017/5/27���������ⲿ����API��
            void TshComm_SetPowerState(uint32_t type, uint32_t state);
      12. 2017/6/2���������ڲ����ⲿ�ɵ���App��
            void TshComm_SetScreenBrightness(int32_t brightness);
            �������ڲ�����App��
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
////  �ڲ�App���ⲿApp��Ҫ���õ�API

// ���ܣ���Դ��ʼ��
// appType��app�������ͣ�ȡֵ�ο�ö������EN_APP_TYPE��֧�ְ�λ��
// cb���̻߳ص�����
// param���ص�������һ������
// ���أ�0 �ɹ���-1 ʧ��
int TshComm_Init(uint32_t appType, MsgProcessCb_t cb, void* param);

// ���ܣ�����ڲ�ͨ����Դ�Ƿ�׼����
// ���أ� TRUE �ڲ�ͨ�ž�����FALSE �ڲ�ͨ��δ����
BOOL TshComm_IsReady();

// ���ܣ���Դ�ͷ�
void TshComm_Exit();

// ���ܣ���ȡ�����룬�ڵ���TshComm_Initʧ��ʱ���Ի�ȡ������
// ���أ��ο�ö������EN_ERROR_CODE
uint32_t TshComm_GetInnerErrorCode();

// ���ܣ�������Ļ����ֵ 
// ����brightness������ֵ��ֵ�ĺ����
// ˵����
void TshComm_SetScreenBrightness(int32_t brightness);


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  �ⲿApp���õ�API

// ���ܣ�����tsh����֪ͨ���ܣ���EN_APP_OUTER���͵�app����
// flag��ȡֵ�ο�ö������EN_TSH_INFORM_MODE��֧�ְ�λ��
void TshComm_SetInformFlag(uint32_t flag);

// ���ܣ����ü����¼�
// eventType�������¼����ͣ�ȡֵ�ο�ö��EN_TSH_KEYEVENT_TYPE
// keyType���������ͣ�ȡֵ�ο�ö������EN_TSH_KEY_TYPE
void TshComm_SetKeyEvent(uint32_t eventType, uint32_t keyType);

// ���ܣ����ô����¼�
// eventType�������¼����ͣ�ȡֵ�ο�EN_TSH_TOUCHEVENT_TYPE
// x����һ���ĺ�����(ֵ��Χ0.0~1.0)
// y����һ����������(ֵ��Χ0.0~1.0)
void TshComm_SetTouchEvent(uint32_t eventType, float x, float y);

// ���ܣ����ó���
// speed������
void TshComm_SetVehicleSpeed(float speed);

// ���ܣ����÷����̽Ƕ�
// angle: �Ƕȣ�<0 ��ʾ�����ң�>0 ��ʾ������  ȡֵ��Χ��-45~45
void TshComm_SetVehicleWheelAngle(float angle);

// ���ܣ�����ʱ��
// year: �꣬����2016
// month���£�1~12
// mday���գ�1~31
// hour��ʱ��0~23
// minute���֣�0~59
// second���룬0~59
void TshComm_SetSysTime(int32_t year, int32_t month, int32_t mday, int32_t hour, int32_t minute, int32_t second);

// ���ܣ�����������Ϣ
// lang��ȡֵ�ο�ö������EN_TSH_LANG_TYPE
void TshComm_SetLangInfo(uint32_t lang);

// ���ܣ����ó����źţ�Ŀǰ֧���źţ���ʻ�š�����ʻ�š�������
// signalFlag���ο�ö������EN_TSH_SIGNAL_TYPE��֧�ְ�λ��
// bOpened��TRUE ����FALSE ��
void TshComm_SetVehicleSignal(uint32_t signalFlag, BOOL bOpened);

// ���ܣ����ó����¼�
// event���ο�ö������EN_TSH_VEHICLE_EVENT
void TshComm_SetVehicleEvent(uint32_t event);

// ���ܣ�����ADAS����
// adasFlag���ο�ö������EN_TSH_ADAS_TYPE��֧�ְ�λ��
// bOpened��TRUE ����FALSE ��
void TshComm_SetAdasFeatures(uint32_t adasFlag, BOOL bOpened);

// ���ܣ����ñ궨��Ұ����
// field���ο�ö������EN_TSH_CALI_TYPE
// ˵�����˺����ѷ�������ο�API����TshComm_SetCalibration(uint32_t type, uint32_t field)
void TshComm_SetCaliFieldOfView(uint32_t field);

// ���ܣ������л��Ľ�����򣬽��ڵ�ǰ����Ϊ������ʱ��Ч
// app���ο�ö������EN_TSH_360APP_TYPE���˴���֧�ְ�λ��
void TshComm_SetDirectEnterApp(uint32_t app);

// ���ܣ�������ʾ�ӽ�
// viewMode����׼��360ȫ����ȡֵ�ο�ö������EN_TSH_VIEWING_TYPE�����ư�360ȫ����ȡֵ�󶨡�
// angle����viewMode��3D�Զ���Ƕ��ӽ�ʱ����Ч���������ʱ������angleȡ0.
void TshComm_SetViewingMode(uint32_t viewMode, float angle);

// ���ܣ�����¼����
// recordingFlag���ο�ö������EN_TSH_RECORDING_TYPE��֧�ְ�λ��
// bOpened��TRUE ����FALSE ��
void TshComm_SetRecording(uint32_t recordingFlag, BOOL bOpened);

// ���ܣ�������Ƶ
// videoMode���ο�ö������EN_TSH_VIDEO_STATE
// ˵�����˺���ֻΪ�趨�궨�Ƿ����Ƶץͼ
void TshComm_SetVideoMode(uint32_t videoMode);

// ���ܣ���ȡ����ͷ״̬
// ���أ��ĸ������״̬���ο�ö������EN_TSH_CAMERA_STATE����Ӧ�ı���λ�����1�������������쳣
uint32_t TshComm_GetCamerasState();

// ���ܣ���ȡ����״̬
// ���أ��ο�ö������EN_TSH_HOST_STATE
uint32_t TshComm_GetHostState();

// ���ܣ���ȡUsb��Դ״̬
// ���أ��ο�ö������EN_TSH_USBPOWER_STATE
uint32_t TshComm_GetUsbPwoerState();

// ��ȡ�汾��Ϣ
void TShComm_GetVersionInfo(stVersion_t* version);

// ���ܣ����ûָ�Ĭ������
void TshComm_SetRecoverConfig(BOOL bRecovered);

// ���ܣ���ȡ�궨�Ĵ�����
// ���أ������룬0��ʾ�ɹ���>0��ʾʧ��
uint32_t TShComm_GetCaliErrorCode();

// ���ܣ���ȡ�궨��״̬
// ���أ��궨��״̬���ο�ö������EN_TSH_CALI_STATE
uint32_t TshComm_GetCaliState();

// ���ܣ���ȡ�Ƿ������궨�ɹ�
// ���أ�TRUE �ѱ궨��FALSE δ�궨
BOOL TshComm_GetLastCaliIfSuccess();

// ���ܣ����ó�����Ϣ
// type���ο�ö������EN_TSH_CARINFO_TYPE
// value: ֵ
void TshComm_SetCarInfomation(uint32_t type, int32_t value);

// ���ܣ����������¼��������Ż���
// ����command���ο�ö��EN_TSH_COMMAND_EVENT
// ˵������API�ʺ��ⲿ�����޲����ֱ�߱궨ǰ�ɵ���TshComm_SetVideoMode�Ƿ�Ҫץͼ
void TshComm_SetCommandEvent(uint32_t command);

// ���ܣ�����ֱ�߱궨�Ĳ����������Ҫֱ�߱궨����ʹ��TshComm_SetCommandEvent
// ����type���ο�ö������EN_TSH_STRAIGHT_CALI_PARAM
// ����value��ֵ
// ˵����
void TshComm_SetStraightCaliParams(uint32_t type, int32_t value);

// ���ܣ������״���Ϣ��ʾ
// ����position���ο�ö��EN_TSH_RADAR_POSITION�������������󣬿������¶���
// ����state���ο�ö��EN_TSH_RADAR_STATE�������������󣬿������¶���
// ˵����
void TshComm_SetRadarInfo(uint32_t position, uint32_t state);

// ���ܣ��������̲���
// ����param���ο��ṹ��stChessParam_t
void TshComm_SetChessParams(stChessParams_t *param);

// ���ܣ����ñ궨
// ����type���궨���ͣ��ο�ö��EN_TSH_CALIBRATION_TYPE
// ����vision���궨��Ұ���ο�ö��EN_TSH_CALI_VISION
void TshComm_SetCalibration(uint32_t type, uint32_t vision);

// ���ܣ����õ�Դ״̬
// ����type����Դ���ͣ��ο�ö��EN_TSH_POWER_TYPE
// ����state����Դ״̬���ο�ö��EN_TSH_POWER_STATE
void TshComm_SetPowerState(uint32_t type, uint32_t state);


////////////////////////////////////////////////////////////////////////////////////////////////////////////
////  �ڲ�App���õ�API

// ���ܣ���ȡtsh����֪ͨ���ܣ���EN_APP_INNER���͵�app����
// ���أ�ö������EN_TSH_INFORM_MODE�İ�λ��
uint32_t TshComm_GetInformFlag();

// ���ܣ���ȡ������Ϣ
// ���أ��ο�ö������EN_TSH_LANG_TYPE
uint32_t TshComm_GetLangInfo();

// ���ܣ���ȡ����
// ���أ�����
float TshComm_GetVehicleSpeed();

// ���ܣ���ȡ�����̽Ƕ�
// ����: �Ƕȣ�<0 ��ʾ�����ң�>0 ��ʾ������
float TshComm_GetVehicleWheelAngle();

// ���ܣ���������ͷ״̬
// cameraFlag���ο�ö������EN_TSH_CAMERA_STATE��֧�ְ�λ��
// bWorked��TRUE ������FALSE �쳣
void TshComm_SetCamerasState(uint32_t cameraFlag, BOOL bWorked);

// ���ܣ���������״̬
// hostState���ο�ö������EN_TSH_HOST_STATE
void TshComm_SetHostState(uint32_t hostState);

// ���ܣ�����Usb��Դ״̬
// usbPowerState���ο�ö������EN_TSH_USBPOWER_STATE
void TshComm_SetUsbPwoerState(uint32_t usbPowerState);

// ���ð汾��
void TshComm_SetVersionInfo(stVersion_t* version);

// ���ܣ�֪ͨ�ⲿ��ǰtsh�˵��������ڲ��Ի��ж��Ƿ�Ҫ����֪ͨ��Ϣ�����˵��仯ʱ�����߱�����ô˺���
// currApp���ο�ö������EN_TSH_360APP_TYPE����֧�ְ�λ��
void TshComm_InformCurrentMenu(uint32_t currApp);

// ֪ͨ�ⲿ��ǰ�ӽǣ������ڲ��Ի��ж��Ƿ�Ҫ����֪ͨ��Ϣ�����ӽǱ仯ʱ�����߱�����ô˺���
// viewMode���ο�ö������EN_TSH_VIEWING_TYPE
// angle����viewMode��3D�Զ���Ƕ��ӽ�ʱ����Ч���������ʱ������angleȡ0
void TshComm_InformCurrentViewingAngle(uint32_t viewMode, float angle);

// ���ܣ�֪ͨ�ⲿ¼��״̬�������ڲ��Ի��ж��Ƿ�Ҫ����֪ͨ��Ϣ����¼��״̬�仯ʱ�����߱�����ô˺���
// state���ο�ö������EN_TSH_RECORDING_STATE
void TshComm_InformRecordingState(uint32_t state);

// ���ܣ�֪ͨ�ⲿ��Ƶ�쳣�������ڲ��Ի��ж��Ƿ�Ҫ����֪ͨ��Ϣ������Ƶ�쳣ʱ�����߱�����ô˺���
void TshComm_InformVideoException();

// ���ܣ�֪ͨ�궨�Ľ�����궨��ɺ������ô˺���
// ret���궨��ɺ�����룬0��ʾ�ɹ���>0 ��Ӧ�Ĵ�����
void TshComm_InformCaliResult(uint32_t ret);

// ���ܣ����ñ궨��״̬
// state���ο�ö������EN_TSH_CALI_STATE
void TshComm_SetCaliState(uint32_t state);

// ���ܣ������Ƿ������궨�ɹ�
// bSuccess��TRUE �ѱ궨��FALSE δ�궨
void TshComm_SetLastCaliIfSuccess(BOOL bSuccess);

// ���ܣ���鲢����ID�б�����EN_APP_SERVICE���͵�app����
void TshComm_TimerForClearMsg();

// ���ܣ���ָ��app���͹㲥��Ϣ��ʹ��TLV����ʱ����
// appType���ο�ö������EN_APP_TYPE��֧�ְ�λ��
// buff����Ϣ������׵�ַ
// len����Ϣ����
// ���أ�-1  ʧ�ܣ�����ڲ�ͨ���Ƿ�����������Ƿ�Ƿ���0 �ɹ�
int TshComm_SendMsg(uint32_t appType, void* buff, uint32_t len);

// ���ܣ�����ϢID������Ϣ����Ե�
// mtype����ϢID��������app�Ľ���ID
// buff����Ϣ������׵�ַ
// len����Ϣ����
// ���أ�-1  ʧ�ܣ�����ڲ�ͨ���Ƿ�����������Ƿ�Ƿ���0 �ɹ�
int TshComm_SendMsg2(uint32_t mtype, void* buff, uint32_t len);

// ���ܣ���ȡ������Ϣ
// carInfo���ο��ṹstCarInfo_t
void TshComm_GetCarInfomation(stCarInfo_t* carInfo);

// ���ܣ�֪ͨ�Զ������
// val���Զ���ֵ�����庬���
void TshComm_InformXXXFlag(uint32_t val);

// ���ܣ���ȡֱ�߱궨����
// ����param���ο��ṹ��stStraightCaliParams_t
void TshComm_GetStraightCaliParams(stStraightCaliParams_t *param);

// ���ܣ���ȡ�״���Ϣ
// ����info����ο��ṹ��stRadarInfo_t
void TshComm_GetRadarInfo(stRadarInfo_t *info);

// ���ܣ���ȡ��Ļ����ֵ
// ���أ���Ļ����ֵ
int32_t TshComm_GetScreenBrightness();

// ���ܣ������ⲿApp��Ļ����ֵ
// ����brightness������ֵ
void TshComm_SendScreenBrightness(int32_t brightness);

//////////////////////////////////////////////////////////////
///////////////////////  TLV����� API ///////////////////////
//// �ڲ�App���ⲿApp�����õ�API
int TlvPacket_Init(stTlvPacket_t* packet, void* buffer, uint32_t len);
///// code /////
//// �ڲ�App���õ�API
// ����ֵ��0 �ɹ���-1 ���󣬼�麯�������򻺴泤��
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
//// �ⲿApp���õ�API
BOOL TlvPacket_ReadHead(stTlvPacket_t* packet, uint16_t* type);
char* TlvPacket_ReadBody(stTlvPacket_t* packet, uint16_t* tag, uint16_t* len);

#ifdef __cplusplus
}
#endif

#endif // __TSHCOMMUNICATION_H__
