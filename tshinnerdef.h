/*
��Ȩ���� (c) 2016, ��������˫�Ƽ����޹�˾
��������Ȩ��.

V 1.0
����: tianshuang

����: CommonlibAPI

��ʷ: 1. 2016/12/29, ����
      2. 2017/1/15���Ż�

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
#define MSG_QUEUE_KEY_VAL           0x1234  // ��Ϣ���е�key
#define MESSAGE_MAX_SIZE            819200  // bytes
#define QUEUE_MAX_SIZE              1638400  // bytes

#define SEMAPHORE_KEY1_VAL          0x1236   // �ź�����key
#define SEMAPHORE_KEY2_VAL          0x1237
#define SHARED_MEMORY_KEY_VAL       0x1235  // �����ڴ��key

#define MSG_MAX_LEN                 2048

#define PID_BUFFER_MAX_NUM  128

//  ������
typedef enum {
    EN_CREATE_MSGQUE_ERROR          = 1,
    EN_CREATE_SEMPHORE_ERROR        = 2,
    EN_CREATE_SHAREDMEMORY_ERROR    = 3,
    EN_CREATE_RECVMSGTHREAD_ERROR   = 4
}EN_ERROR_CODE;

// APP����(uint32_t)
typedef enum {
    EN_APP_NONE         = 0,
    EN_APP_INNER        = 0x00000001, // �ڲ�app������360ϵapp
    EN_APP_OUTER        = 0x00000002, // �ⲿapp�������ȡ���ڵ�app
    EN_APP_SERVICE      = 0x00000004, // �ػ�����service
}EN_APP_TYPE;

// �汾����Ϣ
typedef struct Version {
    uint32_t nHighV;
    uint32_t nLowV;
    int32_t nYear;
    int32_t nMonth;
    int32_t nMday;
}__attribute__((packed)) stVersion_t;

// app�ı��λ(uint32_t)
typedef enum {
    EN_TSH_360APP_NONE     = 0,
    EN_TSH_360APP_TSHMAIN  = 0x00000001, // ������
    EN_TSH_360APP_TSHCALI  = 0x00000002, // �궨����
    EN_TSH_360APP_TSHCONF  = 0x00000004, // ���ý���
    EN_TSH_360APP_TSHPLAY  = 0x00000008, // ���Ž���
    EN_TSH_360APP_SWITCH   = 0x00000010,
}EN_TSH_360APP_TYPE;

// ��Ϣ����(uint16_t) 0-0xffff
typedef enum {
    EN_TSH_MSG_NONE             = 0x0000,
    EN_TSH_MSG_SWITCHAPP        = 0x0001, // �л�app��ֻ����tshmain,tshconf,tshcali,tshplay���͸�switch
    EN_TSH_MSG_EXITAPP          = 0x0002, // ��switch���͸�tshconf��tshcali��tshplay����tshconfֱ�ӷ���tshmain
    EN_TSH_MSG_SHOWAPP          = 0x0003, // ֻ����switch���͸�tshconf��tshcali��tshplay
    EN_TSH_MSG_HIDEAPP          = 0x0004, // ֻ����switch���͸�tshconf��tshcali��tshplay
    EN_TSH_MSG_HIDEAPP_RESP     = 0x0005, // ֻ��tshmain���͸�switch
        
    EN_TSH_MSG_REQUEST          = 0x0006, // ������Ϣ
    EN_TSH_MSG_RESPONE          = 0x0007, // Ӧ����Ϣ
    EN_TSH_MSG_INFORM           = 0x0008, // ֪ͨ��Ϣ
    EN_TSH_MSG_INVALID          = 0xffff
}EN_TSH_MSG_TYPE;

// �ֶ�����(uint16_t) 0-0xffff
enum {
    EN_TSH_FIELD_NONE                           = 0x0000,
    // switch �ڲ�Appʹ��       
    EN_TSH_FIELD_SRCAPP                         = 0x0001, // Դapp(uint32_t)
    EN_TSH_FIELD_DSTAPP                         = 0x0002, // Ŀ��app(uint32_t)
    EN_TSH_FIELD_OPTAPP                         = 0x0003, //  Ҫ������app(uint32_t)
    
    // business  �ڲ�Appʹ��        
    EN_TSH_FIELD_TOUCHEVENT                     = 0x03e9, // �����¼�(stPoint_t)
    EN_TSH_FIELD_KEYEVENT                       = 0x03ea, // �����¼�(uint32_t)
    EN_TSH_FIELD_VEHICLE_SIGNAL                 = 0x03eb, // �����ź�(uint32_t)
    EN_TSH_FIELD_VEHICLE_EVENT                  = 0x03ec, // �����¼�(uint32_t)
    EN_TSH_FIELD_ADAS_FEATURES                  = 0x03ed, // adas����
    EN_TSH_FIELD_CALI_FIELD_OF_VIEW             = 0x03ee, // �궨��Ұ����
    EN_TSH_FIELD_SYSTIME                        = 0x03ef, // ϵͳʱ��(stSysTime_t)
    EN_TSH_FIELD_SWITCH_APP                     = 0x03f0, // 360app�л�����
    EN_TSH_FIELD_VIEWMODE                       = 0x03f1, // �ӽ�ģʽ
    EN_TSH_FIELD_RECORDING                      = 0x03f2, // ¼��
    EN_TSH_FIELD_VIDEOMODE                      = 0x03f3, // ��Ƶ
    EN_TSH_FIELD_RESET_DEFAULTCFG               = 0x03f4, // �ָ�Ĭ������  (int)
    EN_TSH_FIELD_LANGUAGE                       = 0x03f5, // ������Ϣ
    EN_TSH_FIELD_COMMAND_EVENT                  = 0x03f6, // �ⲿ�����¼�
    EN_TSH_FIELD_CHESS_PARAMS                   = 0x03f7, // ���̲��� (stChessParams_t)
    EN_TSH_FIELD_CALIBRATION                    = 0x03f8, // �궨(stCalibration_t)
    EN_TSH_FIELD_POWER                          = 0x03f9, // ��Դ(stPowerState_t)
    
    // inform  �ⲿAppʹ��
    // ��Ҫ�ڻص������н��н��
    EN_TSH_FIELD_INFORM_CURRAPP                 = 0xfffe, // ֪ͨ�ⲿ��ǰapp(uint32_t)  ֵ�ο�EN_TSH_360APP_TYPE
    EN_TSH_FIELD_INFORM_CURRVIEWINGANGLE        = 0xfffd, // ֪ͨ��ǰ�ӽ�(stViewingMode_t)  ֵ�ο�EN_TSH_VIEWING_TYPE
    EN_TSH_FIELD_INFORM_RECORDINGSTATE          = 0xfffc, // ֪ͨ¼��״̬(uint32_t) ֵ�ο�EN_TSH_RECORDING_STATE
    EN_TSH_FIELD_INFORM_VIDEOEXCEPTION          = 0xfffb, // ֪ͨ��Ƶ�쳣(uint32_t)  ֵΪ1
    EN_TSH_FIELD_INFORM_CALIRESULT              = 0xfffa, // ֪ͨ�궨���(uint32_t)  ֵΪ�궨�Ĵ����룬0��ʾ�ɹ���>0 ��ʾʧ��
    EN_TSH_FIELD_INFORM_XXXFLAG                 = 0xfff9, // �Զ���֪ͨ��uint32_t�� ֵ��
    EN_TSH_FIELD_RSP_SCREEN_BRIGHTNESS          = 0xfff8, // Ӧ����Ļ����ֵ��int32_t��
    
    EN_TSH_FIELD_INVALID                        = 0xffff
};

// �����¼�����
typedef enum {
    EN_TSH_KEYEVENT_AUTO    = 1, // һ���Դ���down��up
    EN_TSH_KEYEVENT_UP      = 2, // ��������
    EN_TSH_KEYEVENT_DOWN    = 3  // ��������
}EN_TSH_KEYEVENT_TYPE;

// �����¼�����
typedef enum {
    EN_TSH_TOUCHEVENT_AUTO  = 1, // һ���Դ���down��up
    EN_TSH_TOUCHEVENT_UP    = 2, // ��������
    EN_TSH_TOUCHEVENT_DOWN  = 3, // ��������
    EN_TSH_TOUCHEVENT_MOVE  = 4  // �����ƶ�
}EN_TSH_TOUCHEVENT_TYPE;

// ��������
typedef enum {
    EN_TSH_KEY_NONE         = 0,
    EN_TSH_KEY_UP           = 1, // ������
    EN_TSH_KEY_DOWN         = 2, // ������
    EN_TSH_KEY_LEFT         = 3, // ������
    EN_TSH_KEY_RIGHT        = 4, // ������
    EN_TSH_KEY_RETURN       = 5, // ��������
    EN_TSH_KEY_CONFIRM      = 6, // ����ȷ��
    EN_TSH_KEY_1            = 7, // ��������1
    EN_TSH_KEY_2            = 8  // ��������2
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

// �ӽ�ģʽ(uint32_t)
// �°棺������0ǰ  1��ǰ  2�Һ�  3��  4���  5��ǰ  6����
typedef enum {
    EN_TSH_VIEW_NONE                                    = 0,
    EN_TSH_VIEW_2D_FRONT_AND_SPLICING                   = 1, // ��ʾ2Dǰ���ӽ�+ƴ��
    EN_TSH_VIEW_2D_BACK_AND_SPLICING                    = 2, // ��ʾ2D�����ӽ�+ƴ��
    EN_TSH_VIEW_2D_LEFT_AND_SPLICING                    = 3, // ��ʾ2D�����ӽ�+ƴ��
    EN_TSH_VIEW_2D_RIGHT_AND_SPLICING                   = 4, // ��ʾ2D�����ӽ�+ƴ��
    EN_TSH_VIEW_3D_FRONT_AND_SPLICING                   = 5, // ��ʾ3Dǰ���ӽ�+ƴ��
    EN_TSH_VIEW_3D_RF_AND_SPLICING                      = 6, // ��ʾ3D��ǰ�ӽ�+ƴ��
    EN_TSH_VIEW_3D_RB_AND_SPLICING                      = 7, // ��ʾ3D�Һ��ӽ�+ƴ��
    EN_TSH_VIEW_3D_BACK_AND_SPLICING                    = 8, // ��ʾ3D�����ӽ�+ƴ��
    EN_TSH_VIEW_3D_LB_AND_SPLICING                      = 9, // ��ʾ3D����ӽ�+ƴ��
    EN_TSH_VIEW_3D_LF_AND_SPLICING                      = 10, // ��ʾ3D��ǰ�ӽ�+ƴ��
    EN_TSH_VIEW_3D_REVERSE_AND_SPLICING                 = 11, // ��ʾ3D�����ӽ�+ƴ��
    EN_TSH_VIEW_3D_SPECIFIC_ANGLE_AND_SPLICING          = 12, // ��ʾ3D�Զ���Ƕ��ӽ�+ƴ��
    EN_TSH_VIEW_WIDE_DEFAULT                            = 13, // ��ʾ���ӽǣ�Ĭ�ϣ�
    EN_TSH_VIEW_WIDE_FRONT                              = 14, // ��ʾ���ӽǣ�ǰ��
    EN_TSH_VIEW_WIDE_BACK                               = 15, // ��ʾ���ӽǣ���
    EN_TSH_VIEW_INVALID                                 = 0xffffffff // �Ƿ�
}EN_TSH_VIEWING_TYPE;

typedef struct ViewingMode {
    uint32_t nMode;
    float fAngle;
}__attribute__((packed)) stViewingMode_t;

// ��Ƶ
typedef enum {
    EN_TSH_VIDEO_NONE       = 0,
    EN_TSH_VIDEO_OPEN       = 1, // ��Ƶ��
    EN_TSH_VIDEO_CLOSE      = 2  // ��Ƶ�ر�
}EN_TSH_VIDEO_STATE;

// ������Ϣ
typedef enum {
    EN_TSH_LANG_NONE                    = 0,
    EN_TSH_LANG_SIMPLIFIED_CHINESE      = 1, // ��������
    EN_TSH_LANG_TRADITIONAL_CHINESE     = 2, // ��������
    EN_TSH_LANG_ENGLISH                 = 3  // Ӣ��
}EN_TSH_LANG_TYPE;

// �����ź�
typedef enum {
    EN_TSH_VEHICLE_SIGNAL_NONE              = 0,
    EN_TSH_VEHICLE_SIGNAL_DRIVER_DOOR       = 0x00000001, // ��ʻԱ���ź�
    EN_TSH_VEHICLE_SIGNAL_CODRIVER_DOOR     = 0x00000002, // ����ʻԱ���ź�
    EN_TSH_VEHICLE_SIGNAL_TRUNK_DOOR            = 0x00000004,  // �������ź�
}EN_TSH_SIGNAL_TYPE;

typedef struct VehicleSignal {
    uint32_t nSignal;
    BOOL bOpened;
}__attribute__((packed)) stVehicleSignal_t;

// �����¼�
typedef enum {
    EN_TSH_VEHICLE_EVENT_NONE                   = 0,
    EN_TSH_VEHICLE_EVENT_RFGEAR_CHANGE          = 1, // ���뵹��״̬
    EN_TSH_VEHICLE_EVENT_FRGEAR_CHANGE          = 2, // �˳�����״̬
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

// adas����
typedef enum {
    EN_TSH_ADAS_NONE    = 0,
    EN_TSH_ADAS_BSD     = 0x00000001, // ä�����
    EN_TSH_ADAS_LDW     = 0x00000002, // ����ƫ�뾯ʾ
    EN_TSH_ADAS_FCW     = 0x00000004, // ǰ����ײԤ��
    EN_TSH_ADAS_MOD     = 0x00000008, // �ƶ�������
    EN_TSH_ADAS_TRACE   = 0x00000010, // �켣��
    EN_TSH_ADAS_SOUND   = 0x00000020, // ������ʾ
    EN_TSH_ADAS_DISPLAY = 0x00000040  // ��ʾ��ʾ
}EN_TSH_ADAS_TYPE;

typedef struct AdasFeatures {
    uint32_t nAdasFlag;
    BOOL bOpened;
}__attribute__((packed)) stAdasFeatures_t;

// �궨��Ұ����
typedef enum {
    EN_TSH_CALI_FIELD_NONE          = 0,     // 
    EN_TSH_CALI_FIELD_WIDE          = 1, // ����Ұ�궨
    EN_TSH_CALI_FIELD_MIDDLE        = 2, // ����Ұ�궨
    EN_TSH_CALI_FIELD_SMALL         = 3, // С��Ұ�궨
    EN_TSH_CALI_FIELD_HIGH          = 4  // ����Ұ�궨
}EN_TSH_CALI_TYPE;

// ¼������
typedef enum {
    EN_TSH_RECORDING_NONE       = 0,
    EN_TSH_RECORDING_COMMON     = 0x00000001,   // ��ͨ¼��
    EN_TSH_RECORDING_URGENT     = 0x00000002    // ����¼��
}EN_TSH_RECORDING_TYPE;

typedef struct Recording {
    uint32_t nRecordingFlag;
    BOOL bOpened;
}__attribute__((packed)) stRecording_t;

// ����ͷ����
typedef enum {
    EN_TSH_CAMERA_NONE      = 0,
    EN_TSH_CAMERA_1         = 0x00000001,   // ����ͷ1
    EN_TSH_CAMERA_2         = 0x00000002,   // ����ͷ2
    EN_TSH_CAMERA_3         = 0x00000004,   // ����ͷ3
    EN_TSH_CAMERA_4         = 0x00000008    // ����ͷ4
}EN_TSH_CAMERA_STATE;

// ����״̬
typedef enum {
    EN_TSH_HOST_STATE_NONE      = 0,
    EN_TSH_HOST_STATE_WORKING   = 1,    // �������ڹ���
    EN_TSH_HOST_STATE_EXCEPTION = 2     // �����쳣
}EN_TSH_HOST_STATE;

// USB��Դ״̬
typedef enum {
    EN_TSH_USBPOWER_STATE_NONE      = 0,
    EN_TSH_USBPOWER_STATE_OPEN      = 1, // usb��Դ��
    EN_TSH_USBPOWER_STATE_CLOSE     = 2  // usb��Դ�ر�
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


// tsh��������֪ͨ����
typedef enum {
    EN_TSH_INFORM_NONE                  = 0,
    EN_TSH_INFORM_VIEWING_ANGLE         = 0x00000001, // �ӽǱ仯ʱ֪ͨ��ǰ���ӽ�
    EN_TSH_INFORM_RECORDING_STATE       = 0x00000002, // ¼��ʼ��ֹͣʱ֪ͨ¼���״̬
    EN_TSH_INFORM_CURRENT_MENU          = 0x00000004, // �˵��仯ʱ֪ͨ
    EN_TSH_INFORM_VIDEO_EXCEPTION       = 0x00000008,  // ��Ƶ�쳣ʱ֪ͨ
    EN_TSH_INFORM_CALI_RESULT           = 0x00000010,    // �궨���֪ͨ
    EN_TSH_INFORM_XXXFLAG               = 0x00000020    // �Զ���֪ͨ
}EN_TSH_INFORM_MODE;

// tsh����¼��״̬�仯ʱ֪ͨ
typedef enum {
    EN_TSH_RECORDING_COMMON_OPEN        = 1,  // ��ͨ¼��״̬��
    EN_TSH_RECORDING_COMMON_CLOSE       = 2,  // ��ͨ¼��״̬�ر�
    EN_TSH_RECORDING_URGENT_OPEN        = 3,  // ����¼��״̬��
    EN_TSH_RECORDING_URGENT_CLOSE       = 4   // ����¼��״̬�ر�
}EN_TSH_RECORDING_STATE;

// ʱ��
typedef struct SysTime {
    int32_t year;
    int32_t month;
    int32_t mday;
    int32_t hour;
    int32_t minute;
    int32_t second;
}__attribute__((packed)) stSysTime_t;

// ����
typedef struct Point {
    int32_t x;
    int32_t y;
}__attribute__((packed)) stPoint_t;

typedef enum {
    EN_TSH_CALI_NONE        = 0, // δ���б궨
    EN_TSH_CALI_RUNNING     = 1, // ���ڱ궨
    EN_TSH_CALI_SUCCESS     = 2, // �궨�ɹ�
    EN_TSH_CALI_FAIL        = 3 // �궨ʧ��
}EN_TSH_CALI_STATE;

typedef struct {
    int32_t nCarPhysicalLength; // ����
    int32_t nCarPhysicalWidth; // ����
    int32_t nLRPhysicalDist; // ǰ��
    int32_t nWheelBase; // ���
    int32_t nWheel2Rear; // β��
    int32_t nCaliMode; // ģʽ
}__attribute__((packed)) stCarInfo_t;
typedef enum {
    EN_TSH_CARINFO_NONE         = 0,
    EN_TSH_CARINFO_LENGTH       = 1, // ����
    EN_TSH_CARINFO_WIDTH        = 2, // ����
    EN_TSH_CARINFO_DIST         = 3, // ǰ��
    EN_TSH_CARINFO_WHEELBASE    = 4, // ���
    EN_TSH_CARINFO_WHEEL2REAR   = 5, // β��
    EN_TSH_CARINFO_CALIMODE     = 6 // ģʽ
}EN_TSH_CARINFO_TYPE;
typedef enum {
    EN_TSH_CMD_EVENT_NONE               = 0,
    EN_TSH_CMD_EVENT_OPTIMIZATION       = 1, // �Ż�
}EN_TSH_COMMAND_EVENT;
typedef enum {
    EN_TSH_STRAIGHT_CALI_NONE           = 0, 
    EN_TSH_STRAIGHT_CALI_LANEWIDTH      = 1, // �߾� ��λmm
    EN_TSH_STRAIGHT_CALI_LEFTDIST       = 2, // ��� ��λmm
    EN_TSH_STRAIGHT_CALI_RIGHTDIST      = 3  // �Ҿ� ��λmm
}EN_TSH_STRAIGHT_CALI_PARAM;
typedef struct StraightCaliParams {
    int nLaneWidth;      // �߾� ��λmm
    int nLeftDist;       // ��� ��λmm
    int nRightDist;      // �Ҿ� ��λmm
}__attribute__((packed)) stStraightCaliParams_t;
typedef enum {
    EN_TSH_RADAR_POS_LF     = 0, // ��ǰ
    EN_TSH_RADAR_POS_RF     = 1, // ��ǰ
    EN_TSH_RADAR_POS_LB     = 2, // ���
    EN_TSH_RADAR_POS_RB     = 3,  // �Һ�
    EN_TSH_RADAR_POS_MAX
}EN_TSH_RADAR_POSITION;
typedef enum {
    EN_TSH_RADAR_STATE_NONE         = 0, // �ر�
    EN_TSH_RADAR_STATE_NORMAL       = 1, // ����
    EN_TSH_RADAR_STATE_ATTENTION    = 2, // ע��
    EN_TSH_RADAR_STATE_WARNING      = 3  // ����
}EN_TSH_RADAR_STATE;
typedef struct RadarInfo {
    uint32_t arrPositions[EN_TSH_RADAR_POS_MAX];
}__attribute__((packed)) stRadarInfo_t;
typedef struct ChessParams {
    int32_t nChessMode;                     // ���̸�ģʽ 1 ��װ 2 sd 3 sf
    int32_t nLRChessSpaceDist;              // �������̸�֮��ľ��룬<=0���ʾ���޸�ֵ
    int32_t nFBChessSpaceDist;              // ǰ�����̸�֮��ľ��룬<=0���ʾ���޸�ֵ
    int32_t nFrontLRDist;                   // ǰ���������̸�֮��ľ��룬<=0���ʾ���޸�ֵ
    int32_t nFrontBorderSize;               // ǰ���̸��복���룬<0���ʾ���޸�ֵ
    int32_t nBackBorderSize;                // �����̸��복���룬<0���ʾ���޸�ֵ
    int32_t nLeftBorderSize;                // �����̸��복���룬<0���ʾ���޸�ֵ
    int32_t nRightBorderSize;               // �����̸��복���룬<0���ʾ���޸�ֵ
}__attribute__((packed)) stChessParams_t;
typedef enum {
    EN_TSH_CALIBRATION_NONE    = 0,
    EN_TSH_CALIBRATION_CHESSBOARD      = 1, // ���̸�궨
    EN_TSH_CALIBRATION_BEELINE         = 2  // ֱ�߱궨
}EN_TSH_CALIBRATION_TYPE;
typedef enum {
    EN_TSH_CALI_VISION_NONE         = 0,
    EN_TSH_CALI_VISION_BIG          = 1, // ����Ұ
    EN_TSH_CALI_VISION_MIDDLE       = 2, // ����Ұ
    EN_TSH_CALI_VISION_SMALL        = 3, // С��Ұ
    EN_TSH_CALI_VISION_HIGH         = 4  // ����Ұ
}EN_TSH_CALI_VISION;
typedef struct Calibration {
    uint32_t nCaliType;     // �궨����
    uint32_t nVisionMode;   // �궨��Ұ
}__attribute__((packed)) stCalibration_t;
typedef enum {
    EN_TSH_POWER_TYP_NONE       = 0,
    EN_TSH_POWER_TYP_CAMERAS    = 1     // �ĸ�����ͷ
}EN_TSH_POWER_TYPE;
typedef enum {
    EN_TSH_POWER_STAT_NONE  = 0,
    EN_TSH_POWER_STAT_ON    = 1,    // ��
    EN_TSH_POWER_STAT_OFF   = 2     // ��
}EN_TSH_POWER_STATE;
typedef struct PowerState{
    uint32_t nType;
    uint32_t nState;
}__attribute__((packed)) stPowerState_t;
#endif // __TSHINNERDEF_H__
