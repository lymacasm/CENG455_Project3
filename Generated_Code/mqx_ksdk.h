/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : mqx_ksdk.h
**     Project     : serial_handler
**     Processor   : MK64FN1M0VLL12
**     Component   : MQX_KSDK
**     Version     : Component 1.2.0, Driver 01.00, CPU db: 3.00.000
**     Repository  : KSDK 1.3.0
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-03-24, 15:51, # CodeGen: 11
**     Contents    :
**         _int_default_isr                - void _int_default_isr(void * vector_number);
**         _int_disable                    - void _int_disable();
**         _int_enable                     - void _int_enable();
**         _int_get_default_isr            - INT_ISR_FPTR _int_get_default_isr();
**         _int_get_exception_handler      - INT_EXCEPTION_FPTR _int_get_exception_handler(_mqx_uint vector);
**         _int_get_isr                    - INT_ISR_FPTR _int_get_isr(_mqx_uint vector);
**         _int_get_isr_data               - void* _int_get_isr_data(_mqx_uint vector);
**         _int_get_isr_depth              - _mqx_uint _int_get_isr_depth();
**         _int_install_default_isr        - INT_ISR_FPTR _int_install_default_isr(INT_ISR_FPTR default_isr);
**         _int_install_exception_isr      - INT_ISR_FPTR _int_install_exception_isr();
**         _int_install_isr                - INT_ISR_FPTR _int_install_isr(_mqx_uint vector,INT_ISR_FPTR isr_ptr,void *...
**         _int_install_unexpected_isr     - INT_ISR_FPTR _int_install_unexpected_isr();
**         _int_set_exception_handler      - INT_EXCEPTION_FPTR _int_set_exception_handler(_mqx_uint...
**         _int_set_isr_data               - void* _int_set_isr_data(_mqx_uint vector,void * data);
**         _int_init                       - _mqx_uint _int_init(_mqx_uint first_user_isr_vector_number,_mqx_uint...
**         _mem_alloc                      - void* _mem_alloc(_mem_size size);
**         _mem_alloc_align                - void* _mem_alloc_align(_mem_size size,_mem_size align);
**         _mem_alloc_at                   - void* _mem_alloc_at(_mem_size size,void * addr);
**         _mem_alloc_uncached             - void* _mem_alloc_uncached(_mem_size size);
**         _mem_alloc_align_uncached       - void* _mem_alloc_align_uncached(_mem_size size,_mem_size align);
**         _mem_alloc_from                 - void* _mem_alloc_from(_mem_pool_id pool_id,_mem_size size);
**         _mem_alloc_align_from           - void* _mem_alloc_align_from(_mem_pool_id pool_id,_mem_size size,_mem_size...
**         _mem_alloc_zero                 - void* _mem_alloc_zero(_mem_size size);
**         _mem_alloc_zero_from            - void* _mem_alloc_zero_from(void * pool_id,_mem_size size);
**         _mem_create_pool                - _mem_pool_id _mem_create_pool(void * start,_mem_size size);
**         _mem_extend                     - _mqx_uint _mem_extend(void * start_of_pool,_mem_size size);
**         _mem_extend_pool                - _mqx_uint _mem_extend_pool(_mem_pool_id pool_id,void *...
**         _mem_free_part                  - _mqx_uint _mem_free_part(void * mem_ptr,_mem_size requested_size);
**         _mem_free                       - _mqx_uint _mem_free(void * mem_ptr);
**         _mem_alloc_system_from          - void* _mem_alloc_system_from(_mem_pool_id pool_id,_mem_size size);
**         _mem_alloc_system               - void* _mem_alloc_system(_mem_size size);
**         _mem_alloc_system_align         - void* _mem_alloc_system_align(_mem_size size,_mem_size align);
**         _mem_alloc_system_align_from    - void* _mem_alloc_system_align_from(_mem_pool_id pool_id,_mem_size...
**         _mem_alloc_system_uncached      - void* _mem_alloc_system_uncached(_mem_size size);
**         _mem_get_size                   - _mem_size _mem_get_size(void * mem_ptr);
**         _mem_test_all                   - _mqx_uint _mem_test_all(_mem_pool_id * pool_error_ptr);
**         _mem_test_pool                  - _mqx_uint _mem_test_pool(_mem_pool_id pool_id);
**         _mem_test                       - _mqx_uint _mem_test();
**         _mem_get_highwater              - void* _mem_get_highwater();
**         _mem_get_highwater_pool         - void* _mem_get_highwater_pool(_mem_pool_id pool_id);
**         _mem_get_error                  - void* _mem_get_error();
**         _mem_get_error_pool             - void* _mem_get_error_pool(_mem_pool_id pool_id);
**         _mem_get_system_pool_id         - _mem_pool_id _mem_get_system_pool_id();
**         _mem_get_type                   - _mem_type _mem_get_type(void * mem_ptr);
**         _mem_set_type                   - bool _mem_set_type(void * mem_ptr,_mem_type mem_type);
**         _mem_transfer                   - _mqx_uint _mem_transfer(void * memory_ptr,_task_id source_id,_task_id...
**         _mem_alloc_system_zero_from     - void* _mem_alloc_system_zero_from(_mem_pool_id pool_id,_mem_size size);
**         _mem_alloc_system_zero          - void* _mem_alloc_system_zero(_mem_size size);
**         _mem_alloc_system_zero_uncached - void* _mem_alloc_system_zero_uncached(_mem_size size);
**         _mem_realloc                    - void* _mem_realloc(void * mem_ptr,_mem_size size);
**         _mem_swap_endian                - void _mem_swap_endian(register unsigned char * definition,void * data);
**         _mem_swap_endian_len            - void _mem_swap_endian_len(register unsigned char * definition,void *...
**         _mem_verify                     - _mqx_uint _mem_verify(void * base,void * extent);
**         _mutatr_init                    - _mqx_uint _mutatr_init(register MUTEX_ATTR_STRUCT_PTR attr_ptr);
**         _mutex_create_component         - _mqx_uint _mutex_create_component();
**         _mutatr_destroy                 - _mqx_uint _mutatr_destroy(MUTEX_ATTR_STRUCT_PTR attr_ptr);
**         _mutex_destroy                  - _mqx_uint _mutex_destroy(register MUTEX_STRUCT_PTR mutex_ptr);
**         _mutatr_get_wait_protocol       - _mqx_uint _mutatr_get_wait_protocol(MUTEX_ATTR_STRUCT_PTR...
**         _mutatr_get_priority_ceiling    - _mqx_uint _mutatr_get_priority_ceiling(MUTEX_ATTR_STRUCT_PTR...
**         _mutatr_get_sched_protocol      - _mqx_uint _mutatr_get_sched_protocol(MUTEX_ATTR_STRUCT_PTR...
**         _mutatr_get_spin_limit          - _mqx_uint _mutatr_get_spin_limit(MUTEX_ATTR_STRUCT_PTR attr_ptr,_mqx_uint_ptr...
**         _mutex_init                     - _mqx_uint _mutex_init(register MUTEX_STRUCT_PTR mutex_ptr,register...
**         _mutex_lock                     - _mqx_uint _mutex_lock(register MUTEX_STRUCT_PTR mutex_ptr);
**         _mutatr_set_wait_protocol       - _mqx_uint _mutatr_set_wait_protocol(MUTEX_ATTR_STRUCT_PTR attr_ptr,_mqx_uint...
**         _mutatr_set_priority_ceiling    - _mqx_uint _mutatr_set_priority_ceiling(MUTEX_ATTR_STRUCT_PTR...
**         _mutatr_set_sched_protocol      - _mqx_uint _mutatr_set_sched_protocol(MUTEX_ATTR_STRUCT_PTR attr_ptr,_mqx_uint...
**         _mutatr_set_spin_limit          - _mqx_uint _mutatr_set_spin_limit(MUTEX_ATTR_STRUCT_PTR attr_ptr,_mqx_uint...
**         _mutex_test                     - _mqx_uint _mutex_test(void ** mutex_error_ptr);
**         _mutex_try_lock                 - _mqx_uint _mutex_try_lock(register MUTEX_STRUCT_PTR mutex_ptr);
**         _mutex_unlock                   - _mqx_uint _mutex_unlock(register MUTEX_STRUCT_PTR mutex_ptr);
**         _mutex_set_priority_ceiling     - _mqx_uint _mutex_set_priority_ceiling(MUTEX_STRUCT_PTR mutex_ptr,_mqx_uint...
**         _mutex_get_priority_ceiling     - _mqx_uint _mutex_get_priority_ceiling(MUTEX_STRUCT_PTR...
**         _mutex_get_wait_count           - _mqx_uint _mutex_get_wait_count(register MUTEX_STRUCT_PTR mutex_ptr);
**         _mqx_init_cripple_evaluation    - _mqx_uint _mqx_init_cripple_evaluation(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_tad                   - _mqx_uint _mqx_init_tad(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_internals             - _mqx_uint _mqx_init_internals(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_allocators            - _mqx_uint _mqx_init_allocators(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_interrupt_stack       - _mqx_uint _mqx_init_interrupt_stack(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_lwsem                 - _mqx_uint _mqx_init_lwsem(KERNEL_DATA_STRUCT_PTR kernel_data);
**         _mqx_init_wfi                   - _mqx_uint _mqx_init_wfi(KERNEL_DATA_STRUCT_PTR kernel_data);
**         mqx_init                        - _mqx_uint mqx_init(register const MQX_INITIALIZATION_STRUCT * mqx_init);
**         mqx_start                       - _mqx_uint mqx_start();
**         _mqx                            - _mqx_uint _mqx(register MQX_INITIALIZATION_STRUCT_PTR mqx_init);
**         _mqx_exit                       - void _mqx_exit(_mqx_uint error);
**         _mqx_get_kernel_data            - void* _mqx_get_kernel_data();
**         _mqx_fatal_error                - void _mqx_fatal_error(_mqx_uint error);
**         _mqx_get_counter                - _mqx_uint _mqx_get_counter();
**         _mqx_get_idle_loop_count        - _mqx_uint _mqx_get_idle_loop_count(IDLE_LOOP_STRUCT_PTR loop);
**         _mqx_get_cpu_type               - _mqx_uint _mqx_get_cpu_type();
**         _mqx_set_cpu_type               - void _mqx_set_cpu_type(_mqx_uint cpu_type);
**         _mqx_get_initialization         - MQX_INITIALIZATION_STRUCT_PTR _mqx_get_initialization();
**         _mqx_get_tad_data               - void* _mqx_get_tad_data(void * td);
**         _mqx_set_tad_data               - void _mqx_set_tad_data(void * td,void * tad_data);
**         _mqx_get_exit_handler           - MQX_EXIT_FPTR _mqx_get_exit_handler();
**         _mqx_set_exit_handler           - void _mqx_set_exit_handler(MQX_EXIT_FPTR entry);
**         _mqx_get_io_component_handle    - void* _mqx_get_io_component_handle(_mqx_uint component);
**         _mqx_set_io_component_handle    - void* _mqx_set_io_component_handle(_mqx_uint component,void * handle);
**         _mqx_get_system_task_id         - _task_id _mqx_get_system_task_id();
**         _mqx_link_io_component_handle   - _mqx_uint _mqx_link_io_component_handle(_mqx_uint component,void *...
**         _mqx_unlink_io_component_handle - _mqx_uint _mqx_unlink_io_component_handle(_mqx_uint component,void *...
**         _crt_tls_reference              - void* _crt_tls_reference();
**         _klog_control                   - void _klog_control(uint32_t bit_mask,bool set_bits);
**         _klog_create                    - _mqx_uint _klog_create(_mqx_uint max_size,_mqx_uint flags);
**         _klog_create_at                 - _mqx_uint _klog_create_at(_mqx_uint max_size,_mqx_uint flags,void * where);
**         _klog_disable_logging_task      - void _klog_disable_logging_task(_task_id tid);
**         _klog_enable_logging_task       - void _klog_enable_logging_task(_task_id tid);
**         _klog_display                   - bool _klog_display();
**         _klog_get_task_stack_usage      - _mqx_uint _klog_get_task_stack_usage(_task_id task_id,_mem_size_ptr...
**         _klog_get_interrupt_stack_usage - _mqx_uint _klog_get_interrupt_stack_usage(_mem_size_ptr...
**         _klog_show_stack_usage          - void _klog_show_stack_usage();
**         _klog_log_function              - void _klog_log_function(void * fn);
**         _klog_yield_internal            - void _klog_yield_internal();
**         _lwsem_create                   - _mqx_uint _lwsem_create(LWSEM_STRUCT_PTR sem_ptr,_mqx_int initial_number);
**         _lwsem_create_hidden            - _mqx_uint _lwsem_create_hidden(LWSEM_STRUCT_PTR sem_ptr,_mqx_int...
**         _usr_lwsem_create               - _mqx_uint _usr_lwsem_create(LWSEM_STRUCT_PTR sem_ptr,_mqx_int initial_number);
**         _lwsem_destroy                  - _mqx_uint _lwsem_destroy(LWSEM_STRUCT_PTR sem_ptr);
**         _usr_lwsem_destroy              - _mqx_uint _usr_lwsem_destroy(LWSEM_STRUCT_PTR sem_ptr);
**         _lwsem_is_valid                 - bool _lwsem_is_valid(LWSEM_STRUCT_PTR sem_ptr);
**         _lwsem_poll                     - bool _lwsem_poll(LWSEM_STRUCT_PTR sem_ptr);
**         _usr_lwsem_poll                 - bool _usr_lwsem_poll(LWSEM_STRUCT_PTR sem_ptr);
**         _lwsem_post                     - _mqx_uint _lwsem_post(LWSEM_STRUCT_PTR sem_ptr);
**         _usr_lwsem_post                 - _mqx_uint _usr_lwsem_post(LWSEM_STRUCT_PTR sem_ptr);
**         _lwsem_test                     - _mqx_uint _lwsem_test(void ** lwsem_error_ptr,void ** td_error_ptr);
**         _lwsem_usr_check                - _mqx_uint _lwsem_usr_check(LWSEM_STRUCT_PTR tst_sem_ptr);
**         _lwsem_wait                     - _mqx_uint _lwsem_wait(LWSEM_STRUCT_PTR sem_ptr);
**         _usr_lwsem_wait                 - _mqx_uint _usr_lwsem_wait(LWSEM_STRUCT_PTR sem_ptr);
**         _lwsem_wait_for                 - _mqx_uint _lwsem_wait_for(LWSEM_STRUCT_PTR sem_ptr,MQX_TICK_STRUCT_PTR ticks);
**         _usr_lwsem_wait_for             - _mqx_uint _usr_lwsem_wait_for(LWSEM_STRUCT_PTR sem_ptr,MQX_TICK_STRUCT_PTR...
**         _lwsem_wait_ticks               - _mqx_uint _lwsem_wait_ticks(LWSEM_STRUCT_PTR sem_ptr,_mqx_uint time_in_ticks);
**         _usr_lwsem_wait_ticks           - _mqx_uint _usr_lwsem_wait_ticks(LWSEM_STRUCT_PTR sem_ptr,_mqx_uint...
**         _lwsem_wait_until               - _mqx_uint _lwsem_wait_until(LWSEM_STRUCT_PTR sem_ptr,MQX_TICK_STRUCT_PTR ticks);
**         _usr_lwsem_wait_until           - _mqx_uint _usr_lwsem_wait_until(LWSEM_STRUCT_PTR sem_ptr,MQX_TICK_STRUCT_PTR...
**         _lwevent_create                 - _mqx_uint _lwevent_create(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint flags);
**         _usr_lwevent_create             - _mqx_uint _usr_lwevent_create(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint flags);
**         _lwevent_destroy                - _mqx_uint _lwevent_destroy(LWEVENT_STRUCT_PTR event_ptr);
**         _usr_lwevent_destroy            - _mqx_uint _usr_lwevent_destroy(LWEVENT_STRUCT_PTR event_ptr);
**         _lwevent_clear                  - _mqx_uint _lwevent_clear(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint bit_mask);
**         _usr_lwevent_clear              - _mqx_uint _usr_lwevent_clear(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint bit_mask);
**         _lwevent_get_signalled          - _mqx_uint _lwevent_get_signalled();
**         _usr_lwevent_get_signalled      - _mqx_uint _usr_lwevent_get_signalled();
**         _lwevent_set_auto_clear         - _mqx_uint _lwevent_set_auto_clear(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _usr_lwevent_set_auto_clear     - _mqx_uint _usr_lwevent_set_auto_clear(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _lwevent_set                    - _mqx_uint _lwevent_set(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint bit_mask);
**         _usr_lwevent_set                - _mqx_uint _usr_lwevent_set(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint bit_mask);
**         _lwevent_test                   - _mqx_uint _lwevent_test(void ** event_error_ptr,void ** td_error_ptr);
**         _lwevent_usr_check              - _mqx_uint _lwevent_usr_check(LWEVENT_STRUCT_PTR tst_event_ptr);
**         _lwevent_wait_for               - _mqx_uint _lwevent_wait_for(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _usr_lwevent_wait_for           - _mqx_uint _usr_lwevent_wait_for(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _lwevent_wait_until             - _mqx_uint _lwevent_wait_until(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _usr_lwevent_wait_until         - _mqx_uint _usr_lwevent_wait_until(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _lwevent_wait_ticks             - _mqx_uint _lwevent_wait_ticks(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _usr_lwevent_wait_ticks         - _mqx_uint _usr_lwevent_wait_ticks(LWEVENT_STRUCT_PTR event_ptr,_mqx_uint...
**         _lwlog_create_component         - _mqx_uint _lwlog_create_component();
**         _lwlog_create_at                - _mqx_uint _lwlog_create_at(_mqx_uint log_number,_mqx_uint max_size,_mqx_uint...
**         _lwlog_create                   - _mqx_uint _lwlog_create(_mqx_uint log_number,_mqx_uint max_size,_mqx_uint...
**         _lwlog_destroy                  - _mqx_uint _lwlog_destroy(_mqx_uint log_number);
**         _lwlog_disable                  - _mqx_uint _lwlog_disable(_mqx_uint log_number);
**         _lwlog_enable                   - _mqx_uint _lwlog_enable(_mqx_uint log_number);
**         _lwlog_read                     - _mqx_uint _lwlog_read(_mqx_uint log_number,_mqx_uint...
**         _lwlog_reset                    - _mqx_uint _lwlog_reset(_mqx_uint log_number);
**         _lwlog_calculate_size           - _mem_size _lwlog_calculate_size(_mqx_uint entries);
**         _lwlog_test                     - _mqx_uint _lwlog_test(_mqx_uint * log_error_ptr);
**         _lwlog_write                    - _mqx_uint _lwlog_write(_mqx_uint log_number,_mqx_max_type p1,_mqx_max_type...
**         _lwmsgq_deinit_internal         - _mqx_uint _lwmsgq_deinit_internal(void * location,bool user);
**         _lwmsgq_init                    - _mqx_uint _lwmsgq_init(void * location,_mqx_uint num_messages,_mqx_uint...
**         _usr_lwmsgq_init                - _mqx_uint _usr_lwmsgq_init(void * location,_mqx_uint num_messages,_mqx_uint...
**         _lwmsgq_usr_check               - _mqx_uint _lwmsgq_usr_check(void * tst_lwmsgq_ptr);
**         _lwmsgq_deinit                  - _mqx_uint _lwmsgq_deinit(void * location);
**         _usr_lwmsgq_deinit              - _mqx_uint _usr_lwmsgq_deinit(void * location);
**         _lwmsgq_receive                 - _mqx_uint _lwmsgq_receive(void * handle,_mqx_max_type_ptr message,_mqx_uint...
**         _usr_lwmsgq_receive             - _mqx_uint _usr_lwmsgq_receive(void * handle,_mqx_max_type_ptr...
**         _lwmsgq_send                    - _mqx_uint _lwmsgq_send(void * handle,_mqx_max_type_ptr message,_mqx_uint flags);
**         _usr_lwmsgq_send                - _mqx_uint _usr_lwmsgq_send(void * handle,_mqx_max_type_ptr message,_mqx_uint...
**         _lwtimer_create_periodic_queue  - _mqx_uint _lwtimer_create_periodic_queue(LWTIMER_PERIOD_STRUCT_PTR...
**         _lwtimer_add_timer_to_queue     - _mqx_uint _lwtimer_add_timer_to_queue(LWTIMER_PERIOD_STRUCT_PTR...
**         _lwtimer_cancel_timer           - _mqx_uint _lwtimer_cancel_timer(LWTIMER_STRUCT_PTR timer_ptr);
**         _lwtimer_cancel_period          - _mqx_uint _lwtimer_cancel_period(LWTIMER_PERIOD_STRUCT_PTR period_ptr);
**         _lwtimer_test                   - _mqx_uint _lwtimer_test(void ** period_error_ptr,void ** timer_error_ptr);
**         _sched_get_max_priority         - _mqx_uint _sched_get_max_priority(_mqx_uint policy);
**         _sched_get_min_priority         - _mqx_uint _sched_get_min_priority(_mqx_uint policy);
**         _sched_set_policy               - _mqx_uint _sched_set_policy(_task_id task_id,_mqx_uint policy);
**         _sched_get_policy               - _mqx_uint _sched_get_policy(_task_id task_id,_mqx_uint_ptr policy_ptr);
**         _sched_get_rr_interval          - uint32_t _sched_get_rr_interval(_task_id task_id,uint32_t * ms_ptr);
**         _sched_get_rr_interval_ticks    - _mqx_uint _sched_get_rr_interval_ticks(_task_id task_id,MQX_TICK_STRUCT_PTR...
**         _sched_set_rr_interval          - uint32_t _sched_set_rr_interval(_task_id task_id,uint32_t rr_interval);
**         _sched_set_rr_interval_ticks    - _mqx_uint _sched_set_rr_interval_ticks(_task_id task_id,MQX_TICK_STRUCT_PTR...
**         _sched_yield                    - void _sched_yield();
**         _task_abort                     - _mqx_uint _task_abort(_task_id task_id);
**         _usr_task_abort                 - _mqx_uint _usr_task_abort(_task_id task_id);
**         _task_create                    - _task_id _task_create(_processor_number processor_number,_mqx_uint...
**         _usr_task_create                - _task_id _usr_task_create(_processor_number processor_number,_mqx_uint...
**         _task_create_blocked            - _task_id _task_create_blocked(_processor_number processor_number,_mqx_uint...
**         _task_create_at                 - _task_id _task_create_at(_processor_number processor_number,_mqx_uint...
**         _task_destroy                   - _mqx_uint _task_destroy(_task_id task_id);
**         _usr_task_destroy               - _mqx_uint _usr_task_destroy(_task_id task_id);
**         _task_enable_fp                 - bool _task_enable_fp();
**         _task_disable_fp                - void _task_disable_fp();
**         _task_set_environment           - void* _task_set_environment(_task_id task_id,void * environment_ptr);
**         _task_get_environment           - void* _task_get_environment(_task_id task_id);
**         _task_get_exception_handler     - TASK_EXCEPTION_FPTR _task_get_exception_handler(_task_id task_id);
**         _task_set_exception_handler     - TASK_EXCEPTION_FPTR _task_set_exception_handler(_task_id...
**         _task_get_exit_handler          - TASK_EXIT_FPTR _task_get_exit_handler(_task_id task_id);
**         _task_set_exit_handler          - TASK_EXIT_FPTR _task_set_exit_handler(_task_id task_id,TASK_EXIT_FPTR...
**         _task_get_id_from_name          - _task_id _task_get_id_from_name(char * name_ptr);
**         _task_get_template_ptr          - TASK_TEMPLATE_STRUCT_PTR _task_get_template_ptr(_task_id task_id);
**         _task_get_template_index        - _mqx_uint _task_get_template_index(char * name_ptr);
**         _task_get_id_from_td            - _task_id _task_get_id_from_td(void * td_ptr);
**         _task_get_index_from_id         - _mqx_uint _task_get_index_from_id(_task_id task_id);
**         _task_get_parameter             - uint32_t _task_get_parameter();
**         _task_get_parameter_for         - uint32_t _task_get_parameter_for(_task_id tid);
**         _task_set_parameter             - uint32_t _task_set_parameter(uint32_t new_value);
**         _task_set_parameter_for         - uint32_t _task_set_parameter_for(uint32_t new_value,_task_id tid);
**         _task_get_priority              - _mqx_uint _task_get_priority(_task_id task_id,_mqx_uint_ptr priority_ptr);
**         _task_set_priority              - _mqx_uint _task_set_priority(_task_id task_id,_mqx_uint...
**         _task_ready                     - void _task_ready(void * td);
**         _usr_task_ready                 - void _usr_task_ready(void * td);
**         _task_restart_func              - static _mqx_uint _task_restart_func(_task_id task_id,uint32_t *...
**         _task_restart                   - _mqx_uint _task_restart(_task_id task_id,uint32_t * param_ptr,bool blocked);
**         _task_stop_preemption           - void _task_stop_preemption();
**         _task_start_preemption          - void _task_start_preemption();
**         _task_allow_preemption          - void _task_allow_preemption();
**         _task_check_stack               - bool _task_check_stack();
**         _taskq_create                   - void* _taskq_create(_mqx_uint policy);
**         _taskq_destroy                  - _mqx_uint _taskq_destroy(void * users_task_queue_ptr);
**         _taskq_suspend                  - _mqx_uint _taskq_suspend(void * users_task_queue_ptr);
**         _taskq_resume                   - _mqx_uint _taskq_resume(void * users_task_queue_ptr,bool all_tasks);
**         _taskq_test                     - _mqx_uint _taskq_test(void ** task_queue_error_ptr,void ** td_error_ptr);
**         _taskq_suspend_task             - _mqx_uint _taskq_suspend_task(_task_id task_id,void * users_task_queue_ptr);
**         _taskq_get_value                - _mqx_uint _taskq_get_value(void * users_task_queue_ptr);
**         normalize                       - static bool normalize(int16_t *,int16_t,int16_t *);
**         _time_add_day_to_ticks          - MQX_TICK_STRUCT_PTR _time_add_day_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_hour_to_ticks         - MQX_TICK_STRUCT_PTR _time_add_hour_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_min_to_ticks          - MQX_TICK_STRUCT_PTR _time_add_min_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_msec_to_ticks         - MQX_TICK_STRUCT_PTR _time_add_msec_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_nsec_to_ticks         - MQX_TICK_STRUCT_PTR _time_add_nsec_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_psec_to_ticks         - MQX_TICK_STRUCT_PTR _time_add_psec_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_sec_to_ticks          - MQX_TICK_STRUCT_PTR _time_add_sec_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_add_usec_to_ticks         - MQX_TICK_STRUCT_PTR _time_add_usec_to_ticks(MQX_TICK_STRUCT_PTR...
**         _time_delay                     - void _time_delay(register uint32_t milliseconds);
**         _usr_time_delay                 - void _usr_time_delay(register uint32_t milliseconds);
**         _time_delay_for                 - void _time_delay_for(register MQX_TICK_STRUCT_PTR ticks);
**         _time_delay_ticks               - void _time_delay_ticks(register _mqx_uint time_in_ticks);
**         _usr_time_delay_ticks           - void _usr_time_delay_ticks(register _mqx_uint time_in_ticks);
**         _time_delay_until               - void _time_delay_until(register MQX_TICK_STRUCT_PTR ticks);
**         _time_diff_days                 - int32_t _time_diff_days(MQX_TICK_STRUCT_PTR end_tick_ptr,MQX_TICK_STRUCT_PTR...
**         _time_diff                      - void _time_diff(TIME_STRUCT_PTR start_time_ptr,TIME_STRUCT_PTR...
**         _time_diff_ticks                - _mqx_uint _time_diff_ticks(MQX_TICK_STRUCT_PTR...
**         _time_diff_ticks_int32          - int32_t _time_diff_ticks_int32(MQX_TICK_STRUCT_PTR...
**         _time_diff_hours                - int32_t _time_diff_hours(MQX_TICK_STRUCT_PTR end_tick_ptr,MQX_TICK_STRUCT_PTR...
**         _time_diff_minutes              - int32_t _time_diff_minutes(MQX_TICK_STRUCT_PTR...
**         _time_diff_milliseconds         - int32_t _time_diff_milliseconds(MQX_TICK_STRUCT_PTR...
**         _time_diff_nanoseconds          - int32_t _time_diff_nanoseconds(MQX_TICK_STRUCT_PTR...
**         _time_diff_picoseconds          - int32_t _time_diff_picoseconds(MQX_TICK_STRUCT_PTR...
**         _time_diff_seconds              - int32_t _time_diff_seconds(MQX_TICK_STRUCT_PTR...
**         _time_diff_microseconds         - int32_t _time_diff_microseconds(MQX_TICK_STRUCT_PTR...
**         _time_dequeue                   - void _time_dequeue(_task_id tid);
**         _time_dequeue_td                - void _time_dequeue_td(void * td);
**         _time_get_elapsed               - void _time_get_elapsed(TIME_STRUCT_PTR ts_ptr);
**         _time_get_elapsed_ticks         - void _time_get_elapsed_ticks(MQX_TICK_STRUCT_PTR tick_ptr);
**         _usr_time_get_elapsed_ticks     - void _usr_time_get_elapsed_ticks(MQX_TICK_STRUCT_PTR tick_ptr);
**         _time_get_elapsed_ticks_fast    - void _time_get_elapsed_ticks_fast(MQX_TICK_STRUCT_PTR tick_ptr);
**         _time_from_date                 - bool _time_from_date(DATE_STRUCT_PTR date_ptr,TIME_STRUCT_PTR ts_ptr);
**         _time_get                       - void _time_get(register TIME_STRUCT_PTR ts_ptr);
**         _time_get_hwticks               - uint32_t _time_get_hwticks();
**         _time_get_nanoseconds           - uint32_t _time_get_nanoseconds();
**         _time_get_ticks_per_sec         - _mqx_uint _time_get_ticks_per_sec();
**         _time_get_resolution            - _mqx_uint _time_get_resolution();
**         _time_get_ticks                 - void _time_get_ticks(register MQX_TICK_STRUCT_PTR tick_ptr);
**         _time_get_microseconds          - uint16_t _time_get_microseconds();
**         _time_get_hwticks_per_tick      - uint32_t _time_get_hwticks_per_tick();
**         _time_init_ticks                - _mqx_uint _time_init_ticks(MQX_TICK_STRUCT_PTR tick_ptr,_mqx_uint ticks);
**         _time_notify_kernel             - void _time_notify_kernel();
**         _time_set                       - void _time_set(register TIME_STRUCT_PTR ts_ptr);
**         _time_set_ticks                 - void _time_set_ticks(register MQX_TICK_STRUCT_PTR ticks);
**         _time_check_if_leap             - bool _time_check_if_leap(uint16_t year);
**         _time_set_resolution            - _mqx_uint _time_set_resolution(_mqx_uint resolution);
**         _time_set_ticks_per_sec         - void _time_set_ticks_per_sec(_mqx_uint ticks_per_sec);
**         _time_set_timer_vector          - void _time_set_timer_vector(_mqx_uint vector);
**         _time_set_hwtick_function       - void _time_set_hwtick_function(MQX_GET_HWTICKS_FPTR hwtick_function_ptr,void...
**         _time_set_hwticks_per_tick      - void _time_set_hwticks_per_tick(uint32_t new_val);
**         _time_to_date                   - bool _time_to_date(TIME_STRUCT_PTR ts_ptr,DATE_STRUCT_PTR date_ptr);
**         _time_to_ticks                  - bool _time_to_ticks(TIME_STRUCT_PTR ts_ptr,MQX_TICK_STRUCT_PTR tick_ptr);
**         _ticks_to_time                  - bool _ticks_to_time(MQX_TICK_STRUCT_PTR tick_ptr,TIME_STRUCT_PTR ts_ptr);
**         mktime                          - time_t mktime(struct tm * tm_ptr);
**         gmtime_r                        - struct tm* gmtime_r(const time_t * timep,struct tm * result);
**         timegm                          - time_t timegm(struct tm * tm_ptr);
**         localtime_r                     - struct tm* localtime_r(const time_t * timep,struct tm * result);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file mqx_ksdk.h
** @version 01.00
*/         
/*!
**  @addtogroup mqx_ksdk_module mqx_ksdk module documentation
**  @{
*/         

#ifndef __mqx_ksdk_H
#define __mqx_ksdk_H

/* MODULE mqx_ksdk. */

/* Include inherited beans */
#include "Cpu.h"
#include "mqx.h"
#include "fsl_os_abstraction.h"
  
typedef task_param_t os_task_param_t;

#define PEX_USE_RTOS

/* Type of RTOS used */
#define PEX_RTOS_MQX_TYPE

/* MQX task IDs */
#define MAINTASK_TASK        1U
#define HANDLERTASK_TASK     2U
#define USERTASK_TASK        3U
#define SCHEDULERTASK_TASK   4U
#define PERIODICTASKGEN_TASK 5U
#define PERIODICTASK_TASK    6U

extern const MQX_INITIALIZATION_STRUCT MQX_init_struct;
#define PEX_RTOS_START() _mqx( (MQX_INITIALIZATION_STRUCT_PTR) &MQX_init_struct );

/* END mqx_ksdk. */

#endif 
/* ifndef __mqx_ksdk_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

