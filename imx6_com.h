#ifndef IMX6_COM_H
#define IMX6_COM_H

#include <termios.h>
#include <fcntl.h>
#include <pthread.h>
typedef unsigned int  uint;
typedef unsigned char uchar;



#define MAX_REC_SIZE 2048
#define MAX_SEND_SIZE 2048
#define MSG_HEAD             0xF0
#define MSG_DATA_OFFSET      7
#define MSG_LEN_OFFSET       6
#define MSG_CMD_OFFSET       1
#define MSG_HID_OFFSET       2
#define MSG_HEAD_SIZE        7


#define	CM_GENERAL_ACK		    0x00
#define	CM_ARM_READY			    0x81
#define	CM_GET_MODE_ACK			0X82
#define	CM_ARM_READY_ACK		    0x01
#define	CM_ACC_OFF				    0x02	//¹ØACC
#define	CM_SEND_KEY			    0x03
#define	CM_SEND_VEHICLE_INFO      0x04
#define	CM_SET_MODE			     0x05
#define	CM_GET_MODE			     0x06
#define	CM_SET_WAKE			     0x08
#define	CM_SET_CVBS_DISP		     0x84
#define   CM_ARM_TO_MCU_UPGRADE                0x85
#define	CM_MCU_TO_ARM_UPGRADE	    		0x09

#define	CM_MCU_APP_VERSION				0x86
#define	CM_MCU_APP_VERSION_ACK			0x0A

#define	CM_MCU_BOOT_VERSION				0x87	
#define	CM_MCU_BOOT_VERSION_ACK		0x0B	


#define	CM_RESET_SYSTEM				       0x88


#define	CM_ARM_APP_VERSION				0x0C
#define	CM_ARM_APP_VERSION_ACK			0x89	

#define	CM_ARM_ARI_VERSION				0x0D	
#define	CM_ARM_ARI_VERSION_ACK			0x8A	
#define	CM_HEARTBEAT				    0x8B
#define	CM_SET_DISP_TYPE				       0x0E
#define	CM_SET_DISP_TYPE_ACK				0x8C

#define CM_VIDEO_CALIBRATION        0x0F

#define CM_GET_AVM_INFO             0x10
#define	CM_GET_AVM_INFO_ACK				0x8E

#define	CM_UNVTRANS_TO_ARM				0x16
#define CM_UPDATE_ARM_READY           0x30

#define UPGRADE_APP          0x01
#define UPGRADE_OTHER          0x02

#define SUCCEED_ACK          0x01
#define FAILED_ACK           0x00
#define MCU_SD_PATH              "/run/media/mmcblk0p1/UPDATE/mcu_update.bin"
#define TSH_VER_PATH              "/home/tsh/1_tianshuang/version.txt"
#define T7A_APP_INI_PATH      "/home/chinagps/config.ini"
#define UPDATE_TIME_PATH      "/home/chinagps/update_time.txt"
#define VALID_FLAG_PATH              "/home/chinagps/app_valid_flag.ini"
extern int stopped;
extern  const char* app_ver;
extern void tty1_com_thread(void);

extern int WriteCom(char* buf,int n);
extern unsigned char ReadOneChar(void);
extern int ReadCom(char* buf);
extern void flush_buf(int type);
extern void SendGeneralAck(uchar cmd,uint Hid,uchar isSucc);
extern void SendModeAck(uchar cmd,uint Hid,uchar mode);
extern int SendPkgStruct(uchar cmd,uint Hid,char IsAck, uchar* msg,uchar msgLen);
extern void Set3DMode(uchar is);
extern void SendHeartBeatPkg(void);
#endif
