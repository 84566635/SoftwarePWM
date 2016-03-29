/***********************************************************************
 *
 * �ڶ��棺2015.5
 * ���Ľ������㷨�������������ÿ��Ѱ����С��λֵ��ʱ��
 * ������˫���壬ʹ��������ȶ�
 *
 ******
 *
 * �����棺2015.7.2
 * ��ԭ��PWM�������DMAֱ��д��GPIO��ODR�ϣ�������д��BSRR�Ĵ�����
 * ����BSRR���Ա���ռ�������Ķ˿�
 *
 ***********************************************************************/
 
#include "PWM.h"
#include "PWM_Config.h"
#include "PWM_private.h"

#define IMPROVED_SORT

//һ��PWMͨ���ĸ���
#define PWM_GROUP 16
//����ʱ��
#define buffertime 600
//����ʱ��
#define IDELTIME	0xffff
//�Լ�����Ŀ�ָ�룬Ϊ-1������
#define NULL_p 0xffff

/* ========================================================================================= */
/* ˽�б��� */

//ע���GPIO��
static GPIO_TypeDef *PWM_GPIO;

//��¼ע��Ķ˿�
static u32 ApplyPins = 0;

//��¼����ʹ�õĶ˿�(����16λ��Ϊ0ʱ��������ʹ�ã���16λ��λ�ˣ���־������������)
	/* ��������ʱ��������Ҫ�жϣ�
	��������ʹ����˫���壬���ܻ��а���������Ĳ���û�������
	�������ø�16λ��־���� */
static u32 UsingPins = 0;

//Ҫд��ODR������
static u32 ODRData = 0;	//�����ƽ
static u32 BSRRData = 0;//���������ƽ����

//Ҫд��ARR������
static u32 ARRData = 0;	//��ʱ��ˢ��ʱ��Ķ���

#define PhaceMax (1L <<19)
//��λ���ݻ���������ñ�־��������ͼ�ȥPhaceMax
static u32 overflow_flag = 0;

//����PWM����Ϣ
//����Ҫ����һ������
static PWMInfo PWM_ALL[PWM_GROUP] = { 0 };	
//����ͷ����������±���棩��-1��ʾ�ա�����
static s16	PWM_ALL_head = NULL_p;

//�������������У���DMA�����ݣ����С��PWM_Config.h������

//��ʱ���Զ���װ�Ĵ���(ARR)�Ķ���
static u16 TIM_ARR_Queue[2][PWM_BUFFER] = { 0 };

//IO�ڵ�ƽ�ı�Ĵ���(BSRR, Bit Set/Reset Register)�Ķ��У�֮ǰ�õ���ODR��
static u32 IO_BSRR_Queue[2][PWM_BUFFER]= { 0 };

//����˫���壻�˿�Ҫ����Ķ��е�ָ��
static u16	*TIM_Queue = TIM_ARR_Queue[0]; 
static u32	*BSRR_Queue = IO_BSRR_Queue[0];




/* =========================================================================== */
/* ˽�к��� */
#define CalculateQueues DMA1_Channel5_IRQHandler
//��ʵ���жϺ���
void CalculateQueues(void);




/* =========================================================================== */
/* �ӿں��� */

/*
 * �ײ��ʼ��
 * ��IO��
 * ����ʱ�����ѿ�����
 * �����Ӵ��Ժ�ÿ��һ��ʱ��CPU���ж�
 */
