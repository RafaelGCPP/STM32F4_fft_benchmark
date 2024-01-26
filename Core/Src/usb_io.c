#include <usbd_cdc_if.h>

volatile static uint32_t data_received = 0;

int _read(int file, char *ptr, int len)
{
  int length = 0;

  do
  {
    while(data_received == 0)
    {
      ;
    }

    *ptr = UserRxBufferFS[0];
    ptr++;
    length+= data_received;

    data_received = 0;
  } while((length < len) && (*(ptr-1) != '\r'));

  return length;
}



int _write(int file, char *ptr, int len)
{
	CDC_Transmit_FS((uint8_t *)ptr, len);

  return len;
}
