#pragma once

class CSerialPortProcessor
{
public:
	CSerialPortProcessor(void);
	~CSerialPortProcessor(void);

	// �w��|�[�g���I�[�v������M�J�n 
	void Start(LPCSTR comPortName, DCB* portConfig);
	// COM �|�[�g��������I�� 
	void End();

	// ��M�ς݂̃f�[�^�� buffer �Ɋi�[�� buffer �Ɋi�[�����f�[�^�̃o�C�g����Ԃ� 
	DWORD GetReceivedData(LPVOID buffer);

	// buffer �Ɋi�[���ꂽ�f�[�^�� length �o�C�g���M���� 
	DWORD SendData(LPVOID buffer, int toWriteBytes);

private:
	static DWORD WINAPI ThreadFunc(LPVOID lpParametr);
	DWORD WINAPI ReceiveData();

	HANDLE myHComPort;		// COM �|�[�g�p�n���h�� 

	HANDLE myHThread;    // �X���b�h�p�n���h�� 
	DWORD  myThreadId;   // �X���b�h ID 

	HANDLE myMutex;      // myReceivedData,myReceivedDataLength ��r�����邽�߂� Mutex 

	BYTE* myReceivedData;  // ��M�p�����f�[�^ �o�b�t�@ 
	DWORD myReceivedDataLength;  // myReceivedData �Ɋi�[����Ă����M�f�[�^�̃o�C�g�� 
};