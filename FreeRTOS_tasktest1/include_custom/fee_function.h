

#ifndef INCLUDE_CUSTOM_FEE_FUNCTION_H_
#define INCLUDE_CUSTOM_FEE_FUNCTION_H_

#include "ti_fee.h"

void fee_initial(void);
void fee_write(uint16_t BlockNumber, uint8_t *pData);
void fee_read(uint16_t BlockNumber, uint16_t BlockOffset, uint8_t *pData, uint16_t Length);

#endif /* INCLUDE_CUSTOM_FEE_FUNCTION_H_ */
