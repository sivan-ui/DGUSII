

	//检查指定DGUS变量地址的DATA3清零的话就退出
	void wait_ok(unsigned int addr);

	//读写T5L片内256KW Flash，mod=0x5A 为读取，mod=0xA5为写入
	//addr=DGUS变量地址，必须是偶数；addr_flash=flash读取地址，必须是偶数；len=读取字长度，必须是偶数。
	void T5L_Flash(unsigned char mod,unsigned int addr,long addr_flash,unsigned int len);


	//读取DGUS变量数据 addr=DGUS变量地址  *buf=数据返回指针  len=读取数据字长度
	void read_dgusii_vp(unsigned int addr,unsigned char *buf,unsigned int len);


	//写DGUS变量数据 addr=DGUS变量地址  *buf=写入数据指针  len=写入数据字长度
	void write_dgusii_vp(unsigned int addr,unsigned char *buf,unsigned int len);


	////看门狗配置并开启
	//void wdt_on();


	////看门狗关闭
	//void wdt_off();

	////看门狗喂狗
	//void wdt_rst();

	//// T5L OS CPU初始化       		           	
	//void initcpu();


	//	void SDA_IN();

	//	void SDA_OUT();

	////delay t uS
	//	void delayus(unsigned char t);


	//	void i2cstart();


	//	void i2cstop();

	//	void mack();


	//	void mnak();


	//	void cack();


	//	//I2C 写入1个字节
	//	void i2cbw(unsigned char dat);


	//	//i2c 读取1个字节数据
	//	unsigned char i2cbr();


	//	//检查8130有没有掉电，掉电则初始化8130，设置时间为2017.01.01 星期一 00:00:00
	//	void init_rtc();






