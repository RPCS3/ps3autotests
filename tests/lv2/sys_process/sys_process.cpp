#include <stdio.h>
#include <stdlib.h>
#include <sys/process.h>

#include <sys/synchronization.h>
#include <sys/interrupt.h>
#include <sys/event.h>
#include <sys/timer.h>
#include <sys/prx.h>
#include <sys/fs.h>

const int g_objectTypes[] = {
	SYS_MEM_OBJECT,
	SYS_MUTEX_OBJECT,
	SYS_COND_OBJECT,
	SYS_RWLOCK_OBJECT,
	SYS_INTR_TAG_OBJECT,
	SYS_INTR_SERVICE_HANDLE_OBJECT,
	SYS_EVENT_QUEUE_OBJECT,
	SYS_EVENT_PORT_OBJECT,
	SYS_TRACE_OBJECT,
	SYS_SPUIMAGE_OBJECT,
	SYS_PRX_OBJECT,
	SYS_SPUPORT_OBJECT,
	SYS_LWMUTEX_OBJECT,
	SYS_TIMER_OBJECT,
	SYS_SEMAPHORE_OBJECT,
	SYS_FS_FD_OBJECT,
	SYS_LWCOND_OBJECT,
	SYS_EVENT_FLAG_OBJECT,
};

size_t g_objectCount[sizeof(g_objectTypes)/sizeof(g_objectTypes[0])];

void printNumberOfObjects(const char* prefix) {
	size_t nump;
	printf("%s>", prefix);
	for (size_t i=0; i<sizeof(g_objectTypes)/sizeof(g_objectTypes[0]); i++) {
		sys_process_get_number_of_object(g_objectTypes[i], &nump);
		printf(" %d", nump-g_objectCount[i]);
	}
	printf("\n");
}

void getNumberOfObject() {
	// Count objects before running the test
	for (size_t i=0; i<sizeof(g_objectTypes)/sizeof(g_objectTypes[0]); i++)
		sys_process_get_number_of_object(g_objectTypes[i], &g_objectCount[i]);

	// SYS_MEM_OBJECT
	// TODO: Neither sys_memory_container_create nor sys_memory_allocate modify the SYS_MEM_OBJECT count
	// printNumberOfObjects("01");

	// SYS_MUTEX_OBJECT
	sys_mutex_t mutex_id;
	sys_mutex_attribute_t mutex_attr;
	sys_mutex_attribute_initialize(mutex_attr);
	sys_mutex_create(&mutex_id, &mutex_attr);
	printNumberOfObjects("02");

	// SYS_COND_OBJECT
	sys_cond_t cond_id;
	sys_cond_attribute_t cond_attr;
	sys_cond_attribute_initialize(cond_attr);
	sys_cond_create(&cond_id, mutex_id, &cond_attr);
	printNumberOfObjects("03");
	sys_cond_destroy(cond_id);
	sys_mutex_destroy(mutex_id);

	// SYS_RWLOCK_OBJECT
	sys_rwlock_t rwlock_id;
	sys_rwlock_attribute_t rwlock_attr;
	sys_rwlock_attribute_initialize(rwlock_attr);
	sys_rwlock_create(&rwlock_id, &rwlock_attr);
	printNumberOfObjects("04");
	sys_rwlock_destroy(rwlock_id);

	// TODO: SYS_INTR_TAG_OBJECT
	// sys_interrupt_tag_t intrtag;
	// printf("0x%x\n", sys_interrupt_tag_create(&intrtag, 0, SYS_HW_THREAD_ANY));
	// printNumberOfObjects("05");
	// sys_interrupt_tag_destroy(intrtag);

	// TODO: SYS_INTR_SERVICE_HANDLE_OBJECT
	// printNumberOfObjects("06");

	// SYS_EVENT_QUEUE_OBJECT
	sys_event_queue_t equeue_id;
	sys_event_queue_attribute_t equeue_attr;
	sys_event_queue_attribute_initialize(equeue_attr);
	sys_event_queue_create(&equeue_id, &equeue_attr, SYS_EVENT_QUEUE_LOCAL, 64);
	printNumberOfObjects("07");
	sys_event_queue_destroy(equeue_id, 0);
	
	// SYS_EVENT_PORT_OBJECT
	sys_event_port_t event_port_id;
	sys_event_port_create(&event_port_id, SYS_EVENT_PORT_LOCAL, SYS_EVENT_PORT_NO_NAME);
	printNumberOfObjects("08");
	sys_event_port_destroy(event_port_id);

	// TODO: SYS_TRACE_OBJECT
	// NOTE: This is actually not necessary, since those objects are only available on DEX/DECR
	// printNumberOfObjects("09");

	// TODO: SYS_SPUIMAGE_OBJECT
	// printNumberOfObjects("10");

	// TODO: SYS_PRX_OBJECT
	// printNumberOfObjects("11");

	// TODO: SYS_SPUPORT_OBJECT
	// printNumberOfObjects("12");

	// SYS_LWMUTEX_OBJECT
	sys_lwmutex_t lwmutex;
	sys_lwmutex_attribute_t lwmutex_attr;
	sys_lwmutex_attribute_initialize(lwmutex_attr);
	sys_lwmutex_create(&lwmutex, &lwmutex_attr);
	printNumberOfObjects("13");

	// SYS_TIMER_OBJECT
	sys_timer_t timer_id;
	sys_timer_create(&timer_id);
	printNumberOfObjects("14");
	sys_timer_destroy(timer_id);

	// SYS_SEMAPHORE_OBJECT
	sys_semaphore_t sem;
	sys_semaphore_attribute_t sem_attr;
	sys_semaphore_attribute_initialize(sem_attr);
	sys_semaphore_create(&sem, &sem_attr, 0, 1);
	printNumberOfObjects("15");
	sys_semaphore_destroy(sem);

	// SYS_FS_FD_OBJECT
	FILE* file = fopen("/app_home/PARAM.SFO", "rb");
	printNumberOfObjects("16");
	fclose(file);

	// Add SYS_LWCOND_OBJECT
	sys_lwcond_t lwcond;
	sys_lwcond_attribute_t lwcond_attr;
	sys_lwcond_attribute_initialize(lwcond_attr);
	sys_lwcond_create(&lwcond, &lwmutex, &lwcond_attr);
	printNumberOfObjects("17");
	sys_lwmutex_destroy(&lwmutex);
	sys_lwcond_destroy(&lwcond);

	// SYS_EVENT_FLAG_OBJECT
	sys_event_flag_t event_flag;
	sys_event_flag_attribute_t event_flag_attr;
	sys_event_flag_attribute_initialize(event_flag_attr);
	sys_event_flag_create(&event_flag, &event_flag_attr, 0);
	printNumberOfObjects("18");
	sys_event_flag_destroy(event_flag);
}

