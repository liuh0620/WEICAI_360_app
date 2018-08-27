#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tsh_comm.h"
#include "imx6_com.h"
static uint32_t CurrViewMode = 0;

uint32_t GetCurrViewMode(void)
{
	return CurrViewMode;
}


/*---------------------------------------------------------------
* 函数原型：void SG_SetViewingMode(uint32_t viewMode, float angle)
* 函数功能：设置显示模式 
* 参数说明：
  
* 输入参数：
* 返 回 值：
* 作者：    zd
*---------------------------------------------------------------*/	
void SG_SetViewingMode(uint32_t viewMode, float angle)
{
	if(viewMode == CurrViewMode){
		return;
	}
	printf("set view:%d\n",viewMode);
	TshComm_SetViewingMode(viewMode, angle);
	return;
}



/*---------------------------------------------------------------
* 函数原型：void msg_process_cb(void* param, void* data, uint32_t len)
* 函数功能：回调函数
* 参数说明：
  
* 输入参数：
* 返 回 值：
* 作者：    zd
*---------------------------------------------------------------*/	
void msg_process_cb(void* param, void* data, uint32_t len)
{ // 此为子线程
   (void)param;
   stTlvPacket_t decodePkt;
   //static unsigned char mode = 0,mode_last = 0;
   memset(&decodePkt, 0, sizeof(decodePkt));
   TlvPacket_Init(&decodePkt, data, len);
   uint16_t type = (uint16_t)-1;
   if (!TlvPacket_ReadHead(&decodePkt, &type))
   {
       printf("inner msg read head error.\n");
       return;
   }
   uint16_t tag;
   uint16_t length;
   char* data_temp = NULL;
   if (type != EN_TSH_MSG_INFORM)
   {
   		printf("skip=%d!\n",type);
        return;
   }
   while ((data_temp = TlvPacket_ReadBody(&decodePkt, &tag, &length)))
   {
       switch (tag)
       {
       case EN_TSH_FIELD_INFORM_CURRAPP:
       {
           if (length != sizeof(uint32_t))
               return;
           uint32_t nApp = *((uint32_t*)data_temp);
           if (EN_TSH_360APP_TSHMAIN == nApp)
           {
               // to do
           }
           else if (EN_TSH_360APP_TSHCALI == nApp)
           {
               // to do
           }
           else if (EN_TSH_360APP_TSHCONF == nApp)
           {
               // to do
           }
           else if (EN_TSH_360APP_TSHPLAY == nApp)
           {
               // to do
           }
           else
           {
               // do nothing
           }
           
           break;
       }
       case EN_TSH_FIELD_INFORM_CURRVIEWINGANGLE:
       {
           if (length != sizeof(stViewingMode_t))
               return;
           
           stViewingMode_t* vm = (stViewingMode_t*)data_temp;
     	    CurrViewMode = vm->nMode;
			/*
			if((CurrViewMode>=1)&&(CurrViewMode<=4))
			{
				mode = 0;
			}
			else if((CurrViewMode>=5)&&(CurrViewMode<=12))
			{
				mode = 1;
			}
			if(mode != mode_last)
			{
				Set3DMode(mode);
			}
			mode_last = mode;
			
			*/
			printf("tsh back function CurrViewMode=%d\n",CurrViewMode);	
            break;
       }
       case EN_TSH_FIELD_INFORM_RECORDINGSTATE:
       {
           if (length != sizeof(uint32_t))
               return;
           
           
           break;
       }
       case EN_TSH_FIELD_INFORM_VIDEOEXCEPTION:
       {
           if (length != sizeof(uint32_t))
               return;

           
           break;
       }
       case EN_TSH_FIELD_INFORM_CALIRESULT:
       {
           if (length != sizeof(uint32_t))
               return;  

           
           break;
       }
	   default:break;
       }
   }
}

