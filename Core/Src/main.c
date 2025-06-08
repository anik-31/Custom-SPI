#include "stm32f4xx.h"

#define SPI_CS_PIN GPIO_PIN_6
#define SPI_CS_PORT GPIOB

void SPI_Master_Init(void);
void SPI_Master_Transmit(uint8_t *data, uint8_t length);

int main(void) {
    uint8_t data[] = "Hello ESP32";

    SPI_Master_Init();

    while (1) {
        SPI_Master_Transmit(data, sizeof(data) - 1); // Send 11 bytes
        for (volatile int i = 0; i < 1000000; i++); // Simple delay
    }
}

void SPI_Master_Init(void) {
    // Enable clocks for SPI1, GPIOA, GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;

    // Configure SPI pins:
    // PA5 -> SCK, PA6 -> MISO, PA7 -> MOSI
    GPIOA->MODER &= ~((3 << (5 * 2)) | (3 << (6 * 2)) | (3 << (7 * 2))); // Clear mode bits
    GPIOA->MODER |=  (2 << (5 * 2)) | (2 << (6 * 2)) | (2 << (7 * 2));  // Alternate function mode

    // AF5 for SPI1
    GPIOA->AFR[0] &= ~((0xF << (5 * 4)) | (0xF << (6 * 4)) | (0xF << (7 * 4)));
    GPIOA->AFR[0] |=  (5 << (5 * 4)) | (5 << (6 * 4)) | (5 << (7 * 4));

    // Configure PB6 as output for CS pin
    GPIOB->MODER &= ~(3 << (6 * 2));    // Clear mode bits for PB6
    GPIOB->MODER |=  (1 << (6 * 2));    // Output mode
    GPIOB->OSPEEDR |= (3 << (6 * 2));   // High speed
    GPIOB->OTYPER &= ~(1 << 6);         // Push-pull
    GPIOB->ODR |= SPI_CS_PIN;            // Set CS high (deselect)

    // Configure SPI1 in Master mode
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR        // Master mode
               | SPI_CR1_SSI         // Internal slave select
               | SPI_CR1_SSM         // Software slave management
               | SPI_CR1_BR_0        // Baud rate = fPCLK/4
               /* CPOL and CPHA = 0, so no need to set these bits */
               ;


    SPI1->CR2 = 0;                    // Default

    SPI1->CR1 |= SPI_CR1_SPE;        // Enable SPI1
}

void SPI_Master_Transmit(uint8_t *data, uint8_t length) {
    // Pull CS low to select slave
    GPIOB->ODR &= ~SPI_CS_PIN;

    for (uint8_t i = 0; i < length; i++) {
        while (!(SPI1->SR & SPI_SR_TXE)); // Wait until TX buffer empty
        SPI1->DR = data[i];               // Send byte
        while (!(SPI1->SR & SPI_SR_RXNE)); // Wait until RX buffer not empty
        (void)SPI1->DR;                   // Read received byte (dummy read)
    }

    // Wait until SPI not busy before releasing CS
    while (SPI1->SR & SPI_SR_BSY);

    // Pull CS high to deselect slave
    GPIOB->ODR |= SPI_CS_PIN;
}
