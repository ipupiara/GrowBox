
#ifndef K8055dHeader
#define K8055dHeader


typedef long (CALLBACK* LONG2LONG)(long);
typedef void (CALLBACK* VOID2VOID)(void);
typedef void (CALLBACK* LONGPLONGP2VOID)(long *, long *);
typedef void (CALLBACK* LONGLONG2VOID)(long, long);
typedef void (CALLBACK* LONG2VOID)(long);
typedef bool (CALLBACK* LONG2BOOL)(long);
typedef long (CALLBACK* VOID2LONG)(void);

class k8055d
{
public:
	void exit();
	k8055d(void);
	~k8055d(void);
	long  init(void);
	long  OpenDevice(long CardAddress);
	void  CloseDevice();
	long  ReadAnalogChannel(long Channel);
	void  ReadAllAnalog(long *Data1, long *Data2);
	void  OutputAnalogChannel(long Channel, long Data);
	void  OutputAllAnalog(long Data1, long Data2);
	void  ClearAnalogChannel(long Channel); 
	void  ClearAllAnalog();
	void  SetAnalogChannel(long Channel); 
	void  SetAllAnalog();
	void  WriteAllDigital(long Data);
	void  ClearDigitalChannel(long Channel);
	void  ClearAllDigital();
	void  SetDigitalChannel(long Channel);
	void  SetAllDigital();
	bool  ReadDigitalChannel(long Channel);
	long  ReadAllDigital();
	long  ReadCounter(long CounterNr);
	void  ResetCounter(long CounterNr);
	void  SetCounterDebounceTime(long CounterNr, long DebounceTime);
	
	bool  dout1;
	bool  dout2;
	bool  dout3;
	bool  dout4;
	bool  dout5;
	bool  dout6;
	bool  dout7;
	bool  dout8;

	bool  din1;
	bool  din2;
	bool  din3;
	bool  din4;
	bool  din5;

	long  da1;
	long  da2;
	long  ad1;
	long  ad2;

	long  ct1;
	long  ct2;
	long  dbct1;
	long  dbct2;

private:
	bool initialized;
	HINSTANCE hDLL;
	LONG2LONG		pOpenDevice;
	VOID2VOID		pCloseDevice;
	LONG2LONG		pReadAnalogChannel;
	LONGPLONGP2VOID pReadAllAnalog;
	LONGLONG2VOID	pOutputAnalogChannel;
	LONGLONG2VOID	pOutputAllAnalog;
	LONG2VOID		pClearAnalogChannel;
	VOID2VOID		pClearAllAnalog;
	LONG2VOID		pSetAnalogChannel;
	VOID2VOID		pSetAllAnalog;
	LONG2VOID		pWriteAllDigital;
	LONG2VOID		pClearDigitalChannel;
	VOID2VOID		pClearAllDigital;
	LONG2VOID		pSetDigitalChannel;
	VOID2VOID		pSetAllDigital;
	LONG2BOOL		pReadDigitalChannel;
	VOID2LONG		pReadAllDigital;
	LONG2LONG		pReadCounter;
	LONG2VOID		pResetCounter;
	LONGLONG2VOID	pSetCounterDebounceTime;
};

#endif
