/*
 * �ڶ��棺2015.5
 * ��ʹ�ýṹ�崢��PWMͨ����Ϣ�������ǵ���������
 ******
 * �����棺2015.7.2
 * ��ά��ԭ��
 */

//����PWM��һЩ���ݽṹ
#ifndef _PWM_PRIVATE_H_
#define _PWM_PRIVATE_H_

#include "stm32f10x.h"

/* ��¼ÿһ��ͨ����PWM��Ϣ */
//��ʵ��һ������
typedef struct tag_pwm{
	//ע��һ��ͨ��������Ϣ
	u16 HLength_apply;
	u16 LLength_apply;
	u16 FirstPhace_apply;
	u16 NumberOfPulse_apply;
	
	//ͨ������ʱ���õ���Ϣ
	u16 HLength;
	u16 LLength;
	u32 Phace;
	u16 NumberOfPulse;
	
	//����������һ���ڵ����ţ�
	u16 NextChannel;
} PWMInfo;

#endif
