// =============================================================================
// STM32 HAL/BSP Stubs for Windows Compilation Testing
//
// These are empty implementations that allow the demo to link on Windows.
// They do nothing - the actual implementations exist in the STM32 HAL/BSP
// libraries on the real target.
// =============================================================================

#include <cstdint>

extern "C" {

// GPIO
typedef struct {
      uint32_t dummy;
} GPIO_TypeDef;

typedef struct {
      uint32_t dummy;
} GPIO_InitTypeDef;

typedef enum { GPIO_PIN_RESET = 0, GPIO_PIN_SET = 1 } GPIO_PinState;

void HAL_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_Init) {
}



void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
}

// Delay
void HAL_Delay(uint32_t Delay) {
}

// SPI BSP
int32_t BSP_SPI1_Init(void) {
      return 0;
}



int32_t BSP_SPI1_Send(uint8_t* pData, uint16_t Length) {
      return 0;
}

} // extern "C"
