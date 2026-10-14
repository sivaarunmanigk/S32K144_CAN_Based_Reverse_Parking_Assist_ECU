#include "sdk_project_config.h"
#include <stdint.h>

#define Reverse_gear (1<<16)
#define Warning_light (1<<15)

uint8_t gear;
uint8_t distance;

flexcan_state_t canstate;
flexcan_data_info_t rx_info= {.msg_id_type = FLEXCAN_MSG_ID_STD,.data_length = 1,.is_remote = false};

void get_data(const flexcan_msgbuff_t *rx_buff){

	if(rx_buff->msgId == 0x100){
		gear=rx_buff->data[0];
	}

	if(rx_buff->msgId == 0x101){
		distance=rx_buff->data[0];
	}

}

void parking_control(uint8_t gear,uint8_t distance){

	if(gear==2){
		PINS_DRV_ClearPins(PTD,Reverse_gear);
		if(distance>100){
			PINS_DRV_SetPins(PTD,Warning_light);
		}

		else if((distance>=51)&&(distance<=100)){
			PINS_DRV_TogglePins(PTD,Warning_light);
			OSIF_TimeDelay(1000);
		}

		else if(distance<=50){
			PINS_DRV_TogglePins(PTD,Warning_light);
			OSIF_TimeDelay(500);
		}
	}
	else{
		PINS_DRV_SetPins(PTD,Reverse_gear);
		PINS_DRV_SetPins(PTD,Warning_light);
	}
}

int main(void){

	CLOCK_DRV_Init(&clockMan1_InitConfig0);
	PINS_DRV_Init(NUM_OF_CONFIGURED_PINS0, g_pin_mux_InitConfigArr0);
	FLEXCAN_DRV_Init(INST_FLEXCAN_CONFIG_1, &flexcanState0, &flexcanInitConfig0);

	flexcan_msgbuff_t rx_buff;

	FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, 0, &rx_info, 0x100);
	FLEXCAN_DRV_ConfigRxMb(INST_FLEXCAN_CONFIG_1, 1, &rx_info, 0x101);

	while(1){

		status_t status;
		status=FLEXCAN_DRV_ReceiveBlocking(INST_FLEXCAN_CONFIG_1, 0, &rx_buff, 1000);

		if(status==STATUS_SUCCESS){
			get_data(&rx_buff);
		}

		status=FLEXCAN_DRV_ReceiveBlocking(INST_FLEXCAN_CONFIG_1, 1, &rx_buff, 1000);
		if(status==STATUS_SUCCESS){
			get_data(&rx_buff);
		}

		parking_control(gear,distance);

	}
}
