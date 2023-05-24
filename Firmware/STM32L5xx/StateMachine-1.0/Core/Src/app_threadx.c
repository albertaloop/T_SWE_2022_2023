/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_QS		2
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
 TX_QUEUE qs[NUM_QS];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */


  TX_THREAD state_thrd;
  TX_THREAD uart_thrd;
  TX_THREAD can_thrd;

  tx_queue_create(&qs[CMD_Q], "Command Queue", TX_1_ULONG, memory_ptr, CMD_QUEUE_SIZE*sizeof(UINT));

  tx_queue_create(&qs[CMD_SEND_Q], "CAN Send Queue", TX_1_ULONG, memory_ptr, CAN_SEND_QUEUE_SIZE*sizeof(UINT));

  tx_byte_allocate(byte_pool, (VOID **) &memory_ptr, STATEM_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&state_thrd, "State Machine Thread", state_thrd_fn, (ULONG)qs, memory_ptr, STATEM_THRD_STACK_SIZE,
		 STATEM_THRD_PRIO, STATEM_THRD_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);

  tx_byte_allocate(byte_pool, (VOID **) &memory_ptr, UART_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&uart_thrd, "UART Thread", uart_thrd_fn, (ULONG)&qs[CMD_Q], memory_ptr, UART_THRD_STACK_SIZE,
		 UART_THRD_PRIO, UART_THRD_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);

  tx_byte_allocate(byte_pool, (VOID **) &memory_ptr, STATEM_STACK_SIZE, TX_NO_WAIT);
  tx_thread_create(&can_thrd, "CAN Thread", can_thrd_fn, (ULONG)&qs[CMD_SEND_Q], memory_ptr, CAN_THRD_STACK_SIZE,
		 CAN_THRD_PRIO, CAN_THRD_PRIO, TX_NO_TIME_SLICE, TX_AUTO_START);

  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  App_ThreadX_LowPower_Enter
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Enter(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Enter */

  /* USER CODE END  App_ThreadX_LowPower_Enter */
}

/**
  * @brief  App_ThreadX_LowPower_Exit
  * @param  None
  * @retval None
  */
void App_ThreadX_LowPower_Exit(void)
{
  /* USER CODE BEGIN  App_ThreadX_LowPower_Exit */

  /* USER CODE END  App_ThreadX_LowPower_Exit */
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

bool systemsCheck() {
  // Sound off CAN nodes
  // Check for valid ranges of sensor data
  return true;
}

int get_cmd_ack(int cmd) {
  if(cmd == CMD_ESTOP) {
    return ACK_ESTOP;
  } else if (cmd == CMD_PREP) {
    return ACK_PREP;
  } else if (cmd == CMD_LAUNCH) {
    return CMD_LAUNCH;
  } else if (cmd == CMD_CRAWL) {
    return ACK_CRAWL;
  } else if (cmd == CMD_HEALTHCHK) {
    return ACK_HEALTHCHK;
  } else {
    Serial.println("Invalid CMD");
    return -1;
  }
}

// USART functions
void receive_cmds(int * acked_cmd) {
  int recv_buf[2];
  for(int i = 0; i < 10; i++) {
    if(USART1.available() > 0) {
      recv_buf[0] = USART1.read();
      recv_buf[1] = USART1.read();
      if(recv_buf[0] != -1 && recv_buf[1] != -1) {
        if(recv_buf[0] == CMD_MSG) {
          // Command received
          if(*acked_cmd != recv_buf[1]) {
            *acked_cmd = recv_buf[1];
            usart_recv_cbuf.Write(recv_buf[1]);
            int cmd_ack = get_cmd_ack(recv_buf[1]);
            USART1.print(ACK_MSG);
            USART1.print(cmd_ack);
          }
        }
      }
    }
    else {
      break;
    }
  }
}
void send_telemetry() {

}


// State functions
void state_fn(void(*fn)(int i)) {
  int cmd_msg;
  while(!usart_recv_cbuf.isEmpty()) {
    cmd_msg = usart_recv_cbuf.Read();
    fn(cmd_msg);
  }
}
void fault_state(int cmd_msg) {
  if(systemsCheck()) {
    status = Status::SafeToApproach;
    state_changed = true;
  }
}
void safe_to_approach(int cmd_msg) {
  if(state_changed) {
      // CAN_message_t msg_out;
      // msg_out.id = (uint32_t)0x0fffffff;
      // msg_out.len = (uint8_t)1;
      // output_buffer.Write(msg_out);
      // state_changed = false;
  }
  if(cmd_msg == CMD_PREP) {
    status = Status::ReadyToLaunch;
    state_changed = true;
  } else if (cmd_msg == CMD_ESTOP) {
    status = Status::Fault;
    state_changed = true;
  }
}
void ready_to_launch(int cmd_msg) {
  if(gui_launch_commanded && brake_ready && motor_ready) {
    status = Status::Launching;
  } else if (fault) {
    status = Status::Fault;
  }
}
void launching(int cmd_msg) {
  if(motor_coast) {
    status = Status::Coasting;
  } else if (fault) {
    status = Status::Fault;
  } else if (gui_stop) {
    status = Status::Braking;
  }
}
void coasting(int cmd_msg) {

}

void state_thrd_fn(ULONG) {
	while(1) {
		switch (status) {
			case Fault: {
				state_fn(fault_state);
				break;
			}
			case SafeToApproach: {
				state_fn(safe_to_approach);
				break;
			}
			case ReadyToLaunch: {
				state_fn(ready_to_launch);
				break;
			}
			case Launching: {
				state_fn(launching);
				break;
			}
			case Coasting: {
				state_fn(coasting);
				break;
			}
			default:
				break;
		}
	}
}

void uart_thrd_fn(void) {
	while(1) {

	}
}

void can_thrd_fn(void) {

}

/* USER CODE END 1 */
