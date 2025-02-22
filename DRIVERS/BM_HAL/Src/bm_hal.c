/*
 * bm_hal.c
 *
 *  Created on: 16. tra 2020.
 *      Author: branimir
 */

#include "bm_hal.h"
#include "bm_hal_gpio.h"
#include "bm_hal_adc.h"
#include "bm_hal_can.h"
#ifdef VIRTUAL_MCU
#include <time.h>
#include <stdlib.h>
#endif


SYSTEM_STATE_E systemState = SYSTEM_STATE_INIT;

#ifdef STM32F103xB

static void SystemClock_Config(void);

#endif

#ifdef VIRTUAL_MCU

static DWORD WINAPI serverListenThread( LPVOID lpParam );
static DWORD WINAPI serverTalkThread( LPVOID lpParam );
static SOCKET acceptNewConnection(SOCKET listen_socket);
static void handleSocketRead(SOCKET socket_descriptor);


char rxBuff[1024] =
{ 0 };
char canMessage[64] =
{ 0 };
char req_buff[10] =
{ 0 };

WSADATA wsaData;

SOCKET listenSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;
SOCKET maxSocket    = INVALID_SOCKET;

struct addrinfo *info = NULL;
struct addrinfo hints;

int iResult;
int opt = 1;
int valread;
int addrlen;
struct sockaddr_in server;
struct sockaddr_in client;

fd_set readfds;
fd_set activefds;

HANDLE  hThread[2];
DWORD   dwThreadId[2];
#endif


void BM_HAL_init()
{
#ifdef STM32F103xB
	HAL_Init();
	SystemClock_Config();
#elif defined(VIRTUAL_MCU)
    // Initialize Winsock stack
    printf("(info) Initialising Winsock...\n");
    fflush(stdout);
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("(error) WSAStartup failed with error: %d\n", iResult);
        fflush(stdout);
        return;
    }
    printf("(info) Initialised\n");
    fflush(stdout);

    // info about socket and protocol
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    /* getaddrinfo()
     * Resolve the server address (DNS) and port
     *  return a pointer to a linked list of addrinfo structs (info)
     *
     *  addrinfo: holds information about protocol being used,
     *  socket type, IPv4 or IPv6, ip address, etc...
     */
    getaddrinfo(NULL, DEFAULT_PORT_STR, &hints, &info);

    //create a Listen socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("Error in initializing Listen socket! (%d)\n",
                WSAGetLastError());
        fflush(stdout);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("(info) Listen socket created\n");
    fflush(stdout);

    //set listen socket to allow multiple connections , this is just a good habit, it will work without this
    iResult = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*) &opt, sizeof(opt));
    if (iResult < 0)
    {
        printf("setsockopt error\n");
        fflush(stdout);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    //type of socket created
    ZeroMemory(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(DEFAULT_PORT);

    //bind the socket to localhost port 8080
    iResult = bind(listenSocket, (struct sockaddr*) &server, sizeof(server));
    if (iResult < 0)
    {
        printf("(error) bind failed with error: %d\n", WSAGetLastError());
        fflush(stdout);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    printf("(info) Listen socket bound to ip: localhost (127.0.0.1), port: %d\n", DEFAULT_PORT);

    if (listen(listenSocket, 3))
    {
        printf("(error) listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    printf("I'm listening on port %d\n", DEFAULT_PORT);
    fflush(stdout);

    addrlen = sizeof(client);
    puts("Waiting for connections...\n");
    fflush(stdout);

    /* Use the FD_ZERO routine to initialize the data structures
     * (i.e., readfds, writefds, and exceptfds) */
    FD_ZERO(&readfds);
    /* Use FD_SET to add socket handles for reading to readfds */
    FD_SET(listenSocket, &readfds);
    maxSocket = listenSocket;

    //pData = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));


    hThread[0] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            serverListenThread,       // thread function name
            NULL,                  // argument to thread function
            0,                      // use default creation flags
            &dwThreadId[0]);           // returns the thread identifier

    hThread[1] = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            serverTalkThread,       // thread function name
            NULL,                  // argument to thread function
            0,                      // use default creation flags
            &dwThreadId[1]);           // returns the thread identifier

        if (NULL == hThread[0] || NULL == hThread[1])
        {
           ExitProcess(3);
        }
#endif
}


#ifdef STM32F103xB
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
     while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_1)
     {
     }
     LL_RCC_HSI_SetCalibTrimming(16);
     LL_RCC_HSI_Enable();

      /* Wait till HSI is ready */
     while(LL_RCC_HSI_IsReady() != 1)
     {

     }
     LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_9);
     LL_RCC_PLL_Enable();

      /* Wait till PLL is ready */
     while(LL_RCC_PLL_IsReady() != 1)
     {

     }
     LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
     LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
     LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
     LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

      /* Wait till System clock is ready */
     while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
     {

     }
     LL_SetSystemCoreClock(36000000);

      /* Update the time base */
     if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
     {
       Error_Handler();
     }
     LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_4);
}
#endif


