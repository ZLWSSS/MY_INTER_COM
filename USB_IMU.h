#ifndef USB_IMU_H_
#define USB_IMU_H_

#include "Inter_Board_Com.h"
#include <cstring>

typedef struct {
    float q[4];
    float gyro[3];
    float accel[3];
    uint32_t checksum;
} usb_imu_rx_cmd_t;

typedef struct {
    float q[4];
    float gyro[3];
    float accel[3];
} usb_imu_rx_data_t;

class USB_COM_IMU:public N_Communication::USB_COM {
public:
    USB_COM_IMU(int in_length_8b, int out_length_8b ,uint16_t _vendor_id, uint16_t _product_id, unsigned char _endpoint_in, unsigned char _endpoit_out);
    ~USB_COM_IMU() override;
    void Deal_Out_Data() override;
    void Deal_In_Data() override;
    void USB_Com_Start_Trans_Asy(void(*cbf_wrapper)(struct libusb_transfer *)) override;
    void USB_In_CBF(struct libusb_transfer *transfer) override;
    void USB_Out_CBF(struct libusb_transfer *transfer) override;
    void Print_Rx_Data();

private:
    usb_imu_rx_cmd_t *usb_in_data;
    usb_imu_rx_data_t *usb_data_drv;
    unsigned char endpoint_in;
    unsigned char endpoint_out;
    const int if_print = 1;
};

#endif