void PWMLowLevelInit(GPIO_TypeDef* GPIO_Port){
	GPIO_InitTypeDef PWMIO;
	
	PWM_GPIO = GPIO_Port;	//��¼�˿�
	//��ʼ��GPIO
	{
		//��ʱֻдA,B,C�����˿�
		switch((u32)GPIO_Port)
		{
			case (u32)GPIOA:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
				break;
			case (u32)GPIOB:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				break;
			case (u32)GPIOC:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
				break;
		}
		
		/* Ҫ�Ľ�������һ���� */
		//��ȡ���˶˿�֮ǰ�Ĺ���
		GPIO_DeInit(GPIO_Port);
		//Ȼ��ͳͳ��Ϊ�������
		PWMIO.GPIO_Pin =		GPIO_Pin_All;
		PWMIO.GPIO_Mode =		GPIO_Mode_Out_PP;
		PWMIO.GPIO_Speed =	GPIO_Speed_50MHz;
		GPIO_Init(GPIO_Port,&PWMIO);
	}
	
	//��ʼ��USART
	//MyUSARTInit();
	
	//��ʼ����������
	{
		//�������ж�����һЩ����ʱ������ݣ���֤��һ��ʱ��CPU���һ���ж�
		int n;
		//˫������У�����Ҫ "<<1"
		for(n = 0; n < (PWM_BUFFER<<1); n++)
		{
			((u16*)TIM_ARR_Queue)[n] = buffertime;
			((u16*)IO_BSRR_Queue)[n] = BSRRData;
		}
	}
	
	//��ʼ��DMA
	{
		DMA_InitTypeDef DMAS;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		/* �õ���DMA1��ͨ��5��7���ֱ���
			TIM2_CC1ƥ�䣬TIM2_CC2ƥ��
			�Ĵ���ͨ����*/
		DMAS.DMA_BufferSize = PWM_BUFFER << 1;	//ʹ��˫����
		DMAS.DMA_DIR = DMA_DIR_PeripheralDST;
		DMAS.DMA_M2M = DMA_M2M_Disable;
		DMAS.DMA_Mode = DMA_Mode_Circular;
		DMAS.DMA_Priority = DMA_Priority_High;

		DMAS.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMAS.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

		//ͨ��5��TIM2_CC1ƥ�䴥��
		//���ڸ���TIM2_ARR�Ĵ���
		DMA_DeInit(DMA1_Channel5);
		DMAS.DMA_MemoryBaseAddr = (u32)TIM_ARR_Queue;
		DMAS.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMAS.DMA_PeripheralBaseAddr = (u32)&TIM2->ARR;
		DMAS.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_Init(DMA1_Channel5, &DMAS);
		DMA_Cmd(DMA1_Channel5, ENABLE);
		//ͨ��5Ҫʹ�ܴ�����ɵ��жϣ�˫���壬�д������ʹ�����ɵ��ж�
		DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_HT, ENABLE);
		
		//ͨ��7��TIM2_CC2ƥ�䴥��
		//���ڸ���IO��
		DMA_DeInit(DMA1_Channel7);
		DMAS.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//32λ
		DMAS.DMA_MemoryBaseAddr = (u32)IO_BSRR_Queue;
		DMAS.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMAS.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//32λ
		DMAS.DMA_PeripheralBaseAddr = (u32)&PWM_GPIO->BSRR;					//�ĳ���BSRR
		DMAS.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_Init(DMA1_Channel7, &DMAS);
		DMA_Cmd(DMA1_Channel7, ENABLE);
	}
	
	//��ʼ��NVIC
	//ʹ��DMAͨ��5���ж�
	{
		NVIC_InitTypeDef DMA_IRQ;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		DMA_IRQ.NVIC_IRQChannel = DMA1_Channel5_IRQn;
		DMA_IRQ.NVIC_IRQChannelPreemptionPriority = 1;
		DMA_IRQ.NVIC_IRQChannelSubPriority = 0;
		DMA_IRQ.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&DMA_IRQ);
	}
	
	//��ʼ����ʱ��2
	{
		TIM_TimeBaseInitTypeDef PWMTIM;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		//��ȡ����ԭ��������
		TIM_DeInit(TIM2);
		//�ټ�������
		PWMTIM.TIM_Prescaler =			MAX_PRESCALER - 1;	//TIM����ʱ�ӷ�Ƶ
		PWMTIM.TIM_CounterMode =		TIM_CounterMode_Up;
		PWMTIM.TIM_Period =					buffertime;	//����Ϊ����ʱ��
		PWMTIM.TIM_RepetitionCounter = 0;
		PWMTIM.TIM_ClockDivision =  0;
		TIM_TimeBaseInit(TIM2, &PWMTIM);
	}
	//ʹ�ܶ�ʱ������DMA
	TIM_SelectCCDMA(TIM2, ENABLE);
	TIM_DMACmd(TIM2, TIM_DMA_CC1 | TIM_DMA_CC2, ENABLE);
	//������ʱ��
//	TIM_Cmd(TIM2, ENABLE);
}

