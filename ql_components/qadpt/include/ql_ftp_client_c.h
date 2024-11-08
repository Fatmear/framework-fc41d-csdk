
#ifndef QL_FTP_CLIENT_C_H
#define QL_FTP_CLIENT_C_H
 
#include <curl/curl.h>
#define MAX_HOSTNAME_SIZE 201
#define MAX_PORT_SIZE 32
#define MAX_LINE_SIZE 512
#define MAX_DIR_ENTRIES 200
#define MAX_PATH_SIZE 256
#define MAX_FILE_PATH_SIZE 256
#define MAX_FILE_NAME_SIZE 256
#define MAX_USERNAME_SIZE 256
#define MAX_PASSWORD_SIZE 256
#define MAX_OWNER_SIZE 64
#define MAX_SYSTEM_SIZE 128

#define STATUS_NEW 0
#define STATUS_WAIT_CONNECT 1
#define STATUS_WAIT_LOGIN 2
#define STATUS_IDLE 3
#define STATUS_WAITING 4
#define STATUS_TRANSFER 5
#define STATUS_ERROR 99

#define MAX_FILE_MDTM_LEN 18

#define QUEC_FTP_TIMEOUT_MIN 20
#define QUEC_FTP_TIMEOUT_MAX 180

#define QUEC_FTP_FILE_TYPE_BINARY 0
#define QUEC_FTP_FILE_TYPE_ASCII  1

#define QUEC_FTP_USE_SERVER_ADDR                0
#define QUEC_FTP_USE_CONTROL_SESSION_ADDR       1
#define QUEC_FTP_PRIOR_USE_CONTROL_SESSION_ADDR 2

#define QUEC_FTP_PORT_MODE 0
#define QUEC_FTP_PASV_MODE 1

typedef enum
{
    DS_FTP_OK = 0,
    DS_FTP_ERROR = 601,
    DS_FTP_WOULDBLOCK = 602,
    DS_FTP_BUSY = 603,
    DS_FTP_ERROR_DNS = 604,     /*failed to parse domain name*/
    DS_FTP_ERROR_NETWORK = 605, /*failed to establish socket or the network is deactivated*/
    DS_FTP_CTRL_CLOSE = 606,    /*the ftp session has been closed by the ftp server*/
    DS_FTP_DATA_CLOSE = 607,    /*the data connection was closed by the ftp server*/
    DS_FTP_BEARER_FAIL = 608,
    DS_FTP_ERROR_TIMEOUT = 609,
    DS_FTP_INVALID_PARAM = 610,
    DS_FTP_FILE_NOT_FOUND = 611,
    DS_FTP_FILE_POINT_ERR = 612,
    DS_FTP_FS_FILE_ERROR = 613,
    DS_FTP_SERVICE_END = 614,
    DS_FTP_DATA_FAILED = 615,
    DS_FTP_CLOSE_CONN = 616,
    DS_FTP_FILE_UNOPER = 617,
    DS_FTP_REQUEST_ABORT = 618,
    DS_FTP_OVER_MEMORY = 619,
    DS_FTP_COMMAND_ERROR = 620,
    DS_FTP_PARAM_ERROR = 621,
    DS_FTP_COMMAND_FAILED = 622,
    DS_FTP_COMMAND_BAD_SEQUENCE = 623,
    DS_FTP_COMMAND_NOT_IMPLENT = 624,
    DS_FTP_ERROR_UNLOGIN = 625,
    DS_FTP_NO_ACCOUNT = 626,
    DS_FTP_REQUEST_FAILED = 627,
    DS_FTP_REQUEST_STOP = 628,
    DS_FTP_FILEREQ_STOP = 629,
    DS_FTP_FILENAME_ERROR = 630,
    DS_FTP_SSL_AUTH_FAIL = 631,
    DS_FTP_INTERFACE_FAILED = 632,
    DS_FTP_SEND_ERROR = 633,
    DS_FTP_RECV_ERROR = 634,
} ftp_client_error_code;

typedef enum
{
    FTP_CLIENT_DEBUG_ENABLE,
    FTP_CLIENT_SIM_CID,
    FTP_CLIENT_SSL_CTX,
    FTP_CLIENT_SSL_ENABLE,
    FTP_CLIENT_TIMEOUT,
    FTP_CLIENT_TRANSFER_TEXT,
    FTP_CLIENT_SKIP_PASV_IP,
    FTP_CLIENT_PORT_MODE,
} ftp_client_cfg_type;

