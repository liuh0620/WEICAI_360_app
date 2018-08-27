#include "imx6_com.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include "tsh_comm.h"
#include "types.h"
#include "upgrade.h"

 
 const char* app_ver = "T6A-APP-VER-1.1.0";

//ȫ�ֱ���
int stopped = 0;
static int comfd = -1;
static volatile int avmEnableStat = 0;//��ǰavmʹ��״̬0 �ر� 1 ʹ��
static uchar Is3D = 0; //0 2D  1 3D
static volatile RecCarInfo carInfo = {0};
static  AvmParaSetInfo avmParaSetInfo = {0};
static pthread_mutex_t f_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t com_lock = PTHREAD_MUTEX_INITIALIZER;
static int jumpflag = 0;

#define   MAKE_DWORD(a,b,c,d)  ((a<<24)|(b<<16)|(c<<8)|(d))
#define   MAKE_WORD(a,b)       ((a<<8)|(b))
#define  SPEED_IS_OVER             50

/* BEGIN: Added for ���ⵥ��:liuh-0001 by Liuhuan, 2018/6/15 */
#define SPEED_IS_MAX	50			//360��������������ʱ�����ڴ��ٶȹر�AVM
#define SPEED_IS_MID	20			//360������������ʱ�����ڴ��ٶȣ���Ļ��ʾԭ����
#define SPEED_IS_MIN
/* END:   Added for ���ⵥ��:liuh-0001 by Liuhuan, 2018/6/15 */

#define SPEED_IS_OVER_15(sp)     (((sp) > SPEED_IS_OVER)?1:0)
void ViewChange3D(void);
void ViewChange2D(void);
void DelayFun(int sig);
void ProcessAutoHandle(void);

/*---------------------------------------------------------------
* ����ԭ�ͣ�void DelayFun(void)
* �������ܣ���ʱ�ص����� 
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void DelayFun(int sig)
{

	ProcessAutoHandle();
	return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�int InitTimer(void)
* �������ܣ�װ�ض�ʱ�� setitimerֻ��ʵ��һ����ʱ����
* ����˵���������ε���setitimer����ֵ���ᱻ���ǵ�
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/

int InitTimer(void)
{
	struct itimerval tick;  
    memset(&tick, 0, sizeof(tick));  
    // Timeout to run function first time  
    tick.it_value.tv_sec = 3;  // sec  
    tick.it_value.tv_usec = 0; // micro sec.  
    return setitimer(ITIMER_REAL, &tick, NULL);  
}



/*---------------------------------------------------------------
* ����ԭ�ͣ�int setCom(const char *Dev)
* �������ܣ����ô���ͨ������
* ����˵����
  
* ����������豸���� 
* �� �� ֵ��fd
* ���ߣ�    zd
*---------------------------------------------------------------*/

