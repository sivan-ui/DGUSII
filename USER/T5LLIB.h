

	//���ָ��DGUS������ַ��DATA3����Ļ����˳�
	void wait_ok(unsigned int addr);

	//��дT5LƬ��256KW Flash��mod=0x5A Ϊ��ȡ��mod=0xA5Ϊд��
	//addr=DGUS������ַ��������ż����addr_flash=flash��ȡ��ַ��������ż����len=��ȡ�ֳ��ȣ�������ż����
	void T5L_Flash(unsigned char mod,unsigned int addr,long addr_flash,unsigned int len);


	//��ȡDGUS�������� addr=DGUS������ַ  *buf=���ݷ���ָ��  len=��ȡ�����ֳ���
	void read_dgusii_vp(unsigned int addr,unsigned char *buf,unsigned int len);


	//дDGUS�������� addr=DGUS������ַ  *buf=д������ָ��  len=д�������ֳ���
	void write_dgusii_vp(unsigned int addr,unsigned char *buf,unsigned int len);


	////���Ź����ò�����
	//void wdt_on();


	////���Ź��ر�
	//void wdt_off();

	////���Ź�ι��
	//void wdt_rst();

	//// T5L OS CPU��ʼ��       		           	
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


	//	//I2C д��1���ֽ�
	//	void i2cbw(unsigned char dat);


	//	//i2c ��ȡ1���ֽ�����
	//	unsigned char i2cbr();


	//	//���8130��û�е��磬�������ʼ��8130������ʱ��Ϊ2017.01.01 ����һ 00:00:00
	//	void init_rtc();






