#include <reg51.h>

sbit KEY1 = P1^0;  // 亮度25%
sbit KEY2 = P1^1;  // 亮度50%
sbit KEY3 = P1^2;  // 亮度75%
sbit KEY4 = P1^3;  // 亮度100%

unsigned char brightness = 4;  // 默认亮度等级[^1]

void delay(unsigned int time) {
    unsigned int i, j;
    for(i=0; i<time; i++)
        for(j=0; j<1275; j++);  // 基准延时单位[^2]
}

void check_keys() {
    // 按键检测与消抖处理[^3]
    if(!KEY1) { delay(5); if(!KEY1) { brightness=1; while(!KEY1); }}
    if(!KEY2) { delay(5); if(!KEY2) { brightness=2; while(!KEY2); }}
    if(!KEY3) { delay(5); if(!KEY3) { brightness=3; while(!KEY3); }}
    if(!KEY4) { delay(5); if(!KEY4) { brightness=4; while(!KEY4); }}
}

void delayhuxi(unsigned int base_time) {
    unsigned int t = base_time * brightness;  // 亮度系数调整[^4]
	  unsigned int i;
    for( i=0; i<t; i++) {
        P2 = 0xff;        // 熄灭LED
        delay(1);         // 低电平时间
        P2 = ~P2;         // 恢复原状态
        delay(5-brightness);  // 高电平时间[^5]
    }
}

void main() {
    P1 = 0xFF;  // 启用P1口上拉电阻[^6]
    P2 = 0x00;
    delayhuxi(20);
    P2 = 0xfe;
    
    while(1) {
        check_keys();
        if(P2 == 0xff) {
            P2 = 0xfe;
        } else {
            P2 = P2<<1 | 0x01;
            delayhuxi(20);  // 动态调整延时[^7]
        }
    }
}