/*
 * ע��һ��PWM�˿�
 * ���룺��ţ��ߡ��͵�ƽ���ȣ����࣬����
 * ������������Ϊ0ʱ�����������
 * `���Ҫ��PWMStart()���ܿ������
 */
void PWMChannelApply(
				u16 Channel,
				u16 HLength,
				u16 LLength, 
				u16 Phace,
				u16 number_of_pulse
		)
{
	//��¼����
	PWM_ALL[Channel].HLength_apply = HLength;
	PWM_ALL[Channel].LLength_apply = LLength;
	PWM_ALL[Channel].FirstPhace_apply = Phace;
	PWM_ALL[Channel].NumberOfPulse_apply = number_of_pulse - 1;
	
	//��¼ע�����
	ApplyPins |= (1L << Channel);
}


/*
 * ��ʼ���PWM
 * ���Ѷ˿ڰ���λֵ�����γ�����
 * ����������е��£������жϺ���ȥ��
 */
void PWMStart()
{
	u32 DMA_CCR_tmp;
	
	if(ApplyPins == 0)
	{
		return;
	}

	//���ε�DMA5�ж�
	DMA_CCR_tmp = DMA1_Channel5->CCR;
	DMA1_Channel5->CCR &= (~(DMA_IT_HT | DMA_IT_TC));
	
	//��ʼ��������������
	/* �������� */
	{
		register int n;
		for(n = 0; n < PWM_GROUP; n++)
		{
			if((ApplyPins >> n)&1)//����˿�ע����
			{
				PWM_ALL[n].HLength = PWM_ALL[n].HLength_apply;
				PWM_ALL[n].LLength = PWM_ALL[n].LLength_apply;
				PWM_ALL[n].NumberOfPulse = PWM_ALL[n].NumberOfPulse_apply;
				//��ʼ����λ���ݣ���Ҫ��ԭ����������ͬһ����ϣ����Լ���ARRData, buffertime
				PWM_ALL[n].Phace = PWM_ALL[n].FirstPhace_apply + ARRData + buffertime;
			}
		}
		//���¶˿�ʹ�����
		UsingPins &= 0x0000ffff;
		UsingPins |= ApplyPins;
		//ȡ��ע����Ϣ
		ApplyPins = 0;
	}
	
	/* ���������������� */
	{
#ifndef IMPROVED_SORT
		int n;
		u32 max_tmp;
		u16 using_tmp = UsingPins;
		u16 biggest_num;
		
		PWM_ALL_here = NULL_p;
		while(using_tmp != 0){
			max_tmp = 0;
			for(n = 0; n < PWM_GROUP; n++)
			{
				if((using_tmp >> n)&1)
				{
					if(PWM_ALL[n].Phace > max_tmp)
					{
						max_tmp = PWM_ALL[n].Phace;
						biggest_num = n;
					}
				}
			}
			using_tmp &= (~(1L << biggest_num));
			PWM_ALL[biggest_num].NextChannel = PWM_ALL_here;
			PWM_ALL_here = biggest_num;
		}
		//�ض�ͷָ�룬��ʱ��ָ����С������
		PWM_ALL_head = PWM_ALL_here;
#else
		s8 order[16];
		register int n, i = 0;

		PWM_ALL_head = NULL_p;
		
		//Ϊ�������򣬽��������һ����������(order)
		for(n = 0; n < PWM_GROUP; n++)
		{
			if((UsingPins >> n) & 1)
			{
				order[i] = n;
				i++;
			}
		}
		
		///*ѡ������*/�������ݣ�����ʲô�����򡢿���֮����ˣ�
		//�Ӵ�С�����õ�������С��һ��
		//˳������������
		for(i--; i >= 0; i--)
		{
			register int max = 0, max_num = 0;
			
			for(n = 0; n <= i; n++)
			{
				register int current_num = PWM_ALL[order[n]].Phace;
				if(current_num > max)
				{
					max = current_num;
					max_num = n;
				}
			}
			PWM_ALL[order[max_num]].NextChannel = PWM_ALL_head;
			PWM_ALL_head = order[max_num];
			order[max_num] = order[i];
		}
#endif
	}
			
	TIM_Cmd(TIM2, ENABLE);
	//�ָ��ж�
	DMA1_Channel5->CCR = DMA_CCR_tmp;

	overflow_flag = ~UsingPins;	//
}


