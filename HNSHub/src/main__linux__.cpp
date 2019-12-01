
#ifdef __linux__

#include "../lib/HNSCore/HNSRouting.h"
#include "../lib/HNSCore/HNSTest.h"
#include <stdio.h>
#include <stdlib.h>
#include "../lib/HNSHub/HNSSync.hpp"
#include "../lib/HNSHub/HNSDevice_class.hpp"
#include "../lib/HNSHubLib/HNSTemperatureHumidity/HNSTemperatureHumidity.hpp"
#include "../lib/HNSHub/HNSDatabase.hpp"
#include "../lib/HNSHub/HNSRefresh.hpp"
#include "../lib/HNSHub/sigint_handler.h"
#include "../lib/HNSHub/HNSMQTTPublish.hpp"

//#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <mutex>

static inline void refresh_pusblish(){
	for(int i=0; i< HNSDatabase_deviceList_index; i++){
		HNSDatabase_mutex.lock();
		
		HNSRefresh_refresh(i);
		HNSMQTTPublish_publishDevice(i);

		HNSDatabase_mutex.unlock();
	}
}

int main(){
	sigint_handler_init();
	HNSRouting_init(HNSRouting_role_HNSHub);
	HNSDatabase_init();
	
	HNSMQTTPublish_init("192.168.12.187");

    
	for(int k = 0; k <100 ; k++){
		if(HNSSync_sync()==0) printf("================\033[1;32m NEW DEVICE \033[0m================\n");

		refresh_pusblish();	

		sleep(1);
	}
    

	return 0;
}


#endif /*__linux__*/
