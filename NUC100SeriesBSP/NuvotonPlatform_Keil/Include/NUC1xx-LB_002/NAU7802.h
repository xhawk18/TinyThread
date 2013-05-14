void InitNAU7802(void);
void wNAU7802(uint8_t addr, uint8_t* data, uint8_t lenght);
void rNAU7802(uint8_t addr, uint8_t* data, uint8_t lenght);
uint32_t GetADCResult_NAU7802(void);
void wNAU7802_B(uint8_t addr, uint8_t data);
uint8_t rNAU7802_B(uint8_t addr);
