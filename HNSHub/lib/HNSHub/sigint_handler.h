#ifndef SIGINT_HANDLER_H_
#define SIGINT_HANDLER_H_
#ifdef __linux__

#ifdef __cplusplus
extern "C" {
#endif


void sigint_handler_init();
void sigint_handler(int sig);

#ifdef __cplusplus
}
#endif
#endif/*__linux__*/
#endif /*SIGINT_HANDLER_H_*/
