#include <stdio.h>
#include <stdlib.h>

#include <sys/timer.h>
#include <sys/sys_time.h>
#include <sys/ppu_thread.h>
#include <sys/synchronization.h>

// Globals
sys_semaphore_t g_sem;

void semWait(uint64_t arg) {
	if (sys_semaphore_wait(g_sem, arg)) {
		printf("[!] sys_semaphore_wait: Something went wrong!\n");
		exit(-1);
	}
	sys_ppu_thread_exit(0);
}

void semWaitCheck(uint64_t arg) {
	uint64_t dt;
	uint64_t t0 = sys_time_get_system_time();
	if (sys_semaphore_wait(g_sem, arg)) {
		printf("[!] sys_semaphore_wait: Something went wrong!\n");
		exit(-1);
	}
	dt = sys_time_get_system_time() - t0;
	if (dt / 1000000 < 1) {
		printf("[!] sys_semaphore_wait: Woke up too early!\n");
		exit(-1);
	}
	printf("sys_semaphore_wait: Thread woke up successfully\n");
	sys_ppu_thread_exit(0);
}

void semTrywait(uint64_t arg) {
	if (sys_semaphore_trywait(g_sem)) {
		printf("[!] sys_semaphore_trywait: Something went wrong!\n");
		exit(-1);
	}
	sys_ppu_thread_exit(0);
}

void semPost(uint64_t arg) {
	sys_timer_sleep(1);
	if (sys_semaphore_post(g_sem, arg) & ~0x8001000A) {
		printf("[!] sys_semaphore_post: Something went wrong!\n");
		exit(-1);
	}
	sys_ppu_thread_exit(0);
}

