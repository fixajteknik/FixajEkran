enum UART_BPS_RATE {
  UART_BPS_RATE_1200 = 1200,
  UART_BPS_RATE_2400 = 2400,
  UART_BPS_RATE_4800 = 4800,
  UART_BPS_RATE_9600 = 9600,
  UART_BPS_RATE_19200 = 19200,
  UART_BPS_RATE_38400 = 38400,
  UART_BPS_RATE_57600 = 57600,
  UART_BPS_RATE_115200 = 115200
};
 

typedef unsigned char  BOOLEAN;                                         /* 布尔变量                     */
typedef unsigned char  INT8U;                                           /* 无符号8位整型变量            */
typedef signed   char  INT8S;                                           /* 有符号8位整型变量            */
typedef unsigned short INT16U;                                          /* 无符号16位整型变量           */
typedef signed   short INT16S;                                          /* 有符号16位整型变量           */
typedef unsigned long  INT32U;                                          /* 无符号32位整型变量           */
typedef signed   long  INT32S;                                          /* 有符号32位整型变量           */
typedef float          FP32;                                            /* 单精度浮点数（32位长度）     */
typedef double         FP64;                                            /* 双精度浮点数（64位长度）     */
//typedef unsigned __int64 uint64_t;

typedef unsigned long  uint32;                                          /* 无符号32位整型变量           */
typedef signed   long  int32s;                                          /* 有符号32位整型变量           */
typedef unsigned short uint16;                                          /* 无符号16位整型变量           */
typedef unsigned char  uint8;                                           /* 无符号8位整型变量            */
 

/*********************************************************************************************************
  显示终端坐标定义
*********************************************************************************************************/
#define  HMT_XADDR_MAX       800                                        /* 液晶X轴坐标最大值            */
#define  HMT_YADDR_MAX       480                                        /* 液晶Y轴坐标最大值            */

/*********************************************************************************************************
  内存中图片序号最大值
*********************************************************************************************************/
#define  PAGE_ID_MAX         999                                        /* 图片序号最大值           */
#define  ICON_ID_MAX      	 999 									                      /* ICON图片序号最大值        */
/*********************************************************************************************************
  背光定义
*********************************************************************************************************/
#define  LIGHT_TIME_MAX      256                                        /* 背光时间最大值               */
#define  LIGHT_PWMLEVEL_MAX  0x3F                                       /* 背光时间最大值               */

/*********************************************************************************************************
  蜂鸣器定义
*********************************************************************************************************/
#define  BEEP_LEVEL_MAX      0x3F                                       /* 蜂鸣器等级最大值               */

/*********************************************************************************************************
  其他最大值定义
*********************************************************************************************************/
#define  CMD_HEAD             0xAA
#define  CMD_TAIL             0xCC, 0x33, 0xC3, 0x3C
#define  CMD_SPECIALFUNC      0x55, 0xAA, 0x5A, 0xA5 
#define  PROJECT_MAX          9                                    
#define  BUZZERFRE_MAX        0x32                                   
#define  SETElEMENTTYPE_MAX   1                                       
#define  STRID_MAX            127         
#define  STR_LENGTH_MAX       128  
#define  NUMID_MAX            119                                                                                                                                                  

/*********************************************************************************************************
  显示终端颜色宏定义
*********************************************************************************************************/
#define  BLACK               0x0000                                     /* 黑色：    0,   0,   0        */
#define  NAVY                0x000F                                     /* 深蓝色：  0,   0, 128        */
#define  DGREEN              0x03E0                                     /* 深绿色：  0, 128,   0        */
#define  DCYAN               0x03EF                                     /* 深青色：  0, 128, 128        */
#define  MAROON              0x7800                                     /* 深红色：  128,  0,  0        */
#define  PURPLE              0x780F                                     /* 紫色：    128,  0,128        */
#define  OLIVE               0x7BE0                                     /* 橄榄绿：  128,128,  0        */
#define  LGRAY               0xC618                                     /* 灰白色：  192,192,192        */
#define  DGRAY               0x7BEF                                     /* 深灰色：  128,128,128        */
#define  BLUE                0x001F                                     /* 蓝色：    0,    0,255        */
#define  GREEN               0x07E0                                     /* 绿色：    0,  255,  0        */
#define  CYAN                0x07FF                                     /* 青色：    0,  255,255        */
#define  RED                 0xF800                                     /* 红色：    255,   0, 0        */
#define  MAGENTA             0xF81F                                     /* 品红：    255,   0,255       */
#define  YELLOW              0xFFE0                                     /* 黄色：    255, 255, 0        */
#define  WHITE               0xFFFF                                     /* 白色：    255, 255,255       */

