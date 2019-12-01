
CC=g++
#CFLAGS=-g -Wall -m32
CFLAGS=-g -Wall
LIBS=-lmosquitto
DEFINE_FLAGS=-D_PC_
#DEFINE_FLAGS+=-DPRINT
#DEFINE_FLAGS+=-DDEBUG_CRC
#DEFINE_FLAGS+=-DDEBUG_SYNC
#DEFINE_FLAGS+=-DDEBUG_USART_READ
#DEFINE_FLAGS+=-DDEBUG_USART_WRITE

main: ../src/main__linux__.cpp HNSAddressMap.o HNSLight.o HNSMQTTPublish.o HNSDevice_class.o HNSTemperatureHumidity.o HNSDatabase.o sigint_handler.o HNSRefresh.o libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -o main ../src/main__linux__.cpp HNSAddressMap.o HNSLight.o HNSMQTTPublish.o HNSDevice_class.o HNSTemperatureHumidity.o HNSDatabase.o HNSSync.o sigint_handler.o HNSRefresh.o libHNSCore.a $(LIBS) 

sigint_handler.o: ../lib/HNSHub/sigint_handler.c ../lib/HNSHub/sigint_handler.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHub/sigint_handler.c $(LIBS)

HNSLight.o: ../lib/HNSHubLib/HNSLight/HNSLight.hpp ../lib/HNSHubLib/HNSLight/HNSLight.cpp HNSDevice_class.o
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHubLib/HNSLight/HNSLight.cpp $(LIBS)

HNSTemperatureHumidity.o: ../lib/HNSHubLib/HNSTemperatureHumidity/HNSTemperatureHumidity.hpp ../lib/HNSHubLib/HNSTemperatureHumidity/HNSTemperatureHumidity.cpp HNSDevice_class.o
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHubLib/HNSTemperatureHumidity/HNSTemperatureHumidity.cpp $(LIBS)

HNSRefresh.o: ../lib/HNSHub/HNSRefresh.hpp ../lib/HNSHub/HNSRefresh.cpp libHNSCore.a HNSDatabase.o
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHub/HNSRefresh.cpp  $(LIBS)

HNSAddressMap.o: ../lib/HNSHub/HNSAddressMap.hpp ../lib/HNSHub/HNSAddressMap.cpp 
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHub/HNSAddressMap.cpp $(LIBS)

HNSDatabase.o: HNSAddressMap.o HNSSync.o HNSTemperatureHumidity.o HNSLight.o ../lib/HNSHub/HNSDatabase.hpp ../lib/HNSHub/HNSDatabase.cpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHub/HNSDatabase.cpp $(LIBS)

HNSDevice_class.o: ../lib/HNSCore/HNSMacro.h ../lib/HNSHub/HNSDevice_class.hpp ../lib/HNSHub/HNSDevice_class.cpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/HNSHub/HNSDevice_class.cpp $(LIBS)

HNSMQTTPublish.o: ../lib/HNSHub/HNSMQTTPublish.hpp ../lib/HNSHub/HNSMQTTPublish.cpp ../lib/HNSCore/HNSError.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSHub/HNSMQTTPublish.cpp  ../lib/HNSCore/HNSError.h $(LIBS)

HNSSync.o: ../lib/HNSHub/HNSSync.hpp ../lib/HNSHub/HNSSync.cpp libHNSCore.a
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSHub/HNSSync.cpp $(LIBS)


#////////////////////////////libHNSCore////////////////////////////////////////

libHNSCore.a: HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Linux.o
	ar rvs libHNSCore.a HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Linux.o

HNSTest.o : HNSRouting.o  ../lib/HNSCore/HNSTest.h  ../lib/HNSCore/HNSTest.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSTest.c $(LIBS)

HNSRouting.o: USART_Linux.o HNSError.o CRC.o ../lib/HNSCore/HNSRouting.h ../lib/HNSCore/HNSRouting.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSRouting.c $(LIBS)

CRC.o: ../lib/util/CRC.c ../lib/util/CRC.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c ../lib/util/CRC.c $(LIBS)

USART_Linux.o: ../lib/util/Linux/USART_Linux.h ../lib/util/Linux/USART_Linux.c ../lib/util/Linux/MAX485.h
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/util/Linux/USART_Linux.c ../lib/util/Linux/MAX485.h $(LIBS)

HNSError.o: ../lib/HNSCore/HNSError.h ../lib/HNSCore/HNSError.c
	$(CC) $(CFLAGS) $(DEFINE_FLAGS) -c  ../lib/HNSCore/HNSError.c $(LIBS)

.phony: clean

clean:
	rm -rf main HNSTest.o HNSRouting.o CRC.o HNSError.o USART_Linux.o libHNSCore.a
	rm -rf HNSDatabase.o HNSDevice_class.o HNSSync.o HNSTemperatureHumidity.o
	rm -rf HNSAddressMap.o sigint_handler.o HNSRefresh.o HNSMQTTPublish.o HNSLight.o
