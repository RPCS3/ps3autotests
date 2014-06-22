#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ppu_intrinsics.h>

#include <sys/event.h>
#include <sys/timer.h>
#include <sys/ppu_thread.h>

// Defines
#define EVENT_FLAG_INIT           0
#define NUM_OF_WORKERS            5
#define PPU_THREAD_STACK_SIZE  4096

// Globals
int table[NUM_OF_WORKERS];
sys_event_flag_t eflag_master;
sys_event_flag_t eflag_worker;


void worker_func(uint64_t thr_num)
{
	// Wait until the previous workers are finished (note that thr_num == 0 won't wait)
	if (sys_event_flag_wait(eflag_worker, (1ULL << thr_num)-1, SYS_EVENT_FLAG_WAIT_AND, 0, SYS_NO_TIMEOUT) != CELL_OK) {
		printf("[!]  Worker[%llu]: sys_event_flag_wait failed!\n");
		exit(-1);
	}

	// Wait a moment, modify the table, and notify the master
	sys_timer_sleep(1);
	table[thr_num] = 1234;
	if (sys_event_flag_set(eflag_master, 1ULL << thr_num) != CELL_OK) {
		printf("[!] Worker[%llu]: I failed to my master. Doing harakiri!\n", thr_num);
		exit(-1);
	}

	// Wait for the master's review and check it
	if (sys_event_flag_wait(eflag_worker, 1ULL << thr_num, SYS_EVENT_FLAG_WAIT_AND, 0, SYS_NO_TIMEOUT) != CELL_OK) {
		printf("[!] Worker[%llu]: I failed to my master. Doing harakiri!\n", thr_num);
		exit(-1);
	}
	if (table[thr_num] != 0) {
		printf("[!] Worker[%llu]: Failed my job!\n", thr_num);
		exit(-1);
	}

	sys_ppu_thread_exit(0);
}

void master_func(uint64_t arg)
{
	printf("Master: Starting.\n");

	int ret;
	int my_prio;
	sys_ppu_thread_t me;
	sys_ppu_thread_get_id(&me);
	sys_ppu_thread_get_priority(me, &my_prio);

	// Start workers
	for (int n=0; n < NUM_OF_WORKERS; n++) {
		sys_ppu_thread_t worker;
		ret = sys_ppu_thread_create(&worker, worker_func, n, my_prio + n, PPU_THREAD_STACK_SIZE, SYS_PPU_THREAD_CREATE_JOINABLE, "Worker");
		if (ret != CELL_OK) {
			printf("[!] Master: sys_ppu_thread_create failed with code 0x%x.\n", ret);
			exit(ret);
		}
	}

	// Check worker jobs
	uint64_t result;
	uint64_t flags = (1ULL << NUM_OF_WORKERS) - 1;
	int worker_jobs = NUM_OF_WORKERS;
	while (worker_jobs != 0) {
		ret = sys_event_flag_wait(eflag_master, flags, SYS_EVENT_FLAG_WAIT_OR | SYS_EVENT_FLAG_WAIT_CLEAR, &result, SYS_NO_TIMEOUT);
		if (ret != CELL_OK) {
			printf("[!] Master: sys_event_flag_wait failed with code 0x%x.\n", ret);
			exit(ret);
		}
		size_t workerId = 63 - __cntlzd(result);
		if (table[workerId] != 1234) {
			printf("[!] Master: table[workerId] has an unexpected value.\n");
			exit(-1);
		}
		printf("Worker[%d] did his job successfully.\n", workerId);
		table[workerId] = 0;
		sys_event_flag_set(eflag_worker, (1ULL << workerId));
		worker_jobs--;
	}

	// Wait a moment and exit
	sys_timer_sleep(1);
	printf("Master: Exiting.\n");
	sys_ppu_thread_exit(0);
}

void eventflagWait(uint64_t arg) {
	int ret = sys_event_flag_wait(eflag_master, 1ULL, SYS_EVENT_FLAG_WAIT_OR, 0, SYS_NO_TIMEOUT);
	if (ret & ~0x80010013) {
		printf("[!] sys_event_flag_wait: Something went wrong!\n");
		exit(-1);
	}
	if (ret == 0x80010013) {
		printf("sys_event_flag_wait: Wait canceled successfully.\n");
	}
	sys_ppu_thread_exit(0);
}

void eventflagTrywait(uint64_t arg) {
	if (sys_event_flag_trywait(eflag_master, 1ULL, SYS_EVENT_FLAG_WAIT_OR | SYS_EVENT_FLAG_WAIT_CLEAR, 0)) {
		printf("sys_event_flag_trywait returned an error (probably, this was supposed to happen)\n");
	}
	else {
		printf("sys_event_flag_trywait worked fine\n");
	}
	sys_ppu_thread_exit(0);
}