int createThreadAndJoin(void (*func)(uint64_t), uint64_t arg=0) {
	uint64_t val;
	sys_ppu_thread_t thread;
	std::size_t stackSize = 4096;
	if (sys_ppu_thread_create(&thread, func, arg, 1500, stackSize, SYS_PPU_THREAD_CREATE_JOINABLE, "Worker")) {
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

void testSysSemaphoreErrors()
{
	sys_semaphore_t sem;              // Working semaphore
	sys_semaphore_t sem_f;            // Broken semaphore
	sys_semaphore_attribute_t attr;   // Initialized semaphore attribute
	sys_semaphore_attribute_t attr_u; // Uninitialized semaphore attribute
	sys_semaphore_attribute_initialize(attr);
	sys_semaphore_value_t val;

	// Create the working semaphore
	if (sys_semaphore_create(&sem, &attr, 0, 1)) {
		printf("[!] sys_semaphore_create: Something went wrong!\n");
		exit(-1);
	}

	// Error tests
	printf("[*] Checking sys_semaphore_create errors:\n");
	printf("sys_semaphore_create: max_val < 0 returns: 0x%x\n", sys_semaphore_create(&sem_f, &attr, 0, -1));
	printf("sys_semaphore_create: max_val == 0 returns: 0x%x\n", sys_semaphore_create(&sem_f, &attr, 0, 0));
	printf("sys_semaphore_create: initial_val < 0 returns: 0x%x\n", sys_semaphore_create(&sem_f, &attr, -1, 0));
	printf("sys_semaphore_create: initial_val > max_val returns: 0x%x\n", sys_semaphore_create(&sem_f, &attr, 2, 1));
	printf("sys_semaphore_create: &sem == NULL returns: 0x%x\n", sys_semaphore_create(NULL, &attr, 0, 1));
	printf("sys_semaphore_create: &attr == NULL returns: 0x%x\n", sys_semaphore_create(&sem_f, NULL, 0, 1));
	printf("sys_semaphore_create: uninitialized attr returns: 0x%x\n",  sys_semaphore_create(&sem_f, &attr_u, 0, 1));
	
	// Error tests
	printf("\n[*] Checking sys_semaphore_get_value errors:\n");
	printf("sys_semaphore_get_value: &val == NULL returns: 0x%x\n", sys_semaphore_get_value(sem, NULL));
	printf("sys_semaphore_get_value: failed semaphore id returns: 0x%x\n", sys_semaphore_get_value(sem_f, &val));
	printf("sys_semaphore_get_value: invalid semaphore id returns: 0x%x\n", sys_semaphore_get_value(NULL, &val));

	printf("\n[*] Checking sys_semaphore_wait errors:\n");
	printf("sys_semaphore_wait: short timeout returns: 0x%x\n", sys_semaphore_wait(sem, 1ULL));
	printf("sys_semaphore_wait: failed semaphore id returns: 0x%x\n", sys_semaphore_wait(sem_f, 0ULL));
	printf("sys_semaphore_wait: invalid semaphore id returns: 0x%x\n", sys_semaphore_wait(NULL, 0ULL));

	printf("\n[*] Checking sys_semaphore_trywait errors:\n");
	printf("sys_semaphore_trywait: semaphore value <= 0 returns: 0x%x\n", sys_semaphore_trywait(sem));
	printf("sys_semaphore_trywait: failed semaphore id returns: 0x%x\n", sys_semaphore_trywait(sem_f));
	printf("sys_semaphore_trywait: invalid semaphore id returns: 0x%x\n", sys_semaphore_trywait(NULL));

	printf("\n[*] Checking sys_semaphore_post errors:\n");
	printf("sys_semaphore_post: failed semaphore id returns: 0x%x\n", sys_semaphore_post(sem_f, 0));
	printf("sys_semaphore_post: invalid semaphore id returns: 0x%x\n", sys_semaphore_post(NULL, 0));
	printf("sys_semaphore_post: val < 0 returns: 0x%x\n", sys_semaphore_post(sem, -1));
	printf("sys_semaphore_post: val > max_val returns: 0x%x\n", sys_semaphore_post(sem, 2));
	
	// Destroy the working semaphore
	if (sys_semaphore_destroy(sem)) {
		printf("[!] sys_semaphore_destroy: Something went wrong!\n");
		exit(-1);
	}

	// More error tests
	printf("\n[*] Checking sys_semaphore_destroy errors:\n");
	printf("sys_semaphore_destroy: failed semaphore id returns: 0x%x\n", sys_semaphore_destroy(sem_f));
	printf("sys_semaphore_destroy: invalid semaphore id returns: 0x%x\n", sys_semaphore_destroy(NULL));
	printf("sys_semaphore_destroy: destroyed semaphore id returns: 0x%x\n", sys_semaphore_destroy(sem));
}

int main(void) {
	// Check errors on purpose
	testSysSemaphoreErrors();

	// Create the global semaphore
	sys_semaphore_attribute_t attr;
	sys_semaphore_attribute_initialize(attr);
	if (sys_semaphore_create(&g_sem, &attr, 2, 2)) {
		printf("[!] sys_semaphore_create: Something went wrong!\n");
		exit(-1);
	}
	
	// Decrement semaphore: 2 -> 1 -> 0
	sys_semaphore_value_t val;
	printf("\n[*] Testing sys_semaphore_get_value and sys_semaphore_(try)wait:\n");
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);
	createThreadAndJoin(semWait, 0ULL);
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);
	createThreadAndJoin(semTrywait);
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);

	// Create 3 threads and wake 1 of them after 1 second
	printf("\n[*] Testing sys_semaphore_post and sys_semaphore_wait:\n");
	createThread(semWaitCheck, 0);
	createThread(semWaitCheck, 0);
	createThread(semWaitCheck, 900000000ULL); // 15 min timeout
	createThreadAndJoin(semPost, 1);
	sys_timer_sleep(2);
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);

	// Wake the 2 remaining threads after 1 second
	createThreadAndJoin(semPost, 2);
	sys_timer_sleep(2);
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);

	// Create 1 threads and post 30 to the semaphore after 1 second (it shouldn't wake up)
	createThread(semWaitCheck, 0);
	createThreadAndJoin(semPost, 30);
	sys_timer_sleep(2);
	sys_semaphore_get_value(g_sem, &val);
	printf("sys_semaphore_get_value: 0x%x\n", val);

	return 0;
}
