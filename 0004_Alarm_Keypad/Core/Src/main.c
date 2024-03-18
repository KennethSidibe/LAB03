  /* USER CODE BEGIN Header */
  /**
    ******************************************************************************
    * @file           : main.c
    * @brief          : Main program body
    ******************************************************************************
    * @attention
    *
    * Copyright (c) 2024 STMicroelectronics.
    * All rights reserved.
    *
    * This software is licensed under terms that can be found in the LICENSE file
    * in the root directory of this software component.
    * If no LICENSE file comes with this software, it is provided AS-IS.
    *
    ******************************************************************************
    */
  /* USER CODE END Header */
  /* Includes ------------------------------------------------------------------*/
  #include "main.h"

  /* Private includes ----------------------------------------------------------*/
  /* USER CODE BEGIN Includes */
  #include "Keypad4X4.h"
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "fonts.h"
  #include "ssd1306.h"
  #include <stdarg.h>


  /* USER CODE END Includes */

  /* Private typedef -----------------------------------------------------------*/
  /* USER CODE BEGIN PTD */

  /* USER CODE END PTD */

  /* Private define ------------------------------------------------------------*/
  /* USER CODE BEGIN PD */

  /* USER CODE END PD */

  /* Private macro -------------------------------------------------------------*/
  /* USER CODE BEGIN PM */

  /* USER CODE END PM */

  /* Private variables ---------------------------------------------------------*/
  I2C_HandleTypeDef hi2c1;

  UART_HandleTypeDef huart2;

  /* USER CODE BEGIN PV */
  extern char key;
  char key_2;
  char hold[6];
  bool armed = false;
  /* USER CODE END PV */

  /* Private function prototypes -----------------------------------------------*/
  void SystemClock_Config(void);
  static void MX_GPIO_Init(void);
  static void MX_USART2_UART_Init(void);
  static void MX_I2C1_Init(void);
  /* USER CODE BEGIN PFP */

  /* USER CODE END PFP */

  /* Private user code ---------------------------------------------------------*/
  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /**
    * @brief  The application entry point.
    * @retval int
    */

  char* replaceCharsWithAsterisks(const char* input, int size) {
      // Allocate memory for the new array (+1 for the null terminator)
      char* result = (char*)malloc((size + 1) * sizeof(char));
      // Replace each char with an asterisk
      for (int i = 0; i < size; i++) {
          result[i] = '*';
      }

      // Add null terminator at the end
      result[size] = '\0';

      return result;
  }
  int main(void)
  {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */
    /* USER CODE BEGIN 2 */
      SSD1306_Init();
      SSD1306_GotoXY (0,0);
      //SSD1306_Puts ("Voltage:", &Font_11x18, 1);
      SSD1306_Puts ("Enter Code:", &Font_11x18, 1);
      SSD1306_GotoXY (0, 30);
      SSD1306_UpdateScreen();
      SSD1306_UpdateScreen();
      HAL_Delay (500);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
          /* USER CODE END WHILE */

      /* USER CODE BEGIN 3 */
    /* D10 to D7 as input pins for row 0 to row 3. D6 to D3 as output for column pins C1 to C3*/

      // configure code //

      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

      // counter is used to replace user input with *
      int counter = 0;

      // We initialize the system here and let the user arm the system with his pwd
      // The user can have a pwd of 4 or 6 of length, to check that we make our loop
      // of max length 6 
      for(int i = 0; i <= 6; i++){
        // key stores the current input of the user received by Get_Key()
        key = Get_Key();
        counter++;

        // Here we check if the user has finalized entering his pwd 
        // -> finished entering pwd : press # and pwd must be of len 4/6 characters 
        if(key == '#' && (strlen(hold) == 4 || strlen(hold) == 6)){

          // We update the screen showing successfull arming and arm the system
          SSD1306_GotoXY (0, 30);
          SSD1306_UpdateScreen();
          SSD1306_Puts ("ARME  ", &Font_11x18, 1);
          SSD1306_UpdateScreen();
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
          armed = true;

          // We update the screen showing successfull arming and arm the system

          break;
        }

        // If the user has not finished entering his pwd 
        // -> We print '*' instead of what he has entered as an added security measure
        // We take into account the number of characters he has entered and 
        // print the according numbers of stars

        // OR 

        // If the input is not of len 4/6 characters and the user has not 
        // confirm his entry with the key '#'

        // We enter the 'else' statement


        else{
          // This concatenate what is in the key variable in the hold variable 
          strcat(hold, &key);

          SSD1306_GotoXY (0, 30);
          SSD1306_UpdateScreen();
          
          // Result is what will be shown to the user
          char* result = replaceCharsWithAsterisks(hold,counter);
          SSD1306_Puts (result, &Font_11x18, 1);
          SSD1306_UpdateScreen();
        }
      }

      key_2 = key;
      
      // NON-ARMÉ
      while(1){

      char temp[6];

      // If the user has successfully armed the system
        if(armed == true){

          // We show that the system is armed by printing ARME on the screen

          counter = 0;
          SSD1306_GotoXY (0, 30);
          SSD1306_Clear();
          SSD1306_UpdateScreen();
          SSD1306_Puts ("ARME", &Font_11x18, 1);
          SSD1306_UpdateScreen();

          // We show that the system is armed by printing ARME on the screen 

          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);


          HAL_Delay(1500);

          SSD1306_GotoXY (0, 30);
          SSD1306_Clear();
          SSD1306_UpdateScreen();


          // We clear whatever is in the temp variable and set it basically to NULL
          memset(temp, '\0', sizeof(temp));

          // We check again for user input 
          // Here we let the user un-arm the system
          for(int i = 0; i <= 6; i++){

            // This first part let the user enter his pwd that he did, and unarm the system

                key = Get_Key();
                counter++;

                // If the confirmation key ('#') is entered and the 
                // pwd is the same as the one previously entered
                // Then we can successfully unarm the system

                if(key == '#'){

                  // the line 0 == strcmp(hold,temp) check if hold and temp are equal
                  // If so strcmp returns 0 which we check and if it is 
                  // effectively true, our condition is true !

                  if(0 == strcmp(hold, temp))
                  armed = false;
                  break;
                }
              else if (key == hold[i]){
                // If the confirmation key has not been entered but
                // the key entered by the user is similar to the ith character that 
                // he is supposed to enter
                // We concat it and unarm the system

                strcat(temp, &key);
                armed = false;
                SSD1306_GotoXY (0, 30);
                SSD1306_UpdateScreen();
                // We don't forget to replace the actual input with *
                char* result = replaceCharsWithAsterisks(hold,counter);
                SSD1306_Puts (result, &Font_11x18, 1);
                SSD1306_UpdateScreen();

              }
              else {
                // If the entry is not the confirmation key
                // and the entry is not similar to the ith character that 
                // he is supposed to enter

                // We just concat it
                strcat(temp, &key);

                SSD1306_GotoXY (0, 30);
                SSD1306_UpdateScreen();
                SSD1306_Puts (temp, &Font_11x18, 1);
                SSD1306_UpdateScreen();
              }
            }

        }

        // If the user has not successfully armed the system before
        // We give him another chance to do so after having the user wait for a small delay
        // We let the user know that the sys was not armed 
        // By priting "NON-ARME"

        else {
          SSD1306_GotoXY (0, 30);
          SSD1306_Clear();
          SSD1306_UpdateScreen();
          SSD1306_Puts ("NON-ARME", &Font_11x18, 1);
          SSD1306_UpdateScreen();
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // green LED
          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // red LED


          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

          char temp[6];

          HAL_Delay(1500);

          SSD1306_Clear();
          SSD1306_UpdateScreen();

            memset(temp, '\0', sizeof(temp));
            char star[6];
            char ja = '*';

            // Here we let the user arm the system again using the same method as above 

          for(int i = 0; i <= 6; i++){
              key = Get_Key();

              if(key == '#'){
                  if(0 == strcmp(hold, temp))
                armed = true;
                break;
              }
              else{
                strcat(temp, &key);
                SSD1306_GotoXY (0, 30);
                SSD1306_UpdateScreen();

                SSD1306_Puts (temp, &Font_11x18, 1);
                SSD1306_UpdateScreen();
              }
          }
        }

      }


    }
    /* USER CODE END 3 */
  }

  /**
    * @brief System Clock Configuration
    * @retval None
    */
  void SystemClock_Config(void)
  {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /**
    * @brief I2C1 Initialization Function
    * @param None
    * @retval None
    */
  static void MX_I2C1_Init(void)
  {

    /* USER CODE BEGIN I2C1_Init 0 */

    /* USER CODE END I2C1_Init 0 */

    /* USER CODE BEGIN I2C1_Init 1 */

    /* USER CODE END I2C1_Init 1 */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN I2C1_Init 2 */

    /* USER CODE END I2C1_Init 2 */

  }

  /**
    * @brief USART2 Initialization Function
    * @param None
    * @retval None
    */
  static void MX_USART2_UART_Init(void)
  {

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */

    /* USER CODE END USART2_Init 2 */

  }

  /**
    * @brief GPIO Initialization Function
    * @param None
    * @retval None
    */
  static void MX_GPIO_Init(void)
  {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, KC0_Pin|GPIO_PIN_13|GPIO_PIN_14|KC3_Pin
                            |KC1_Pin|KC2_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin : PA5 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pins : KC0_Pin PB13 PB14 KC3_Pin
                            KC1_Pin KC2_Pin */
    GPIO_InitStruct.Pin = KC0_Pin|GPIO_PIN_13|GPIO_PIN_14|KC3_Pin
                            |KC1_Pin|KC2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pin : KR1_Pin */
    GPIO_InitStruct.Pin = KR1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KR1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : KR3_Pin KR2_Pin */
    GPIO_InitStruct.Pin = KR3_Pin|KR2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : KR0_Pin */
    GPIO_InitStruct.Pin = KR0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(KR0_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
  }

  /* USER CODE BEGIN 4 */

  /* USER CODE END 4 */

  /**
    * @brief  Period elapsed callback in non blocking mode
    * @note   This function is called  when TIM6 interrupt took place, inside
    * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
    * a global variable "uwTick" used as application time base.
    * @param  htim : TIM handle
    * @retval None
    */
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
  {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
      HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
  }

  /**
    * @brief  This function is executed in case of error occurrence.
    * @retval None
    */
  void Error_Handler(void)
  {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
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
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
  }
  #endif /* USE_FULL_ASSERT */