void getSdkVersion() {
	int version;
	system_call_2(SYS_PROCESS_GET_SDK_VERSION, sys_process_getpid(), (std::uint64_t)&version);
	printf("sys_process_get_sdk_version:\n0x%x\n\n", version);
}

void getParamSfo() {
	char buf[64];
	system_call_1(30, (std::uint64_t)buf);
	printf("sys_process_get_paramsfo:\n");
	for (size_t i=0; i<sizeof(buf); i++)
		printf("%02X ", (char)buf[i]);
	printf("\n\n");
}

int main(void) {
	// sys_process_getpid, sys_process_getppid, sys_process_get_ppu_guid
	printf("sys_process_getpid:\n0x%x\n\n", sys_process_getpid());
	printf("sys_process_getppid:\n0x%x\n\n", sys_process_getppid());
	printf("sys_process_get_ppu_guid:\n0x%x\n\n", sys_process_getppid());

	// sys_process_get_number_of_object
	printf("sys_process_get_number_of_object:\n");
	getNumberOfObject();
	printf("\n");

	// sys_process_get_sdk_version
	// TODO: It's not working properly, it returns -1 on real PS3s.
	getSdkVersion();

	// sys_process_get_paramsfo
	// TODO: I don't think it's detecting/parsing any PARAM.SFO's properly. How could we fix this?
	getParamSfo();

	// sys_process_is_stack
	void* p = malloc(64);
	printf("sys_process_is_stack: (void* p = malloc(64))\n");
	printf("(&p) -> %s\n",   sys_process_is_stack(&p) ? "True" : "False");
	printf("(p)  -> %s\n\n", sys_process_is_stack(p)  ? "True" : "False");

	// sys_process_is_spu_lock_line_reservation_address
	// TODO: ?

	return 0;
}