/*********************************************************************************************************
  显示终端字符显示类型
*********************************************************************************************************/
#define  GBK2132_24x24       0                                         /* 24x24 GBK2132内码            */
#define  GBK2132_48x48       1                                         /* 48x48 GBK2132内码            */
#define  NUMBER_120x200      2                                         /* 120x200 0~9数字              */
#define  ASC_6x8             3                                         /* 6x8 ASCII字符                */
#define  ASC_8x8             4                                         /* 8x8 ASCII字符                */
#define  ASC_8x16            5                                         /* 8x16 ASCII字符                */
#define  ASC_ISO_10x16       6                                         /* 10x16 ASCII字符               */
#define  ASC_ISO_BOLD_10x16  7                                         /* 10x16 ASCII字符 ISO+Bold      */
#define  ASC_ISO_17x24       8                                         /* 17x24 ASCII字符               */
#define  ASC_ISO_BOLD_15x24  9                                         /* 15x24 ASCII字符 ISO+Bold      */
#define  ASC_COMIC_17x24    10                                         /* 17x24 ASCII字符  Bold         */
#define  ASC_ISO_20x32      11                                         /* 20x32 ASCII字符  ISO          */
#define  ASC_ISO_BOLD_20x32 12                                         /* 20x32 ASCII字符 ISO+Bold      */
#define  NUMBER_47x48       13                                         /* 47x48 0~9数字  '+','-',':'    */
#define  NUMBER_63x64       14                                         /* 63x64 0~9数字  '+','-',':'    */
#define  NUMBER_79x80       15                                         /* 79x80 0~9数字  '+','-',':'    */

/*********************************************************************************************************
其他宏定义
*********************************************************************************************************/
#define SET_BAUD1200    0x00
#define SET_BAUD2400    0x01
#define SET_BAUD4800    0x02
#define SET_BAUD9600    0x03
#define SET_BAUD19200   0x04
#define SET_BAUD38400   0x05
#define SET_BAUD57600   0x06
#define SET_BAUD115200  0x07
#define SET_NONE_RESPONSE             0x00   
#define SET_PRESS_RESPONSE_POSITION   0x81
#define SET_RELEASE_RESPONSE_POSITION 0x82
#define SET_RESPONSE_KEYID            0x83
#define MAX_NUMARRAY_SIZE             40  
#define VERSION_LENGTH                5  
#define TOPWAY_CONNECT_OK             "Topway HMT Ready"  
#define ELEMENTTYPE_STR               0x00
#define ELEMENTTYPE_NUM               0x01
#define REFRESH_ON                    0x01
#define REFRESH_OFF                   0x00
#define VP_N16                        0x08
#define VP_N32                        0x02
#define VP_N64                        0x03
#define VP_STR                        0x00
#define FALSE                         0
#define TRUE                          1
#define TIMER_INCREASE                0x03 
#define TIMER_REDUSE                  0x01
#define TIMER_TRUN_OFF                0x00
#define BEEP_CTRL_LOOPCOUNT_1         0x01
#define BEEP_CTRL_TIME1_100MS         1
#define BEEP_CTRL_TIME2_0MS           0
#define BEEP_CTRL_FRE1_5KHZ           50
#define BEEP_CTRL_FRE2_0HZ            0

#define QUANTITYOF_ARRAY_MEMBER(ARRAY) sizeof(ARRAY)/sizeof(ARRAY[0]) 

//cmd que

#define  CMD_TAIL_CC            0XCC
#define  CMD_TAIL_33            0X33
#define  CMD_TAIL_C3            0XC3
#define  CMD_TAIL_3C            0X3C
#define  CMD_CHECK_TAIL_CC      0
#define  CMD_CHECK_TAIL_33      1
#define  CMD_CHECK_TAIL_C3      2
#define  CMD_CHECK_TAIL_3C      3
#define  CMD_CHECK_TAIL_OK      4




/*********************************************************************************************************
  返回消息类型 宏定义
*********************************************************************************************************/
#define  RELEASE_RESPONSE_POSITION 0x72
#define  PRESS_RESPONSE_POSITION   0x73
#define  RELEASE_RESPONSE_KEYID    0x78
#define  PRESS_RESPONSE_KEYID      0x79
#define  RESPONSE_VPADDR_VALUE     0x77

#define  HAND_SHAKE                0x30
#define  READ_VERSION              0x31
#define  READ_PAGE_ID              0x32
#define  READ_VP_N16               0x3E
#define  READ_VP_STR               0x43
#define  READ_VP_N32               0x45
#define  READ_VP_N64               0x49
#define  READ_FLASH                0x91
#define  READ_USER_BIN             0x93
#define  READ_RTC                  0x9B

/**********************************************************************************************************/
#define WORD_FROM_BUF(WRD) ((((INT8U *)(WRD))[0]<<8)|((INT8U *)(WRD))[1])  // 
#define DWORD_FROM_BUF(DWRD) (((INT32U)(((INT8U *)(DWRD))[0])<<24)|(((INT32U)((INT8U *)(DWRD))[1])<<16)|(((INT8U *)(DWRD))[2]<<8)|((INT8U *)(DWRD))[3]) 


#define CMD_MAX_BUFFER          128 
#define PARAM_MAX_LENGTH        (CMD_MAX_BUFFER-2)
#define QUEUE_MAX_SIZE          CMD_MAX_BUFFER

 