void eventflagCancel(uint64_t arg) {
	int num;
	if (sys_event_flag_cancel(eflag_master, &num)) {
		printf("[!] sys_event_flag_cancel: Something went wrong!\n");
		exit(-1);
	}
	sys_timer_sleep(1);
	printf("sys_event_flag_cancel: Woke up %d threads\n", num);
	sys_ppu_thread_exit(0);
}


int createThreadAndJoin(void (*func)(uint64_t), uint64_t arg=0) {
	uint64_t val;
	sys_ppu_thread_t thread;
	std::size_t stackSize = 4096;
	if (sys_ppu_thread_create(&thread, func, arg, 1500, stackSize, SYS_PPU_THREAD_CREATE_JOINABLE, "Thread")) {
		printf("[!] sys_ppu_thread_create: Something went wrong!\n");
		exit(-1);
	}
	if (sys_ppu_thread_join(thread, &val)) {
		printf("[!] sys_ppu_thread_join: Something went wrong!\n");
		exit(-1);
	}
}

int createThread(void (*func)(uint64_t), uint64_t arg=0) {
	uint64_t val;
	sys_ppu_thread_t thread;
	std::size_t stackSize = 4096;
	if (sys_ppu_thread_create(&thread, func, arg, 1500, stackSize, 0, "Worker")) {
		printf("[!] sys_ppu_thread_create: Something went wrong!\n");
		exit(-1);
	}
}

void testSysEventFlagErrors()
{
	sys_event_flag_t id;               // Working event flag
	sys_event_flag_t id_f;             // Broken event flag
	sys_event_flag_attribute_t attr;   // Initialized event flag attribute
	sys_event_flag_attribute_t attr_z; // Zeroed event flag attribute
	sys_event_flag_attribute_initialize(attr);
	memset(&attr_z, 0x00, sizeof(sys_event_flag_attribute_t));
	uint64_t result, flags;

	// Create the working event flag
	if (sys_event_flag_create(&id, &attr, 0)) {
		printf("[!] sys_event_flag_create: Something went wrong!\n");
		exit(-1);
	}

#define AND SYS_EVENT_FLAG_WAIT_AND
#define OR SYS_EVENT_FLAG_WAIT_OR

	// Error tests
	printf("[*] Checking sys_event_flag_create errors:\n");
	printf("sys_event_flag_create: &sem == NULL returns: 0x%x\n", sys_event_flag_create(NULL, &attr, 0));
	printf("sys_event_flag_create: &attr == NULL returns: 0x%x\n", sys_event_flag_create(&id_f, NULL, 0));
	printf("sys_event_flag_create: uninitialized attr returns: 0x%x\n",  sys_event_flag_create(&id_f, &attr_z, 0));

	printf("\n[*] Checking sys_event_flag_wait errors:\n");
	printf("sys_event_flag_wait: short timeout returns: 0x%x\n", sys_event_flag_wait(id, 1ULL, AND, &result, 1ULL));
	printf("sys_event_flag_wait: mode == 0 returns: 0x%x\n", sys_event_flag_wait(id, 1ULL, 0, &result, 0ULL));
	printf("sys_event_flag_wait: mode == AND|OR returns: 0x%x\n", sys_event_flag_wait(id, 1ULL, AND|OR, &result, 0ULL));
	printf("sys_event_flag_wait: failed event flag id returns: 0x%x\n", sys_event_flag_wait(id_f, 0ULL, AND, &result, 0ULL));
	printf("sys_event_flag_wait: invalid event flag id returns: 0x%x\n", sys_event_flag_wait(NULL, 0ULL, AND, &result, 0ULL));
	
	printf("\n[*] Checking sys_event_flag_trywait errors:\n");
	printf("sys_event_flag_trywait: mode == 0 returns: 0x%x\n", sys_event_flag_trywait(id, 1ULL, 0, &result));
	printf("sys_event_flag_trywait: mode == AND|OR returns: 0x%x\n", sys_event_flag_trywait(id, AND|OR, -1, &result));
	printf("sys_event_flag_trywait: failed event flag id returns: 0x%x\n", sys_event_flag_trywait(id_f, 0ULL, AND, &result));
	printf("sys_event_flag_trywait: invalid event flag id returns: 0x%x\n", sys_event_flag_trywait(NULL, 0ULL, AND, &result));

	printf("\n[*] Checking sys_event_flag_set errors:\n");
	printf("sys_event_flag_set: failed event flag id returns: 0x%x\n", sys_event_flag_set(id_f, 0ULL));
	printf("sys_event_flag_set: invalid event flag id returns: 0x%x\n", sys_event_flag_set(NULL, 0ULL));

	printf("\n[*] Checking sys_event_flag_clear errors:\n");
	printf("sys_event_flag_clear: failed event flag id returns: 0x%x\n", sys_event_flag_clear(id_f, 0ULL));
	printf("sys_event_flag_clear: invalid event flag id returns: 0x%x\n", sys_event_flag_clear(NULL, 0ULL));

	printf("\n[*] Checking sys_event_flag_cancel errors:\n");
	printf("sys_event_flag_cancel: failed event flag id returns: 0x%x\n", sys_event_flag_cancel(id_f, 0));
	printf("sys_event_flag_cancel: invalid event flag id returns: 0x%x\n", sys_event_flag_cancel(NULL, 0));

	printf("\n[*] Checking sys_event_flag_get errors:\n");
	printf("sys_event_flag_get: &flags == NULL returns: 0x%x\n", sys_event_flag_get(id, NULL));
	printf("sys_event_flag_get: failed event flag id returns: 0x%x\n", sys_event_flag_get(id_f, &flags));
	printf("sys_event_flag_get: invalid event flag id returns: 0x%x\n", sys_event_flag_get(NULL, &flags));

	// Destroy the working event flag
	if (sys_event_flag_destroy(id)) {
		printf("[!] sys_event_flag_destroy: Something went wrong!\n");
		exit(-1);
	}

	// More error tests
	printf("\n[*] Checking sys_event_flag_destroy errors:\n");
	printf("sys_event_flag_destroy: failed event flag id returns: 0x%x\n", sys_event_flag_destroy(id_f));
	printf("sys_event_flag_destroy: invalid event flag id returns: 0x%x\n", sys_event_flag_destroy(NULL));
	printf("sys_event_flag_destroy: destroyed event flag id returns: 0x%x\n", sys_event_flag_destroy(id));
}