#ifdef VIRTUAL_MCU
static DWORD WINAPI serverListenThread( LPVOID lpParam )
{
    SOCKET sd = 0; // socket descriptor
    SOCKET newSocket = INVALID_SOCKET;

    while (1)
    {
        activefds = readfds;

        if (select(FD_SETSIZE, &activefds, NULL, NULL, NULL) < 0)
        {
            printf("select error!\n");
            exit(EXIT_FAILURE);
        }

        for(sd = 0; sd <= maxSocket; sd++)
        {
            // i from readfds will be set if there is something to read on that socket
            if (FD_ISSET(sd, &activefds))
            {
                if(sd == listenSocket)
                {
                    // listenSocket is active that means there is a new socket waiting for connection
                    newSocket = acceptNewConnection(listenSocket);
                    FD_SET(newSocket, &readfds);
                    printf("New connection, new socket descriptor is: %d, ip is: %s, port: %d\n",
                            newSocket, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                    fflush(stdout);

                    maxSocket = (newSocket > maxSocket) ? newSocket : maxSocket;
                }
                else
                {
                    // some other socket is activated, and it needs to be handled
                    handleSocketRead(sd);
                }
            }
        }
    }
    return 0;
}

static DWORD WINAPI serverTalkThread( LPVOID lpParam )
{
    int result = 0;
    SOCKET sd = 0; // socket descriptor
    char send_message[256] = {0};

    while(1)
    {
        if(hcan.sendMsgFlag && readfds.fd_count > 1)
        {
            hcan.sendMsgFlag = 0;
            sprintf(send_message, "0x125: 0x%x 0x%x 0x%x 0x%x 0x%x\0", CAN_tx_data[0],
                    CAN_tx_data[1], CAN_tx_data[2], CAN_tx_data[3], CAN_tx_data[4]);
            // readfds.fd_array[0] is listen socket
            printf("< sending: %s\n", send_message);
            fflush(stdout);
            for(int j=1; j < readfds.fd_count; j++)
            {
                sd = readfds.fd_array[j];
                result = send(sd, send_message, (int) strlen(send_message)+1, 0);
                if (result == SOCKET_ERROR)
                {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    fflush(stdout);
                    closesocket(sd);
                    WSACleanup();
                }

            }
        }
    }
    return 0;
}

static SOCKET acceptNewConnection(SOCKET listen_socket)
{
    SOCKET new_socket = INVALID_SOCKET;
    // accept the connection
    new_socket = accept(listen_socket, (struct sockaddr*) &client, &addrlen);
    if (new_socket < 0)
    {
        printf("accept error\n");
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

static void handleSocketRead(SOCKET socket_descriptor)
{
    static uint8_t error_counter = 0u;
    char* strPtr;
    int index = 0;
    char adcVal[4] = {0};

    valread = recv(socket_descriptor, rxBuff, sizeof(rxBuff), 0);
    if (0 > valread)
    {
        error_counter++;
        if(error_counter > 10)
        {
            error_counter = 0u;
            FD_CLR(socket_descriptor, &readfds);
        }
        printf("Error in receiving data %d\n", WSAGetLastError());
        fflush(stdout);
    }
    else if (0 == valread)
    {
        getpeername(socket_descriptor, info->ai_addr, (int*)&(info->ai_addrlen));
        printf("Host disconnected\n");
        fflush(stdout);
        FD_CLR(socket_descriptor, &readfds);
    }
    else
    {
        printf("> received: %s\n", rxBuff);
        fflush(stdout);
        /*
         * check if received message is for example "SET INPUT0 HIGH"
         * and then set the correct variable (in inputPort) to that value
         */
        strPtr = strstr(rxBuff, "SET INPUT");
        if (strPtr != NULL)
        {
            strPtr += 9;                           // expecting pin number at this place in message (0 or 1)
            strncpy((char*)&index, strPtr, 1);     // copy that number (char) to index
            index -= 48;                           // get a number from that ascii char
            if(0 <= index && index <= 3)
            {
                if(strstr(rxBuff, "HIGH"))
                {
                    inputPort[index] = BM_GPIO_HIGH;
                }
                else
                {
                    inputPort[index] = BM_GPIO_LOW;
                }
            }
        }
        /*
         * ADC messages handling
         * e.g. SET ADC0 1024
         */
        strPtr = strstr(rxBuff, "SET ADC");
        if (strPtr != NULL)
        {
            for(int i=0; *((strPtr+9)+i) != '\0';i++)
            {
                adcVal[i] = *((strPtr+9)+i);
            }
            if(*(strPtr+7) == '0')
            {
                adcValue0 = atoi(adcVal);
            }
            else
            {
                adcValue1 = atoi(adcVal);
            }
        }
        else
        {
        }
    }
}
#endif

//int stringToHex(char *inputString)
//{
//    int i;
//    int result = 0;
//    int n = strlen(inputString)-2;  //how many digits w/o the 1st two (0x)
//    int exp=n;
//    ERROR_E error = ERROR_OK;
//
//    for (i = 0; i < n; i++)
//    {
//        if(error == ERROR_OK){
//            result += hexCharToDec(inputString[i+2], &error) * (int)pow(16, exp-1);
//            exp--;
//        }
//        else{
//            result = -1;
//            printf("(error) Unable to decode input string, wrong hex values!\n");
//            break;
//        }
//    }
//    return result;
//}
//
//int hexCharToDec(char c, ERROR_E *error)
//{
//    int result;
//
//    if ((c == '0') || (c == '1') || (c == '2') || (c == '3') || (c == '4')
//            || (c == '5') || (c == '6') || (c == '7') || (c == '8')
//            || (c == '9'))
//    {
//        result = c - 48;
//    }
//    else if (c == 'A' || c == 'a')
//    {
//        result = 10;
//    }
//    else if (c == 'B' || c == 'b')
//    {
//        result = 11;
//    }
//    else if (c == 'C' || c == 'c')
//    {
//        result = 12;
//    }
//    else if (c == 'D' || c == 'd')
//    {
//        result = 13;
//    }
//    else if (c == 'E' || c == 'e')
//    {
//        result = 14;
//    }
//    else if (c == 'F' || c == 'f')
//    {
//        result = 15;
//    }
//    else{
//        result = -1;
//        *error = ERROR_INVALID_HEX;
//    }
//
//    return result;
//}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

void BM_HAL_delay(uint32_t mSec)
{
#ifdef STM32F103xB
    HAL_Delay(mSec); // miliseconds
#elif defined(VIRTUAL_MCU)
    Sleep(mSec);
#endif
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

