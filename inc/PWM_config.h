//�û������һЩ������
#ifndef _PWM_CONFIG_H_
#define _PWM_CONFIG_H_

/* ��Ҫ�����GPIO�ڣ�Ҫ����ʱ�� */
#define RCC_APB2Periph_GPIO_Out RCC_APB2Periph_GPIOA 

/* ���ã��ֶ�ֵ */
//��ʱ���ķ�Ƶ����Խ���Խ��
//72M����Ƶ�£�
/* ��Ƶ��Ϊ72������ʱ��ʱ��Ƶ��Ϊ1M���ߵ͵�ƽ���ȵĵ�λ��1us */
#define MAX_PRESCALER 72
/* 10us */
// #define MAX_PRESCALER 720
/* 100us */
// #define MAX_PRESCALER 7200
/* 1ms */
// #define MAX_PRESCALER 72000


//PWM�������Ĵ�С��ԽС��ʵʱ��Խǿ������CPU�ж�ԽƵ��
#define PWM_BUFFER 32

/**
	* ���ע����4���˿ڣ�
	* ��������ԼҪ��6000���������ڣ�������ÿһ�ֵ�16�����ݣ�
	* ���PWMһ����ʱ����ò�Ҫ����15us�������λ����
	* ���ң��������Ƶ��̫�ߣ�CPU��æ�ڼ���PWM���ݶ������������¡�����
	*/

#endif
