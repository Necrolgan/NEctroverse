#define SERVERINCLUDES
//Edit below this line only, the above line MUST STAY HERE! -- This prevents double calling.

typedef struct
{
  char *data;
  void *next;
  void **prev;
} svBufferDef, *svBufferPtr;


typedef struct
{
// server
  int socket;
  struct sockaddr_in sockaddr;
  void *next;
  void **previous;

  char *recv;
  char recv_buf[SERVER_RECV_BUFSIZE+1];
  int recv_pos;
  int recv_max;
  int time;
  int flags;

  svBufferPtr sendbuf;
  svBufferPtr sendbufpos;
  int sendpos;
  int sendsize;

  svBufferPtr sendflushbuf;
  int sendflushpos;


  char *sendstatic;
  int sendstaticsize;

// ioInterfacePtr
  void *io;

// pointer for the interface to allocate its data
  void *iodata;

// database
  dbUserPtr dbuser;

} svConnectionDef, *svConnectionPtr;


int svTime();
int svInit();
int svListen();
void svEnd();
void svSelect();
void svRecv();

int file_exist (char *filename);
char *trimwhitespace(char *str);
char** str_split(char* str, char delim, int* numSplits );
void dirstructurecheck(char *directory);

void svShutdown( svConnectionPtr cnt );
void svClose( svConnectionPtr cnt );
int svFree( svConnectionPtr cnt );

void svPipeScan(int pipefileid);
int svPipeSend(int pipedirection, char *message);

void svSendEnd( svConnectionPtr cnt );
void svSendInit( svConnectionPtr cnt, int size );
int svSendFlush( svConnectionPtr cnt );
void svSendStaticFlush( svConnectionPtr cnt );


// Functions used by the Output interface
void svSend( svConnectionPtr cnt, void *data, int size );
void svSendString( svConnectionPtr cnt, char *string );
/*
void svSendPrintf( svConnectionPtr cnt, char *string, ... );
*/
__attribute__ ((format (printf, 2, 3))) void svSendPrintf( svConnectionPtr cnt, char *string, ... );

void svSendStatic( svConnectionPtr cnt, void *data, int size );


#define SV_FLAGS_NEED_WRITE (0x1)
#define SV_FLAGS_WRITE_BUFFERED (0x2)
#define SV_FLAGS_WRITE_STATIC (0x4)
#define SV_FLAGS_TO_CLOSE (0x8)
#define SV_FLAGS_WAIT_CLOSE (0x10)
#define SV_FLAGS_TIMEOUT (0x20)


extern int svTickNum;
extern int svTickTime;
extern int svTickStatus;
extern int svTickAutoStart;
extern int svRoundEnd;

extern int svDebugTickPass;
extern int svDebugTickId;


/*

__attribute__ ((format (printf, 1, 2)))

*/


extern int svBanNum;
extern char *svBanList[];

