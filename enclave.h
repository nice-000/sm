#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"
#include "thread.h"

/* TODO: does not support multithreaded enclave yet */
#define MAX_ENCL_THREADS 1

typedef enum {
  DESTROYED = -2,
  INVALID = -1,
  FRESH = 0,
  INITIALIZED,
  RUNNING,
} enclave_state_t;


struct enclave_t
{
  unsigned int eid; //enclave id
  int rid; //region id
  unsigned long host_satp; //supervisor satp
  unsigned long encl_satp; // enclave's page table base
  enclave_state_t state; // global state of the enclave
  unsigned int n_thread;

  /* execution context */
  unsigned long host_stvec[MAX_HARTS]; // supervisor stvec
  
  /* enclave execution context */
  struct thread_state_t threads[MAX_ENCL_THREADS];
};

unsigned long get_host_satp(unsigned int eid);
enclave_ret_t create_enclave(uintptr_t base, size_t size, unsigned int* eidptr);
enclave_ret_t destroy_enclave(unsigned int eid);

enclave_ret_t run_enclave(uintptr_t* regs, unsigned int eid, uintptr_t entry, unsigned long* retptr);
enclave_ret_t exit_enclave(uintptr_t* regs, unsigned long retval);
enclave_ret_t stop_enclave(uintptr_t* regs, uint64_t request);
enclave_ret_t resume_enclave(uintptr_t* regs, unsigned int eid);

#endif