typedef enum
{
    QL_FTP_CLIENT_SSL_TYPE_NONE,    // ftp server
    QL_FTP_CLIENT_SSL_TYPE_IMP,     //  Implicitly encrypted ftps server
    QL_FTP_CLIENT_SSL_TYPE_EXP,     //  encrypted ftps server
    QL_FTP_CLIENT_SSL_TYPE_SSH,     // sftp server
}QL_FTP_CLIENT_SSL_TYPE_E;


typedef struct FTPCONNECTION
{
    CURL *curl;
    char hostname[MAX_HOSTNAME_SIZE];
    char username[MAX_USERNAME_SIZE];
    char password[MAX_PASSWORD_SIZE];
    int timeout;
    int server_port;
    int ssl_enable;
    int transfer;
    int skip_pasv_ip;
    int status;
    int sslCtx;
    int sim_cid;
    int debug_enable;
    int ftp_port; //默认使用的是被动模式，1设置为主动模式
    int last_reply_code;

    char *currentDir;
    char *last_reply_Dir;
    char *server_ap; //FTP服务器的绝对路径
} ql_ftp_context, *ql_ftp_context_ptr;

typedef size_t (*QUEC_FTP_CLIENT_WRITE_CB)(void *ptr, size_t size, size_t nmemb, void *stream);
typedef size_t (*QUEC_FTP_CLIENT_READ_CB)(void *ptr, size_t size, size_t nmemb, void *stream);

ql_ftp_context_ptr ql_ftp_client_new_c(void);

int ql_ftp_client_open_c(ql_ftp_context_ptr ctx_ptr,const char *username, const char *password, const char *hostname, int hostport);

int ql_ftp_client_close_c(ql_ftp_context_ptr ctx_ptr);

int ql_ftp_client_get_c(ql_ftp_context_ptr ctx_ptr, const char *remotefile, QUEC_FTP_CLIENT_WRITE_CB write_cb, void *user_data, unsigned int start_pos);

int ql_ftp_client_put_c(ql_ftp_context_ptr ctx_ptr, const char *remotefile, QUEC_FTP_CLIENT_READ_CB read_cb, void *user_data, unsigned int start_pos);

int ql_ftp_client_delete_c(ql_ftp_context_ptr ctx_ptr, const char *remotefile);

int ql_ftp_client_pwd_c(ql_ftp_context_ptr ctx_ptr, char *path);

int ql_ftp_client_cwd_c(ql_ftp_context_ptr ctx_ptr, const char *pPathName);

int ql_ftp_client_mkd_c(ql_ftp_context_ptr ctx_ptr, const char *pPathName);

int ql_ftp_client_rmd_c(ql_ftp_context_ptr ctx_ptr, const char *pPathName);

int ql_ftp_client_list_c(ql_ftp_context_ptr ctx_ptr, const char *path_or_file, const char *filename, QUEC_FTP_CLIENT_READ_CB readcb, void *userdata);

int ql_ftp_client_setopt_c(void *handle, int options, void *opt_val);

long ql_ftp_client_get_last_error_c(ql_ftp_context_ptr ctx_ptr);

int ql_ftp_client_size_c(ql_ftp_context_ptr ctx_ptr, const char *filename, double *nSize);

int ql_ftp_client_mdtm_c(ql_ftp_context_ptr ctx_ptr, const char *filename, char *time);

int ql_ftp_client_rename_c(ql_ftp_context_ptr ctx_ptr, const char *oldname, const char *newname);

int ql_ftp_client_nlst_c(ql_ftp_context_ptr ctx_ptr, const char *path_or_file, QUEC_FTP_CLIENT_READ_CB readcb, void *userdata);

int ql_ftp_client_mlsd_c(ql_ftp_context_ptr ctx_ptr, const char *path_or_file, QUEC_FTP_CLIENT_READ_CB readcb, void *userdata);

int ql_ftp_get_last_reply_code_c(ql_ftp_context_ptr ctx_ptr);

void ql_ftp_client_cleanup_c(ql_ftp_context_ptr ctx_ptr);

ftp_client_error_code ql_ftp_client_get_error_code(int app_ftp_code);
void ql_ftp_demo_thread_creat(void);

#endif
