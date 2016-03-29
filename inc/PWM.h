//�û��ӿ�
#ifndef _MY_PWM_H_
#define _MY_PWM_H_

#define _PWM_VER_3_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"


/* �ײ��ʼ�� */
void PWMLowLevelInit(GPIO_TypeDef *GPIO_Port);

/* ע��һ��ͨ�� */
void PWMChannelApply(
				u16 Channel,
				u16 HLength,
				u16 LLength, 
				u16 Phace,
				u16 number_of_pulse
		);

/* ��ʼ������� */
void PWMStart(void);

/* ��ѯPWMʣ����� */
int 	PWMCheckRest(int channel);

/* ǿ��ֹͣPWM��� */
void PWMStopAll(void);

/* ֹͣĳһ��ͨ������� */
void PWMStop(int channel);

/*
 * �ڶ��棺2015.5
 * ���Ÿ��û���дGPIO�˿ڵĺ���
 * ��Ҫ�ÿ⺯����GPIO_SetBits��GPIO_ResetBits !!!
 ******
 * �����棺2015.7.2
 * �������ر�ע�⣬�����治��Ҫ��Щ����������
 */
#ifndef _PWM_VER_3_
void PWMSetBits(u16 GPIO_Pin);
void PWMResetBits(u16 GPIO_Pin);
#endif

#endif