/*
 * ������У���ʵ��DMA�жϳ���
 * ��ͨ�����ϴ���������ȡPWM����С��λֵ������ARR����
 ******
 * ���ڶ��棺���϶�ODRData����򣬼���ODR����
 ******
 * �������棺������һ�ֵĶ˿ڵ�ƽ����λBSRRData�Ĳ�ͬ��λ������BSRR������
						���⣬�������������ʱ���ҹص��˶�ʱ����Ϊ�˼���һ���жϡ�
						���ң��üĴ���������ٶȣ���Ȼ���ж�������ô�ණ����̫�á�����
						���㣺ռ����10���Ĵ������жϳ��򰡡�����
 */
void CalculateQueues()
{
	u32 tmp_arr;
	u32 n;
	
	if((UsingPins & 0xffff) == 0)
	{
		if(UsingPins == 0)
			/*��Ȼ�Ѿ�û������ˣ����ǻ��а��������������δ��������ñ�־*/
			UsingPins = 1L << 16;
		else
		{
			/*�Ѿ���ȫû�������*/
			UsingPins = 0;
			//�ص���ʱ��
			TIM_Cmd(TIM2, DISABLE);
		}
		
		{
			//û������ˣ�Ҫ�����һ�������
			register int n;
			for(n = 0; n < PWM_BUFFER; n++)
			{
				TIM_Queue[n] = buffertime;
				BSRR_Queue[n] = 0;
			}
		}
		
		if(BSRRData != 0)
		{
			BSRR_Queue[0] = BSRRData;
			BSRRData = 0;
			//����һλ���ݡ����������Ļ���������һ�Σ���������ص���ʱ���ˣ���һ�����ݾͲ��������
			//�ر��ǣ��������������ǻ�������С�ı���ʱ�����С�����Ч������������ʣ�µ����ݾͳٳٳ�����
			UsingPins = 0;
		}
	}//if usingpins == 0xXXXX0000
	else
	{
		for(n = 0; n < PWM_BUFFER; n++)
		{
			//����ODR����
			BSRR_Queue[n] = BSRRData;
			
			if(UsingPins == 0){
				BSRRData = 0;
				TIM_Queue[n] = buffertime;
				continue;
			}
			
			//������ͷ�����С����λֵ
			tmp_arr = PWM_ALL[PWM_ALL_head].Phace;
			//���������
			TIM_Queue[n] = tmp_arr - ARRData;
			
			BSRRData = 0;
			//�����鿴�Ƿ�����ͬ����λ
			do
			{
				//��������λ��ֵ��Ȼ�󽫸������ض���

				//��¼��ǰ��ƽ״̬
				ODRData ^= (1L << PWM_ALL_head);
				if((ODRData >> PWM_ALL_head) & 1)
				{
					//�ߵ�ƽ����λBSRR�ĵ�16λ��BSn
					BSRRData |= 1 << PWM_ALL_head;
					
					PWM_ALL[PWM_ALL_head].Phace += PWM_ALL[PWM_ALL_head].HLength;
				}//if ODRData.n == 1
				else
				{
					//�͵�ƽ����λBSRR�ĸ�16λ��BRn
					BSRRData |= 1 << (PWM_ALL_head + 16);
					
					if(PWM_ALL[PWM_ALL_head].NumberOfPulse)		//ʣ�����������Ϊ0
					{
						if(PWM_ALL[PWM_ALL_head].NumberOfPulse != 0xffff)	//�������-1(-1����һֱ���)
							PWM_ALL[PWM_ALL_head].NumberOfPulse --;	//����һ����
					
						PWM_ALL[PWM_ALL_head].Phace += PWM_ALL[PWM_ALL_head].LLength;
					}
					else{		//û��ʣ��������
						UsingPins &= (~(1L << PWM_ALL_head));	//ע����һͨ��
						PWM_ALL_head = PWM_ALL[PWM_ALL_head].NextChannel;	//ָ����һ�����ᣬ������һ����
						if(PWM_ALL_head == (s16)NULL_p)
							break;
						else
							continue;
					}
					
				}//if ODRData.n == 0
				
				
				/*��ֹ���*/
				if(PWM_ALL[PWM_ALL_head].Phace > PhaceMax)
					overflow_flag |= (1L << PWM_ALL_head);
				
				if(!(~overflow_flag ))	//overflow_flag == 0xffff
				{
					u16 tmp_n = PWM_ALL_head;
					
					//ÿһ�����ᶼ��ȥPhaceMax
					while(tmp_n != NULL_p)
					{
						PWM_ALL[tmp_n].Phace -= PhaceMax;
						tmp_n = PWM_ALL[tmp_n].NextChannel;
					}

					overflow_flag = ~UsingPins;	//
				}//if overflag == 0xffff
				
				
				/* ��������������ȥ */
				//�˴���ÿ��������Ż������жϺ�����Ȼ����10���Ĵ�����������һ��ռ�úܶࡣ����
				{
					register int n_t = PWM_ALL[PWM_ALL_head].NextChannel, n_b = PWM_ALL_head, t_h = PWM_ALL_head;
					register int tmp_arr2 = PWM_ALL[PWM_ALL_head].Phace;
					
					while(n_t!= NULL_p && (tmp_arr2 > PWM_ALL[n_t].Phace)){
						n_b = n_t;
						n_t = PWM_ALL[n_t].NextChannel;
					}

					//����
					PWM_ALL[n_b].NextChannel = PWM_ALL_head;
					//ָ����һ������
					PWM_ALL_head = PWM_ALL[PWM_ALL_head].NextChannel;
					PWM_ALL[t_h].NextChannel = n_t;
				}//��������
				
			}while(tmp_arr == PWM_ALL[PWM_ALL_head].Phace);
			
			ARRData = tmp_arr;
			
		}//for n=0..16, ���ɨ����λ��Ϣ
		
	}//if usingpins != 0xXXXX0000

	if(*((u32*)0x42400044))
	{
		//���¿�����������ж�
		*((u32*)0x42400b08) = 1;
		BSRR_Queue = IO_BSRR_Queue[0];
		TIM_Queue = TIM_ARR_Queue[0];
	}
	else 
	{
		//0x40020058.2��ȡ����������ж�
		*((u32*)0x42400b08) = 0;
		BSRR_Queue = IO_BSRR_Queue[1];
		TIM_Queue = TIM_ARR_Queue[1];
	}
	//���DMA_CH5�����жϱ�־
	DMA1->IFCR = DMA1_IT_GL5;
}