int setCom(const char *Dev)
{
        int fd;
        struct termios termios_new;
        fd = open(Dev, O_RDWR|O_NDELAY|O_NOCTTY);
        if (-1 == fd)
        {
            printf("open com dev filed!\n");
            return -1;
        }
        bzero(&termios_new, sizeof(termios_new));
        cfmakeraw(&termios_new);
        termios_new.c_cflag = (B576000);
        //termios_new.c_cflag = (B115200);
        termios_new.c_cflag |= CLOCAL | CREAD;
        termios_new.c_cflag &= ~CSIZE;
        termios_new.c_cflag |= CS8;
        termios_new.c_cflag &= ~PARENB;
        termios_new.c_cflag &= ~CSTOPB;
        termios_new.c_cc[VTIME] = 1;
        termios_new.c_cc[VMIN] = 1;
        tcflush(fd, TCIOFLUSH);
        tcsetattr(fd, TCSANOW, &termios_new);
        return fd;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void Set3DMode(uchar is)
* �������ܣ�����3Dģʽ
* ����˵����
  
* ���������
* �� �� ֵ����
* ���ߣ�    zd
*---------------------------------------------------------------*/
void Set3DMode(uchar is)
{
	int Parafd = -1;
	Is3D = is;
	Parafd = open(T7A_APP_INI_PATH,O_RDWR);
    if(Parafd < 0)
    {
        close(Parafd);
        printf("open config file err!\n");
    } else{
		lseek(Parafd,0,SEEK_SET);
	  	write(Parafd,&Is3D,1);
    	close(Parafd);
 	}
	return;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�int WriteCom(char* buf,int n)
* �������ܣ�����д����
* ����˵����
  
* ���������
* �� �� ֵ����
* ���ߣ�    zd
*---------------------------------------------------------------*/
int WriteCom(char* buf,int n)
{
    int nleft;
    int nwrite;
    char  *ptr;
    ptr = buf;
    nleft = n;
    pthread_mutex_lock(&f_lock);
    while (nleft > 0) {
        if ( (nwrite = write(comfd, ptr, nleft)) <= 0) {
	     pthread_mutex_unlock(&f_lock);
            perror("write: ");
            return (-1);			/* error */
        }

        nleft -= nwrite;
        ptr   += nwrite;
    }
    pthread_mutex_unlock(&f_lock);
    return (n);
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�int ReadCom(char* buf)
* �������ܣ����ڶ�����
* ����˵����
  
* ���������
* �� �� ֵ�����Ĵ�С
* ���ߣ�    zd
*---------------------------------------------------------------*/
int ReadCom(char* buf)
{
    int size = 0;
	pthread_mutex_lock(&f_lock);
    while(read(comfd,buf,1) > 0)
    {
        buf++;
        size++;
        if(size == MAX_REC_SIZE)
        {
            break;
        }
    }
	pthread_mutex_unlock(&f_lock);
    return size;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�unsigned char ReadOneChar(void)
* �������ܣ���ȡһ���ֽ�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
unsigned char ReadOneChar(void)
{
	unsigned char c = '\0';
	//����
	pthread_mutex_lock(&f_lock);
	while(1){
		if(read(comfd, (void*)&c,1) == 1 ){
			break;
		}
	}
	pthread_mutex_unlock(&f_lock);
	return c;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�void flush_buf(int type)
* �������ܣ���ϴ����
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void flush_buf(int type)
{
	tcflush(comfd, type);	
}

#define PRINTFBUF(size,buf)   do{ \
    printf("[REC_BUF]"); \
    int s=size; \
    char* b = buf; \
    while(s--){ \
        printf("%x ",*b++); \
    } \
    printf("\n");\
    }while(0)



/*---------------------------------------------------------------
* ����ԭ�ͣ�uchar* arrchr(uchar* pBuf,uchar ch,int size)
* �������ܣ������ض��ַ�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
uchar* arrchr(uchar* pBuf,uchar ch,int size)
{

    if(pBuf == NULL)
    {
        return NULL;
    }
    else if(*pBuf == ch)
    {
        return pBuf;
    }
    size--;
    while(size--)
    {
        if(*(++pBuf) == ch)
        {

            return pBuf;
        }
    }
    return NULL;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ParseGeneralAckCmd(uchar* msg)
* �������ܣ�������Ӧ�����Ƿ�ɹ�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ParseGeneralAckCmd(uchar* msg)
{
    uchar cmdId = msg[0];
    uchar result = msg[1];
    if(result == FAILED_ACK)
    {
        printf("failed ID:%d\n",cmdId);
    }
    return;

}


/*---------------------------------------------------------------
* ����ԭ�ͣ�void ParseReadyAckCmd(uchar* msg)
* �������ܣ�����ready����
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ParseReadyAckCmd(uchar* msg)
{
    struct tm time_tm;
    struct timeval time_tv;
    time_t timep;
    time_tm.tm_year = (msg[0]>>4)*10 + (msg[0] & 0x0F) + 2000 - 1900;
    time_tm.tm_mon = (msg[1]>>4)*10 + (msg[1] & 0x0F) -1;
    time_tm.tm_mday = (msg[2]>>4)*10 + (msg[2] & 0x0F);
    time_tm.tm_hour = (msg[3]>>4)*10 + (msg[3] & 0x0F);
    time_tm.tm_min = (msg[4]>>4)*10 + (msg[4] & 0x0F);
    time_tm.tm_sec = (msg[5]>>4)*10 + (msg[5] & 0x0F);
    time_tm.tm_wday = 0;
    time_tm.tm_yday = 0;
    time_tm.tm_isdst = 0;
    timep = mktime(&time_tm);
    time_tv.tv_sec = timep;
    time_tv.tv_usec = 0;
    if(settimeofday(&time_tv, NULL) != 0)
    {
       printf("settimeofday failed!\n");
    }
    return;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�void ParseVehicleInfo(uchar* msg)
* �������ܣ�����������Ϣ
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ParseVehicleInfo(uchar* msg){
	if(carInfo.CarSpeed != (((msg[1]<<4)&0x0FF0) |((msg[0]>>4)&0x0F))){
		if((msg[0] & 0x03) == 1){//m/s
			carInfo.CarSpeed = (((msg[1]<<4)&0x0FF0) |((msg[0]>>4)&0x0F))*36;
		}else{
			carInfo.CarSpeed = (((msg[1]<<4)&0x0FF0) |((msg[0]>>4)&0x0F));
		}
		TshComm_SetVehicleSpeed(carInfo.CarSpeed);
	}
	if(carInfo.CarSteeringWheel !=MAKE_WORD(msg[3],msg[2])){
		carInfo.CarSteeringWheel =MAKE_WORD(msg[3],msg[2]);
		
		TshComm_SetVehicleWheelAngle((float)carInfo.CarSteeringWheel);
	}

	if(carInfo.CarStat !=MAKE_DWORD(msg[7],msg[6],msg[5],msg[4])){
		carInfo.CarStat = MAKE_DWORD(msg[7],msg[6],msg[5],msg[4]);
		carInfo.Light = (carInfo.CarStat & (0x0003<<4))>>4;
		if(carInfo.Light == 3){ //˫��
			carInfo.Light = 0;
			carInfo.alarm = 1;
		}
		carInfo.IsR= (carInfo.CarStat & (0x0007<<8))>>8;
		if(carInfo.IsR != 1){
			carInfo.IsR = 0;
		}
	}
	printf("light=%d,R=%d,speed=%f,whell=%f\n",carInfo.Light,carInfo.IsR,carInfo.CarSpeed,(float)carInfo.CarSteeringWheel );
}


//return NULL if not found  
/*---------------------------------------------------------------
* ����ԭ�ͣ�char* memstr(char* full_data, int full_data_len, const char* substr) 
* �������ܣ������ַ���
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
char* memstr(char* full_data, int full_data_len, const char* substr)  
{  
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) {  
        return NULL;  
    }  
  
    if (*substr == '\0') {  
        return NULL;  
    }  
  
    int sublen = strlen(substr);  
  
    int i;  
    char* cur = full_data;  
    int last_possible = full_data_len - sublen + 1;  
    for (i = 0; i < last_possible; i++) {  
        if (*cur == *substr) {  
            //assert(full_data_len - i >= sublen);  
            if (memcmp(cur, substr, sublen) == 0) {  
                //found  
                return cur;  
            }  
        }  
        cur++;  
    }  
  
    return NULL;  
}  

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ParseVersionInfo(uchar* msg)
* �������ܣ������汾��Ϣ
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ParseVersionInfo(uchar* msg){
	FILE *fp;
	uchar *pbuffer = NULL;
	int nFileLen = 0;
	char* ptr = NULL;
	fp = fopen(MCU_SD_PATH, "rb"); 
	if(fp != NULL){
		fseek(fp,0,SEEK_END); 
		nFileLen = ftell(fp);
		fseek(fp,0,SEEK_SET);
		pbuffer = (uchar *)malloc(nFileLen);
		if(pbuffer == NULL){
			printf("Not Enough Memory!/n");
			return;
		}
		if(fread(pbuffer, 1, nFileLen, fp) == nFileLen){
			fclose(fp);
			ptr = memstr((char*)pbuffer, nFileLen, "T7A-MCU-VER-");//������T7A��ʾ��MCU������ַ���Ӧ
			printf("sd card mcu ver is:%s\n",ptr);
			if(ptr != NULL){
				if((msg[3] == (ptr[12]-48)) &&(msg[2] == (ptr[14]-48))&&(msg[1] == (ptr[16]-48))&&(msg[0] == (ptr[18]-48))){
					printf("version the same!\n");
				}
				else{
					uchar SendMsgBuf[2];
					SendMsgBuf[0] = 1;//app
					SendMsgBuf[1] = 0;
					printf("send 0x85 cmd!\n");
					SendPkgStruct(CM_ARM_TO_MCU_UPGRADE,0,0,SendMsgBuf,2);
				}
			}else{
				printf("not found ver info!\n");
			}	
		}
		else{
			printf("fread count err!\n");
			fclose(fp);
		}
		free(pbuffer);
	}
	return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void SetCvbsDispCmd(uchar isOpen)
* �������ܣ�����CVBS��ʾ
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void SetCvbsDispCmd(uchar isOpen)//isOpen 1��   2�ر�
{
    uchar SendMsgBuf[MAX_SEND_SIZE];
    SendMsgBuf[0] = isOpen ;
    SendPkgStruct(CM_SET_CVBS_DISP,0,0,SendMsgBuf,1);
	if(isOpen ==1){
		//system("echo 0 > /sys/class/graphics/fb0/blank");
	}
	else if(isOpen == 2){
		//system("echo 1 > /sys/class/graphics/fb0/blank");
	}
    return;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�int SendPkgStruct(uchar cmd,uint Hid,char IsAck, uchar* msg,uchar msgLen)
* �������ܣ��������Ͱ�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
int SendPkgStruct(uchar cmd,uint Hid,char IsAck, uchar* msg,uchar msgLen)
{
    static unsigned int SendCount = 1;
    int i,ret;
    uchar crc;
    uchar SendBuf[MAX_SEND_SIZE];
    SendBuf[0] = MSG_HEAD;
    SendBuf[MSG_CMD_OFFSET] = cmd;
    if(IsAck)
    {
        SendBuf[MSG_HID_OFFSET] = (Hid>>24)&0xFF;
        SendBuf[MSG_HID_OFFSET+1] = (Hid>>16)&0xFF;
        SendBuf[MSG_HID_OFFSET+2] = (Hid>>8)&0xFF;
        SendBuf[MSG_HID_OFFSET+3] = Hid&0xFF;
    }
    else
    {
        SendBuf[MSG_HID_OFFSET] = (SendCount>>24)&0xFF;
        SendBuf[MSG_HID_OFFSET+1] = (SendCount>>16)&0xFF;
        SendBuf[MSG_HID_OFFSET+2] = (SendCount>>8)&0xFF;
        SendBuf[MSG_HID_OFFSET+3] = SendCount&0xFF;
    }
    SendBuf[MSG_LEN_OFFSET] = msgLen;
    if(msgLen > 0)
    {
        memcpy(&SendBuf[MSG_DATA_OFFSET],msg,msgLen);
    }
    crc = 0;
    for(i = 1;i<(msgLen + MSG_HEAD_SIZE);i++)
    {
        crc ^= SendBuf[i];
    }
    SendBuf[msgLen+MSG_HEAD_SIZE] = crc;
    ret = WriteCom((char*)SendBuf,MSG_HEAD_SIZE+msgLen+1);
    if(ret < 0)
    {
        return -1;
    }
    if(!IsAck)
    {
        SendCount++;
    }
    return 0;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�void SendGeneralAck(uchar cmd,uint Hid,uchar isSucc)
* �������ܣ�����ͨ��Ӧ��
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void SendGeneralAck(uchar cmd,uint Hid,uchar isSucc)
{
    uchar SendMsgBuf[MAX_SEND_SIZE];
    SendMsgBuf[0] = cmd;
    SendMsgBuf[1] = isSucc;
    SendPkgStruct(CM_GENERAL_ACK,Hid,1,SendMsgBuf,2);
    return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void SendModeAck(uchar cmd,uint Hid,uchar isSucc)
* �������ܣ���Ӧ��ǰ��ʾģʽ��2D/3D��
* ����˵����

* ���������
* �� �� ֵ��
* ���ߣ� liuh
*---------------------------------------------------------------*/
void SendModeAck(uchar cmd,uint Hid,uchar mode)
{
    uchar SendMsgBuf[MAX_SEND_SIZE];
    SendMsgBuf[0] = mode+1;
    SendPkgStruct(CM_GET_MODE_ACK,Hid,1,SendMsgBuf,1);
    return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ParseSendKey(uchar* pMsgData,uint Hid)
* �������ܣ�������������
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ParseSendKey(uchar* pMsgData,uint Hid)
{
	
	KeyType  keytype = TNot;
	/* �� �� ��: liuh-0002     �޸���:Liuh,   ʱ��:2018/7/17 
	   �޸�ԭ��: ��ת��Ƶ�ʱ�����ΰ������� */
	if((carInfo.CarSpeed > SPEED_IS_OVER) || (carInfo.Light != 0))
	{
		return;
	}
	if((pMsgData[0] & (0x01<<0)) != 0)
	{
		keytype = TEnter;
	}
	else if((pMsgData[0] & (0x01<<1)) != 0)
	{
		keytype = TBack;
	}
	else if((pMsgData[0] & (0x01<<2)) != 0)
	{
		keytype = TFront;
	}
	else if((pMsgData[0] & (0x01<<3)) != 0)
	{
		keytype = TRear;
	}
	else if((pMsgData[0] & (0x01<<4)) != 0)
	{
		keytype = TLeft;
	}
	else if((pMsgData[0] & (0x01<<5)) != 0)
	{
		keytype = TRight;
	}
	else
	{
		keytype = TNot;
	}
	printf("keytype=%d\n",keytype);
	switch(keytype)
	{
		case TEnter: 
/* �� �� ��: liuh-0001     �޸���:Liuhuan,   ʱ��:2018/6/15 
   �޸�ԭ��: ����Ϋ����������360��������ʱ��һЩ���� */
   /* BEGIN: Added for ���ⵥ��:liuh-0001 by Liuhuan, 2018/6/15 */
			if((carInfo.IsR == 1) || (carInfo.Light != 0)){
				if( carInfo.IsR == 1 )		//���뵹��
				{
					if( carInfo.CarSpeed <= SPEED_IS_MID )
					{
						if(Is3D)
						{
							SG_SetViewingMode(EN_TSH_VIEW_3D_BACK_AND_SPLICING,0);//Ĭ��ǰ��ͼ
						}
						else
						{
							SG_SetViewingMode(EN_TSH_VIEW_2D_BACK_AND_SPLICING,0);//Ĭ��ǰ��ͼ
						}
						
					}
					else
					{
						SetCvbsDispCmd(2);
						avmEnableStat = 0;
						return ;
					}

				}
				else		//��������ת��
				{
					if( 1 == carInfo.Light )		//����ת���
					{
						if(Is3D)
						{
							SG_SetViewingMode(EN_TSH_VIEW_3D_LF_AND_SPLICING,0);
						}
						else
						{
							SG_SetViewingMode(EN_TSH_VIEW_2D_LEFT_AND_SPLICING,0);
						}
					}
					else if( 2 == carInfo.Light )		//����ת���
					{
						if(Is3D)
						{
							SG_SetViewingMode(EN_TSH_VIEW_3D_RF_AND_SPLICING,0);
						}else{
							SG_SetViewingMode(EN_TSH_VIEW_2D_RIGHT_AND_SPLICING,0);
						}
					}
				}
			}
			else{
				if(carInfo.CarSpeed <= SPEED_IS_MID)		//����<= 20km/h
				{
					if(Is3D)
					{
						SG_SetViewingMode(EN_TSH_VIEW_3D_FRONT_AND_SPLICING,0);//Ĭ��ǰ��ͼ
					}
					else
					{
						SG_SetViewingMode(EN_TSH_VIEW_2D_FRONT_AND_SPLICING,0);//Ĭ��ǰ��ͼ
					}
				}
				else		//���� > 20km/h, �ر�AVM
				{
					SetCvbsDispCmd(2);
					avmEnableStat = 0;
					return ;
				}
				
			}
	/* END:   Added for ���ⵥ��:liuh-0001 by Liuhuan, 2018/6/15 */
			SetCvbsDispCmd(1);
			avmEnableStat = 1;
		break;
		
		case TBack: //�ֶ��˳�AVM �������Զ�����AVM���ֶ��˳���Ч
			if((carInfo.IsR == 1) || (carInfo.Light != 0)){
				return;
			}
			SetCvbsDispCmd(2);
			avmEnableStat = 0;
		break;
		
		case TFront: 
			if(Is3D){
				SG_SetViewingMode(EN_TSH_VIEW_3D_FRONT_AND_SPLICING,0);
			}else{
				SG_SetViewingMode(EN_TSH_VIEW_2D_FRONT_AND_SPLICING,0);
			}
		break;
		case TRear: 
			if(Is3D){
				SG_SetViewingMode(EN_TSH_VIEW_3D_BACK_AND_SPLICING,0);
			}else{
				SG_SetViewingMode(EN_TSH_VIEW_2D_BACK_AND_SPLICING,0);
			}
		break;
		case TLeft: 
			if(Is3D){
				SG_SetViewingMode(EN_TSH_VIEW_3D_LF_AND_SPLICING,0);
			}else{
				SG_SetViewingMode(EN_TSH_VIEW_2D_LEFT_AND_SPLICING,0);
			}
		break;
		case TRight: 
			if(Is3D){
				SG_SetViewingMode(EN_TSH_VIEW_3D_RF_AND_SPLICING,0);
			}else{
				SG_SetViewingMode(EN_TSH_VIEW_2D_RIGHT_AND_SPLICING,0);
			}
		break;
		default:break;
	}
				
	return;
}

/*--------------------------------------------------------------------------
 *  Function:
 *  MainWindow.ParseProtocol
 *
 *  Parameters:
 *
 *
 *  Returns value:
 *
 *
 *  Description:
 * 		��������Э��
 *
 *--------------------------------------------------------------------------*/
void ParseProtocol(uchar* msgData,uchar cmd)
{
    uint Hid = (msgData[0]<<24)|(msgData[1]<<16)|(msgData[2]<<8)| msgData[3];
    uchar* pMsgData = &msgData[5];
	FILE *fp;
	int Parafd = -1;
	uchar SendMsgBuf[15];
	int app_flag = MAKE_DWORD('F','A','I','L');//ʧЧ
    switch(cmd)
    {
        case CM_GENERAL_ACK:
            ParseGeneralAckCmd(pMsgData);
            break;

        case CM_ARM_READY_ACK:
            ParseReadyAckCmd(pMsgData);
            break;
        case CM_ACC_OFF:
			SetCvbsDispCmd(2);
			stopped = 1;	
            break;     
	   case CM_SEND_KEY:
	 	    SendGeneralAck(cmd,Hid,SUCCEED_ACK);//������ͨ��Ӧ��
	 	    ParseSendKey(pMsgData,Hid);
	        break;
       case CM_SEND_VEHICLE_INFO:
			ParseVehicleInfo(pMsgData);
	        break;
#if 0
	   case	CM_UNVTRANS_TO_ARM:
	   		if(pMsgData[0] != 0x01){
				break;
			}
			stopped = 1; //jump to boot
			jumpflag = UPGRADE_APP;
			if(((pMsgData[2] >> 1) & 0x07) == 0){//����APP ������־λ
				Parafd = open(VALID_FLAG_PATH,O_RDWR);
			    if(Parafd < 0)
			    {
			        printf("open app flag err!\n");
			    }else{
				  	lseek(Parafd,0,SEEK_SET);
				    write(Parafd,&app_flag,4);	
					close(Parafd);
			    }
			}
			else{
				jumpflag = UPGRADE_OTHER;
			}
		break;
#endif
	   case	CM_UPDATE_ARM_READY:
            printf("[liuh>>>]Rcv cmd: CM_UPDATE_ARM_READY\n");
			stopped = 1; //jump to boot
			jumpflag = UPGRADE_APP;
			Parafd = open(VALID_FLAG_PATH,O_RDWR);
			if(Parafd < 0)
			{
			    printf("open app flag err!\n");
			}else{
				lseek(Parafd,0,SEEK_SET);
				write(Parafd,&app_flag,4);	
				close(Parafd);
			}
		break;
	   case CM_SET_MODE:
		   	SendGeneralAck(cmd,Hid,SUCCEED_ACK);
		 	if(pMsgData[0] == 1){
				Is3D = 0;
				ViewChange2D();
			}else if(pMsgData[0] == 2){
				Is3D = 1;
				ViewChange3D();
			}
		
		/*
		printf("is3d=%d\n",Is3D);
		 Parafd = open(T7A_APP_INI_PATH,O_RDWR);
	        if(Parafd < 0)
	        {
	            close(Parafd);
	            printf("open config file err!\n");
	            SendGeneralAck(cmd,Hid,FAILED_ACK);
	        } else{
			lseek(Parafd,0,SEEK_SET);
        	  	write(Parafd,&Is3D,1);
	        	close(Parafd);
			SendGeneralAck(cmd,Hid,SUCCEED_ACK);
		 }
		 */
	     break;
		case CM_GET_MODE:
            printf("[liuh>>>]Rcv cmd: CM_GET_MODE\n");
		   	SendModeAck(cmd,Hid,Is3D);
	     break;
        case CM_SET_WAKE:
		SendGeneralAck(cmd,Hid,SUCCEED_ACK);
	     break;
	  case CM_MCU_TO_ARM_UPGRADE:
		if(pMsgData[0] == 1){
			if(access(MCU_SD_PATH,F_OK) >= 0){
				pthread_mutex_lock(&com_lock);
				UpgradeHander();
				pthread_mutex_unlock(&com_lock);
			}else{
			printf("not found sd card!\n");
			}	
		}
		break;
	  case  CM_MCU_APP_VERSION_ACK:
		ParseVersionInfo(pMsgData);
		break;

	case CM_ARM_APP_VERSION:
		SendMsgBuf[0] = 0;
		SendMsgBuf[1] = app_ver[12]  -48;
		SendMsgBuf[2] = app_ver[14]  -48;
		SendMsgBuf[3] = app_ver[16]  -48;
		SendPkgStruct(CM_ARM_APP_VERSION_ACK,Hid,1,SendMsgBuf,4);
		break;
		
	case CM_ARM_ARI_VERSION:
		fp = fopen(TSH_VER_PATH, "rb"); 
		if(fp == NULL){
			printf("open version.txt failed!\n");
			break;
		}
		fseek(fp,1,SEEK_SET);
		fread(&SendMsgBuf[1] , 1, 1, fp);
		fseek(fp,3,SEEK_SET);
		fread(&SendMsgBuf[2] , 1, 1, fp);
		fseek(fp,4,SEEK_SET);
		fread(&SendMsgBuf[3] , 1, 1, fp);
		fclose(fp);
		SendMsgBuf[0] = 0;
		SendMsgBuf[1] -= 48;
		SendMsgBuf[2] -= 48;
		SendMsgBuf[3] -= 48;
		SendPkgStruct(CM_ARM_ARI_VERSION_ACK,Hid,1,SendMsgBuf,4);
		break;
	case CM_SET_DISP_TYPE:  //�û�ͨ���˻��������ⰴ��������ͼ����
		SG_SetViewingMode(pMsgData[0],0);
		usleep(100000);//�ȴ��л��ɹ�
		SendMsgBuf[0] = GetCurrViewMode();
		if(pMsgData[0] == SendMsgBuf[0])
		{
			printf("Set view success!!\n");
		}
		SendPkgStruct(CM_SET_DISP_TYPE_ACK,Hid,1,SendMsgBuf,1);
#if 0
		if((carInfo.IsR == 1) || (carInfo.Light != 0))
		{
			if(InitTimer())
			{
				printf("Set timer failed!!\n");
			}
		}
#endif
		break;
	case CM_VIDEO_CALIBRATION:
		TshComm_SetCarInfomation(EN_TSH_CARINFO_LENGTH, 4880);
		TshComm_SetCarInfomation(EN_TSH_CARINFO_WIDTH, 2120);
		TshComm_SetCarInfomation(EN_TSH_CARINFO_DIST, 800);
		if(pMsgData[0] == 0){
			TshComm_SetVideoMode(EN_TSH_VIDEO_OPEN);
		}
		else
		{
			TshComm_SetVideoMode(EN_TSH_VIDEO_CLOSE);
		}
		TshComm_SetCalibration(EN_TSH_CALIBRATION_CHESSBOARD,EN_TSH_CALI_VISION_BIG);
		break;

	case CM_GET_AVM_INFO:
	    //��ȡ�ܿ���״̬
		SendMsgBuf[0] = 0;
		//��ȡ��ǰ������ɫ
		SendMsgBuf[1] = 0;
		//��ȡ�����߿���״̬
		SendMsgBuf[2] = 0;
		//��ȡADAS����״̬
		SendMsgBuf[3] = 0;
		//��ȡADAS�����ٶ�
		SendMsgBuf[4] = 0;
		//��ȡARM�������¸���ʱ��
		fp = fopen(UPDATE_TIME_PATH, "rb"); 
		if(fp == NULL){
			printf("open update_time.txt failed!\n");
			break;
		}
		fseek(fp,2,SEEK_SET);
		fread(&SendMsgBuf[5] , 1, 1, fp);
		fseek(fp,3,SEEK_SET);
		fread(&SendMsgBuf[6] , 1, 1, fp);
		SendMsgBuf[5] = (SendMsgBuf[5] - 48)*10 + (SendMsgBuf[6] - 48);  //��
		
		fseek(fp,5,SEEK_SET);
		fread(&SendMsgBuf[6] , 1, 1, fp);
		fseek(fp,6,SEEK_SET);
		fread(&SendMsgBuf[7] , 1, 1, fp);
		SendMsgBuf[6] = (SendMsgBuf[6] - 48)*10 + (SendMsgBuf[7] - 48);  //��
		

		fseek(fp,8,SEEK_SET);
		fread(&SendMsgBuf[7] , 1, 1, fp);
		fseek(fp,9,SEEK_SET);
		fread(&SendMsgBuf[8] , 1, 1, fp);
		SendMsgBuf[7] = (SendMsgBuf[7] - 48)*10 + (SendMsgBuf[8] - 48);  //��

		fseek(fp,11,SEEK_SET);
		fread(&SendMsgBuf[8] , 1, 1, fp);
		fseek(fp,12,SEEK_SET);
		fread(&SendMsgBuf[9] , 1, 1, fp);
		SendMsgBuf[8] = (SendMsgBuf[8] - 48)*10 + (SendMsgBuf[9] - 48);  //ʱ
		
		fseek(fp,14,SEEK_SET);
		fread(&SendMsgBuf[9] , 1, 1, fp);
		fseek(fp,15,SEEK_SET);
		fread(&SendMsgBuf[10] , 1, 1, fp);
		SendMsgBuf[9] = (SendMsgBuf[9] - 48)*10 + (SendMsgBuf[10] - 48);  //��
		

		fseek(fp,17,SEEK_SET);
		fread(&SendMsgBuf[11] , 1, 1, fp);
		fseek(fp,18,SEEK_SET);
		fread(&SendMsgBuf[12] , 1, 1, fp);
		SendMsgBuf[10] = (SendMsgBuf[11] - 48)*10 + (SendMsgBuf[12] - 48);  //��
		fclose(fp);
		SendPkgStruct(CM_GET_AVM_INFO_ACK,Hid,1,SendMsgBuf,11);
		break;
		
        default: 
		printf("parse cmd default!\n");
	      break;
    }
    return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ProcessComHandle(char* buf ,int size)
* �������ܣ���������Э��
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/

void ProcessComHandle(char* buf ,int size)
{
    uchar* pbuf;
    uchar* Srcbuf;
    uchar crc;
    int CalCnt, i,DataLen;

    PRINTFBUF(size,buf);

    if(size <= MSG_HEAD_SIZE)
    {
        return;
    }

    Srcbuf =(uchar*)buf;


    while((pbuf= arrchr(Srcbuf,MSG_HEAD,(size- (Srcbuf - (uchar*)buf)))) != NULL) //ճ������
    {

        DataLen = pbuf[MSG_LEN_OFFSET];
        if((pbuf - (uchar*)buf + DataLen + MSG_HEAD_SIZE +1 ) > size )
        {
          break;
        }

       crc = 0;
       CalCnt = DataLen + MSG_HEAD_SIZE - 1;
       i = 1;
       while(CalCnt--)
       {
           crc ^= pbuf[i++];
       }

       if(pbuf[DataLen + MSG_HEAD_SIZE] != crc)
       {
           printf("check ecc err!\n");
           break;
       }
       ParseProtocol(&pbuf[MSG_HID_OFFSET], pbuf[MSG_CMD_OFFSET]);
       Srcbuf = pbuf + DataLen + MSG_HEAD_SIZE +1;
       if(Srcbuf > ((uchar*)&buf[size - 1] - MSG_HEAD_SIZE))
       {
           break;
       }
    }
return;

}
/*---------------------------------------------------------------
* ����ԭ�ͣ�void ViewChange3D(void)
* �������ܣ��Զ�3D�л�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ViewChange3D(void){
	if(carInfo.IsR == 0){
		if(carInfo.Light ==0){
			SG_SetViewingMode(EN_TSH_VIEW_3D_FRONT_AND_SPLICING,0);	
		}
		else if(carInfo.Light ==1){
			SG_SetViewingMode(EN_TSH_VIEW_3D_LF_AND_SPLICING,0);
		}
		else if(carInfo.Light ==2){
			SG_SetViewingMode(EN_TSH_VIEW_3D_RF_AND_SPLICING,0);
		}
	}
	else{
		if(carInfo.Light ==0){
			SG_SetViewingMode(EN_TSH_VIEW_3D_REVERSE_AND_SPLICING,0);	
		}
		else if(carInfo.Light ==1){
			SG_SetViewingMode(EN_TSH_VIEW_3D_LB_AND_SPLICING,0);
		}
		else if(carInfo.Light ==2){
			SG_SetViewingMode(EN_TSH_VIEW_3D_RB_AND_SPLICING,0);
		}
	}

}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ViewChange2D(void){
* �������ܣ��Զ�2D�л�
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ViewChange2D(void){
	if(carInfo.IsR == 0){
		if(carInfo.Light ==0){
			SG_SetViewingMode(EN_TSH_VIEW_2D_FRONT_AND_SPLICING,0);	
		}
		else if(carInfo.Light ==1){
			SG_SetViewingMode(EN_TSH_VIEW_2D_LEFT_AND_SPLICING,0);
		}
		else if(carInfo.Light ==2){
			SG_SetViewingMode(EN_TSH_VIEW_2D_RIGHT_AND_SPLICING,0);
		}
	}
	else{
		SG_SetViewingMode(EN_TSH_VIEW_2D_BACK_AND_SPLICING,0);	
	}

}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void ProcessAutoHandle(void)
* �������ܣ��Զ��л��߼�����
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void ProcessAutoHandle(void){

	if(Is3D)
	{
		ViewChange3D();
	}
	else
	{
		ViewChange2D();
	}
	return;
}

/*---------------------------------------------------------------
* ����ԭ�ͣ�void InitSavePara(void)
* �������ܣ�������ʼ��
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
void InitSavePara(void)
{
    int Parafd;
    if(access(T7A_APP_INI_PATH,F_OK) >= 0){
		 Parafd = open(T7A_APP_INI_PATH,O_RDWR);
	        if(Parafd < 0)
	        {
	            printf("open para err!\n");
	            return;
	        }
		  lseek(Parafd,0,SEEK_SET);
        	  read(Parafd,&Is3D,1);	
	}else{
		 Parafd = open(T7A_APP_INI_PATH,O_CREAT|O_RDWR,777);
	        if(Parafd < 0)
	        {
	            printf("open para err!\n");
	            return;
	        }
	        lseek(Parafd,0,SEEK_SET);
               write(Parafd,&Is3D,1);

	}
#if 0
	int fd;
	uchar vel = 0,temp;
	char cmd[100] = {0};	
	if(access("/home/chinagps/logidx.ini",F_OK) >= 0){
		 fd = open("/home/chinagps/logidx.ini",O_RDWR);
	        if(fd < 0)
	        {
	            printf("open para err!\n");
	            return;
	        }
		  lseek(fd,0,SEEK_SET);
    	  read(fd,&vel,1);
		  temp = (vel + 1);
		  if(temp > 255){
			temp = 0;
		  }
		  lseek(fd,0,SEEK_SET);
          write(fd,&temp,1);
	}else{
		 fd = open("/home/chinagps/logidx.ini",O_CREAT|O_RDWR,777);
	        if(fd < 0)
	        {
	            printf("open para err!\n");
	            return;
	        }
	        lseek(fd,0,SEEK_SET);
               write(fd,&vel,1);

	}
	sprintf(cmd,"cat /dev/kmsg > /home/chinagps/%d.log &",vel);
	system(cmd);
#endif
    return;
}
/*---------------------------------------------------------------
* ����ԭ�ͣ�int  AvmStatChange(void)
* �������ܣ�״̬�ı� ������Զ��л� 
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
int AvmStatChange(void)
{
     
	static RecCarInfo lastInfo={0};
	int ret = 0;
    if((lastInfo.IsR != carInfo.IsR) || (lastInfo.Light != carInfo.Light))
    {
		ret = 1;
	}
	lastInfo.IsR = carInfo.IsR;
	lastInfo.Light = carInfo.Light;
	return ret;
}


/*---------------------------------------------------------------
* ����ԭ�ͣ�void InitSavePara(void)
* �������ܣ�tty1�߳�ѭ������ 
* ����˵����
  
* ���������
* �� �� ֵ��
* ���ߣ�    zd
*---------------------------------------------------------------*/
 void tty1_com_thread(void)
{

	int size;
	char buf[MAX_REC_SIZE];
	comfd = setCom("/dev/ttymxc1");
	if(comfd < 0){
	        return;
	}
	SendPkgStruct(CM_ARM_READY,0,0, NULL,0);
	if(access(MCU_SD_PATH,F_OK) >= 0){
		SendPkgStruct(CM_MCU_APP_VERSION,0,0, NULL,0);
	}
	//InitSavePara();
	SetCvbsDispCmd(2);//avmEnableStat=0  AVM��ʼ״̬�ر�
	//signal(SIGALRM, DelayFun);//������ʱ����  
	while(!stopped){
		size =ReadCom(buf);
		if(size > 0){
			ProcessComHandle(buf,size);
		}
		if(AvmStatChange()) 
		{
			ProcessAutoHandle();
		}

		if((carInfo.CarSpeed > SPEED_IS_OVER))
		{
				if(avmEnableStat && (carInfo.IsR == 0))//���ǵ���ʱ ������������ٶ� ��ر���Ƶ ����ʱ�ٶ�������
				{
					SetCvbsDispCmd(2);
					avmEnableStat = 0;
				}
		}
		else
		{
				if((carInfo.IsR == 1) ||(carInfo.Light != 0))
				{
					if(avmEnableStat == 0)
					{
						SetCvbsDispCmd(1);
						avmEnableStat = 1;
					}
				}
				else
				{
					/* �� �� ��: liuh-0002     �޸���:Liuh,   ʱ��:2018/7/17 
					   �޸�ԭ��: �ٶ�>20km/hʱ��360ϵͳ�ر�        */
					if((carInfo.CarSpeed > SPEED_IS_MID) && (carInfo.Light == 0))
					{
						SetCvbsDispCmd(2);
						avmEnableStat = 0;
					}
				}
		}
				
				
		usleep(10000);
	}
	close(comfd);
	comfd= -1;
    printf("[liuh>>>]Jump to boot.....\n");
	if(jumpflag == UPGRADE_APP){
		system("/home/chinagps/360_boot &");
	}
	else if(jumpflag == UPGRADE_OTHER){
		system("/home/chinagps/360_boot 1 &");
	}

	return;
}


void SendHeartBeatPkg(void)
{
	while(!stopped)
	{
		if(comfd > 0)
		{
			pthread_mutex_lock(&com_lock);	
			SendPkgStruct(CM_HEARTBEAT,0,0,NULL,0);
			pthread_mutex_unlock(&com_lock);
		}
		usleep(100000);
	}
	return;
}