int main(void) {
	// Check errors on purpose
	testSysEventFlagErrors();

	// Creating event flags
	printf("\n[*] Testing sys_event_flag_wait and sys_event_flag_set:\n");
	sys_event_flag_attribute_t attr;
	sys_event_flag_attribute_initialize(attr);
	if (sys_event_flag_create(&eflag_master, &attr, EVENT_FLAG_INIT) != CELL_OK ||
		sys_event_flag_create(&eflag_worker, &attr, EVENT_FLAG_INIT) != CELL_OK) {
		printf("[!] Main: Failed when creating the event flags.\n");
		exit(-1);
	}

	// Run master thread
	createThreadAndJoin(master_func);
	
	// Display and modify the flags
	printf("\n[*] Testing sys_event_flag_get and sys_event_flag_(clear|set):\n");
	uint64_t flags_m, flags_w;
	sys_event_flag_get(eflag_master, &flags_m);
	sys_event_flag_get(eflag_worker, &flags_w);
	printf("sys_event_flag_get: eflag_master -> 0x%016llx\n", flags_m);
	printf("sys_event_flag_get: eflag_worker -> 0x%016llx\n", flags_w);
	sys_event_flag_set(eflag_master,   0xAAAAAAAAAAAAAAABULL);
	sys_event_flag_clear(eflag_worker, 0xAAAAAAAAAAAAAAAAULL);
	sys_event_flag_get(eflag_master, &flags_m);
	sys_event_flag_get(eflag_worker, &flags_w);
	printf("sys_event_flag_get: eflag_master -> 0x%016llx\n", flags_m);
	printf("sys_event_flag_get: eflag_worker -> 0x%016llx\n", flags_w);
	sys_event_flag_clear(eflag_master, 1ULL);
	sys_event_flag_clear(eflag_worker, 0ULL);
	sys_event_flag_get(eflag_master, &flags_m);
	sys_event_flag_get(eflag_worker, &flags_w);
	printf("sys_event_flag_get: eflag_master -> 0x%016llx\n", flags_m);
	printf("sys_event_flag_get: eflag_worker -> 0x%016llx\n", flags_w);

	// Create 4 threads, trywait the first 2 and wait the 2 others, then cancel them.
	printf("\n[*] Testing sys_event_flag_trywait and sys_event_flag_cancel:\n");
	createThread(eventflagTrywait);
	createThread(eventflagTrywait);
	createThread(eventflagWait);
	createThread(eventflagWait);
	createThreadAndJoin(eventflagCancel);
	
	// Delete flags
	if (sys_event_flag_destroy(eflag_master) || sys_event_flag_destroy(eflag_worker)) {
		printf("[!] Main: Failed when deleting the event flags.\n");
		exit(-1);
	}

	return 0;
}
