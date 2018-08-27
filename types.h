#ifndef __types_H__
#define __types_H__

typedef enum cKeyType{
	TNot = 0,
	TEnter =1,
    TBack =2,
    TFront =3,
    TRear =4,
    TLeft = 5,
    TRight = 6,   
}KeyType;


typedef enum cCarColour{
	TRed,
    Tblue,
    TGreen,
}CarColour;

typedef struct cRecCarInfo{
    float CarSpeed;
    short CarSteeringWheel;
    unsigned int CarStat;
    unsigned char Light;//0����1 �� 2��
    unsigned char IsR;	// 0 ���� 1����
    unsigned char alarm; //0���� 1˫��
}RecCarInfo;


typedef struct cAvmParaSetInfo{
	unsigned char allSwitch;//0:open  1:close
	CarColour carColour;
	unsigned char carSubLineStat;
	unsigned char AdasStat;
	unsigned char AdasSpeed;
}AvmParaSetInfo;


#endif