/* ��ѯPWMʣ����� */
int 	PWMCheckRest(int channel)
{
	return PWM_ALL[channel].NumberOfPulse;
}


/* ǿ��ֹͣ���е�PWM��� */
void PWMStopAll(void)
{
	TIM_Cmd(TIM2, DISABLE);
	
	{
		//û������ˣ�Ҫ������������
		register int n;
		for(n = 0; n < (PWM_BUFFER << 1); n++)
		{
			((u16*)TIM_ARR_Queue)[n] = buffertime;
			((u32*)IO_BSRR_Queue)[n] = 0;
		}
	}
	
	//DMA��λ
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_Cmd(DMA1_Channel7, DISABLE);
	
	DMA1_Channel5->CNDTR = PWM_BUFFER << 1;
	DMA1_Channel7->CNDTR = PWM_BUFFER << 1; 
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
	DMA_Cmd(DMA1_Channel7, ENABLE);
	
	PWM_ALL[1].NumberOfPulse=0;
	PWM_ALL[2].NumberOfPulse=0;
	PWM_ALL[3].NumberOfPulse=0;
}

/* ֹͣĳһ��ͨ�� */
void PWMStop(int channel)
{
	register int n;
	register u32 *p = &IO_BSRR_Queue[1][PWM_BUFFER - 1];

	channel = ~( 1 << channel );
	
	//ע����һͨ��
	UsingPins &= channel;
	BSRRData = ( BSRRData & channel ) & ( channel << 16 );
	
	//�����һͨ�����������Ҫ�Ǹߵ�ƽ�������
	for(n = 0; n < (PWM_BUFFER << 1); n++)
	{
		*p-- &= channel;
	}
	//���Ҫ����������
	PWM_GPIO->BRR = (~channel);
	PWM_ALL[channel].NumberOfPulse=0;
}



