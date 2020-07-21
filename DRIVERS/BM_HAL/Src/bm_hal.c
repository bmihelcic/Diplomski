/*
 * bm_hal.c
 *
 *  Created on: 16. tra 2020.
 *      Author: branimir
 */

#include "bm_hal.h"


#ifdef STM32
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_CAN_Init(void);
static void MX_USART1_UART_Init(void);
#endif

#ifdef VIRTUAL_MCU

static SOCKET acceptNewConnection(SOCKET listen_socket);
static void handleSocketRead(SOCKET socket_descriptor);


char rxBuff[1024] =
{ 0 };
char canMessage[64] =
{ 0 };
char req_buff[10] =
{ 0 };
char *message = "Greeting message";

WSADATA wsaData;

SOCKET listenSocket = INVALID_SOCKET;
SOCKET clientSocket = INVALID_SOCKET;
SOCKET maxSocket    = INVALID_SOCKET;

struct addrinfo *info = NULL;
struct addrinfo hints;

char recvbuf[DEFAULT_BUFLEN];

int iResult;
int opt = 1;
int valread;
socklen_t addrlen;
struct sockaddr_in server;
struct sockaddr_in client;

fd_set readfds;
fd_set activefds;

HANDLE  hThread;
//PMYDATA pData;
DWORD   dwThreadId;
#endif


void BM_HAL_init()
{
#ifdef STM32
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_ADC1_Init();
	MX_CAN_Init();
	MX_USART1_UART_Init();

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
    getaddrinfo(NULL, DEFAULT_PORT, &hints, &info);

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
    server.sin_port = htons(TELNET_PORT);

    //bind the socket to localhost port 8888
    iResult = bind(listenSocket, (struct sockaddr*) &server, sizeof(server));
    if (iResult < 0)
    {
        printf("(error) bind failed with error: %d\n", WSAGetLastError());
        fflush(stdout);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    printf("(info) Listen socket bound to ip: localhost (127.0.0.1), port: %d\n", TELNET_PORT);

    if (listen(listenSocket, 3))
    {
        printf("(error) listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    printf("I'm listening on port %d\n", TELNET_PORT);
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

    hThread = CreateThread(
        NULL,                   // default security attributes
        0,                      // use default stack size
        MyThreadFunction,       // thread function name
        NULL,                  // argument to thread function
        0,                      // use default creation flags
        &dwThreadId);           // returns the thread identifier

    if (hThread == NULL)
    {
       ExitProcess(3);
    }
#endif
}



#ifdef STM32
/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**ADC1 GPIO Configuration
  PA0-WKUP   ------> ADC1_IN0
  PA1   ------> ADC1_IN1
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /** Common config
  */
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_0, LL_ADC_SAMPLINGTIME_1CYCLE_5);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART1 GPIO Configuration
  PA9   ------> USART1_TX
  PA10   ------> USART1_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOD);
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

	  /**/
	  LL_GPIO_ResetOutputPin(ON_BOARD_LED_GPIO_Port, ON_BOARD_LED_Pin);

	  /**/
	  LL_GPIO_ResetOutputPin(GPIOA, OUTPUT0_Pin|OUTPUT1_Pin|OUTPUT2_Pin|OUTPUT3_Pin);

	  /**/
	  GPIO_InitStruct.Pin = ON_BOARD_LED_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  LL_GPIO_Init(ON_BOARD_LED_GPIO_Port, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = OUTPUT0_Pin|OUTPUT1_Pin|OUTPUT2_Pin|OUTPUT3_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = INPUT1_Pin|INPUT2_Pin|INPUT3_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /**/
	  GPIO_InitStruct.Pin = INPUT0_Pin;
	  GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	  LL_GPIO_Init(INPUT0_GPIO_Port, &GPIO_InitStruct);
}


/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

	if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
	{
		Error_Handler();
	}
	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while(LL_RCC_HSI_IsReady() != 1)
	{
	}
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);
	LL_RCC_PLL_Enable();

	   /* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1)
	{
	}
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	   /* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
	{
	}
	LL_SetSystemCoreClock(64000000);

	/* Update the time base */
	if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
	{
		Error_Handler();
	};
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);
}
#endif


#ifdef VIRTUAL_MCU
DWORD WINAPI MyThreadFunction( LPVOID lpParam )
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
                // if listenSocket is active that means there is a new socket waiting for connection
                if(sd == listenSocket)
                {
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
        getpeername(socket_descriptor, info->ai_addr, info->ai_addrlen);
        printf("Host disconnected\n");
        fflush(stdout);
        FD_CLR(socket_descriptor, &readfds);
    }
    else
    {
        printf("Message received: %s\n", rxBuff);
        fflush(stdout);
        if (strstr(rxBuff, "REQ") != NULL)
        {
            strncpy(req_buff, rxBuff, 10);
            if (strstr(req_buff, "CAN"))
            {
                iResult = send(socket_descriptor, canMessage, (int) strlen(canMessage), 0);
                if (iResult == SOCKET_ERROR)
                {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    fflush(stdout);
                    closesocket(socket_descriptor);
                    WSACleanup();
                    return;
                }
            }
        }
        else
        {
        }
        //send(socket_descriptor, rxBuff, strlen(rxBuff), 0);
    }
}
#endif

int stringToHex(char *inputString)
{
    int i;
    char temp;
    int result = 0;
    int n = strlen(inputString)-2;  //how many digits w/o the 1st two (0x)
    int exp=n;
    ERROR_E error = ERROR_OK;

    for (i = 0; i < n; i++)
    {
        if(error == ERROR_OK){
            result += hexCharToDec(inputString[i+2], &error) * (int)pow(16, exp-1);
            exp--;
        }
        else{
            result = -1;
            printf("(error) Unable to decode input string, wrong hex values!\n");
            break;
        }
    }
    return result;
}

int hexCharToDec(char c, ERROR_E *error)
{
    int result;

    if ((c == '0') || (c == '1') || (c == '2') || (c == '3') || (c == '4')
            || (c == '5') || (c == '6') || (c == '7') || (c == '8')
            || (c == '9'))
    {
        result = c - 48;
    }
    else if (c == 'A' || c == 'a')
    {
        result = 10;
    }
    else if (c == 'B' || c == 'b')
    {
        result = 11;
    }
    else if (c == 'C' || c == 'c')
    {
        result = 12;
    }
    else if (c == 'D' || c == 'd')
    {
        result = 13;
    }
    else if (c == 'E' || c == 'e')
    {
        result = 14;
    }
    else if (c == 'F' || c == 'f')
    {
        result = 15;
    }
    else{
        result = -1;
        *error = ERROR_INVALID_HEX;
    }

    return result;
}


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

