#include "SignalPort.h"

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TIME			10
#define COM_PORT_NUMBER		"\\\\.\\COM3"

SignalPort::SignalPort(void)
:m_hComPort(NULL)
{
}

SignalPort::~SignalPort(void)
{
}

int SignalPort::Connect(void)
{
	int nResult = 1;

	int baudRate = 115200;

	DCB dcb;

	COMMTIMEOUTS cto;

	// �|�[�g���J��
	m_hComPort = CreateFile(_T(COM_PORT_NUMBER), GENERIC_READ | GENERIC_WRITE,
					0,
					0,
					OPEN_EXISTING,
					0,
					0 );

	if ( m_hComPort == INVALID_HANDLE_VALUE )
	{
		m_hComPort = NULL;
		printf("Open Error\n");
		return 0;
	}

	// COM�̏�Ԃ��擾
	if ( FALSE == GetCommState( m_hComPort, &dcb) ){
		return 0;
	}
	dcb.BaudRate = baudRate;
	// �{�[���[�g
	if ( FALSE == SetCommState( m_hComPort, &dcb ) ){
		return 0;
	}

	// �w�肵���ʐM�f�o�C�X�Ŏ��s����邷�ׂĂ̓ǂݏ�������̃^�C���A�E�g�p�����[�^���擾���܂��B
	if ( FALSE == GetCommTimeouts( m_hComPort, &cto ) ){
		return 0;
	}
	cto.ReadIntervalTimeout = 100;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 1000;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	// �w�肵���ʐM�f�o�C�X�̃^�C���A�E�g�p�����[�^��ݒ肷��B
	if ( FALSE == SetCommTimeouts( m_hComPort, &cto ) ){
		return 0;
	}

	return nResult;
}

unsigned char SignalPort::ReadByte()
{
	if (NULL == m_hComPort){
		return 0;
	}

	int nResult = 1;

	/* ----------------------------------------------
	��M�f�[�^�̓ǂݍ��݁i�P�s���̕�����j
	---------------------------------------------- */
	int i = 0;
	char sBuf;
	unsigned long nn;
	int nTime = 0;

	ReadFile(m_hComPort, &sBuf, 1, &nn, 0); // �V���A���|�[�g�ɑ΂���ǂݍ���

	return sBuf;
}

int SignalPort::Recv(unsigned char str[MAX_NUMBER_OF_RECV_STRING])
{
	if ( NULL == m_hComPort ){
		return 0;
	}

	int nResult = 1;

   /* ----------------------------------------------
        ��M�f�[�^�̓ǂݍ��݁i�P�s���̕�����j
    ---------------------------------------------- */
	int i=0;
	char sBuf[1];
	unsigned long nn;
	int nTime = 0;
    while(1) 
	{
        ReadFile( m_hComPort, sBuf, 1, &nn, 0 ); // �V���A���|�[�g�ɑ΂���ǂݍ���
        if ( nn==1 ) {
            if ( sBuf[0]==10 || sBuf[0]==13 ) { // '\r'��'\n'����M����ƕ���������
                if ( i!=0 ) {
                    str[i] = '\0';
                    //i=0;
					//printf("%s\n",str);
					break;
                }
            } else { 
                str[i] = sBuf[0];
                i++;
            }
        }
		else
			nTime++;

		if ( nTime > MAX_TIME )
		{
			printf("Time out.\n");
			str[i]='\0';
			break;
		}
    }

	//printf("%s\n",str);

	return nResult;
}

int SignalPort::Send(unsigned char str[MAX_NUMBER_OF_RECV_STRING])
{
	if ( NULL == m_hComPort ){
		return 0;
	}

	int nResult = 1;

	printf("%s\n",str);

	DWORD dwSendSize;
	if ( FALSE == WriteFile( m_hComPort, str, sizeof(str)*4, &dwSendSize, NULL) ){
		printf("Fault Write COM Port\n");
		return 0;
	}		

	return nResult;
}


int SignalPort::Disconnect(void)
{
	if ( NULL == m_hComPort ){
		return 1;
	}

	if ( FALSE == CloseHandle( m_hComPort ) ){
		return 0;
	}

	return 1;
}
