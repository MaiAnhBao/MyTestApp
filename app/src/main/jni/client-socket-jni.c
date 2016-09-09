#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Android log function wrappers
static const char* kTAG = "hello-jni";
#define LOGI(...)   ((void)__android_log_print(ANDROID_LOG_INFO, kTAG, __VA_ARGS__))
#define LOGW(...)   ((void)__android_log_print(ANDROID_LOG_WARN, kTAG, __VA_ARGS__))
#define LOGE(...)   ((void)__android_log_print(ANDROID_LOG_ERROR, kTAG, __VA_ARGS__))

JNIEXPORT jstring JNICALL
Java_com_example_hnnguyen_myapplication_MainActivity_getMsgFromJni(JNIEnv *env, jobject instance) {
    return (*env)->NewStringUTF(env, "Hello From Jni");
}

void error(char *msg)
{
  perror(msg);
  exit(0);
}

struct  hostent
{
  char    *h_name;        /* official name of host */
  char    **h_aliases;    /* alias list */
  int     h_addrtype;     /* host address type */
  int     h_length;       /* length of address */
  char    **h_addr_list;  /* list of addresses from name server */
  #define h_addr  h_addr_list[0]  /* address, for backward compatiblity */
};